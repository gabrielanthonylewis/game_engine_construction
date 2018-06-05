#include "Boss.h"

#include "AI.h"
#include "Visualiser.h" 
#include "Sprite.h"
#include "Rectangle.h"
#include "World.h"
#include <ctime>


Boss::Boss(const std::string& uniqueGraphicsID, Texture& texture, Vector2 initialPosition)
	: Boss::AI(uniqueGraphicsID, texture, initialPosition, 1, 4)
{
	// Initialise variables
	_health = 500; // boss has a lot of health
	_initialHealth = _health;
	_speed = 1; // boss is slow

	// Calculate next time the boss will fire
	_nextTime = (int)(HAPI.GetTime() / 1000.F + rand() % 3 + 2); // 2 to 3 seconds
}

Boss::~Boss()
{
}


void Boss::Update()
{
	if (!_isVisible)
		return;

	// Shoot when the time is right
	if (HAPI.GetTime() / 1000.F >= _nextTime)
	{
		// only shoot if the player is visible though.
		if(World::instance()->GetPlayerVisibility() == true)
		{
			float xPos = Visualiser::instance()->GetSprite(this->GetGraphicsID())->GetFrameRectangle()->GetWidth() / 2.F;
			float yPos = Visualiser::instance()->GetSprite(this->GetGraphicsID())->GetFrameRectangle()->GetHeight() / 2.F;

			// Burst of three, diagonal left, diagonal right, middle
			World::instance()->Shoot(this->GetPosition() + Vector2(xPos, yPos), this->GetSide(), Vector2(0.5 * 0.707106781F, 2 * 0.707106781F));
			World::instance()->Shoot(this->GetPosition() + Vector2(xPos, yPos), this->GetSide(), Vector2(-0.5 * 0.707106781F, 2 * 0.707106781F));
			World::instance()->Shoot(this->GetPosition() + Vector2(xPos, yPos), this->GetSide(), Vector2(0, 1));
		}

		_nextTime = (int)(HAPI.GetTime() / 1000.F + rand() % 3 + 2); // 2 to 3 seconds.
	}


	// Left/Right movement (go to edge of the screen)
	if (goingRight)
	{
		if (_position.x + Visualiser::instance()->GetSprite(_graphicsID)->GetFrameWidth()
			+ _speed <= Visualiser::instance()->GetScreenWidth())
			Move(Direction::Right, _speed);
		else
			goingRight = false;
	}
	else
	{
		if (_position.x - _speed >= 0)
			Move(Direction::Left, _speed);
		else
			goingRight = true;
	}

}

void Boss::OnEndAnimation()
{
}