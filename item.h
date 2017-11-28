#ifndef item_h
#define item_h
#include<string>
class tile;
class creature;
class material;
class item{
	short price,volume,rarity;
	short sharpness;
	char img;
	creature* owner;
	creature* holder;
	tile* place;
	material* materia;
	std::string quality;
public:
	item();
	item(char img, short volume, short sharpness, material* materia);
	~item();
	void SetQuality(std::string quality);
	void SetRarity(short rarity);
	void SetPrice(short price);
	void Transfer(creature* hewholder);
	void Gift(creature* newowner);
	void Move(tile* newplace);
	void Move(creature* newholder);
	std::string GetQuality();
	char GetImg();
	short GetX();
	short GetY();
	short GetZ();
	short GetPrice();
	short GetWeight();
	short GetVolume();
	short GetRarity();
	short GetSharpness();
	creature* GetOwner();
	creature* GetHolder();
	tile* GetPlace();
	material* GetMaterial();
};
#endif
