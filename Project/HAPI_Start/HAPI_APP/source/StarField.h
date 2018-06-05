#pragma once

// -----------------------
// Gabriel Lewis Q5094111
// -----------------------

#include <vector>
#include <HAPI_lib.h> // Include the HAPI header to get access to all of HAPIs interfaces

using namespace HAPISPACE; // HAPI itself is wrapped in the HAPISPACE namespace

// A Star is an object with 3d coordinates.
//----------
// An improvement may be adding a colour to the star so each star can have a personalised colour.
// Perhaps even converting the struct to a class which functions such as "Draw()"
//---------
struct Star
{
public:
	float x = 0.F;
	float y = 0.F;
	float z = 0.F;
};

// An entire star field containing a desired number of stars.
class StarField
{
public:
	StarField(unsigned int numStars, HAPI_TColour colour = HAPI_TColour(255, 255, 255), unsigned int eyeDistance = 100);
	~StarField();

	// The tick function is updates the star field by a frame 
	void Tick();

	void ShowUI();

private:
	// --- Functions ---

	// Checks for keyboard input and then updates the velocity accordingly
	void UpdateVelocity();
	// Checks for keyboard input and then updates the eye distance accordingly
	void UpdateEyeDistance();

	// Sets a stars position to a random point depending on the min values inputted.
	void SpawnStar(Star& star, unsigned int minX, unsigned int minY, unsigned int minZ);

	// --- Variables ---

	const HAPI_TKeyboardData &keyboardData;
	std::vector<Star*> _stars;
	unsigned int _eyeDistance = 100;
	float _velocity = 0.5F;
	HAPI_TColour _starColour;
};


