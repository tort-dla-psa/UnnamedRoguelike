#include "tile.h"
#include "item.h"
#include "creature.h"

const short dmgcoef = 1;

creature::creature(std::string name, char img, short hp, short dp):
	name(name), img(img), hp(hp), dp(dp)
{
	place = NULL;
	inhand = NULL;
	inventorysize=10;
	sightsize=10;
	inventory = new item*[inventorysize];
	for(short i=0; i<inventorysize; i++)
		inventory[i]=NULL;
}

creature::~creature(){
	if(place!=NULL)
		place->PlaceCreature(NULL);
}

short creature::Attack(creature* target){
	target->GetDamage(dp);
	return dp;
}

short creature::Dig(tile* target){
	short newdp=dp;
	if(inhand!=NULL){
		newdp+= inhand->GetVolume()*
			inhand->GetWeight()*
			inhand->GetSharpness();
	}
	target->GetDamage(newdp);
	return newdp;
}

void creature::GetDamage(short dp){	
	hp-=dp;
}

void creature::Move(tile* place){
	if(this->place!=NULL)
		this->place->PlaceCreature(NULL);	
	this->place=place;
	if(place!=NULL)
		this->place->PlaceCreature(this);
}

bool creature::PickUp(item* target){
	for(short i=0; i<inventorysize; i++){
		if(inventory[i]==NULL){
			inventory[i] = target;
			if(inhand==NULL)
				Take(target);
			itemcount++;
			return true;
		}
	}
	return false;
}

void creature::Take(item* target){
	inhand = target;
}
void creature::Take(short itemplace){
	inhand = GetItem(itemplace);
}
void creature::Draw(){
	inhand=NULL;
}

void creature::Drop(short itemplace, tile* target){
	if(inventory[itemplace]!=NULL){
		if(inventory[itemplace]==inhand)
			inhand=NULL;
		inventory[itemplace]->Transfer(NULL);
		inventory[itemplace]->Move(target);
		inventory[itemplace]=NULL;
		itemcount--;
	}
}
void creature::Drop(short itemplace){
	if(inventory[itemplace]!=NULL){
		if(inventory[itemplace]==inhand)
			inhand=NULL;
		inventory[itemplace]->Transfer(NULL);
		inventory[itemplace]->Move(place);
		inventory[itemplace]=NULL;
		itemcount--;
	}
}
std::string creature::GetName(){	return name;}
tile* creature::GetPlace(){		return place;}
char creature::GetImg(){		return img;}
short creature::GetItemsCount(){	return itemcount;}
short creature::GetInvSize(){		return inventorysize;}
short creature::GetSightSize(){		return sightsize;}
short creature::GetHp(){		return hp;}
short creature::GetX(){			return place->GetX();}
short creature::GetY(){			return place->GetY();}
short creature::GetZ(){			return place->GetZ();}
item* creature::GetItem(short place){	return inventory[place];}
item* creature::GetInHand(){		return inhand;}
