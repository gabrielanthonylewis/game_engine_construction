#include "Sprite.h"

#include "Texture.h"// To use a texture
#include "Rectangle.h"

Sprite::Sprite(const std::string &texturePath, int sheetRows, int sheetColumns)
{
	_texture = new Texture(texturePath);
	_rowFrames = sheetRows;
	_columnFrames = sheetColumns;
	Constructor();
}


Sprite::Sprite(Texture& texture, int sheetRows, int sheetColumns)
{
	_shouldDeleteTex = false;

	_texture = &texture;
	_rowFrames = sheetRows;
	_columnFrames = sheetColumns;
	Constructor();
}

void Sprite::Constructor()
{
	_rectangle = new Rectangle(0, _texture->GetWidth(), 0, _texture->GetHeight());

	if(_columnFrames > 0)
	_frameWidth = _texture->GetWidth() / _columnFrames;
	if (_rowFrames > 0)
	_frameHeight = _texture->GetHeight() / _rowFrames;
	_numFrames = _rowFrames * _columnFrames;

	_frameRect = new Rectangle(0, _frameWidth, 0, _frameHeight);
}

Sprite::~Sprite()
{
	// clean up
	if(_shouldDeleteTex)
		delete _texture;

	delete _rectangle;
	delete _frameRect;
}

