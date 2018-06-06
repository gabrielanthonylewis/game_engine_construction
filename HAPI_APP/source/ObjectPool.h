#pragma once

#include <vector>

// Forward declarations (objects to be pooled)
class Entity;
class Bullet;
class Missile;
class Explosion;
class Texture;
class Drone;
class KamikazeDrone;

/*Singleton object that can be called from anywhere.
The object pool is in place to avoid creating and deleting 
instances during run time. This is very efficient as objects 
can be reused too.*/
class ObjectPool
{
	static ObjectPool *s_instance;

public:
	ObjectPool();
	~ObjectPool();

	void Initialise(int bulletCount, int explosionCount, int missileCount, int enemyCount, int kamikazeCount);

	// Getter functions
	Bullet* GetFirstFreeBullet();
	Explosion* GetFirstFreeExplosion();
	Missile* GetFirstFreeMissile();
	Drone* GetFirstFreeEnemy();  // for the drone path
	KamikazeDrone* GetFirstFreeKamikazeDrone(); // for the drone maker
	inline std::vector<Entity*> GetEntitiesVector() { return _entities; }

	// Get the instance, if none exists then creat a new one
	inline static ObjectPool *instance()
	{
		if (!s_instance)
			s_instance = new ObjectPool;
		return s_instance;
	}

private:

	// Numbers of each entity to be spawned
	int _bulletCount = 12;
	int _explosionCount = 10;
	int _missileCount = 8;
	int _enemyCount = 10;
	int _kamikazeCount = 20;
	
	// Storage of all entities
	std::vector<Bullet*> _bullets;
	std::vector<Missile*> _missiles;
	std::vector<Explosion*> _explosions;
	std::vector<Entity*> _entities;
	std::vector<Drone*> _enemies;
	std::vector<KamikazeDrone*> _kamikazeDrones;

	// Resuable Textures
	Texture* _bulletTexture = nullptr;
	Texture* _explosionTexture = nullptr;
	Texture* _missileTexture = nullptr;
	Texture* _pathdroneTexture = nullptr;
	Texture* _kamikazeTexture = nullptr;
};

