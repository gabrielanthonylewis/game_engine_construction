#include "ObjectPool.h"

#include "Bullet.h"
#include "Missile.h"
#include "Explosion.h"
#include "Texture.h"
#include "Drone.h"
#include "KamikazeDrone.h"
#include "World.h"
#include <string>

#include <HAPI_lib.h> // Include the HAPI header to get access to all of HAPIs interfaces
using namespace HAPISPACE; // HAPI itself is wrapped in the HAPISPACE namespace



ObjectPool::ObjectPool()
{
}

ObjectPool::~ObjectPool()
{
	// Clean Up
	for (Entity* entity : _entities)
		delete entity;

	delete _bulletTexture;
	_bulletTexture = nullptr;

	delete _missileTexture;
	_missileTexture = nullptr;

	delete _explosionTexture;
	_explosionTexture = nullptr;

	delete _pathdroneTexture;
	_pathdroneTexture = nullptr;

	delete _kamikazeTexture;
	_kamikazeTexture = nullptr;
}

void ObjectPool::Initialise(int bulletCount, int explosionCount, int missileCount, int enemyCount, int kamikazeCount)
{
	// Clean Up
	{
		for (Entity* entity : _entities)
		{
			if(entity != nullptr)
				delete entity;
		}
		
		if (_bulletTexture != nullptr)
		delete _bulletTexture;
		_bulletTexture = nullptr;

		if (_missileTexture != nullptr)
		delete _missileTexture;
		_missileTexture = nullptr;

		if (_explosionTexture != nullptr)
		delete _explosionTexture;
		_explosionTexture = nullptr;

		if (_pathdroneTexture != nullptr)
		delete _pathdroneTexture;
		_pathdroneTexture = nullptr;

		if (_kamikazeTexture != nullptr)
			delete _kamikazeTexture;
		_kamikazeTexture = nullptr;
	}

	// Set up textures
	_bulletTexture = new Texture("images//bullet.png");
	_explosionTexture = new Texture("images//exp2_0.png");//"images//explosion.png");
	_missileTexture = new Texture("images//missile.png");
	_pathdroneTexture = new Texture("images//pathdrone.png"); //greenSpinner.tga
	_kamikazeTexture = new Texture("images//kazidrone.png"); //greenSpinner.tga

	// assign counts
	_bulletCount = bulletCount;
	_explosionCount = explosionCount;
	_missileCount = missileCount;
	_enemyCount = enemyCount;
	_kamikazeCount = kamikazeCount;


	_entities.clear();

	// Bullets
	_bullets.clear();
	for (int i = 0; i < _bulletCount; i++)
	{
		Bullet* newBullet = new Bullet("bullet" + std::to_string(i), *_bulletTexture, Vector2(0, 0), Vector2(0,0));
		newBullet->SetVisibility(false);

		_bullets.push_back(newBullet);
		_entities.push_back(newBullet);
	}

	// Missiles
	_missiles.clear();
	for (int i = 0; i < _missileCount; i++)
	{
		Missile* newMissile = new Missile("missile" + std::to_string(i), *_missileTexture, Vector2(0, 0));
		newMissile->SetVisibility(false);

		_missiles.push_back(newMissile);
		_entities.push_back(newMissile);
	}

	// Explosions
	_explosions.clear();
	for (int i = 0; i < _explosionCount; i++)
	{
		Explosion* newExplosion = new Explosion("explosion" + std::to_string(i), *_explosionTexture, Vector2(0, 0));
		newExplosion->SetVisibility(false);

		_explosions.push_back(newExplosion);
		_entities.push_back(newExplosion);
	}

	// Enemies (for drone path)
	_enemies.clear();
	for (int i = 0; i < _enemyCount; i++)
	{
		Drone* newEnemy = new Drone("enemyPool" + std::to_string(i), *_pathdroneTexture, Vector2(0, 0));
		newEnemy->SetVisibility(false);

		_enemies.push_back(newEnemy);
		_entities.push_back(newEnemy);
	}

	// Kamikaze Drones (for drone maker)
	_kamikazeDrones.clear();
	for (int i = 0; i < _kamikazeCount; i++)
	{
		KamikazeDrone* newEnemy = new KamikazeDrone("kamikazePool" + std::to_string(i), *_kamikazeTexture, Vector2(0, 0), nullptr);
		newEnemy->SetVisibility(false);
	
		_kamikazeDrones.push_back(newEnemy);
		_entities.push_back(newEnemy);
	}
}


Bullet* ObjectPool::GetFirstFreeBullet()
{
	Bullet* bullet = nullptr;
	if(_bullets.size() > 0)
		bullet = _bullets[0];

	bool success = false;
	for (size_t i = 0; i < _bullets.size(); i++)
	{
		if (_bullets[i]->GetVisibility() == false)
		{
			bullet =  _bullets[i];
			success = true;
			break;
		}
	}

	if(!success)
		HAPI.UserMessage("Not enough BULLETS, returned 1st in vector. INCREASE pool size!!!", "Warning");
	
	return bullet;
}

Explosion* ObjectPool::GetFirstFreeExplosion()
{
	Explosion* explosion = nullptr;
	if (_explosions.size() > 0)
		explosion = _explosions[0];

	bool success = false;
	for (size_t i = 0; i < _explosions.size(); i++)
	{
		if (_explosions[i]->GetVisibility() == false)
		{
			explosion = _explosions[i];
			success = true;
			break;
		}
	}

	if (!success)
		HAPI.UserMessage("Not enough EXPLOSIONS, returned 1st in vector. INCREASE pool size!!!", "Warning");

	return explosion;
}


Missile* ObjectPool::GetFirstFreeMissile()
{
	Missile* missile = nullptr;
	if (_missiles.size() > 0)
		missile = _missiles[0];

	bool success = false;
	for (size_t i = 0; i < _missiles.size(); i++)
	{
		if (_missiles[i]->GetVisibility() == false)
		{
			missile = _missiles[i];
			success = true;
			break;
		}
	}

	if (!success)
		HAPI.UserMessage("Not enough MISSILES, returned 1st in vector. INCREASE pool size!!!", "Warning");

	return missile;
}



Drone* ObjectPool::GetFirstFreeEnemy()
{
	Drone* enemy = nullptr;
	if (_enemies.size() > 0)
		enemy = _enemies[0];

	bool success = false;
	for (size_t i = 0; i < _enemies.size(); i++)
	{
		if (_enemies[i]->GetPoolUsing() == false)
		{
			enemy = _enemies[i];
			success = true;
			enemy->SetPoolUsing(true);
			break;
		}
	}

	if (!success)
		HAPI.UserMessage("Not enough ENEMIES, returned 1st in vector. ENEMIES pool size!!!", "Warning");

	return enemy;
}


KamikazeDrone* ObjectPool::GetFirstFreeKamikazeDrone()
{

	KamikazeDrone* enemy = nullptr;
	if (_kamikazeDrones.size() > 0)
		enemy = _kamikazeDrones[0];

	bool success = false;
	for (size_t i = 0; i < _kamikazeDrones.size(); i++)
	{
		if (_kamikazeDrones[i]->GetPoolUsing() == false)
		{
			enemy = _kamikazeDrones[i];
			success = true;
			enemy->SetPoolUsing(true);
			break;
		}
	}

	if (!success)
		HAPI.UserMessage("Not enough KAMIKAZEDRONES, returned 1st in vector. increase KAMIKAZEDRONES pool size!!!", "Warning");

	return enemy;
}