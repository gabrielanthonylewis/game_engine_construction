#pragma once

#include "Entity.h"

#include <string>

// forward declarations
class Texture;
class Vector2;

/*A missile is much like a bullet, except as the player shoots it,
it will always go up vertically.
*/
class Missile : public Entity
{
public:
	Missile(const std::string& uniqueGraphicsID, Texture& texture, Vector2 initialPosition);
	~Missile();
	
	// Overrides
	void Update() override final;
	void OnDeath() override final;
	void OnEndAnimation() override final;
	void OnCollision(Entity& other) override final;
};

