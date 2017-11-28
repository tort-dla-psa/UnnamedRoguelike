#include "tile.h"
#include "creature.h"
#include "item.h"
#include <ncurses.h>

tile::~tile(){};

tile::tile(short x,short y,short z,material* mat):
	x(x),y(y),z(z),mat(mat),hp(100){}

void tile::GetDamage(short dmg){
	hp-=dmg;
}

std::string tile::GetName(){	return mat->GetName();}
bool tile::GetPass(){		return mat->GetPass();}
bool tile::IsSpace(){		return (mat->GetPass())?true:false;}
creature* tile::GetCreature(){	return (GetPass()&&cr!=NULL)?cr:NULL;}
item* tile::GetItem(){		return (GetPass()&&it!=NULL)?it:NULL;}
short tile::GetHp(){		return hp;}
short tile::GetX(){		return x;}
short tile::GetY(){		return y;}
short tile::GetZ(){		return z;}
void tile::PlaceCreature(creature* target){	cr=(GetPass())?target:NULL;}
void tile::PlaceItem(item* target){		it=(GetPass())?target:NULL;}

char tile::GetChar(){
	if(GetPass()){
		if(cr!=NULL)
			return cr->GetImg();
		if(it!=NULL)
			return it->GetImg();
		return ' ';
	}else{
		return mat->GetChar();
	}
}
