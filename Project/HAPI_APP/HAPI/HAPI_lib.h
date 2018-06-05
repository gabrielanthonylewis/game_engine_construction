#pragma once
/*
HAPI_lib.h
All the includes required to use the Hardware API (HAPI).

For full details on using the library please see the online documentation (CTRL-Click to open):
https://scm-intranet.tees.ac.uk/users/u0018197/HAPI/HAPI-Reference/hapi-reference.html

Last modified by Keith Ditchburn: 19th November 2016

Version 2.10 - 19/11/2016
- Added XML loading, creating and saving via a seperate CHapiXML type	
	- See examples for how to use
	- Note: built using pugixml (http://pugixml.org/)

Version 2.03 - 12/11/2016
- FileDialog call now starts in the working directory (rather than Documents)
- FileDialog now adds default extension to filename
- Removed the maximum limit set on text entries - now dynamically resises as required
- ALT key now correctly releases

Version 2.02 - 30/10/2016
- screen pointer is now guaranteed to be aligned to a 16 byte boundary (128 bit)

Version 2.01 - 19/10/2016
- linked to Windows 7 version of XInput so HAPI can be used on Windows 7

Older version changes are listed in the ReadMe.txt file
*/

// Standard library types required by HAPI
#include <string>
#include <vector>

// Key and controller codes header
#include "HAPI_InputCodes.h"

// Enums and structs used with HAPI
#include "HAPI_Types.h"

// All of HAPI is inside the HAPISPACE namespace
namespace HAPISPACE {

	// Public HAPI interface
	class IHapi
	{
	public:
		// Initialise HAPI, pass in the required screen size and any init flags
		// Note: width and height values may be altered to a legal size by HAPI
		virtual bool Initialise(unsigned int &width, unsigned int &height, const std::string &title=std::string(), unsigned int flags = eDefaults) = 0;

		// Causes HAPI to close the window and return false in the next update call
		virtual bool Close() = 0;

		// Tells HAPI to display (or not) the frames per second at 0,0 (top left)
		virtual bool SetShowFPS(bool set) = 0;

		// As above but allows the screen position to be specified
		virtual bool SetShowFPS(bool set, unsigned int x, unsigned int y) = 0;

		// Tells HAPI to display or not the cursor (mouse pointer)
		virtual bool SetShowCursor(bool set) = 0;

		// Update HAPI, must be called each game loop. Returns false when user closes the window or an error occurs
		virtual bool Update() = 0;

		// Returns a pointer to the screen data. There are 4 bytes per pixel, in RGBA byte order
		// On error returns nullptr, use HAPI_GetLastError to determine error reason or look in output pane
		virtual BYTE* GetScreenPointer() const = 0;

		// Returns a structure with the current mouse state
		virtual const HAPI_TMouseData& GetMouseData() const = 0;

		// Returns a structure with the current keyboard state
		virtual const HAPI_TKeyboardData& GetKeyboardData() const = 0;

		// Returns the maximum number of controller devices that could be installed (not how many actually are)
		virtual unsigned int GetMaxControllers() const = 0;

		// Returns a structure with the specified controller index (up to GetMaxControllers) state
		// Note: you must check that the isAttached member is true
		virtual const HAPI_TControllerData& GetControllerData(unsigned int controller) const = 0;

		// Sets the rumble motor speeds for the specified controller
		virtual bool SetControllerRumble(unsigned int controller, WORD leftMotor, WORD rightMotor) const = 0;

		// Load a sound file, most formats are supported:
		// ogg, wav, flac, aiff, au, raw, paf, svx, nist, voc, ircam, w64, mat4, mat5 pvf, htk, sds, avr, sd2, caf, wve, mpc2k, rf64.
		virtual bool LoadSound(const std::string &filename) = 0;

		// Note: there is a hardware limit on how many sounds can be playing at once (1024 on PC but 32 on mobiles)
		virtual bool PlaySound(const std::string &filename) = 0;

		// As above but also allows volume, pan etc. to be changed from the defaults
		virtual bool PlaySound(const std::string &filename, const HAPI_TSoundOptions &options) = 0;

		// As above but also returns instanceId (for use when querying sound status)
		virtual bool PlaySound(const std::string &filename, const HAPI_TSoundOptions &options, unsigned int &instanceId) = 0;

		// You can change things like pitch, volume and loop as the sound is playing
		virtual bool ChangePlayingSoundOptions(unsigned int &soundInstanceId, const HAPI_TSoundOptions &options) = 0;

		// Stop a sound early. Use the id retrieved from PlaySound
		virtual bool StopSound(unsigned int soundInstanceId, bool togglePause = false) = 0;

		// Retrieve info on a sound instance e.g. to see if it is paused, finished etc.
		virtual const HAPI_MediaStatus GetSoundStatus(unsigned int soundInstanceId, float &playbackPosition) const = 0;

		// Allows access to the raw sample data of a sound
		// Note: to see how to interpret the raw data use the values from HAPI_TSoundFormat
		virtual bool GetSoundRawData(const std::string& filename, std::vector<SWORD>& rawData, HAPI_TSoundFormat &soundInfo) const = 0;

		// Allows changing a sound's sample data. Note: if sound is currently playing it will be stopped.
		virtual bool SetSoundRawData(const std::string& filename, const std::vector<SWORD>& rawData, const HAPI_TSoundFormat &soundInfo) = 0;

		// Allows a sound to be created from scratch.
		virtual bool CreateSound(const std::string& name, const std::vector<SWORD>& rawData, const HAPI_TSoundFormat &soundInfo) = 0;

		// Saves a sound under a new filename
		// Supported formats (derived from filename extension) :
		// ogg, wav, flac, aiff, au, raw, paf, svx, nist, voc, ircam, w64, mat4, mat5 pvf, htk, sds, avr, sd2, caf, wve, mpc2k, rf64.
		virtual bool SaveSound(const std::string& name, const std::string& newFilename) const = 0;

		// Plays streamed video / music using default options
		virtual bool PlayStreamedMedia(const std::string &filename) = 0;

		// Plays streamed media and allows playback options to be set and instance obtained
		virtual bool PlayStreamedMedia(const std::string &filename, const HAPI_TStreamedMediaOptions& options, unsigned int &instanceId) = 0;

		// Restarts streamed media that has already been played (more efficient than reloading it)
		virtual bool PlayStreamedMedia(unsigned int instanceId) = 0;

		// Stops or pauses / unpauses streaming media started via PlayStreamedMedia
		virtual bool StopStreamedMedia(unsigned int instanceId, bool togglePause = false) const = 0;

		// Returns info about the media e.g. like current position
		virtual const HAPI_TStreamedMediaInfo& GetStreamedMediaInfo(unsigned int instanceId) const = 0;

		// If a HAPI call fails this can be used to retrieve the error code and system the error occurred in
		// (see HAPI_ErrorCodes enum above and online documentation for more details on errors)
		virtual HAPI_ErrorCodes GetLastError(HAPI_System &system) const = 0;

		// Returns the time since HAPI was initialised in milliseconds
		virtual DWORD GetTime() const = 0;

		// Allows the font to be changed. This version uses fonts registered with the OS
		virtual bool ChangeFont(const std::string &fontName) = 0;

		// Allows the font to be changed. This version allows it to be loaded from a file.
		virtual bool ChangeFontDirect(const std::string &fontFilename) = 0;

		// Allows your own debug text to be displayed in the output pane of Visual Studio and the log
		virtual void DebugText(const std::string &str) = 0;

		// Allows text to be drawn on the screen at a position and with a specified fill colour with the current font
		// Text size and flags for bold, italic etc. are optional
		virtual bool RenderText(unsigned int x, unsigned int y, const HAPI_TColour &fillColour, const std::string& text,
			unsigned int textSize=12,unsigned int styleFlags=eRegular) = 0;

		// As above but allows you to specify another colour to act as an outline to the text and how thick it should be
		// DISABLED: due to memory leaks in SFML outlining is temporarily disabled
		virtual bool RenderText(unsigned int x, unsigned int y, const HAPI_TColour &fillColour, const HAPI_TColour &outlineColour,
			float outlineThickness, const std::string& text,
			unsigned int textSize = 12, unsigned int styleFlags = eRegular) = 0;

		// Loads a texture, supports most PC formats. There are 4 bytes per texel in RGBA format
		// Caller is responsible for deleting the allocated data (use delete[])
		// Also returns width and height of texture
		virtual bool LoadTexture(const std::string &filename, BYTE** data, unsigned int &width, unsigned int &height) = 0;

		// Saves the data in rawImageData to a file
		// Width and height must be correct and assumes 4 bytes per pixel.
		// If no path information is in the filename it is saved to the working directory
		// supported formats are bmp, png, tga and jpg - format is derived from filename extension
		virtual bool SaveImage(BYTE *rawImageData, unsigned int width, unsigned int height, const std::string &filename) const = 0;

		// Display a message box to the user
		virtual bool UserMessage(const std::string &text, const std::string& title,
			HAPI_ButtonType buttonType = HAPI_ButtonType::eButtonTypeOk, HAPI_UserResponse *userResponse = nullptr) = 0;

		// Display a file open or file save dialog
		virtual bool FileDialog(const std::vector<HAPI_TFileDialogFilters>& filters, bool saveAsDialog,
			std::string &filename, HAPI_UserResponse &response) = 0;

		// Allows the icon shown in the window and task bar to be changed from the default HAPI logo
		virtual bool SetIcon(BYTE *rawImageData, unsigned int width, unsigned int height) const = 0;
	};
}

// Returns the HAPI instance
HAPISPACE::IHapi& GetHAPI();

// the HAPI instance. A shortcut to the above
#define HAPI GetHAPI()
