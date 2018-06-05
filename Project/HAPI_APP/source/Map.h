#pragma once

// Forward Declarations
class Tile;

#include <vector>

/*A tile map containing a bunch of individual tiles*/
class Map 
{
public:
	Map(int width, int height);
	~Map();

	// Calculate a cost to enter the tile from the source tile supplied
	float CostToEnterTile(Tile* source, Tile* target);

	// Getter functions
	const int GetWidth() const { return _width; }
	const int GetHeight() const { return _height; }
	std::vector<std::vector<Tile*>> GetTiles() { return _tiles; }
	float GetTileWidth() const { return tileWidth; }
	float GetTileHeight() const { return tileHeight; }

private:
	// Generates all of the tiles and sets them up
	// such as their neighbours.
	void GeneratePathfindingGraph();

private:
	std::vector<std::vector<Tile*>> _tiles;

	// Size of the map
	const int _width;
	const int _height;

	float tileWidth = 32, tileHeight = 32;
};

