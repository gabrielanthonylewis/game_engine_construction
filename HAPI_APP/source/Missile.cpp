#include "Missile.h"

#include "Visualiser.h" // To get screen information and add
#include "World.h"
#include "Sprite.h"
#include "Rectangle.h"


Missile::Missile(const std::string& uniqueGraphicsID, Texture& texture, Vector2 initialPosition)
{
	// Initialise variables
	_graphicsID = uniqueGraphicsID;
	Visualiser::instance()->AddSprite(texture, _graphicsID, 1, 1);
	_speed = 30.0f;
	_health = 1;
	_damage = 100;
	_position = initialPosition;
	_oldPosition = initialPosition;
	_isPlayer = false;
	_animationFrameRate = 0;
}

Missile::~Missile()
{
}


void Missile::Update()
{
	if (_isVisible == false)
		return;

	Move(Direction::Up, _speed);

	// Destroy itself if offscreen
	if (_position.y < -Visualiser::instance()->GetSprite(_graphicsID)->GetFrameRectangle()->GetHeight())
		this->TakeDamage(_health);
}

void Missile::OnCollision(Entity& other)
{
	// Kill the enemy adding its health to the score
	if (other.GetIsPlayer() == false)
	{
		int score = 0;
		if (other.GetPreDeathHealth() - _damage < 0)
			score = other.GetPreDeathHealth();
		else
			score = _damage;

		World::instance()->AddScore(score);
	}

	this->TakeDamage(_health);
}

void Missile::OnEndAnimation()
{
}

void Missile::OnDeath()
{
	SetVisibility(false);
	_currFrame = 0;
}
