#include "creature.h"
#include "tile.h"
#include "material.h"
#include "item.h"
#include <ncurses.h>
item::item(){
	place=0;
	volume=0;
	rarity=0;
	owner=NULL;
	holder=NULL;
	place=NULL;
}

item::item(char img, short volume, short sharpness, material* materia):
	img(img),volume(volume),sharpness(sharpness),materia(materia)
{
	owner=NULL;
	holder=NULL;
	place=NULL;
}

item::~item(){
	owner=NULL;
	holder=NULL;
	place=NULL;
	materia=NULL;
}
void item::Transfer(creature* newholder){
	holder=newholder;
}

void item::Gift(creature* newowner){
	owner=newowner;
}

void item::Move(tile* place){
	if(this->place!=NULL)
		this->place->PlaceItem(NULL);
	this->place = place;
	if(place!=NULL)
		place->PlaceItem(this);
}
void item::Move(creature* newholder){
	if(this->place!=NULL)
		this->place->PlaceItem(NULL);
	this->place=NULL;
	this->holder = newholder;
}
void item::SetQuality(std::string quality){	this->quality = quality;}
void item::SetRarity(short rarity){		this->rarity = rarity;}
void item::SetPrice(short price){		this->price = price;}

std::string item::GetQuality(){	return quality;}
char item::GetImg(){		return img;}
short item::GetX(){		return place->GetX();}
short item::GetY(){		return place->GetY();}
short item::GetZ(){		return place->GetZ();}
short item::GetPrice(){		return price;}
short item::GetWeight(){	return materia->GetDensity()*volume;}
short item::GetVolume(){	return volume;}
short item::GetSharpness(){	return sharpness;}
short item::GetRarity(){	return rarity;}
creature* item::GetOwner(){	return owner;}
creature* item::GetHolder(){	return holder;}
tile* item::GetPlace(){		return place;}
material* item::GetMaterial(){	return materia;}
