#include "Visualiser.h"

#include "Texture.h" // To use and draw textures
#include "Sprite.h" // To draw sprites
#include "Rectangle.h"

// -----------------------
// Gabriel Lewis Q5094111
// -----------------------

Visualiser::~Visualiser()
{

	if(HAPI.GetControllerData(0).isAttached == true)
		HAPI.SetControllerRumble(0, 0, 0);

	for (auto const& iter : _sprites)
		delete iter.second;

	delete _destRectangle;
}

bool Visualiser::Initialise(unsigned int screenWidth, unsigned int screenHeight)
{
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;

	// Attempt to create a window with the requested size
	// note: HAPI will downsize if the dimentions are too big
	if (!HAPI.Initialise(_screenWidth, _screenHeight))
		return false;

	_screen = HAPI.GetScreenPointer();

	_centerX = _screenWidth / 2.F;
	_centerY = _screenHeight / 2.F;

	_destRectangle = new Rectangle(0, _screenWidth, 0, _screenHeight);

	HAPI.SetShowFPS(true);

	return true;
}

void Visualiser::SetPixel(unsigned int x, unsigned int y, HAPI_TColour colour) const
{
	// Fast way of setting a pixel to a colour
	unsigned int offset = (x + y * _screenWidth) * 4; // usefull equation, remember it
	memcpy(&_screen[offset], &colour, sizeof(HAPI_TColour));
}

void Visualiser::DrawRectangle(unsigned int x, unsigned int y, unsigned int width, unsigned int height, HAPI_TColour colour) const
{
	 int left = (int)(x - width / 2.f);
	 int top = (int)(y - height / 2.f);

	// Set the pixels drawing from left to right in columns  (tested, havent noticed a significant difference either way)
	for (unsigned int i = left; i < left + width; i++)
		for (unsigned int j = top; j < top + height; j++)
			SetPixel(i, j, colour);
}

void Visualiser::ClearTo(unsigned int r, unsigned int g, unsigned int b) const
{
	ClearTo(HAPI_TColour(r, g, b));
}

void Visualiser::ClearTo(int greyscale) const
{
	// Fastes clear but greyscale
	memset(_screen, greyscale, _screenWidth * _screenHeight * 4);
}

void Visualiser::ClearTo(HAPI_TColour colour) const
{
	// Fastest with colour
	for (unsigned int i = 0; i < (_screenWidth * _screenHeight); i++)
		memcpy(&_screen[i * 4], &colour, sizeof(HAPI_TColour));
}

void Visualiser::Draw(Sprite& sprite, Vector2 position, float s, int currFrame, int customXOffset) const
{
	BlitAlpha(sprite.GetTexture()->GetData(), 
		(int)position.x, (int)position.y, *sprite.GetRectangle(), sprite.GetFrameWidth(), sprite.GetFrameHeight(), sprite.GetNumberFrames(), currFrame, sprite.GetRowFrames(), sprite.GetColumnFrames(), customXOffset); // Before: *sprite.GetRectangle()
}

void Visualiser::Draw(Texture& texture, Vector2 position, float s, int currFrame) const
{
	BlitSimple(texture.GetData(), texture.GetWidth(), texture.GetHeight(), (int)position.x, (int)position.y);
}

void Visualiser::Draw(const std::string& name, Vector2 position, float s, int currFrame, int customXOffset) const
{
	if (_sprites.find(name) == _sprites.end())
	{
		HAPI.UserMessage("Sprite '" + name + "' not found! (attempting to access it)", "Error");
		return;
	}
	

	Draw(*_sprites.at(name), position, s, currFrame, customXOffset);
}


void Visualiser::BlitSimple(BYTE* source, unsigned int width, unsigned int height, int xPos, int yPos) const
{
	// Create a new source rectangle without position
	Rectangle clippedRect(0, width, 0, height);

	// Transform the source rectangle into destination space
	clippedRect.Translate(xPos, yPos);

	// Off screen so doesnt need to be rendered
	if (clippedRect.CompletelyOutside(*_destRectangle))
		return;

	if (!clippedRect.CompletelyInside(*_destRectangle))
	{
		// Clip against the destination rectangle
		clippedRect.ClipTo(*_destRectangle);

		// Add the original source rectangle position to the new source rectangle
		clippedRect.Translate(xPos, yPos);
	}

	// Transform it back into source space
	clippedRect.Translate(-xPos, -yPos);

	// x and y position must be greater than 0
	if (xPos < 0)
		xPos = 0;

	if (yPos < 0)
		yPos = 0;


	unsigned int offset = (xPos + yPos * _screenWidth) * 4;

	BYTE* tempDestination = _screen + offset;
	BYTE* tempSource = source;

	for (int i = 0; i < clippedRect.GetHeight(); i++)
	{	
		// Copy a line over
		memcpy(tempDestination, tempSource, clippedRect.GetWidth() * 4);

		// Move source pointer to next line
		tempSource += width * 4;

		//Move destination pointer to next line
		tempDestination += _screenWidth * 4;
	}
}

void Visualiser::BlitAlpha(BYTE* source, int xPos, int yPos, Rectangle& sourceRect, int frameWidth, int frameHeight, int numFrames, int currFrame, int rowFrames, int columnFrames, int custXOffset) const
{
 	frameWidth += custXOffset;
	// Create a new source rectangle without position
	Rectangle clippedRect(0, frameWidth, 0, frameHeight); //clippedRect(0, sourceRect.GetWidth(), 0, sourceRect.GetHeight())

	// Transform the source rectangle into destination space
	clippedRect.Translate(xPos, yPos);

	// Off screen so doesnt need to be rendered
	if (clippedRect.CompletelyOutside(*_destRectangle))
		return;

	if (!clippedRect.CompletelyInside(*_destRectangle))
	{
		// Clip against the destination rectangle
		clippedRect.ClipTo(*_destRectangle);

		// Add the original source rectangle position to the new source rectangle
		clippedRect.Translate(sourceRect._left, sourceRect._top);
	}
	
	// Transform it back into source space
	clippedRect.Translate(-xPos, -yPos);

	// x and y position must be greater than 0
	if (xPos < 0)
		xPos = 0;

	if (yPos < 0)
		yPos = 0;
	
	unsigned int destOffset = (xPos + yPos * _screenWidth) * 4;
	BYTE* tempDestination = _screen + destOffset;



	// Go over x times (e.g. 16)
	// until find current frame, then we know what col and row
	// we are on
	int actualCol = 0;
	int actualRow = 0;
	int frameOn = 0;
	
	for (int y = 0; y < rowFrames; y++)
	{
		for (int x = 0; x < columnFrames; x++, frameOn++)
		{
			if (frameOn == currFrame)
			{
				actualCol = x;
				break;
			}
		}
		if (frameOn == currFrame)
		{
			actualRow = y;
			break;
		}
	}

	const unsigned int yOffset = (actualRow * frameHeight * sourceRect.GetWidth() )
								+ (clippedRect._top * sourceRect.GetWidth());
	const unsigned int xOffset = (actualCol * frameWidth) + clippedRect._left ;
	const unsigned int offset = (yOffset * 4+ xOffset * 4) ;

	BYTE* tempSource = source + offset;

	
	for (int y = 0; y < clippedRect.GetHeight(); y++)
	{
		for (int x = 0; x < clippedRect.GetWidth(); x++)
		{
			BYTE red = tempSource[0];
			BYTE green = tempSource[1];
			BYTE blue = tempSource[2];
			BYTE alpha = tempSource[3];

			// Ensure the pixel is visible (0 = transparent)
			if (alpha > 0)
			{
				// If the pixel is fully visible then...
				if (alpha == 255)
				{
					// ...copy the pixel over
					memcpy(tempDestination, tempSource, 4);
				}
				else
				{
					// Apply transparency 
					tempDestination[0] = tempDestination[0] + ((alpha*(red - tempDestination[0])) >> 8);
					tempDestination[1] = tempDestination[1] + ((alpha*(green - tempDestination[1])) >> 8);
					tempDestination[2] = tempDestination[2] + ((alpha*(blue - tempDestination[2])) >> 8);
				}
			}
			
			// Increase both source and destination by 1 pixel
			tempSource += 4;
			tempDestination += 4;
		}

		// Move to next line
		tempSource += (sourceRect.GetWidth() - clippedRect.GetWidth()) * 4;
		tempDestination += (_screenWidth - clippedRect.GetWidth()) * 4;
	}


}

void Visualiser::AddSprite(const std::string& texturePath, const std::string& name, int sheetRows, int sheetColumns)
{
	if (_sprites[name] != nullptr)
		delete _sprites[name];
	_sprites[name] = new Sprite(texturePath, sheetRows, sheetColumns);
}


void Visualiser::AddSprite(Texture& texture, const std::string& name, int sheetRows, int sheetColumns)
{
	if (_sprites[name] != nullptr)
		delete _sprites[name];
	_sprites[name] = new Sprite(texture, sheetRows, sheetColumns);
}


Sprite* Visualiser::GetSprite(const std::string& name) const
{
	if (_sprites.find(name) == _sprites.end())
	{
		HAPI.UserMessage("Sprite '" + name + "' not found! (attempting to access it)", "Error");
		return nullptr;
	}
	else
		return _sprites.at(name);
}
