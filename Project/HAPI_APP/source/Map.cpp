#include "Map.h"

#include "Tile.h"

Map::Map(const int width, const int height)
	: _width(width),_height(height)
{
	// Initialise Array of tiles to cover the map
	for (int y = 0; y < _height; y++)
	{
		std::vector<Tile*> row;
		for (int x = 0; x < _width; x++)
		{
			Tile* newTile = new Tile();
			newTile->x = (float)x;
			newTile->y = (float)y;
			
			newTile->worldX = x * 32 + (tileWidth / 2.F); // center
			newTile->worldY = y * 32 + (tileWidth / 2.F); // center 

			row.push_back(newTile);
			
		}
		_tiles.push_back(row);
	}

	GeneratePathfindingGraph();
}


Map::~Map()
{
	// Clean Up
	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x < _width; x++)
		{
			delete _tiles[y][x];
		}
	}

}

float Map::CostToEnterTile(Tile* source, Tile* target)
{

	if (source == nullptr || target == nullptr)
		return 0.F;

	if (target->x >= _tiles.size())
		return 0.f;

	if (target->y >= _tiles[(unsigned int)target->x].size())
		return 0.f;

	Tile* tile = _tiles[(unsigned int)target->x][(unsigned int)target->y];

	if (tile->isWalkable == false)
		return 99999.F;

	float cost = tile->movementCost;

	// to prefer straight lines
	// If diagonal...
	if (source->x != target->x && source->y != target->y)
	{
		cost += 0.001F;
	}

	return cost;
}

void Map::GeneratePathfindingGraph()
{
	// Loop through entire map and set up neighbours
	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x < _width; x++)
		{	
			// We have an 8 way connected map

			// Left
			if (x > 0)
				_tiles[y][x]->_neighbours.push_back(_tiles[y][x - 1]);
			//Right
			if (x < _width - 1)
				_tiles[y][x]->_neighbours.push_back(_tiles[y][x + 1]);

			// Up
			if (y > 0)
				_tiles[y][x]->_neighbours.push_back(_tiles[y - 1][x]);
			//Down
			if (y < _height - 1)
				_tiles[y][x]->_neighbours.push_back(_tiles[y + 1][x]);


			// Diagonal Up Right
			if (x < _width - 1 && y > 0)
				_tiles[y][x]->_neighbours.push_back(_tiles[y - 1][x + 1]);

			// Diagonal Up Left
			if (x > 0 && y > 0)
				_tiles[y][x]->_neighbours.push_back(_tiles[y - 1][x - 1]);

			// Diagonal Down Right
			if (x < _width - 1 && y < _height - 1)
				_tiles[y][x]->_neighbours.push_back(_tiles[y + 1][x + 1]);

			// Diagonal Down Left
			if (x > 0 && y < _height - 1)
				_tiles[y][x]->_neighbours.push_back(_tiles[y + 1][x - 1]);

		}

	}
}