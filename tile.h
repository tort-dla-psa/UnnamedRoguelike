#ifndef tile_h
#define tile_h
#include<string>
#include"material.h"
class creature;
class item;
class tile{
	short x,y,z;
	material* mat;
	short hp;
	creature* cr;
	item* it;
public:
	tile(short x, short y, short z);
	tile(short x, short y, short z, material* mat);
	~tile();
	void GetDamage(short dmg);
	std::string GetName();
	char GetChar();
	bool GetPass();
	bool IsSpace();
	material* GetMat();
	short GetDensity();
	short GetMelting();
	short GetX();
	short GetY();
	short GetZ();
	short GetHp();
	creature* GetCreature();
	item* GetItem();
	void PlaceCreature(creature* target);
	void PlaceItem(item* target);
};

#endif
