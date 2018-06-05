#pragma once

// -----------------------
// Gabriel Lewis Q5094111
// -----------------------

#include <string>

// forward declarations
class Texture;
class Rectangle;

/*A Sprite is an object that can be seen on the screen.
It has a rectangle (that can be used for collision) and
a rectangle that represents one frame of an animation.*/
class Sprite
{
public:
	Sprite(const std::string &texturePath, int sheetRows, int sheetColumns);
	Sprite(Texture& texture, int sheetRows, int sheetColumns);
	~Sprite();

	// Getter Functions
	inline Texture* GetTexture() const { return _texture; }
	inline Rectangle* GetRectangle() const { return _rectangle; }
	inline Rectangle* GetFrameRectangle() const { return _frameRect; }
	inline int GetFrameWidth() const { return _frameWidth; }
	inline int GetFrameHeight() const { return _frameHeight; }
	inline int GetNumberFrames() const { return _numFrames; }
	inline int GetRowFrames() const { return _rowFrames; }
	inline int GetColumnFrames() const { return _columnFrames; }

private:
	// shared constructor
	void Constructor();

private:
	// Texture itself
	Texture* _texture = nullptr;
	// Rectangle surrounding the texture
	Rectangle* _rectangle = nullptr;

	// Animation frame details
	int _frameWidth = 0;
	int _frameHeight = 0;
	int _numFrames = 0;
	Rectangle* _frameRect = nullptr;
	int _rowFrames = 1;
	int _columnFrames = 8;

	// Required because if the texture is shared, we only want to delete it once.
	// this happens in the World where the shared textures are created, so in that
	// case we dont want to delete it.
	bool _shouldDeleteTex = true;
};



