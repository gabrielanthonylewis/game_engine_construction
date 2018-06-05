#include "Player.h"

// -----------------------
// Gabriel Lewis Q5094111
// -----------------------

#include "Visualiser.h" 
#include "ObjectPool.h"
#include "Explosion.h"
#include "Sprite.h" // Access sprite information
#include "Rectangle.h"
#include "Vector2.h"


Player::Player(const std::string& uniqueGraphicsID, Texture& texture, Vector2 initialPosition, float speed)
	: _keyboardData(HAPI.GetKeyboardData()), _controllerData(HAPI.GetControllerData(0))
{
	// Initialise variables
	_speed = speed;
	_position = initialPosition;
	_oldPosition = initialPosition;
	_graphicsID = uniqueGraphicsID;
	Visualiser::instance()->AddSprite(texture, _graphicsID, 1, 4); 
	SetSide(ESide::ePlayer);
	_isPlayer = true;
	_score = 0;

	InitialiseTilePosition();
}

void Player::CheckForInput()
{
	if (_isVisible == false)
		return;

	// ---Keyboard input---
	// Reduce speed if going diagonal
	float tempSpeed = _speed;
	int keyCount = 0;
	if (_keyboardData.scanCode[HK_DOWN])
		keyCount++;
	if (_keyboardData.scanCode[HK_UP])
		keyCount++;
	if (_keyboardData.scanCode[HK_RIGHT])
		keyCount++;
	if (_keyboardData.scanCode[HK_LEFT])
		keyCount++;

	if (keyCount == 2)
	{
		// 1/sqrt(2)=sin(45)=cos(45)=0.0.707106781 
		// reference: https://forum.yoyogames.com/index.php?threads/how-to-normalize-diagonal-movement-without-vector.1039/
		tempSpeed *= 0.707106781F;

		if	(((_keyboardData.scanCode[HK_UP] && (_keyboardData.scanCode[HK_LEFT] || _keyboardData.scanCode[HK_RIGHT])) 
				&& _position.y - tempSpeed < 0)
			|| ((_keyboardData.scanCode[HK_LEFT] && (_keyboardData.scanCode[HK_UP] || _keyboardData.scanCode[HK_DOWN]))
				&& _position.x - tempSpeed < 0)
			|| ((_keyboardData.scanCode[HK_RIGHT] && (_keyboardData.scanCode[HK_UP] || _keyboardData.scanCode[HK_DOWN]))
				&& _position.x + tempSpeed + Visualiser::instance()->GetSprite(_graphicsID)->GetFrameWidth() > Visualiser::instance()->GetScreenWidth()) 
			|| ((_keyboardData.scanCode[HK_DOWN] && (_keyboardData.scanCode[HK_LEFT] || _keyboardData.scanCode[HK_RIGHT]))
				&& _position.y + tempSpeed + Visualiser::instance()->GetSprite(_graphicsID)->GetFrameHeight() > Visualiser::instance()->GetScreenHeight()))
		{
			tempSpeed /= 0.707106781F;
		}
	}

	// Move the player depending on input
	if (_keyboardData.scanCode[HK_DOWN] && _position.y + tempSpeed + Visualiser::instance()->GetSprite(_graphicsID)->GetFrameHeight() <= Visualiser::instance()->GetScreenHeight())
		Move(Direction::Down, tempSpeed);
	if (_keyboardData.scanCode[HK_UP] && _position.y - tempSpeed >= 0)
		Move(Direction::Up, tempSpeed);
	if (_keyboardData.scanCode[HK_RIGHT] && _position.x + tempSpeed + Visualiser::instance()->GetSprite(_graphicsID)->GetFrameWidth()  <= Visualiser::instance()->GetScreenWidth())
		Move(Direction::Right, tempSpeed);
	if (_keyboardData.scanCode[HK_LEFT] && _position.x - tempSpeed >= 0)
		Move(Direction::Left, tempSpeed);


	// ---Xbox 360 Controller---
	if (HAPI.GetControllerData(0).isAttached == true)
	{
		// Get controller input
		int leftXValue = HAPI.GetControllerData(0).analogueButtons[HK_ANALOGUE_LEFT_THUMB_X];
		int leftYValue = HAPI.GetControllerData(0).analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y];

		// Normalise controller input between 0 and 1
		float normalisedX = (leftXValue - _minInputVal) / (_maxInputVal - _minInputVal);
		float normalisedY = (leftYValue - _minInputVal) / (_maxInputVal - _minInputVal);

		// Reduce speed if going diagonal
		float tempSpeed = _speed;

		int keyCount = 0;
		if (leftYValue < _deadzoneYDown)
			keyCount++;
		if (leftYValue > _deadzoneYUp)
			keyCount++;
		if (leftXValue > _deadzoneXRight)
			keyCount++;
		if (leftXValue < _deadzoneXLeft)
			keyCount++;

		if (keyCount == 2)
		{
			tempSpeed *= 0.707106781F;


			if (((_keyboardData.scanCode[HK_UP] && (_keyboardData.scanCode[HK_LEFT] || _keyboardData.scanCode[HK_RIGHT]))
				&& _position.y - tempSpeed * normalisedY < 0)
				|| ((_keyboardData.scanCode[HK_LEFT] && (_keyboardData.scanCode[HK_UP] || _keyboardData.scanCode[HK_DOWN]))
					&& _position.x - tempSpeed * normalisedX < 0)
				|| ((_keyboardData.scanCode[HK_RIGHT] && (_keyboardData.scanCode[HK_UP] || _keyboardData.scanCode[HK_DOWN]))
					&& _position.x + tempSpeed * normalisedX + Visualiser::instance()->GetSprite(_graphicsID)->GetFrameWidth() > Visualiser::instance()->GetScreenWidth())
				|| ((_keyboardData.scanCode[HK_DOWN] && (_keyboardData.scanCode[HK_LEFT] || _keyboardData.scanCode[HK_RIGHT]))
					&& _position.y + tempSpeed * normalisedY + Visualiser::instance()->GetSprite(_graphicsID)->GetFrameHeight() > Visualiser::instance()->GetScreenHeight()))
			{
				tempSpeed /= 0.707106781F;
			}
		}

		// Move
		if (leftYValue < _deadzoneYDown && _position.y + tempSpeed * normalisedY + Visualiser::instance()->GetSprite(_graphicsID)->GetFrameHeight() <= Visualiser::instance()->GetScreenHeight())
			Move(Direction::Down, tempSpeed * (1.f - normalisedY));
		if (leftYValue > _deadzoneYUp && _position.y - tempSpeed * normalisedY >= 0)
			Move(Direction::Up, tempSpeed * normalisedY);
		if (leftXValue > _deadzoneXRight && _position.x + Visualiser::instance()->GetSprite(_graphicsID)->GetFrameWidth() + tempSpeed * normalisedX <= Visualiser::instance()->GetScreenWidth())
			Move(Direction::Right, tempSpeed * normalisedX);
		if (leftXValue < _deadzoneXLeft &&  _position.x - tempSpeed * normalisedX >= 0)
			Move(Direction::Left, tempSpeed * (1.f - normalisedX));
	}
}

void Player::Update()
{
}

void Player::OnCollision(Entity& other)
{
	if (HAPI.GetControllerData(0).isAttached == true)
		HAPI.SetControllerRumble(0, 65535, 65535);

	this->TakeDamage(other.GetDamage());
}

void Player::OnEndAnimation()
{
	if (HAPI.GetControllerData(0).isAttached == true)
		HAPI.SetControllerRumble(0, 0, 0);
}

void Player::OnDeath()
{
	SetVisibility(false);
	_currFrame = 0;

	// Explode
	Explosion* newExplosion = ObjectPool::instance()->GetFirstFreeExplosion();
	if (newExplosion != nullptr)
	{
		Vector2 thisMid(_position.x + Visualiser::instance()->GetSprite(_graphicsID)->GetFrameWidth() / 2.F,
			_position.y + Visualiser::instance()->GetSprite(_graphicsID)->GetFrameHeight() / 2.F);

		newExplosion->SetPosition(Vector2(thisMid.x - Visualiser::instance()->GetSprite(newExplosion->GetGraphicsID())->GetFrameWidth() / 2.F,
			thisMid.y - Visualiser::instance()->GetSprite(newExplosion->GetGraphicsID())->GetFrameHeight() / 2.F));
		newExplosion->SetSide(ESide::eNeutral);
		newExplosion->SetVisibility(true);

		newExplosion = nullptr;
	}
}
