#include "StarField.h"

#include "Visualiser.h" // To access the screen to draw the stars
#include <time.h> // Include the time header to get a unique seed with srand()

// -----------------------
// Gabriel Lewis Q5094111
// -----------------------

StarField::StarField(unsigned int numStars, HAPI_TColour colour, unsigned int eyeDistance) 
	: _starColour(colour), _eyeDistance(eyeDistance), keyboardData(HAPI.GetKeyboardData())
{
	srand((unsigned int)time(NULL));

	_stars.reserve(numStars);

	// Spawn each star and add it to the vector
	for (unsigned int i = 0; i < numStars; i++)
	{
		Star* newStar = new Star();
		SpawnStar(*newStar, 0, 0, 0);
		_stars.push_back(newStar);
	}
	
}

StarField::~StarField()
{
	for (size_t i = 0; i < _stars.size(); i++)
	{
		delete _stars[i];
	}
}

void StarField::SpawnStar(Star& star, unsigned int minX, unsigned int minY, unsigned int minZ)
{
	// Formula: min + (rand() % (max - min + 1) 
	star.x = minX + (float)(rand() % (Visualiser::instance()->GetScreenWidth() - minX + 1));
	star.y = minY + (float)(rand() % (Visualiser::instance()->GetScreenHeight() - minY + 1));
	star.z = minZ + (float)(rand() % (500 - minZ + 1));
}

void StarField::Tick()
{
	UpdateVelocity();
	UpdateEyeDistance();

	for (unsigned int i = 0; i < _stars.size(); i++)
	{
		// If the star is out of viewing distance..
		if (_stars[i]->z <= 0)
		{
			// ..respawn it as if it was a new star.
			SpawnStar(*_stars[i], 0, 0, 500);
			continue;
		}

		// Calculate screen position of the star
		int screenX = (int)(((_eyeDistance * (_stars[i]->x - Visualiser::instance()->GetScreenCenterX()))
			/ (_eyeDistance + _stars[i]->z)) + Visualiser::instance()->GetScreenCenterX());

		int screenY = (int)(((_eyeDistance * (_stars[i]->y - Visualiser::instance()->GetScreenCenterY()))
			/ (_eyeDistance + _stars[i]->z)) + Visualiser::instance()->GetScreenCenterY());

		// The closer the star, the bigger.
		int sizeXY = 1;
		if (_stars[i]->z <= 35.F)
			sizeXY = 3;
		else if (_stars[i]->z <= 70.F)
			sizeXY = 2;

		// Draw star
		Visualiser::instance()->DrawRectangle(screenX, screenY, sizeXY, sizeXY, _starColour);

		// Move star closer to the screen.
		_stars[i]->z -= _velocity;
	}
}

void StarField::ShowUI()
{
	// Display the variable properties and controls
	HAPI.RenderText(0, 35, HAPI_TColour(255, 255, 255), "(Down/Up) Velocity: " + std::to_string(_velocity), 12);
	HAPI.RenderText(0, 50, HAPI_TColour(255, 255, 255), "(Left/Right) Eye Distance: " + std::to_string(_eyeDistance), 12);
}

void StarField::UpdateVelocity()
{
	// Speed up
	if (keyboardData.scanCode[HK_UP])
	{
		_velocity += 0.01F;
		//if (_velocity >= 5.F)
		//	_velocity = 5.F;
	}
	// Slow down
	if (keyboardData.scanCode[HK_DOWN])
	{
		_velocity -= 0.01F;
		//if (_velocity <= 0.F)
		//	_velocity = 0.F;
	}
}

void StarField::UpdateEyeDistance()
{
	// Increase
	if (keyboardData.scanCode[HK_RIGHT])
		_eyeDistance += 1;

	//Decrease
	if (keyboardData.scanCode[HK_LEFT])
	{
		if (_eyeDistance >= 1)
			_eyeDistance -= 1;
	}
}
