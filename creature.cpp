#include <algorithm>
#include "tile.h"
#include "item.h"
#include "creature.h"

creature::creature(std::string name, char img, ushort hp, ushort dp):
	gameobjectmovable(), dp(dp)
{
	inhand = NULL;
	inventorysize=10;
	sightsize=10;
	inventory = new gameobjectmovable*[inventorysize];
	for(ushort i=0; i<inventorysize; i++)
		inventory[i]=NULL;
	SetName(name);
	SetImg(img);
	SetHp(hp);
}

creature::~creature(){
	for(ushort i=0; i<inventorysize; i++)
		inventory[i]=NULL;
	delete[] inventory;
}

ushort creature::Attack(gameobject* target){
	ushort newdp=dp;
	if(inhand!=NULL){
		newdp+= inhand->GetVolume()*
			inhand->GetWeight();
	}
	target->GetDamage(newdp);
	if(target->IsMovable()){
		gameobjectmovable* temp = (gameobjectmovable*) target;
		if(target->GetHp()>0){
			if(!IsAttacked(temp)){
				attackedobjects.push_back(temp);
			}
		}else{
			if(IsAttacked(temp)){
				attackedobjects.erase(std::find(attackedobjects.begin(),attackedobjects.end(),temp),attackedobjects.end());
			}
		}
	}
	return newdp;
}

bool creature::PickUp(gameobjectmovable* target){
	for(ushort i=0; i<inventorysize; i++){
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

void creature::Take(gameobjectmovable* target){
	inhand = target;
}

void creature::Take(ushort itemplace){
	inhand = GetItem(itemplace);
}

void creature::Draw(){
	inhand=NULL;
}

void creature::Drop(ushort itemplace, tilewspace* target){
	if(inventory[itemplace]!=NULL){
		if(inventory[itemplace]==inhand)
			inhand=NULL;
		inventory[itemplace]->Move(target);
		itemcount--;
	}
}

void creature::Drop(ushort itemplace){
	if(inventory[itemplace]!=NULL){
		if(inventory[itemplace]==inhand)
			inhand=NULL;
		inventory[itemplace]->Move(place);
		itemcount--;
	}
}

void creature::Drop(gameobjectmovable* target){
	for(ushort i=0; i<inventorysize; i++){
		if(inventory[i]==target){
			inventory[i]=nullptr;
		}
	}
}

void creature::SetPath(std::vector<tilewspace*> newpath){
	path = newpath;
}

void creature::FollowPath(){
	if(!path.empty()){
		Move(path.back());
		path.pop_back();
	}
}

ushort creature::GetItemsCount(){	return itemcount;}
ushort creature::GetInvSize(){		return inventorysize;}
ushort creature::GetSightSize(){	return sightsize;}
gameobjectmovable* creature::GetItem(ushort place){	return inventory[place];}
gameobjectmovable* creature::GetInHand(){		return inhand;}
std::vector<tilewspace*> creature::GetPath(){	return path;}
bool creature::IsAttacked(gameobjectmovable* target){
	for(auto it:attackedobjects){
		if(it==target)
			return true;
	}
	return false;
}
