#ifndef eng_h
#define eng_h
#include<ncurses.h>
#include<string>
#include<stdlib.h>
#include<time.h>
#include<vector>
#include<algorithm>
#include"tile.h"
#include"map.h"
#include"creature.h"
#include"material.h"
#include"item.h"
#include"interface.h"
class engine{
	map* mp;
	creature* player;
	camera* cam;
	interface* win;
	std::vector<material*> materials;
	std::vector<item*> oreideas;
	std::vector<tiletype*> tileideas;
	std::vector<item*> items;
	std::vector<creature*> creatures;
	void WriteLog(tile* place, short dp);
	void WriteLog(creature* target, short dp);
	void DelCreature(creature* cr);
	item* CreateItem(char img, short volume, short sharpness, material* materia);
	item* CreateItem(char img, short volume, short sharpness, material* materia, short x, short y, short z);
	creature* AddCreature(std::string name, char img, short hp, short dp, short x, short y, short z);
	void PickUp(char dir);
	void Drop();
	void DelItem(item* target);
	void MovePlayer(char dir);
	void MoveCam(char dir);
	void PerformAttack(char dir);
	void DrawMap();
	void DoGravity();
	void HandleKey(char ch);
	short* GetDir(char ch);
	void PrintTile(char ch,short y,short x,short color);
	void PrintPlayerStat();
	void PrintStat(item*cr);
	void drawrecurse(short x, short y, short z, short iter, short max);
public:
	engine();
	~engine();
	void CreatePerlinMap();
	void MainLoop();
	void WriteLog(std::string mes);
};
#endif
