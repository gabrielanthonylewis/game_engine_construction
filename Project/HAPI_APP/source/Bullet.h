#pragma once

#include "Entity.h"

#include <string>

// forward declarations
class Texture;
class Vector2;

/*A bullet, once spawned will move depending on the 
step direction set.*/
class Bullet : public Entity
{
public:
	Bullet(const std::string& uniqueGraphicsID, Texture& texture, Vector2 initialPosition, Vector2 rawStepDirection); // raw meaning without speed
	~Bullet();

	// Overrides
	void Update() override final;
	void OnDeath() override final;
	void OnEndAnimation() override final;
	void OnCollision(Entity& other) override final;

	// Setter Functions
	void SetRawStepDirection(Vector2 rawStepDirection) { _rawStepDirection = rawStepDirection; }

private: 
	// direction to move by each update (as a step, e.g. up 100, left 20)
	Vector2 _rawStepDirection;
};

