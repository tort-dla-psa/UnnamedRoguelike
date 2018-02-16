#ifndef item_h
#define item_h
#include<string>
#include"gameobject.h"

class item:public gameobjectmovable{
        ushort price,rarity;
        std::string quality;
public:
        item(std::string name, char img, material* materia);
        ~item();
        void SetQuality(std::string quality);
        void SetRarity(short rarity);
        void SetPrice(short price);
        std::string GetQuality();
        ushort GetPrice();
        ushort GetRarity();
};

#endif
