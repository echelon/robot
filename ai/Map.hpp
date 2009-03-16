#ifndef __defined_ai_Map_hpp
#define __defined_ai_Map_hpp

#include <vector>

#include "../serial/Serializer.hpp"
#include "../tts/Festival.hpp"

const int MAP_LOCATION_UNVISITED = 0;
const int MAP_LOCATION_OBSTACLE = -1;
const int MAP_LOCATION_CLEAR = 1;
const int MAP_LOCATION_START_POINT = 2;
const int MAP_LOCATION_GOAL = 3;

struct position
{
	int x;
	int y;
	int d; // direction 1UP 2RIGHT 3DOWN 4LEFT 0NULL
};

class Map
{
	public:
		Map();
		Map(int w, int h);
		
		position getCurrentPos();
		position getStartPos();

		void markObstacleAhead();

	private:
		int width;
		int height;

		std::vector< std::vector<int> > map;

		position currentpos;
		position startpos;
		position goalpos;
};

#endif
