/*
	HAPI Start
	----------
	This solution contains an already set up HAPI project and this main file
	
	The directory structure and main files are:

	HAPI_Start - contains the Visual Studio solution file (.sln)
		HAPI_APP - contains the Visual Studio HAPI_APP project file (.vcxproj) and source code
			HAPI - the directory with all the HAPI library files
*/

// -----------------------
// Gabriel Lewis Q5094111
// -----------------------

#include "source\Visualiser.h" // To display and manipulate the screen itself
#include "source\World.h"
#include "source\ObjectPool.h"
#include "source\SoundSystem.h"

#include <HAPI_lib.h> // Include the HAPI header to get access to all of HAPIs interfaces
using namespace HAPISPACE; // HAPI itself is wrapped in the HAPISPACE namespace


// The singletons 
Visualiser *Visualiser::s_instance = 0;
ObjectPool *ObjectPool::s_instance = 0;
World *World::s_instance = 0;
SoundSystem *SoundSystem::s_instance = 0;

// Every HAPI program has a HAPI_Main as an entry point
// When this function exits the program will close down
void HAPI_Main()
{	
	// If initialisation fails then an error will be thrown,
	// otherwise, the world will run.
	if (!World::instance()->LoadLevel(700, 760))
		HAPI.UserMessage("Level failed to load!", "Error");
	else
		World::instance()->Run();

	// cleanup singletons
	delete Visualiser::instance();
	delete ObjectPool::instance();
	delete World::instance();
	delete SoundSystem::instance();

	// Ensure that there is no controller rumble
	if (HAPI.GetControllerData(0).isAttached == true)
		HAPI.SetControllerRumble(0, 0, 0);
}
