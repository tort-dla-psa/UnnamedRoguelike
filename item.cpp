#include <ncurses.h>
#include "item.h"
item::item(std::string name,char img, material* materia):
	gameobjectmovable()
{
	SetName(name);
	SetImg(img);
	SetMat(materia);
}

item::~item(){}

void item::SetQuality(std::string quality){	this->quality = quality;}
void item::SetRarity(short rarity){		this->rarity = rarity;}
void item::SetPrice(short price){		this->price = price;}

std::string item::GetQuality(){	return quality;}
ushort item::GetPrice(){	return price;}
ushort item::GetRarity(){	return rarity;}
