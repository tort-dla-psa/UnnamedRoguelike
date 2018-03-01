#ifndef eng_h
#define eng_h
#include<ncurses.h>
#include<string>
#include<stdlib.h>
#include<time.h>
#include<vector>
#include<algorithm>
#include"gameobject.h"
#include"item.h"
#include"creature.h"
#include"tile.h"
#include"map.h"
#include"material.h"
#include"./interface/interface.h"
#include"pathfinding.h"

class engine{
	map* mp;
	creature* player;
	interface* win;
	generator* pathfinder;
	std::vector<material*> materials;
	std::vector<item*> oreideas;
	std::vector<item*> items;
	std::vector<tile*> tileideas;
	std::vector<creature*> creatures;
	void WriteLog(gameobject* target, ushort dp);
	void DelObject(gameobjectmovable* it);
	item* CreateItem(char img, material* materia);
	item* CreateItem(char img, material* materia, ushort x, ushort y, ushort z);
	creature* AddCreature(std::string name, char img, ushort hp, ushort dp, ushort x, ushort y, ushort z);
	void PickUp(iConstInt keycode);
	void MovePlayer(iConstInt keycode);
	void MoveCam(iConstInt keycode);
	void PerformAttack(iConstInt keycode);
	short* GetDir(iConstInt keycode);
	void DrawMap();
	void DoGravity();
	void HandleKey(char ch);
	void DrawRecurse(ushort x, ushort y, ushort z, ushort iter, ushort max);
	void InitKeys();
public:
	engine();
	~engine();
	void MainLoop();
};
#endif
