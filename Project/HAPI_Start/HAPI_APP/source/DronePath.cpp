#include "DronePath.h"

#include "World.h"
#include "Vector2.h"
#include "Drone.h"
#include "ObjectPool.h"
#include "EnemyWaves.h"

#include <HAPI_lib.h> // Include the HAPI header to get access to all of HAPIs interfaces
using namespace HAPISPACE; // HAPI itself is wrapped in the HAPISPACE namespace


DronePath::DronePath(const std::string& uniqueGraphicsID, Texture& texture, Vector2 initialPosition, int numDrones)
	: DronePath::AI(uniqueGraphicsID, texture, Vector2(-100,-100), 0, 0), _targetDroneCount(numDrones), _firstPos(initialPosition)
{
	// Choose one of the passes (50/50 choices)
	int a=  rand() % 100;
	if (a <= 50)
	{
		steps.push_back(Vector2(-200, 200)); // left 200, down 200
		steps.push_back(Vector2(100, 100));
		steps.push_back(Vector2(-250, 200));
		steps.push_back(Vector2(-50, 0)); // off-screen to loop
	}
	else
	{
		steps.push_back(Vector2(300, 200)); // left 300, down 200
		steps.push_back(Vector2(-100, 100));
		steps.push_back(Vector2(250, 250)); // off-screen to loop	
	}
}

DronePath::~DronePath()
{
}


void DronePath::ResetDrone(Drone* drone)
{
	// Put the drone back to the first position
	drone->SetInitialPosition(_firstPos);
	drone->SetPosition(_firstPos);
	
	_currDroneCount--;
}

void DronePath::SpawnDrone()
{
	if (!_isVisible)
		return;

	// Generate target path
	std::vector<Vector2> targetPath;
	Vector2 pathV = _firstPos;
	for (size_t i = 0; i < steps.size(); i++)
	{
		pathV = Vector2(pathV.x + steps[i].x, pathV.y + steps[i].y);
		targetPath.push_back(pathV);
	}

	// Setup the drone
	Drone* nextDrone = ObjectPool::instance()->GetFirstFreeEnemy();
	if (nextDrone != nullptr)
	{
		nextDrone->SetParent(this);
		nextDrone->SetPosition(_firstPos);
		nextDrone->SetInitialPosition(_firstPos);
		nextDrone->SetVisibility(true);
		nextDrone->SetCurPahIdx(0);
		nextDrone->SetPath(targetPath);

		if (_wave != nullptr)
		{
			nextDrone->SetWave(_wave);
			_wave->_entites.push_back(nextDrone);
		}
	}

	_currDroneCount++;
}

void DronePath::Update()
{
	if (!_isVisible)
		return;

	/* If we dont need any more drones,
	 destroy the "drone generator" (this object)
	 and return.*/
 	if (_currDroneCount >= _targetDroneCount)
	{
		// Dont TakeDamage() because we dont want to run the AI 
		// OnDeath() function
		
		// Increase the amount of entities killed in the wave
		// if all of the enemies are killed go to the next wave 
		if (_wave != nullptr)
		{
			if (_wave->KillEntity() == true)
				World::instance()->WaveComplete();
		}

		_isVisible = false;
		return;
	}

	/* Spawn a drone after the small interval.
	 A small interval is required otherwise the 
	 drones will overlap.*/
	if (HAPI.GetTime() > nextTime)
	{
		SpawnDrone();
		nextTime = HAPI.GetTime() + 1000.F;
	}
}

void DronePath::OnEndAnimation()
{
}