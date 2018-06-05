#include "KamikazeDrone.h"

#include "Tile.h"

#include <Map>
#include "Map.h"

#include <algorithm> // std::min_element
#include <iterator> // std::begin, std::end
#include <limits>

#include "World.h"
#include "Visualiser.h"
#include "Sprite.h"
#include "Rectangle.h"



KamikazeDrone::KamikazeDrone(const std::string& uniqueGraphicsID, Texture& texture, Vector2 initialPosition, Map* map)
	: KamikazeDrone::AI(uniqueGraphicsID, texture, initialPosition, 1, 2)
{
	// Initialise Variables
	_speed = 4.F; // 2.f
	_health = 1;
	_initialHealth = _health;

	// Next time to recalculate the path (as the player moves)
	_nextTime = (int)(HAPI.GetTime() / 1000.F + 2.f); // every 2 seconds
}


KamikazeDrone::~KamikazeDrone()
{
	// dont delete map as it is done in world
	_map = nullptr;
}


void KamikazeDrone::Update()
{
	if (_isVisible == false)
		return;

	
	// If ready to recalculate the path, do so
	if (HAPI.GetTime() / 1000.F >= _nextTime)
	{
		Vector2 playerTile = World::instance()->GetPlayerTile();
		GeneratePathTo((int)playerTile.x, (int)playerTile.y);
		_nextTime = (int)(HAPI.GetTime() / 1000.F + 2.f); // every 2 seconds
		return;
	}


	// Return if there isnt a path
	if (_currentPath.size() <= 0)
		return;

	
	float posX = _position.x + Visualiser::instance()->GetSprite(_graphicsID)->GetFrameWidth() / 2.F;
	float posY = _position.y + Visualiser::instance()->GetSprite(_graphicsID)->GetFrameHeight() / 2.F;

	// We want our position to be on the player perfectly
	// the slack stops the stutter on the way there though
	float slack = 2.F;
	if (_currentPath.size() == 1)
		slack = -1.F;

	// If at the target move to the next tile
	if (posX >= targetTileX  - slack
		&& posX <= targetTileX  + slack
		&& posY >= targetTileY  - slack
		&& posY <= targetTileY  + slack)
	{
		MoveToNextTile();
		return;
	}

	// Movement
	if (targetTileX + 1.F < posX )
		Move(Direction::Left, _speed);
	else if (targetTileX - 1.F > posX )
		Move(Direction::Right, _speed);

	if (targetTileY + 1.F  < posY )
		Move(Direction::Up, _speed);
	else if (targetTileY - 1.F > posY )
		Move(Direction::Down, _speed);
}

void KamikazeDrone::OnEndAnimation()
{
}

void KamikazeDrone::MoveToNextTile()
{
	if (_currentPath.size() <= 0)
		return;

	if (_currentPath.size() == 1)
	{
		targetTileX = _currentPath[0]->worldX;
		targetTileY = _currentPath[0]->worldY;
		_currentPath.clear();
		return;
	}

	// Now grab the new first node and move to that pos.
	targetTileX = _currentPath[1]->worldX;
	targetTileY = _currentPath[1]->worldY; 

	// remove the old current node from the path.
	_currentPath.erase(std::begin(_currentPath));
}

void KamikazeDrone::GeneratePathTo(int destX, int destY)
{
	std::vector<Tile*> newPath;

	if (_map->GetTiles()[destY][destX]->isWalkable == false)
		return;

	// Where we are coming from
	Tile* source = _map->GetTiles()[_tileY][_tileX];
	Tile* target = _map->GetTiles()[destY][destX];

	// the list of nodes already checked
	std::vector<Tile*> closedSet;	
	// the list of nodes that we havent checked
	std::vector<Tile*> openSet;
	openSet.push_back(source);
	
	std::map<Tile*, Tile*> cameFrom;
	std::map<Tile*, float> gScore;
	std::map<Tile*, float> fScore;
		
	// initialise Everything to have INFINITY distance.
	for (int y = 0; y < _map->GetHeight(); y++)
	{
		for (int x = 0; x < _map->GetWidth(); x++)
		{
			Tile* v = _map->GetTiles()[y][x];

			if (v != source)
			{
				gScore[v] = std::numeric_limits<float>::infinity();
				gScore[v] = std::numeric_limits<float>::infinity();
				cameFrom[v] = nullptr;
			}
		}
	}

	cameFrom[source] = nullptr;
	gScore[source] = 0;
	fScore[source] = source->DistanceTo(*target);


	// Calculate path from target to the source
	while (!openSet.empty())
	{
		// "c" is going to be the unvisted node with the smallest distance
		Tile* current = nullptr;
		for each(Tile* lowestF in openSet)
		{
			if (lowestF == nullptr)
				continue;

			if(current == nullptr || fScore[lowestF] < fScore[current])
				current = lowestF;
		}

		if (current == target)
			break; // exit while loop

		// remove it as visited
		openSet.erase(std::remove(openSet.begin(), openSet.end(), current), openSet.end());
		closedSet.push_back(current);
	
		for each(Tile* neighbor in current->_neighbours)
		{
			if (neighbor == nullptr)
				continue;

			// can allow, ignores the neigbor which is already evaluated
			if (std::find(closedSet.begin(), closedSet.end(), neighbor) != closedSet.end())
				continue;
			

			// What is the movement cost 			
			// todo? In my case could check to see if entity there and make tentative_gScore cost 999 otherwise 1
			float tentative_gScore = gScore[current] + _map->CostToEnterTile(current, neighbor);


			if (!(std::find(openSet.begin(), openSet.end(), neighbor) != openSet.end()))
				openSet.push_back(neighbor);
			else if (tentative_gScore >= gScore[neighbor])
				continue; // This is not a better path to continue
			

			// record the path
			cameFrom[neighbor] = current;
			gScore[neighbor] = tentative_gScore;
			fScore[neighbor] = gScore[neighbor] + neighbor->DistanceTo(*target);
		}
	}


	// If we get here then either we have found the shortest route or we cannot get 
	// to our target.
	if (cameFrom[target] == nullptr)
	{
		// no route
		return;
	}


	// Step through the chain and add it to our path
	Tile* curr = target;
	while (curr != nullptr)
	{ 
		newPath.push_back(curr);
		curr = cameFrom[curr];
	}

	// We have a route from our target to our source, so we reverse it
	std::reverse(std::begin(newPath), std::end(newPath));

	_currentPath = newPath;
		
	MoveToNextTile();
}

void KamikazeDrone::OnDeath()
{
	AI::OnDeath();

	// reinitialise for reuse
	_speed = 4.F; 
	_health = 1;
}

