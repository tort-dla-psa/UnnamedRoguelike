#ifndef crearure_h
#define creature_h

#include<string>
#include<vector>
#include"gameobject.h"

class item;
class tilewspace;
class creature:public gameobjectmovable{
	ushort dp;
	ushort sightsize, inventorysize;
	std::vector<tilewspace*> path;
	gameobjectmovable** inventory;
	gameobjectmovable* inhand;
	ushort itemcount;
public:
	creature(std::string name, char img, ushort hp, ushort dp);
	~creature();

        ushort Attack(gameobject* target);
        void GetDamage(ushort dhp);
        bool PickUp(gameobjectmovable* target);
        void Draw();
        void Take(gameobjectmovable* target);
        void Take(ushort itemplace);
        void Drop(ushort itemplace, tilewspace* tagret);
        void Drop(ushort itemplace);
	void Drop(gameobjectmovable* target);
        void SetPath(std::vector<tilewspace*> newpath);
        void FollowPath();
        gameobjectmovable* GetItem(ushort place);
        gameobjectmovable* GetInHand();
        ushort GetInvSize();
        ushort GetItemsCount();
        ushort GetSightSize();
        std::vector<tilewspace*> GetPath();
};
#endif
