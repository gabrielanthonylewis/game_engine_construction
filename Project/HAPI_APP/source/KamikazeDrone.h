#pragma once

#include "AI.h"

#include <vector>

// Forward declarations
class Map;
class Tile;

/*The kamikaze drone will move to the player's position
to collide with them them and thereby killing them.
The drone is weak but fast.
*/
class KamikazeDrone : public AI
{
public:
	KamikazeDrone(const std::string& uniqueGraphicsID, Texture& texture, Vector2 initialPosition, Map* map);
	~KamikazeDrone();

	// Generates a path to this tile positions
	// using A* pathfinding algorithm
	void GeneratePathTo(int x, int y);

	// Overrides
	void Update() override final;
	void OnEndAnimation() override final;
	void OnDeath() override;

	// Setter functions
	void SetMap(Map* map) { _map = map; }

private:
	// Update target variables so that
	// the drone can move to the next tile.
	void MoveToNextTile();

private:
	// next time to recalculate path
	int _nextTime = 0;

	// points to the map for tile reference
	Map* _map = nullptr;

	// path to follow
	std::vector<Tile*> _currentPath;

	// World X and Y positions to go to
	float targetTileX;
	float targetTileY;
};

