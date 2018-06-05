#include "Texture.h"

Texture::Texture(const std::string &texturePath)
{
	LoadTexture(texturePath, &_textureData, _textureWidth, _textureHeight);
}

Texture::~Texture()
{
	delete _textureData;
}

bool Texture::LoadTexture(const std::string &path, BYTE** data, unsigned int &width, unsigned int &height)
{
	if (!HAPI.LoadTexture(path, data, width, height))
	{
		// Could replace texture with pink one, or draw pink pixels
		HAPI.UserMessage("Texture not found, attempting to replace with a pink texture...", "Error");

		if (!HAPI.LoadTexture("images//pink.png", data, width, height))
		{
			HAPI.UserMessage("Pink texture not found", "Error");
			return false;
		}

		return false;
	}
	return true;
}