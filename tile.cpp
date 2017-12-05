#include "tile.h"
#include "creature.h"
#include "item.h"
#include <ncurses.h>

tiletype::tiletype(char img, double dropchance, material* mat):
	img(img),dropchance(dropchance),mat(mat){}

tiletype::~tiletype(){}

material* tiletype::GetMat(){	return mat;}
char tiletype::GetImg(){	return img;}
double tiletype::GetChance(){	return dropchance;}

tile::tile(short x,short y,short z,tiletype* idea):
	x(x),y(y),z(z),idea(idea),hp(100){}

tile::~tile(){};

void tile::GetDamage(short dmg){
	hp-=dmg;
}

std::string tile::GetName(){	return idea->GetMat()->GetName();}
bool tile::GetPass(){		return idea->GetMat()->GetPass();}
bool tile::IsSpace(){		return (idea->GetMat()->GetPass())?true:false;}
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
		return idea->GetImg();
	}
}
