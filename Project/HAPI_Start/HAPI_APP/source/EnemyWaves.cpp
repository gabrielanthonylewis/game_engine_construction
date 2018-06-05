#include "EnemyWaves.h"

#include "AI.h"


EnemyWaves::~EnemyWaves()
{
	// clean up
	for each(Wave* wave in _waves)
	{
		if (wave == nullptr)
			continue;

		delete wave;
	}
}


void EnemyWaves::SpawnWave(int wave)
{
	// spawn the wave by making them visible 
	// (Update and such only occur when visible is true).
	for each(AI* entity in _waves[wave]->_entites)
		entity->SetVisibility(true);
}