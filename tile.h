#ifndef tile_h
#define tile_h
#include<vector>
#include<memory>
#include"gameobject.h"

class creature;
class item;
class tile:public gameobjectstatic{
protected:
        ushort hp;
        double dropchance;
        item* ore;
public:
        tile(ushort x, ushort y, ushort z);
        tile(ushort x, ushort y, ushort z, tile* sample);
	tile(char img, std::string name, double dropchance, item* ore);
        ~tile();
        
	virtual bool IsSpace();
	void SetOre(item* ore);
        item* GetOre();
        double GetChance();
	char GetImg() override;
};
class tilewspace:public tile{
        std::vector<gameobjectmovable*> objects;
public:
        tilewspace(ushort x, ushort y, ushort z, tilewspace* sample);
	tilewspace(char img, std::string name, item* ore);
        ~tilewspace();

	bool IsSpace() override;
        void RemoveObject(gameobjectmovable* target);
	void AddObject(gameobjectmovable* target);
	ushort GetObjectsCount();
	bool HasObjects();
        std::vector<gameobjectmovable*> GetObjects();
	char GetImg() override;
};
#endif
