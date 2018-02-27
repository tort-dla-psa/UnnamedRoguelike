#ifndef map_h
#define map_h
#include<vector>
#include<string>
#include"gameobject.h"
#include"tile.h"
class map{
	ushort width,height,depth;
protected:
	tile****tiles;
	tile* air;
	tile* borderstone;
	std::vector<tile*> surfstone;
	std::vector<tile*> midstone;
	std::vector<tile*> deepstone;
public:
	map(ushort width, ushort height, ushort depth);
	~map();
	tile* GetTile(ushort x, ushort y, ushort z);
	void DelTile(tile *target);
	void DelTile(ushort x, ushort y, ushort z);
	tile* FindTileOnVertical(ushort x, ushort y);
	ushort GetWidth();
	ushort GetHeight();
	ushort GetDepth();
};

class Perlin: public map{
	tile* FindTile(std::string name, std::vector<tile*> tiletypes);
	tile* PickRand(std::vector<tile*> tiletypes);
public:
	Perlin(ushort width, ushort height, ushort depth, std::vector<tile*> tiletypes);
};
#endif
