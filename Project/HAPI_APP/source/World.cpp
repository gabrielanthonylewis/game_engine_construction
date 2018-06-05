#include "World.h"

// -----------------------
// Gabriel Lewis Q5094111
// -----------------------

// Forward Declarations
#include "ObjectPool.h"
#include "SoundSystem.h"
#include "Visualiser.h"
#include "Texture.h"
#include "Entity.h"
#include "Player.h"
#include "Bullet.h"
#include "Missile.h"
#include "Explosion.h"
#include "Vector2.h"
#include "Rectangle.h"
#include "Sprite.h"
#include "Drone.h"
#include "DronePath.h"
#include "DroneMaker.h"
#include "Boss.h"
#include "KamikazeDrone.h"
#include "EnemyWaves.h"
#include "HUDElement.h"
#include "Map.h"


World::World()
{
}

World::~World()
{
	// Clean Up
	for (Entity* entity : _toDeleteEntities)
		delete entity;

	_entities.clear();
	_toDeleteEntities.clear();

	delete _bulletSprite;
	delete _missileSprite;

	_player = nullptr;

	delete _playerTexture;
	_playerTexture = nullptr;

	delete _droneMakerTexture;
	_droneMakerTexture = nullptr;

	delete _bossTexture;
	_bossTexture = nullptr;

	delete _backgroundTexture;
	_backgroundTexture = nullptr;
	delete _mainMenuTexture;
	_mainMenuTexture = nullptr;

	delete _HUDMissileTexture;
	_HUDMissileTexture = nullptr;

	delete _HUDBulletTexture;
	_HUDBulletTexture = nullptr;

	delete _HUDHealthTexture;
	_HUDHealthTexture = nullptr;
	
	delete _map;
	_map = nullptr;

	if (_enemyWaves != nullptr)
		delete _enemyWaves;
}

Vector2 World::GetPlayerPosition() const
{ 
	if (_player == nullptr)
		return Vector2(0, 0);

	return _player->GetPosition();
}

Vector2 World::GetPlayerTile() const
{
	if (_player == nullptr)
		return Vector2(0, 0);

	return _player->GetTile();
}

bool World::GetPlayerVisibility() const
{
	if (_player == nullptr)
		return false;

	return _player->GetVisibility();
}

bool World::LoadLevel(int screenWidth, int screenHeight)
{
	if (!Visualiser::instance()->Initialise(screenWidth, screenHeight))
		return false;

	// Create Tile Map
	{
		// Clean up
		if (_map != nullptr)
		{
			delete _map;
			_map = nullptr;
		}

		const int tileWidth = 32;
		const int tileHeight = 32;

		const int numTilesX = (int)(screenWidth / (float)tileWidth);
		const int numTilesY = (int)(screenHeight / (float)tileHeight);
		
		_map = new Map(numTilesX, numTilesY);
	}

	// Load the main menu texture
	if (_mainMenuTexture != nullptr)
		delete _mainMenuTexture;
	_mainMenuTexture = new Texture("images//mainmenu.png");

	// Load in all of the sound effects and music
	SoundSystem::instance()->LoadSound("audio//blaster.wav", "blaster");
	SoundSystem::instance()->LoadSound("audio//collision.wav", "collision");
	SoundSystem::instance()->LoadSound("audio//explosion.wav", "explosion");
	SoundSystem::instance()->LoadSound("audio//missle.wav", "missle");
	SoundSystem::instance()->LoadSound("audio//noammo.wav", "noammo");
	SoundSystem::instance()->LoadMusic("audio//gamemusic.mp3", "gamemusic");
	SoundSystem::instance()->LoadMusic("audio//mainmenumusic.mp3", "mainmenumusic");

	// We default to the main menu so play the music 
	SoundSystem::instance()->PlayMusic("mainmenumusic");

	return true;
}

bool World::LoadLevelActual()
{
	// Create Textures
	if (_playerTexture != nullptr)
		delete _playerTexture;
	_playerTexture = new Texture("images//ship3.png");
	
	if (_droneMakerTexture != nullptr)
		delete _droneMakerTexture;
	_droneMakerTexture = new Texture("images//dronemakership.png");
	
	if (_bossTexture != nullptr)
		delete _bossTexture;
	_bossTexture = new Texture("images//bossship.png");

	if (_backgroundTexture != nullptr)
		delete _backgroundTexture;
	_backgroundTexture = new Texture("images//gamebackground.png");

	if (_HUDMissileTexture != nullptr)
		delete _HUDMissileTexture;
	_HUDMissileTexture = new Texture("images//missile.png");

	if (_HUDBulletTexture != nullptr)
		delete _HUDBulletTexture;
	_HUDBulletTexture = new Texture("images//bullet.png");

	if (_HUDHealthTexture != nullptr)
		delete _HUDHealthTexture;
	_HUDHealthTexture = new Texture("images//health.png");
	

	// Create helper sprites (used to query later)
	if (_bulletSprite != nullptr)
		delete _bulletSprite;
	_bulletSprite = new Sprite("images//bullet.png", 1, 1);
	if (_missileSprite != nullptr)
		delete _missileSprite;
	_missileSprite = new Sprite("images//missile.png", 1, 1);


	// Create Entities

	// clear out old entities
	for (Entity* entity : _toDeleteEntities)
	{ 
		if(entity != nullptr)
			delete entity;
	}
	_toDeleteEntities.clear();
	_entities.clear();

	// Player
	_player = new Player("player1", *_playerTexture, Vector2(300, 500), 5.F); //0.5f
	_entities.push_back(_player);

	// Drone Maker
	DroneMaker* droneMaker = new DroneMaker("enemyDroneMaker", *_droneMakerTexture, Vector2(100.F, 100.F));
	droneMaker->SetVisibility(false);
	_entities.push_back(droneMaker);

	// Boss
	Boss* boss = new Boss("boss", *_bossTexture, Vector2(400, -20));
	boss->SetVisibility(false);
	_entities.push_back(boss);
	
	// Drone Path
	DronePath* dronePath = new DronePath("dronePathWave2", *_playerTexture, Vector2(300, 100), 5);
	dronePath->SetVisibility(false);
	_entities.push_back(dronePath);

	// Create HUD
	_entities.push_back(new HUDElement("HUDmissile", *_HUDMissileTexture, Vector2(30, 660)));
	_entities.push_back(new HUDElement("HUDbullet", *_HUDBulletTexture, Vector2(30, 630)));
	_entities.push_back(new HUDElement("HUDhealth", *_HUDHealthTexture, Vector2(30, 700)));

	_toDeleteEntities = _entities;
		
	ObjectPool::instance()->Initialise(12, 10, 8, 32, 12);

	std::vector<Entity*> temp = ObjectPool::instance()->GetEntitiesVector();
	for (size_t i = 0; i < temp.size(); i++)
		_entities.push_back(temp[i]);
	

	// Reset/Clean up
	if (_enemyWaves != nullptr)
		delete _enemyWaves;
	_enemyWaves = new EnemyWaves();

	_enemyWaves->_currentWave = 0;

	// Wave 1 - Drone Maker
	_enemyWaves->_waves.push_back(new Wave());
	_enemyWaves->_waves[0]->_entites.push_back(droneMaker);
	droneMaker->SetWave(_enemyWaves->_waves[0]);

	// Wave 2 - Path
	_enemyWaves->_waves.push_back(new Wave());
	_enemyWaves->_waves[1]->_entites.push_back(dronePath);
	dronePath->SetWave(_enemyWaves->_waves[1]);

	// Wave 3 - Boss
	_enemyWaves->_waves.push_back(new Wave());
	_enemyWaves->_waves[2]->_entites.push_back(boss);
	boss->SetWave(_enemyWaves->_waves[2]);

	// Start on wave one
	_enemyWaves->SpawnWave(0);

	return true;
}

void World::ApplyDifficulty(Difficulty diff)
{
	_currDifficulty = diff;

	float healthMulti = 1.F;

	switch (diff)
	{
	case E_Easy:
		healthMulti = -0.25F; // increase health by 25%
		break;
	case E_Normal:
		healthMulti = 0.F; // do nothing
		break;
	case E_Hard:
		healthMulti = 0.25F; // reduce health by 25%
		break;
	default:
		break;
	}

	/* I chose to rather than change a bunch of code intialising entities at
	 different health, damage and speed values, I can just reduce the damage the 
	 player deals and reduce the players health. If done correctly this saves a lot of hassle*/
	if(_player != nullptr)
		_player->TakeDamage((int)(_player->GetInitialHealth() * healthMulti)); // reduce 25% of health for example

	for (Entity* entity : _entities)															
	{
		if (entity->GetIsPlayer() == false)
		{
			// increase enemies health
			// this shouldnt affect bullets but if it does It doesnt matter as it works on a collision basis
			entity->TakeDamage((int)(-entity->GetInitialHealth() * healthMulti)); // opposite effect on the enemy. If the player gets reduced health the enemy gets increase health.
			// this is the equiv of reducing the damage of the bullets fired from the player (without messy code)
		}
	}
}


void World::Run()
{
	DWORD _lastTimeUpdated = 0;

	// Down variables created so that you have to
	// let the key go to press it again.
	bool spacebarDown = false;
	bool cntrlDown = false;
	bool upKeyDown = false;
	bool downKeyDown = false;
	bool spaceKeyDown = false;

	// Which option the is selected
	int currMainMenuOption = 0;
	int currEndScreenOption = 0;

	Vector2 backgroundPos = { 0,0 };

	// World loop
	while (HAPI.Update())
	{
		// Check if key is up
		if ((HAPI.GetControllerData(0).isAttached == false && HAPI.GetKeyboardData().scanCode[HK_UP] == false)
			|| (HAPI.GetControllerData(0).isAttached == true && HAPI.GetControllerData(0).digitalButtons[HK_DIGITAL_DPAD_UP] == false))
			upKeyDown = false;

		if ((HAPI.GetControllerData(0).isAttached == false && HAPI.GetKeyboardData().scanCode[HK_DOWN] == false)
			|| (HAPI.GetControllerData(0).isAttached == true && HAPI.GetControllerData(0).digitalButtons[HK_DIGITAL_DPAD_DOWN] == false))
			downKeyDown = false;

		if ((HAPI.GetControllerData(0).isAttached == false && HAPI.GetKeyboardData().scanCode[HK_SPACE] == false)
			|| (HAPI.GetControllerData(0).isAttached == true && HAPI.GetControllerData(0).digitalButtons[HK_DIGITAL_A] == false))
			spaceKeyDown = false;


		switch (_currScreen)
		{
		case E_MainMenu:
		
			// Ensure no controller rumble
			if (HAPI.GetControllerData(0).isAttached == true)
				HAPI.SetControllerRumble(0, 0, 0);

			// Clear the screen to black
			Visualiser::instance()->ClearTo(0);
		
			// Draw background
			if(_mainMenuTexture != nullptr)
				Visualiser::instance()->Draw(*_mainMenuTexture, backgroundPos, 0);

			HAPI.RenderText(180, 70, HAPISPACE::HAPI_TColour(228, 170, 254), "Main Menu", 68);

			// Option selections
			if (downKeyDown == false && (HAPI.GetKeyboardData().scanCode[HK_DOWN]
				|| (HAPI.GetControllerData(0).isAttached == true && HAPI.GetControllerData(0).digitalButtons[HK_DIGITAL_DPAD_DOWN] == true)))
			{
				downKeyDown = true;
				currMainMenuOption++;
				if (currMainMenuOption > 2)
					currMainMenuOption = 2;
			}
			if (upKeyDown == false && (HAPI.GetKeyboardData().scanCode[HK_UP]
				|| (HAPI.GetControllerData(0).isAttached == true && HAPI.GetControllerData(0).digitalButtons[HK_DIGITAL_DPAD_UP] == true)))
			{
				upKeyDown = true;
				currMainMenuOption--;
				if (currMainMenuOption < 0)
					currMainMenuOption = 0;
			}

			// Draw Text
			if(currMainMenuOption == 0)
				HAPI.RenderText(250, 300, HAPISPACE::HAPI_TColour(183, 12, 204), "Easy Mode", 30);
			else
				HAPI.RenderText(250, 300, HAPISPACE::HAPI_TColour(244, 214, 252), "Easy Mode", 24);

			if (currMainMenuOption == 1)
				HAPI.RenderText(250, 400, HAPISPACE::HAPI_TColour(183, 12, 204), "Normal Mode", 30);
			else
				HAPI.RenderText(250, 400, HAPISPACE::HAPI_TColour(244, 214, 252), "Normal Mode", 24);
		
			if (currMainMenuOption == 2)
				HAPI.RenderText(250, 500, HAPISPACE::HAPI_TColour(183, 12, 204), "Hard Mode", 30);
			else
				HAPI.RenderText(250, 500, HAPISPACE::HAPI_TColour(244, 214, 252), "Hard Mode", 24);
		

			HAPI.RenderText(110, 620, HAPISPACE::HAPI_TColour(255, 255, 255), "keyboard: press Up/Down to select and Space to enter", 18);
			HAPI.RenderText(110, 644, HAPISPACE::HAPI_TColour(255, 255, 255), "controller: press Up/Down D-pad to select and A to enter", 18);


			// Select the option
			if (spaceKeyDown == false && (HAPI.GetKeyboardData().scanCode[HK_SPACE] 
				|| (HAPI.GetControllerData(0).isAttached == true && HAPI.GetControllerData(0).digitalButtons[HK_DIGITAL_A] == true)))
			{
				spaceKeyDown = true;
				LoadLevelActual();
				_currScreen = CurrentScreen::E_Game;

				SoundSystem::instance()->PlayMusic("gamemusic");

				ApplyDifficulty((Difficulty)currMainMenuOption);
			}
			
			break;
		
		case E_Game:
			{
				DWORD timePassed = HAPI.GetTime() - _lastTimeUpdated;

				// Shoot

				if ((HAPI.GetControllerData(0).isAttached == false && HAPI.GetKeyboardData().scanCode[HK_SPACE])
					|| (HAPI.GetControllerData(0).isAttached == true && HAPI.GetControllerData(0).digitalButtons[HK_DIGITAL_A] == true))
				{
					if (_player != nullptr && _player->GetVisibility() == true && spacebarDown == false)
					{
						float xPos = Visualiser::instance()->GetSprite(_player->GetGraphicsID())->GetFrameRectangle()->GetWidth() / 2.F;
						xPos -= _bulletSprite->GetFrameRectangle()->GetWidth() / 2.F;
						Shoot(_player->GetPosition() + Vector2(xPos, 0.F), _player->GetSide(), Vector2(0.F, -1.F));
						//SoundSystem::instance()->PlaySound("blaster");
					}

					spacebarDown = true;
				}
				else
					spacebarDown = false;


				// Missile
				if ((HAPI.GetControllerData(0).isAttached == false && HAPI.GetKeyboardData().scanCode[HK_LCONTROL])
					|| (HAPI.GetControllerData(0).isAttached == true && HAPI.GetControllerData(0).digitalButtons[HK_DIGITAL_B] == true))
				{
					if (_player != nullptr && _player->GetVisibility() == true && cntrlDown == false)
					{
						if (_player->GetMissileAmmo() > 0)
						{
							float xPos = Visualiser::instance()->GetSprite(_player->GetGraphicsID())->GetFrameRectangle()->GetWidth() / 2.F;
							xPos -= _missileSprite->GetFrameRectangle()->GetWidth() / 2.F;
							ShootMissile(_player->GetPosition() + Vector2(xPos, 0.F), _player->GetSide());
							_player->ReduceMissileAmmo(1);
						
						}
						else
						{
							SoundSystem::instance()->PlaySound("noammo");
						}
					}

					cntrlDown = true;
				}
				else
					cntrlDown = false;


			
			


				if (timePassed >= _TICK_TIME)// changed to >= instead of >
				{
					// Check for input
					if (_player != nullptr)
						_player->CheckForInput();

					// Update each entity
					for (Entity* entity : _entities)
						entity->Update();

					

					// Collision
					for (size_t index = 0; index < _entities.size(); index++)
					{
						for (size_t against = index + 1; against < _entities.size(); against++)
						{
							if (_entities[index]->IsEnemyOf(*_entities[against]) &&
								_entities[index]->CheckForCollision(*_entities[against]))
							{
								_entities[index]->OnCollision(*_entities[against]);
								_entities[against]->OnCollision(*_entities[index]);
								break;
							}
						}
					}


					// Check for lose/win conditions
					if (_player != nullptr && _player->GetHealth() <= 0)// || HAPI.GetKeyboardData().scanCode[52])
					{
						_endScreenText = "You Lose!";
						_endScreenText += "\n\n\nScore: ";
						if (_player != nullptr)
							_endScreenText += std::to_string(_player->GetScore());

						currEndScreenOption = 0;
						_currScreen = CurrentScreen::E_EndScreen;
						continue;
					}
					if (_winstate == true)
					{
						_endScreenText = "You Win!!!";
						_endScreenText += "\n\n\nScore: ";
						if (_player != nullptr)
							_endScreenText += std::to_string(_player->GetScore());
						_winstate = false;
						currEndScreenOption = 0;
						_currScreen = CurrentScreen::E_EndScreen;
						continue;
					}

					// Animation
					for (Entity* entity : _entities)
						entity->Animate();

					_lastTimeUpdated = HAPI.GetTime();
			}

			float s = (float)timePassed / _TICK_TIME;

			// Clear the screen to black
			Visualiser::instance()->ClearTo(0);

			// Draw the background
			Vector2 backgroundPos = { 0,0 };
			Visualiser::instance()->Draw(*_backgroundTexture, backgroundPos, s);

			// Draw each player
			for (Entity* entity : _entities)
			{
				if (entity->GetVisibility())
					entity->Render(s);
			}


			// Draw grid, debug!!
			/*for (int i = 0; i <= _map->GetWidth(); i++)
				Visualiser::instance()->DrawRectangle(i * _map->GetTileWidth(), Visualiser::instance()->GetScreenHeight() / 2.f, 1, Visualiser::instance()->GetScreenHeight(), HAPISPACE::HAPI_TColour(255, 255, 0));

			for (int j = 0; j <= _map->GetHeight(); j++)
				Visualiser::instance()->DrawRectangle(Visualiser::instance()->GetScreenWidth() / 2.f, j * _map->GetTileHeight(),
					Visualiser::instance()->GetScreenWidth(), 1,
					HAPISPACE::HAPI_TColour(255, 255, 0));
				*/

			// HUD
			if (_player != nullptr)
			{
				HAPI.RenderText(50, 630, HAPISPACE::HAPI_TColour(255, 255, 255), "inf", 20);
				HAPI.RenderText(50, 660, HAPISPACE::HAPI_TColour(255, 255, 255), std::to_string(_player->GetMissileAmmo()), 20);
				HAPI.RenderText(50, 700, HAPISPACE::HAPI_TColour(255, 255, 255), std::to_string(_player->GetHealth()), 20);
				HAPI.RenderText(350, 10, HAPISPACE::HAPI_TColour(255, 255, 255), std::to_string(_player->GetScore()), 20);
			}

			if(_enemyWaves != nullptr)
				HAPI.RenderText(20, 10, HAPISPACE::HAPI_TColour(255, 255, 255), "Wave: " + std::to_string(_enemyWaves->_currentWave + 1), 20);

		}

		break;

	case E_EndScreen:
		// Ensure there is no rumble		
		if (HAPI.GetControllerData(0).isAttached == true)
			HAPI.SetControllerRumble(0, 0, 0);
		
		// Clear the screen to black
		Visualiser::instance()->ClearTo(0);

		// Draw background
		if (_mainMenuTexture != nullptr)
			Visualiser::instance()->Draw(*_mainMenuTexture, backgroundPos, 0);

				
		// Cycle between options
		if (downKeyDown == false && (HAPI.GetKeyboardData().scanCode[HK_DOWN]
			|| (HAPI.GetControllerData(0).isAttached == true && HAPI.GetControllerData(0).digitalButtons[HK_DIGITAL_DPAD_DOWN] == true)))
		{
			downKeyDown = true;
			currEndScreenOption++;
			if (currEndScreenOption > 2)
				currEndScreenOption = 2;
		}
		if (upKeyDown == false && (HAPI.GetKeyboardData().scanCode[HK_UP]
			|| (HAPI.GetControllerData(0).isAttached == true && HAPI.GetControllerData(0).digitalButtons[HK_DIGITAL_DPAD_UP] == true)))
		{
			upKeyDown = true;
			currEndScreenOption--;
			if (currEndScreenOption < 1)
				currEndScreenOption = 1;
		}
		
		// Draw text
		HAPI.RenderText(220, 100, HAPISPACE::HAPI_TColour(228, 170, 254), _endScreenText, 50);

		if (currEndScreenOption == 1)
			HAPI.RenderText(250, 400, HAPISPACE::HAPI_TColour(183, 12, 204), "Restart", 30);
		else
			HAPI.RenderText(250, 400, HAPISPACE::HAPI_TColour(244, 214, 252), "Restart", 24);
			
		if (currEndScreenOption == 2)
			HAPI.RenderText(250, 500, HAPISPACE::HAPI_TColour(183, 12, 204), "Main Menu", 30);
		else
			HAPI.RenderText(250, 500, HAPISPACE::HAPI_TColour(244, 214, 252), "Main Menu", 24);

		HAPI.RenderText(110, 620, HAPISPACE::HAPI_TColour(255, 255, 255), "keyboard: press Up/Down to select and Space to enter", 18);
		HAPI.RenderText(110, 644, HAPISPACE::HAPI_TColour(255, 255, 255), "controller: press Up/Down D-pad to select and A to enter", 18);

		// Select the option
		if (spaceKeyDown == false && (HAPI.GetKeyboardData().scanCode[HK_SPACE]
			|| (HAPI.GetControllerData(0).isAttached == true && HAPI.GetControllerData(0).digitalButtons[HK_DIGITAL_A] == true)))
		{
			spaceKeyDown = true;
			if (currEndScreenOption == 1)
			{
				LoadLevelActual();
				_currScreen = CurrentScreen::E_Game;
				SoundSystem::instance()->PlayMusic("gamemusic");
				ApplyDifficulty(_currDifficulty);
			}
			else if (currEndScreenOption == 2)
			{
				_currScreen = CurrentScreen::E_MainMenu;
				SoundSystem::instance()->PlayMusic("mainmenumusic");
			}

			continue;
		}

		break;

		}
	}
}

void World::Shoot(Vector2 position, ESide side, Vector2 rawStepDirection)
{
	Bullet* freeBullet = ObjectPool::instance()->GetFirstFreeBullet();

	if (freeBullet == nullptr)
		return;

	freeBullet->SetPosition(position);
	freeBullet->SetRawStepDirection(rawStepDirection);
	freeBullet->SetSide(side);
	freeBullet->SetVisibility(true);
	freeBullet = nullptr;

	SoundSystem::instance()->PlaySound("blaster");
}

void World::ShootMissile(Vector2 position, ESide side)
{
	Missile* freeMissile = ObjectPool::instance()->GetFirstFreeMissile();

	if (freeMissile == nullptr)
		return;

	freeMissile->SetPosition(position);
	freeMissile->SetSide(side);
	freeMissile->SetVisibility(true);
	freeMissile = nullptr;

	SoundSystem::instance()->PlaySound("missle");
}

void World::AddEntity(Vector2 position)
{
	Drone* freeEnemy = ObjectPool::instance()->GetFirstFreeEnemy();

	if (freeEnemy == nullptr)
		return;

	freeEnemy->SetPosition(position);
	freeEnemy->SetVisibility(true);
}

KamikazeDrone* World::AddKamikazeDrone(Vector2 position)
{
	KamikazeDrone* freeEnemy = ObjectPool::instance()->GetFirstFreeKamikazeDrone();

	if (freeEnemy == nullptr)
		return nullptr;

	freeEnemy->SetMap(_map);

	freeEnemy->SetPosition(position);
	freeEnemy->InitialiseTilePosition();

	if(_player != nullptr)
		freeEnemy->GeneratePathTo((int)_player->GetTile().x, (int)_player->GetTile().y);

	freeEnemy->SetVisibility(true);

	return freeEnemy;
}

void World::WaveComplete()
{
	_enemyWaves->_currentWave++;

	if (_enemyWaves->_currentWave >= (int)_enemyWaves->_waves.size())
		_winstate = true;
	else
		_enemyWaves->SpawnWave(_enemyWaves->_currentWave);
}

void World::AddScore(int val)
{
	_player->AddScore(val);
}

int World::GetPlayerHealth() const
{
	if (_player == nullptr)
		return -1;

	return _player->GetHealth();
}

int World::GetPlayerInitialHealth() const
{
	if (_player == nullptr)
		return -1;

	return _player->GetInitialHealth();
}