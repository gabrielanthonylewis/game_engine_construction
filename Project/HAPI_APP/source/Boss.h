#pragma once

#include "AI.h"

/*The boss entity is a slow enemy but has a huge amount
of health. The player will have to dodge the burst fire.
The script just holds the functionallity to move from edge
to edge and shoot the burst after random intervals of time.*/
class Boss : public AI
{
public:
	Boss(const std::string& uniqueGraphicsID, Texture& texture, Vector2 initialPosition);
	~Boss();

	// Overrides
	void Update() override final;
	void OnEndAnimation() override final;

private:
	// If true, move right, otherwise move left.
	bool goingRight = true;
	// The next time the boss will shoot its bullets
	int _nextTime = 0;
};

