#pragma once

// -----------------------
// Gabriel Lewis Q5094111
// -----------------------

#include <string>
#include <unordered_map>

#include "Vector2.h"

#include <HAPI_lib.h> // Include the HAPI header to get access to all of HAPIs interfaces

using namespace HAPISPACE; // HAPI itself is wrapped in the HAPISPACE namespace

// forward declarations
class Sprite;
class Texture;
class Rectangle;

// To display and manipulate the screen itself, including drawing.
class Visualiser
{
	static Visualiser *s_instance;

public:

	inline Visualiser() {}
	~Visualiser();
	
	// Initialise the screen and HAPI at once. 
	// This is because HAPI is useless without a screen and the screen is useless without HAPI
	bool Initialise(unsigned int screenWidth, unsigned int screenHeight);

	// Set a specific pixel on the screen to a desired colour
	void SetPixel(unsigned int x, unsigned int y, HAPI_TColour colour) const;
	// Draw a rectangle on the screen, x & y being the middle of the rectangle.
	void DrawRectangle(unsigned int x, unsigned int y, unsigned int width, unsigned int height, HAPI_TColour colour) const;

	// Add a sprite to the map
	void AddSprite(const std::string& texturePath, const std::string& name, int sheetRows, int sheetColumns);
	void AddSprite(Texture& texture, const std::string& name, int sheetRows, int sheetColumns);

	// Helper Functions used to draw any sprite or texture appropriately.
	void Draw(const std::string& name, Vector2 position, float s, int currFrame = 0, int customXOffset = 0) const;
	void Draw(Sprite& sprite, Vector2 position, float s, int currFrame = 0, int customXOffset = 0) const; // using Alpha Blit
	void Draw(Texture& texture, Vector2 position, float s, int currFrame = 0) const; // uses Simple Blit

	// Clear the screen to any colour. Greyscale being the fastest.
	void ClearTo(unsigned int r, unsigned int g, unsigned int b) const;
	void ClearTo(int greyscale) const;
	void ClearTo(HAPI_TColour colour) const;

	
	// Getter functions
	inline unsigned int GetScreenWidth() const { return _screenWidth; }
	inline unsigned int GetScreenHeight() const { return _screenHeight; }
	inline float GetScreenCenterX() const { return _centerX; }
	inline float GetScreenCenterY() const { return _centerY; }
	inline BYTE* GetScreen() const { return _screen; }
	Sprite* GetSprite(const std::string& name) const;

	// Get the instance, if none exists then creat a new one
	inline static Visualiser *instance()
	{
		if (!s_instance)
			s_instance = new Visualiser;
		return s_instance;
	}

private:
	// Draw a texture to a desired position on the screen. 
	// "Simple" - For drawing textures without an alpha property. 
	void BlitSimple(BYTE* source, unsigned int width, unsigned int height, int xPos, int yPos) const;
	// "Alpha" - For drawing texture with an alpha property (for transparency).
	void BlitAlpha(BYTE* source, int xPos, int yPos, Rectangle& sourceRect, int frameWidth = 0, int frameHeight = 0, int numFrames = 1, int currFrame = 0, int rowFrames = 0, int columnFrames = 0
		, int custXOffset = 0) const;

private:
	BYTE* _screen = nullptr; 
	unsigned int _screenWidth;
	unsigned int _screenHeight;
	float _centerX; // center x of the screen
	float _centerY; // center y of the screen
	Rectangle* _destRectangle = nullptr;

	// stored sprites
	std::unordered_map<std::string, Sprite*> _sprites;
};
