#pragma once

#include <HAPI_lib.h> // Include the HAPI header to get access to all of HAPIs interfaces
using namespace HAPISPACE; // HAPI itself is wrapped in the HAPISPACE namespace

/*Stores texture data itself loaded in from HAPI*/
class Texture
{
public:
	Texture(const std::string &texturePath);
	~Texture();

	// Getter Functions
	inline BYTE* GetData() const { return _textureData; }
	inline unsigned int GetWidth() const { return _textureWidth; }
	inline unsigned int GetHeight() const { return _textureHeight; }

private:
	// To load a texture into memory. 
	//NOTE: path is relative to the working directory
	bool LoadTexture(const std::string &path, BYTE** data, unsigned int &width, unsigned int &height);

private:
	unsigned int _textureWidth = 0, _textureHeight = 0;
	BYTE* _textureData = nullptr;
};

