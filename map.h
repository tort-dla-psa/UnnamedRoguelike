#ifndef map_h
#define map_h
#include<vector>
#include<string>
#include"gameobject.h"
#include"tile.h"
class map{
	ushort width,height,depth;
protected:
	tile**** tiles;
	bool*** revealed;
	bool*** visible;
	tile* air;
	tile* borderstone;
	tile* CastRay(tile* start, tile* end);
public:
	map(ushort width, ushort height, ushort depth);
	virtual ~map();
	void SetTile(ushort x, ushort y, ushort z, tile* target);
	tile* GetTile(ushort x, ushort y, ushort z);

	bool GetRevealed(tile* place);
	bool GetVisible(tile* place);
	void SetRevealed(ushort x, ushort y, ushort z, bool key);
	void SetVisible(ushort x, ushort y, ushort z, bool key);
	
	void DelTile(tile *target);
	void DelTile(ushort x, ushort y, ushort z);
	tile* FindTileOnVertical(ushort x, ushort y);
	ushort GetWidth();
	ushort GetHeight();
	ushort GetDepth();
	//std::vector<tile*> GetSphere(tile* center, ushort radius);
	map* GetSphere(tile* center, ushort radius);
};

class Perlin: public map{
	std::vector<tile*> coverstone;
	std::vector<tile*> surfstone;
	std::vector<tile*> midstone;
	std::vector<tile*> deepstone;
	tile* FindTile(std::string name, std::vector<tile*> tiletypes);
	tile* PickRand(std::vector<tile*> tiletypes);
	tile* PickCover(std::vector<tile*> tiletypes);
	tile* PickSurface(std::vector<tile*> tiletypes);
	tile* PickMiddle(std::vector<tile*> tiletypes);
	tile* PickDeep(std::vector<tile*> tiletypes);
public:
	Perlin(ushort width, ushort height, ushort depth, std::vector<tile*> tiletypes);
	~Perlin();
};
#endif
