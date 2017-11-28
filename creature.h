#ifndef crearure_h
#define creature_h
#include <string>
class item;
class tile;
class creature{
	std::string name;
	short hp, dp;
	short sightsize, inventorysize;
	char img;
	tile* place;
	item** inventory;
	item* inhand;
	short itemcount;
public:
	creature(std::string name, char img, short hp, short dp);
	~creature();

	short Attack(creature* target);
	void GetDamage(short dhp);
	short Dig(tile* target);
	bool PickUp(item* target);
	void Draw();
	void Take(item* target);
	void Take(short itemplace);
	void Drop(short itemplace, tile* tagret);
	void Drop(short itemplace);
	std::string GetName();
	tile* GetPlace();
	item* GetItem(short place);
	item* GetInHand();
	void Move(tile* place);
	char GetImg();
	short GetInvSize();
	short GetItemsCount();
	short GetSightSize();
	short GetHp();
	short GetX();
	short GetY();
	short GetZ();
};
#endif
