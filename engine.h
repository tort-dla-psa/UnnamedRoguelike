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
#include"interface.h"
#include"pathfinding.h"

class engine{
	map* mp;
	creature* player;
	camera* cam;
	interface* win;
	generator* pathfinder;
	std::vector<material*> materials;
	std::vector<item*> oreideas;
	std::vector<item*> items;
	std::vector<tile*> tileideas;
	std::vector<creature*> creatures;
	void WriteLog(tile* place, ushort dp);
	void WriteLog(creature* target, ushort dp);
	void DelCreature(creature* cr);
	item* CreateItem(char img, material* materia);
	item* CreateItem(char img, material* materia, ushort x, ushort y, ushort z);
	creature* AddCreature(std::string name, char img, ushort hp, ushort dp, ushort x, ushort y, ushort z);
	void PickUp(char dir);
	void DelItem(item* target);
	void MovePlayer(char dir);
	void MoveCam(char dir);
	void PerformAttack(char dir);
	void DrawMap();
	void DoGravity();
	void HandleKey(char ch);
	short* GetDir(char ch);
	void drawrecurse(ushort x, ushort y, ushort z, ushort iter, ushort max);
public:
	engine();
	~engine();
	void MainLoop();
	void WriteLog(std::string mes);
};
#endif
