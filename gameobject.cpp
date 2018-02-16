#include "gameobject.h"
#include "tile.h"
#include "creature.h"
#include "material.h"

gameobject::gameobject(){
	owner = nullptr;
	mat = nullptr;
}

gameobject::~gameobject(){}


void gameobject::SetName(std::string name){	this->name = name;}
void gameobject::SetImg(char img){	this->img = img;}
void gameobject::SetMat(material* mat){	this->mat = mat;}
void gameobject::SetSizes(vect3i sizes){	this->sizes = sizes;}
void gameobject::GetDamage(ushort dmg){	hp -= dmg;}
void gameobject::SetHp(ushort hp){	this->hp=hp;}

ushort gameobject::GetXsize(){	return sizes.x;}
ushort gameobject::GetYsize(){	return sizes.y;}
ushort gameobject::GetZsize(){	return sizes.z;}
ushort gameobject::GetVolume(){	return sizes.x * sizes.y * sizes.z;}
ushort gameobject::GetWeight(){	return GetVolume()*mat->GetDensity();}
ushort gameobject::GetHp(){	return hp;}
std::string gameobject::GetName(){	return name;}
char gameobject::GetImg(){	return img;}
creature* gameobject::GetOwner(){	return owner;}
material* gameobject::GetMat(){	return mat;}


gameobjectmovable::gameobjectmovable():
	gameobject()
{
	place = nullptr;
	holder = nullptr;
}

gameobjectmovable::gameobjectmovable(creature* holder):
	gameobject(), holder(holder)
{
	place = nullptr;
}

gameobjectmovable::gameobjectmovable(tilewspace* place):
	gameobject(), place(place)
{
	place = nullptr;
}

gameobjectmovable::~gameobjectmovable(){}

creature* gameobjectmovable::GetHolder(){	return holder;}
tilewspace* gameobjectmovable::GetPlace(){	return place;}

void gameobjectmovable::Move(tilewspace* newplace){
	if(place){
		place->RemoveObject(this);
	}else if(holder){
		holder->Drop(this);
		holder=nullptr;
	}
	place = newplace;
	if(place){
		place->AddObject(this);
	}
}

void gameobjectmovable::Move(creature* newholder){
	if(place){
		place->RemoveObject(this);
		place = nullptr;
	}else if(holder){
		holder->Drop(this);
	}
	holder=newholder;
	if(holder){
		holder->PickUp(this);
	}
}

ushort gameobjectmovable::GetX(){	return (place)?place->GetX():holder->GetX();}
ushort gameobjectmovable::GetY(){	return (place)?place->GetY():holder->GetY();}
ushort gameobjectmovable::GetZ(){	return (place)?place->GetZ():holder->GetZ();}


gameobjectstatic::gameobjectstatic(){}

gameobjectstatic::~gameobjectstatic(){}

gameobjectstatic::gameobjectstatic(vect3i coords):
	coords(coords){}

gameobjectstatic::gameobjectstatic(ushort x, ushort y, ushort z){
	coords = vect3i{ x, y, z };
}

ushort gameobjectstatic::GetX(){	return coords.x;}
ushort gameobjectstatic::GetY(){	return coords.y;}
ushort gameobjectstatic::GetZ(){	return coords.z;}
