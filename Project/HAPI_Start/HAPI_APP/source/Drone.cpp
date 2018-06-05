#include "Drone.h"

#include "Visualiser.h" 
#include "Texture.h" // Acess texture information
#include "Vector2.h"
#include "Sprite.h"
#include "Rectangle.h"
#include "DronePath.h"
#include "World.h"
#include <ctime>


Drone::Drone(const std::string& uniqueGraphicsID, Texture& texture, Vector2 initialPosition)
	: Drone::AI(uniqueGraphicsID, texture, initialPosition, 1, 2)
{
	// Intialise variables
	_speed = 1.F;
	_health = 1;
	_initialHealth = _health;
	_path.clear();
	_currPathIdx = -1;

	// Next random interval time to shoot the player
	_nextTime = (int)(HAPI.GetTime() / 1000.F + rand() % 3 + 1); // 1 to 3 seconds
}


Drone::~Drone()
{
}


void Drone::Update()
{
	if (_isVisible == false)
		return;

	// shoot
	if (HAPI.GetTime() / 1000.F >= _nextTime)
	{
		//If player is visible...
		if(World::instance()->GetPlayerVisibility())
		{
			float xPos = Visualiser::instance()->GetSprite(this->GetGraphicsID())->GetFrameRectangle()->GetWidth() / 2.F;

			////////// 
			//http://forums.cgsociety.org/archive/index.php?t-522931.html
			float targetx = World::instance()->GetPlayerPosition().x + Visualiser::instance()->GetSprite("player1")->GetFrameRectangle()->GetWidth() / 2.F;
			float targety = World::instance()->GetPlayerPosition().y + Visualiser::instance()->GetSprite("player1")->GetFrameRectangle()->GetHeight() / 2.F;

			float myx = _position.x + Visualiser::instance()->GetSprite(this->GetGraphicsID())->GetFrameRectangle()->GetWidth() / 2.F;
			float myy = _position.y + Visualiser::instance()->GetSprite(this->GetGraphicsID())->GetFrameRectangle()->GetHeight() / 2.F;

			float delta_x = targetx - myx; // difference in x positions
			float delta_y = targety - myy; // difference in y positions

			// add slight randomness for fairness
			delta_x += rand() % 50 + 0;
			delta_y += rand() % 50 + 0;

			// normalize vector
			float length = sqrt(delta_x * delta_x + delta_y * delta_y);
			delta_x /= length;
			delta_y /= length;

			// movement vector
			Vector2 direction(delta_x, delta_y);

			if (direction.x != 0 && direction.y != 0)
				direction *= 0.707106781F;

			World::instance()->Shoot(this->GetPosition() + Vector2(xPos, 0.F), this->GetSide(), direction);
		}

		// next random interval time to shoot the player
		_nextTime = (int)(HAPI.GetTime() / 1000.F + rand() % 4 + 2); // 2 to 4 seconds
	}

	// If there is a path avaliable then we want the drone to follow it
	if (_path.size() <= 0 || _currPathIdx <= -1)
		return;

	float posX = _position.x + Visualiser::instance()->GetSprite(_graphicsID)->GetFrameWidth() / 2.F;
	float posY = _position.y + Visualiser::instance()->GetSprite(_graphicsID)->GetFrameHeight() / 2.F;
	
	// point to go to
	float pathX = _path[_currPathIdx].x + Visualiser::instance()->GetSprite(_graphicsID)->GetFrameWidth() / 2.F;
	float pathY = _path[_currPathIdx].y + Visualiser::instance()->GetSprite(_graphicsID)->GetFrameHeight() / 2.F;

	// if the drone is at the point move to the next
	if (posX >= pathX - 1.F
		&& posX <= pathX + 1.F
		&& posY >= pathY - 1.F
		&& posY <= pathY + 1.F)
	{
		_currPathIdx++;
	
		// Loop around if at the end
		if (_currPathIdx >= (int)_path.size())
		{
			_currPathIdx = 0;
			_parent->ResetDrone(this);
		}

		return;
	}


	// Move towards the point
	if (pathX < posX)
		Move(Direction::Left, _speed);
	else if(pathX > posX)
		Move(Direction::Right, _speed);

	if (pathY < posY)
		Move(Direction::Up, _speed);
	else if (pathY > posY)
		Move(Direction::Down, _speed);
}

void Drone::OnEndAnimation()
{
}

void Drone::OnDeath()
{
	AI::OnDeath();

	// reset back to original state for reuse
	_speed = 1.F;
	_health = 1;
	_path.clear();
	_currPathIdx = -1;

	srand((unsigned int)time(NULL));
	_nextTime = (int)(HAPI.GetTime() / 1000.F + rand() % 5 + 3); // 3 to 5 seconds
}