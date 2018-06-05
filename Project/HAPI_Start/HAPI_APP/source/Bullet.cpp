#include "Bullet.h"

#include "Visualiser.h" // To get screen information and add
#include "World.h"
#include "Sprite.h"
#include "Rectangle.h"


Bullet::Bullet(const std::string& uniqueGraphicsID, Texture& texture, Vector2 initialPosition, Vector2 rawStepDirection)
{
	// Initialise variables
	_graphicsID = uniqueGraphicsID;
	Visualiser::instance()->AddSprite(texture, _graphicsID, 1, 1);
	_speed = 20.0f;
	_health = 1;
	_initialHealth = _health;
	_damage = 20;
	_initialDamage = _damage;
	_position = initialPosition;
	_oldPosition = initialPosition;
	_isPlayer = false;
	_animationFrameRate = 0;
	_rawStepDirection = rawStepDirection;
}

Bullet::~Bullet()
{
}


void Bullet::Update()
{
	// Should I not move if invisible?
	if (_isVisible == false)
		return;


	Move(Direction::Right, _speed * _rawStepDirection.x);
	Move(Direction::Down, _speed * _rawStepDirection.y);

	// Destroy if off screen
	if (_position.y < -Visualiser::instance()->GetSprite(_graphicsID)->GetFrameRectangle()->GetHeight()
		|| 
		_position.y > Visualiser::instance()->GetScreenHeight() + Visualiser::instance()->GetSprite(_graphicsID)->GetFrameRectangle()->GetHeight()
		||
		_position.x < -Visualiser::instance()->GetSprite(_graphicsID)->GetFrameRectangle()->GetWidth()
		||
		_position.y > Visualiser::instance()->GetScreenWidth() + Visualiser::instance()->GetSprite(_graphicsID)->GetFrameRectangle()->GetWidth())
	{
		this->TakeDamage(_health);
	}
}

void Bullet::OnCollision(Entity& other)
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

void Bullet::OnEndAnimation()
{

}

void Bullet::OnDeath()
{
 	SetVisibility(false);
	_currFrame = 0;
}

