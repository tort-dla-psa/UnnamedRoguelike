#ifndef map_h
#define map_h
#include<vector>
#include<string>
#include"tile.h"
class creature;
class map{
	short width,height,depth;
protected:
	tile****tiles;
	tiletype* air;
	tiletype* surfstone;
	tiletype* midstone;
	tiletype* deepstone;
	tiletype* borderstone;
public:
	map(short a, short b, short c);
	~map();
	tile* GetTile(short a, short b, short c);
	void DelTile(tile *target);
	void DelTile(short a, short b, short c);
	short GetWidth();	
	short GetHeight();	
	short GetDepth();	
};
class Perlin: public map{
	tiletype* findtile(std::string name, std::vector<tiletype*> tiletypes);
public:
	Perlin(short width, short height, short depth, std::vector<tiletype*> tiletypes);
};
class camera{
	creature* cr;
	tile* place;
	short width, height;
public:
	camera(creature* cr);
	camera(tile* place);
	~camera();
	bool Flying();
	void FollowCreature(creature* cr);
	void GoToPlace(tile* place);
	void SetParams(short width, short height);
	short GetX();
	short GetY();
	short GetZ();
	short GetOffsetX();
	short GetOffsetY();
	short* GetCoords();
};
#endif
