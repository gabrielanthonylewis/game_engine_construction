#pragma once

#include <vector>
#include "Vector2.h"
#include "AI.h"

// Forward declarations
class Drone;
class Wave;
class Texture;

/*A drone path is in charge of spawning drones on a path
until no more are needed to be spawned.
*/
class DronePath : public AI
{
public:
	DronePath(const std::string& uniqueGraphicsID, Texture& texture, Vector2 initialPosition, int numDrones);
	~DronePath();

	// Spawn and setup a drone so that it is 
	// ready to follow its path
	void SpawnDrone();

	// Reset a Drone back to the intial position
	void ResetDrone(Drone* drone);

	// Overrides
	void Update() override final;
	void OnEndAnimation() override final;

	// Getter functions
	inline int GetCurrentDrones() const { return _currDroneCount; }
	inline int GetTargetDrones() const { return _targetDroneCount; }

private:
	// current drones spawned
	int _currDroneCount = 0;
	// target drones to spawn
	int _targetDroneCount = 0;

	// a bunch of steps/instructions describing movement
	// between points.
	std::vector<Vector2> steps;

	// intial position of the first point
	Vector2 _firstPos;
	
	/*small interval between spawning dronesto prevent
	overlap.*/
	float nextTime = 1000.F;
};

