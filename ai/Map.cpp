#include "Map.hpp"

Map::Map()
{
	Map(100, 100);
}

Map::Map(int w, int h)
{
	this->width = w;
	this->height = h;
	//this->map(h, std::vector<int>(w)); 

	this->startpos.x = w/2;
	this->startpos.y = h/2;

	this->currentpos.x = this->startpos.x;
	this->currentpos.y = this->startpos.y;
}

position Map::getCurrentPos()
{
	return this->currentpos;
}

position Map::getStartPos()
{
	return this->startpos;
}


