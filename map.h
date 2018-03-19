#ifndef map_h
#define map_h
#include<vector>
#include<string>
#include"gameobject.h"
#include"tile.h"
class tilearray{
protected:
	tile** tiles;
	ushort width, height, depth;
	unsigned int size;
	tile* GetTileDangerous(ushort x, ushort y, ushort z);
public:
	tilearray(ushort width, ushort height, ushort depth);
	virtual ~tilearray();
	void SetTile(ushort x, ushort y, ushort z, tile* target);
	tile* GetTile(ushort x, ushort y, ushort z);
	tile* GetTileDangerous(unsigned int i);
	void DelTile(tile *target, tilewspace* replacement);
	void DelTile(ushort x, ushort y, ushort z, tilewspace* replacement);
	ushort GetWidth();
	ushort GetHeight();
	ushort GetDepth();
};
class map:public tilearray{
protected:
	bool* revealed;
	bool* visible;
	tile* air;
	tile* borderstone;
	tile* CastRay(tile* start, tile* end);
public:
	map(ushort width, ushort height, ushort depth);
	virtual ~map();

	bool GetRevealed(tile* place);
	bool GetVisible(tile* place);
	void SetRevealed(tile* place, bool key);
	void SetVisible(tile* place, bool key);
	void SetRevealed(tilearray* sphere, bool key);
	void SetVisible(tilearray* sphere, bool key);
	
	void DelTile(tile* target);
	void DelTile(ushort x, ushort y, ushort z);
	tile* FindTileOnVertical(ushort x, ushort y);
	tilearray* GetSphere(tile* center, ushort radius);
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
