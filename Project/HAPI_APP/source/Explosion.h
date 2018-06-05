#pragma once

#include "Entity.h"

// Forward Declarations
class Texture;
class Vector2;

/*An explosion is only for aesthetic purposes only. 
It will destroy itself after the animation completes.
*/
class Explosion : public Entity
{
public:
	Explosion(const std::string& uniqueGraphicsID, Texture& texture, Vector2 position);
	~Explosion();

	// Overrides
	void Update() override final;
	void OnDeath() override final;
	void OnEndAnimation() override final;
	 void OnCollision(Entity& other) override final;

private:
	// So that the sound is only played once when it is visible
	bool _soundUsed = false;
};

