#pragma once

#include <vector>

/*A tile is found on the map itself.
Each tile has a world position and other properties
for the path finder algorithm.*/
class Tile
{
public:
	Tile() = default;
	inline ~Tile() {};

	// tilemap position
	float x = 0, y = 0;
	// world positions
	float worldX = 16, worldY = 16;

	std::vector<Tile*> _neighbours;

	// if not walkable the pathfinder will go around it
	// not over it
	bool isWalkable = true;

	// cost to enter the tile
	float movementCost = 1;

	// Calculate the distance to a tile using pythagoras
	inline float DistanceTo(Tile& n)
	{
		float distX = x - n.x;
		float distY = y - n.y;

		float dist = sqrt(distX * distX + distY * distY);

		return dist;
	}
};

