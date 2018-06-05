#pragma once

// -----------------------
// Gabriel Lewis Q5094111
// -----------------------

#include <string>

#include "Vector2.h"

// Direction used for movement
// this is because Up is down from some perspectives.
// Direction clears up any confusion.
enum Direction
{
	Up = 0,
	Down = 1,
	Left = 2,
	Right = 3
};

// Which side the entity is on, used for collision
enum class ESide
{
	eNeutral, ePlayer, eEnemy
};

/*An entity is a physical world entity that deals with movement,
calls rendering and handles animations etc. 
There are many pure virtual functions that must be implemented but
not necessarily used. Functionallity is avaible such as update, death and collision
which are called in the world loop.*/
class Entity
{
public:
	inline Entity() {};
	inline virtual ~Entity() {};

	// To be called x times every frame to update the state of the entity
	virtual void Update() = 0;
	// What should happen if there is a collision?
	virtual void OnCollision(Entity& other) = 0;
	// What should happen when the entity dies?
	virtual void OnDeath() = 0;
	// What should happen when the entity's animation dies?
	virtual void OnEndAnimation() = 0;

	// Call the visualiser to draw this entity to the screen
	void Render(float s, int frameNumber = 0);

	// Move the entity in any direction with the desired speed
	void Move(Direction direction, float speed);
	void Move(float x, float y, float speed); // Move Raw

	// Performs a check to see if the entity inputted is an ememy of this entity
	inline bool IsEnemyOf(const Entity& other) const
	{
		if (other.GetSide() == ESide::eNeutral || _side == ESide::eNeutral)
			return false;

		bool result = (other.GetSide() != _side);
		return result;
	}

	// Calculate if there is a collision
	bool CheckForCollision(const Entity& other);

	// Update the animation frame when the time is right
	void Animate();
	// Go to the next animation frame
	void NextFrame();

	// Reduce or increase health. 
	// The entity will die if the health is below 0.
	inline void TakeDamage(int val)
	{
		_health -= val;

		if (_health <= 0)
		{
			_health = 0;
			OnDeath();
		}
	}

	// Calculate which tile the entity is on
	void InitialiseTilePosition();
	
	// Setters
	inline void SetVisibility(bool state) { _isVisible = state; }
	inline void SetPosition(Vector2 position) { _position = position; _oldPosition = position; }
	inline void SetSide(ESide side) { _side = side; }
	
	//Getters
	inline bool GetVisibility() const { return _isVisible; }
	inline Vector2 GetPosition() const { return _position; }
	inline std::string GetGraphicsID() const { return _graphicsID; }
	inline ESide GetSide() const { return _side; }
	inline int GetDamage() const { return _damage; }
	inline int GetInitialDamage() const { return _initialDamage; }
	inline int GetInitialHealth() const { return _initialHealth; }
	inline int GetHealth() const { return _health; }
	inline bool GetIsPlayer() const { return _isPlayer; }
	inline unsigned long GetAnimationFrameRate() const { return _animationFrameRate; }
	inline Vector2 GetTile() const { return Vector2((float)_tileX, (float)_tileY); }
	inline int GetPreDeathHealth() const { return _preDeathHealth; }

protected:
	// position
	Vector2 _position;
	Vector2 _oldPosition; // for lerping
	
	std::string _graphicsID; // to access the sprite from the sprite map stored on the visualier
	
	// Acts the same as an active state.
	// if false no updates will happen but also no
	// rendering will happen.
	bool _isVisible = true;
	
	// movement speed
	float _speed = 1.F;

	// Which side the entity is on. 
	// Used for collision
	ESide _side = ESide::eNeutral;

	// health
	int _preDeathHealth = 0;
	int _initialHealth = 100;
	int _health = 100;

	// damage
	int _initialDamage = 0;
	int _damage = 0;
	
	// required as enemies are treated different
	// from the player. Side cannot be used as 
	// bullet will take the side of the player.
	bool _isPlayer = false;

	// animation
	int _currFrame = 0;
	unsigned long _animationFrameRate = 1000 / 16;
	unsigned long _lastTimeUpdatedANIM = 0;

	// tile position on the map
	int _tileX = 0;
	int _tileY = 0;
};

