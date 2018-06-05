#pragma once

// -----------------------
// Gabriel Lewis Q5094111
// -----------------------

#include <vector>

// Forward declartions
class Entity;
class Player;
class Texture;
class Sprite;
class Bullet;
class Explosion;
class Vector2;
enum class ESide;
class Drone;
class DroneMaker;
class KamikazeDrone;
class Map;
class AI;
class EnemyWaves;


/* Which game screen we are on*/
enum CurrentScreen
{
	E_MainMenu,
	E_Game,
	E_EndScreen
};

// Which difficulty the game is on
// See "ApplyDifficulty" implementation
// for a description on difficulty
enum Difficulty
{
	E_Easy,
	E_Normal,
	E_Hard
};

//The world model black box dealing with loading/setting up the level and running it
class World
{
	static World *s_instance;

public:
	World();
	~World();

	// Initialise the level creating all of the textures and entities and initialising the visualiser
	bool LoadLevel(int screenWidth, int screenHeight);
	bool LoadLevelActual();

	// Updates all entities and the player with the new variables
	// changed with difficulty (health specifically)
	void ApplyDifficulty(Difficulty diff);

	// Run the main world/game loop  (Hapi.Loop)
	void Run();

	// Shoot a projectile using the Object pool
	void Shoot(Vector2 position, ESide side, Vector2 rawStepDirection);
	void ShootMissile(Vector2 position, ESide side);

	// Make an eitity at the position using the Object pool
	void AddEntity(Vector2 position);
	KamikazeDrone* AddKamikazeDrone(Vector2 position);

	// When the wave is complete either spawn the next wave if 
	// there is one or if all waves are complete then end the game (player has won).
	void WaveComplete();

	// Setter functions
	void AddScore(int val);

	// Getter functions
	Vector2 GetPlayerPosition() const;
	Vector2 GetPlayerTile() const;
	int GetPlayerHealth() const;
	int GetPlayerInitialHealth() const;
	bool GetPlayerVisibility() const;
	inline Map* GetMap() { return _map; }

	// Get the instance, if none exists then creat a new one
	inline static World *instance()
	{
		if (!s_instance)
			s_instance = new World;
		return s_instance;
	}

private:
	// All entities in the game to be updated and rendered
	std::vector<Entity*> _entities;
	// All entites that need to be deleted (needed because the Object Pool
	// deletes its own entities for example)
	std::vector<Entity*> _toDeleteEntities;

	// Reference to these projectile sprites to spawn them
	// in the right place.
	Sprite* _bulletSprite = nullptr;
	Sprite* _missileSprite = nullptr;

	// A reference to the player so we can do more
	// things specific to the player in the world.
	Player*  _player = nullptr;


	Map* _map = nullptr;

	// Current screen variables
	CurrentScreen _currScreen = CurrentScreen::E_MainMenu;
	std::string _endScreenText = "NOT SET"; // so I can reuse the end screen for win or lose
	Difficulty _currDifficulty = Difficulty::E_Normal;

	// Reference to the waves, there is no limit to the amount 
	// of waves. If all waves are cleared the win state will be true.
	EnemyWaves* _enemyWaves = nullptr;
	bool _winstate = false;

	// Update rate, set at 60 times a second
	const unsigned long _TICK_TIME = 1000 / 60;

	//Reusable textures
	Texture* _playerTexture = nullptr;
	Texture* _droneMakerTexture = nullptr;
	Texture* _bossTexture = nullptr;
	Texture* _backgroundTexture = nullptr;
	Texture* _HUDMissileTexture = nullptr;
	Texture* _HUDBulletTexture = nullptr;
	Texture* _HUDHealthTexture = nullptr;
	Texture* _mainMenuTexture = nullptr;
};

