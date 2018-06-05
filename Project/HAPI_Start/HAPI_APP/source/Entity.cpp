#include "Entity.h"

// -----------------------
// Gabriel Lewis Q5094111
// -----------------------

#include "Visualiser.h" // To draw the entity
#include "Rectangle.h"
#include "Sprite.h"
#include "World.h"


void Entity::Render(float s, int frameNumber)
{
	// Lerp
	Vector2 interp = _oldPosition + (_position - _oldPosition) * s;

	// In our case the HUD health element is the only sprite
	// that is rendered different. The health sprite decreases
	// as health is reduced.
	if (_graphicsID == "HUDhealth")
	{
		// Calculate how much of the sprite should be visible
		int diff = World::instance()->GetPlayerInitialHealth() - World::instance()->GetPlayerHealth();
		if (diff < 0)
			diff = 0;
		float multi = diff / (float)World::instance()->GetPlayerInitialHealth();
		
		Visualiser::instance()->Draw(_graphicsID, interp, s, 1, -(int)(multi * (float)Visualiser::instance()->GetSprite(_graphicsID)->GetFrameRectangle()->GetWidth()));
	}
	else
		Visualiser::instance()->Draw(_graphicsID, interp, s, _currFrame);

	_oldPosition = _position;
}


bool Entity::CheckForCollision(const Entity& other)
{
	if (other.GetVisibility() == false || _isVisible == false)
  		return false;

	Rectangle rect = *Visualiser::instance()->GetSprite(_graphicsID)->GetFrameRectangle();
	Rectangle otherRect = *Visualiser::instance()->GetSprite(other.GetGraphicsID())->GetFrameRectangle();
	
	//reduce rect by 10% due to inaccuracies when dealing with rectangles
	rect *= 0.9F;
	otherRect *= 0.9F;

	// translate to check if there is a collision
	rect.Translate((int)_position.x, (int)_position.y);
	otherRect.Translate((int)other._position.x, (int)other._position.y);

	// Calculate result
	bool result = !rect.CompletelyOutside(otherRect);

	// pull them back away
	rect.Translate((int)-_position.x, (int)-_position.y);
	otherRect.Translate((int)-other._position.x, (int)-other._position.y);

	return result;
}

void Entity::NextFrame()
{
	_currFrame++;

	if (_currFrame >= Visualiser::instance()->GetSprite(_graphicsID)->GetNumberFrames() )
	{
		_currFrame = 0;
		OnEndAnimation();		
	}
}

void Entity::Animate()
{
	if (_isVisible == false)
		return;

	if (_animationFrameRate <= 0)
		return;

	// Go to the next animation frame when the time is right.
	// (this stops the animation going too fast)
	unsigned long timePassedANIM = HAPI.GetTime() - _lastTimeUpdatedANIM;
	if (timePassedANIM >= _animationFrameRate)
	{
		NextFrame();
		_lastTimeUpdatedANIM = HAPI.GetTime();
	}
}

void Entity::InitialiseTilePosition()
{
	_tileX = (int)(_position.x / (float)32);
	_tileY = (int)(_position.y / (float)32);
}

void Entity::Move(Direction direction, float speed)
{
	_oldPosition = _position;

	// The use of these is to get the tile from the around the middle
	// of the sprite so that the pathfinding will ALWAYS collide with the
	// player after pathfinding
	float spriteWidth = Visualiser::instance()->GetSprite(_graphicsID)->GetFrameRectangle()->GetWidth() / 2.f;
	float spriteHeight = Visualiser::instance()->GetSprite(_graphicsID)->GetFrameRectangle()->GetHeight() / 2.f;

	// Update physical position and update the current Tile 
	// the entity is on.
	switch (direction)
	{
	case Up:
		_position.y -= speed;

		if (_position.y - spriteHeight <= _tileY * 32 - 32)
		{
			float distTravelled = (_position.y - _tileY * 32);
			_tileY += (int)(distTravelled / (float)32);
		}

		break;
	case Down:
		_position.y += speed;

		if (_position.y + spriteHeight >= _tileY * 32 - 32)
		{
			float distTravelled = (_position.y - _tileY * 32);
			_tileY += (int)(distTravelled / (float)32);
		}

		break;
	case Left:
		_position.x -= speed;

		if (_position.x - spriteWidth <= _tileX * 32 - 32)
		{
			float distTravelled = (_position.x - _tileX * 32);
			_tileX += (int)(distTravelled / (float)32);
		}

		break;
	case Right:
		_position.x += speed;

		if (_position.x + spriteWidth >= _tileX * 32 + 32)
		{
			float distTravelled = (_position.x - _tileX * 32);
			_tileX += (int)(distTravelled / (float)32);
		}

		break;
	default:
		break;
	}

	
}

void Entity::Move(float x, float y, float speed)
{
	_oldPosition = _position;

	_position.x += x / speed;
	_position.y += y / speed;
}