#pragma once

#include "AI.h"

#include <vector>

//Forward Declarations
class DronePath;

/*This drone follows a set path specified beforehand. 
The drone is weak and standard speed but they are powerful
in numbers due to their accuracy and rate of fire. The 
class encapsulates this functionallity.*/
class Drone : public AI
{
public:
	Drone(const std::string& uniqueGraphicsID, Texture& texture, Vector2 initialPosition);
	~Drone();

	// Overrides
	void Update() override final;
	void OnEndAnimation() override final;
	
	// override standard OnDeath function (while still calling it)
	// to reset variables for reuse.
	void OnDeath();

	// Setter functions
	inline void SetPath(std::vector<Vector2> path) { _path = path; _currPathIdx = 0; }
	inline void SetCurPahIdx(int val) { _currPathIdx = val; }
	inline void SetInitialPosition(Vector2 pos) { _initialPos = pos; }
	inline void SetParent(DronePath* parent) { _parent = parent; }

private:

	// current path point
	int _currPathIdx = 0;
	// stores a path of instructions (e.g. go left 100, down 0)
	std::vector<Vector2> _path;

	// initial spawn position
	Vector2 _initialPos;
	int _nextTime = 0;

	// reference to the drone path itself 
	DronePath* _parent = nullptr;
};


