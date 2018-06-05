#pragma once

#include "Visualiser.h" // To get screen information and add
#include "Entity.h"

// Forward Declarations
class Texture;

/*A HUDElement is an entity thats only purpose is to be rendered.
*/
class HUDElement : public Entity
{
public:
	HUDElement(const std::string& uniqueGraphicsID, Texture& texture, Vector2 initialPosition);
	~HUDElement();

	// Overrides (not used, I have inherited from Entity so 
	// it can be a physical entity on the screen to be rendered.
	void Update() override final;
	void OnDeath() override final;
	void OnEndAnimation() override final;
	void OnCollision(Entity& other) override final;
};

