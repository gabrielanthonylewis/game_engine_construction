#pragma once

// -----------------------
// Gabriel Lewis Q5094111
// -----------------------

#include "Entity.h" // As we are inheriting from Enity

#include <HAPI_lib.h> // Include the HAPI header to get access to all of HAPIs interfaces
using namespace HAPISPACE; // HAPI itself is wrapped in the HAPISPACE namespace

// Forward declarations
class Texture;

/*The player unlike all other entities takes input.
The player has moderate health (depending on the difficulty) 
and has a standard movement.*/
class Player : public Entity
{
public:
	Player(const std::string& uniqueGraphicsID, Texture& texture, Vector2 initialPosition, float speed);
	inline ~Player() {}

	// Overrides
	void Update() override final;
	void OnDeath() override final;
	void OnEndAnimation() override final;
	void OnCollision(Entity& other) override final;

	// Check for input from either the Keyboard or Xbox Controller
	// Note: the player goes faster depending on how far to the edge the 
	// stick is so there is variable control.
	void CheckForInput();

	// Getter
	inline int GetScore() const { return _score; }
	inline int GetMissileAmmo() const { return _missileAmmo; }

	// Setter
	inline void AddScore(int val) { _score += val; }
	inline void ReduceMissileAmmo(int val) { _missileAmmo -= val; }

private:

	int _score = 0;
	int _bulletAmmo = -1; // Infinity
	int _missileAmmo = 5;

	const HAPI_TKeyboardData &_keyboardData;
	const HAPI_TControllerData &_controllerData;

	// Xbox 360 Deadzone Values
	 int _deadzoneXLeft = -HK_GAMEPAD_LEFT_THUMB_DEADZONE;
	 int _deadzoneXRight = HK_GAMEPAD_LEFT_THUMB_DEADZONE;
	 int _deadzoneYDown = -HK_GAMEPAD_LEFT_THUMB_DEADZONE;
	 int _deadzoneYUp = HK_GAMEPAD_LEFT_THUMB_DEADZONE;

	// Xbox 360 Input 
	const float _maxInputVal = 32768.F;
	const float  _minInputVal = -32768.F;
};

