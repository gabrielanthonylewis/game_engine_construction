#pragma once

#include "Entity.h"
#include <string>

// Forward Declarations
class Texture;
class Vector2;
class Wave;

/* The AI script is the parent to all AI entities. 
	It implements functionality used by all AI types such as
	collision or death. Inheritance is used in the child AI entities
	for code control and cleanliness.*/
class AI : public Entity
{
public:

	~AI();

	// Overrides
	void OnDeath() override ;
	void OnCollision(Entity& other) override final;

	// Setter Functions
	inline void SetWave(Wave* wave) { _wave = wave; }
	inline void SetPoolUsing(bool val) { poolUsing = val; }

	// Getter Functions
	inline bool GetPoolUsing() const { return poolUsing; }


protected:
	AI(const std::string& uniqueGraphicsID, Texture& texture, Vector2 initialPosition, int sheetRows, int sheetCol);

	/* Reference to the wave the AI belongs to (if one at all).
	 If there is a wave the wave will be oupdated on the status
	 of the AI, specifically when it is dead.*/
	Wave* _wave = nullptr; // DO NOT DELETE HERE

private:
	// Stores the state of the AI entity, is it in the pool ready or in the world?
	bool poolUsing = false;
	
};

