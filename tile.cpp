#include "tile.h"
#include "creature.h"
#include "item.h"
#include "material.h"
#include <ncurses.h>
#include <algorithm>
#include "interface/MyPalette.h"
#include "interface/MyColor.h"
#include "interface/MyPair.h"

tile::tile(ushort x, ushort y, ushort z):
	gameobjectstatic(x,y,z){}

tile::tile(ushort x, ushort y, ushort z, tile* sample):
	gameobjectstatic(x, y, z)
{
	SetName(sample->GetName());
	SetOre(sample->GetOre());
	SetMat(sample->GetMat());
	SetImg(sample->GetImg());
	SetColor(sample->GetColor());
	SetHp(10);
	vect3i sizes { 100, 100, 100 };
	SetSizes(sizes);
	this->dropchance = sample->GetChance();
	this->depth = depth;
}

tile::tile(char img, std::string name, double dropchance, item* ore, tileenums::DepthType depth, myPair* color):
	gameobjectstatic()
{
	vect3i sizes { 100, 100, 100 };
	this->dropchance = dropchance;
	this->depth = depth;
	SetSizes(sizes);
	SetImg(img);
	SetName(name);
	SetOre(ore);
	SetMat(ore->GetMat());
	SetColor(color);
}

tile::~tile(){};

bool tile::IsSpace(){	return false;}
void tile::SetOre(item* ore){	this->ore = ore;}
item* tile::GetOre(){	return ore;}
double tile::GetChance(){	return dropchance;}
char tile::GetImg(){	return img;}
tileenums::DepthType tile::GetDepthType(){	return depth;}
void tile::SetColor(myPair* color){	this->color = color;}
myPair* tile::GetColor(){	return color;}

tilewspace::tilewspace(ushort x, ushort y, ushort z, tilewspace* sample):
	tile(x, y, z)
{
	SetName(sample->GetName());
	SetOre(sample->GetOre());
	SetMat(sample->GetMat());
	SetImg(sample->GetImg());
	SetHp(100);
	vect3i sizes { 100, 100, 100 };
	SetSizes(sizes);
	this->dropchance = 0;
	this->depth = sample->GetDepthType();
}

tilewspace::tilewspace(char img, std::string name, item* ore, tileenums::DepthType depth):
	tile(img,name,0,ore,depth,nullptr){};

tilewspace::~tilewspace(){};

bool tilewspace::IsSpace(){	return true;}

void tilewspace::RemoveObject(gameobjectmovable* target){
	for(auto it:objects){
		if(it==target){
			objects.erase(std::find(objects.begin(),objects.end(),it),objects.end());
		}
	}
}

void tilewspace::AddObject(gameobjectmovable* target){
	objects.push_back(target);
}

ushort tilewspace::GetObjectsCount(){	return objects.size();}
bool tilewspace::HasObjects(){	return !objects.empty();}

std::vector<gameobjectmovable*> tilewspace::GetObjects(){
	return objects;
}

char tilewspace::GetImg(){
	if(HasObjects()){
		return objects.back()->GetImg();
	}
	return img;
}
