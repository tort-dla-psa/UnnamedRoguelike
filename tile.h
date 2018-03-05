#ifndef tile_h
#define tile_h
#include<vector>
#include<memory>
#include"gameobject.h"

class creature;
class item;
class myPair;
namespace tileenums{
	enum DepthType { none, cover, surface, middle, deep };
}
class tile:public gameobjectstatic{
protected:
        ushort hp;
        double dropchance;
	tileenums::DepthType depth;
        item* ore;
	myPair* color;
public:

        tile(ushort x, ushort y, ushort z);
        tile(ushort x, ushort y, ushort z, tile* sample);
	tile(char img, std::string name, double dropchance, item* ore, tileenums::DepthType depth, myPair* color);
        virtual ~tile();
        
	virtual bool IsSpace();
	void SetOre(item* ore);
        item* GetOre();
        double GetChance();
	char GetImg() override;
	tileenums::DepthType GetDepthType();
	void SetColor(myPair* color);
	myPair* GetColor();
};
class tilewspace:public tile{
        std::vector<gameobjectmovable*> objects;
public:
        tilewspace(ushort x, ushort y, ushort z, tilewspace* sample);
	tilewspace(char img, std::string name, item* ore, tileenums::DepthType depth);
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
