#pragma once


#include "AI.h"

/*The drone maker moves across the screen from edge to edge.
It spawns a drone every so often (small random interval).
It has a standard amount of health.
*/
class DroneMaker : public AI
{
public:
	DroneMaker(const std::string& uniqueGraphicsID, Texture& texture, Vector2 initialPosition);
	~DroneMaker();

	// Overrides
	void Update() override final;
	void OnEndAnimation() override final;
	
private:
	// next time to spawn a drone
	int _nextTime = 0;
	// if going right or left (for movement)
	bool goingRight = true;
};

