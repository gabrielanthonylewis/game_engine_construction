#pragma once

#include <vector>

// Forward declartions
class AI;

/*!!!!!!!!!!!!!!!!!!!!!!
NOTE: Variables in these classes are public because they are 
practically structures.
!!!!!!!!!!!!!!!!!!!!!!!!*/

/*A wave consists of a number of entities all
to be spawned in the future when the wave is needed.*/
class Wave
{
public:
	// Entities in the wave
	std::vector<AI*> _entites;

	// Theoretically kill an entity by
	// increasing the number of entities killed.
	// If all entities have been killed the function returns true.
	inline bool KillEntity()
	{
		_entitiesKilled++;
		return (_entitiesKilled >= (int)_entites.size());
	}

private:
	int _entitiesKilled = 0;
};


/*EnemyWaves is the manager of a level's waves.
It is responsible for spawning and managing.*/
class EnemyWaves
{
public:
	EnemyWaves() = default;
	~EnemyWaves();

	// Spawn all of the entities within a specified wave
	void SpawnWave(int wave);

public:
	// a list of all the waves in the level
	std::vector<Wave*> _waves;
	// the current wave
	int _currentWave = 0;	
};

