#include "AI.h"

#include "Visualiser.h" 
#include "ObjectPool.h"
#include "Explosion.h"
#include "Texture.h" // Access texture information
#include "Vector2.h"
#include "Sprite.h"

#include "EnemyWaves.h"
#include "World.h"

#include "SoundSystem.h"

AI::AI(const std::string& uniqueGraphicsID, Texture& texture, Vector2 initialPosition, int sheetRows, int sheetCol)
{
	// Initialise values
	_position = initialPosition;
	_oldPosition = initialPosition;
	_graphicsID = uniqueGraphicsID;

	// Add sprite to visualiser making it known
	Visualiser::instance()->AddSprite(texture, _graphicsID, sheetRows, sheetCol);

	SetSide(ESide::eEnemy);
	_isPlayer = false;

	InitialiseTilePosition();
}


AI::~AI()
{
}

void AI::OnCollision(Entity& other)
{
	/* Store health prior to a change so that
	 the health can be used for the store.*/
	_preDeathHealth = _health;

	/* If there is a collision wiht the player 
	then we want to play a sound. We also want to kill
	the player straight up.*/
	if (other.GetIsPlayer())
	{
		SoundSystem::instance()->PlaySound("collision");
		other.TakeDamage(other.GetHealth());
		return;
	}

	this->TakeDamage(other.GetDamage());
}

void AI::OnDeath()
{
	_currFrame = 0;
	SetVisibility(false);

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

		// No longer used in the pool as it is in the world
		poolUsing = false;
	}


	// Only the player can kill AI, this means I can update the score here.
	// The added score here is for destruction, as opposed to regular score gained
	// by reducing the enemies health.
	World::instance()->AddScore(100);

	// Increase the amount of entities killed in the wave
	// if all of the enemies are killed go to the next wave 
  	if (_wave != nullptr)
	{
 		if (_wave->KillEntity() == true)
			World::instance()->WaveComplete();
	}
}