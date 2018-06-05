#include "DroneMaker.h"

#include "Visualiser.h" 
#include "World.h"
#include "Texture.h" // Acess texture information
#include "Vector2.h"
#include "Sprite.h"
#include "EnemyWaves.h"
#include "KamikazeDrone.h"
#include <ctime>


DroneMaker::DroneMaker(const std::string& uniqueGraphicsID, Texture& texture, Vector2 initialPosition)
	: DroneMaker::AI(uniqueGraphicsID, texture, initialPosition, 1, 4)
{
	_speed = 5.F;

	srand((unsigned int)time(NULL));

	// Spawn a drone basically straight away so that the play knows what this entity does.
	_nextTime = (int)(HAPI.GetTime() / 1000.F + rand() % 2 + 1);
}

DroneMaker::~DroneMaker()
{
}


void DroneMaker::Update()
{
	if (!_isVisible)
		return;

	// Spawn a drone when the time is right
	if (HAPI.GetTime() / 1000.F >= _nextTime)
	{
		KamikazeDrone* kam = World::instance()->AddKamikazeDrone(_position);

		// If there is a wave add the new drone to it
		if (_wave != nullptr)
		{
			kam->SetWave(_wave);
			_wave->_entites.push_back(kam);
		}

		_nextTime = (int)(HAPI.GetTime() / 1000.F + rand() % 7 + 4); // 4 to 7 seconds
	}

	// Left/Right movement (to the edges)
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

void DroneMaker::OnEndAnimation()
{
}

