#include "HUDElement.h"

#include "Texture.h"

HUDElement::HUDElement(const std::string& uniqueGraphicsID, Texture& texture, Vector2 initialPosition)
{
	// Initalise Variables
	_graphicsID = uniqueGraphicsID;
	Visualiser::instance()->AddSprite(texture, _graphicsID, 1, 1);
	_health = 1;
	_initialHealth = _health;
	_damage = 0;
	_initialDamage = _damage;
	_position = initialPosition;
	_oldPosition = initialPosition;
	_isPlayer = false;
	_animationFrameRate = 0;
}

HUDElement::~HUDElement()
{
}


void HUDElement::Update()
{
}

void HUDElement::OnDeath()
{
}

void HUDElement::OnEndAnimation()
{
}

void HUDElement::OnCollision(Entity& other)
{
}