#include "Explosion.h"

#include "Visualiser.h" // To get screen information and add
#include "SoundSystem.h"
#include "Vector2.h"
#include "Sprite.h"


Explosion::Explosion(const std::string& uniqueGraphicsID, Texture& texture, Vector2 position)
{
	// Initialise variables
	_graphicsID = uniqueGraphicsID;
	Visualiser::instance()->AddSprite(texture, _graphicsID, 4, 4);
	_speed = 20.0f;
	_health = 1;
	_damage = 0;
	_position = position;
	_oldPosition = position;
	_isPlayer = false;
	_animationFrameRate = 1000 / 26;
}

Explosion::~Explosion()
{
}


void Explosion::Update()
{
	if (_isVisible == false)
		return;

	// Play sound once when visible
	if (_soundUsed == false)
	{
		SoundSystem::instance()->PlaySound("explosion");
		_soundUsed = true;
	}
}

void Explosion::OnEndAnimation()
{
	// Destroy self when the animation is finished
	TakeDamage(_health);
}

void Explosion::OnDeath()
{
	SetVisibility(false);
	_currFrame = 0;
	_soundUsed = false;
}

void Explosion::OnCollision(Entity& other)
{
}
