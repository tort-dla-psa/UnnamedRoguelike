#include<vector>
#include<algorithm>
#include<math.h>
#include"pathfinding.h"
#include"tile.h"
#include"map.h"

node::node(tile* maptile, node* parent):
	maptile(maptile),parent(parent)
{
	g = h = 0;
}

node::~node(){
	maptile=nullptr;
}

void node::Reparent(node* newparent){
	parent = newparent;
}
void node::SetG(uint newg){
	g = newg;
}
void node::SetH(uint newh){
	h = newh;
}
uint node::GetScore(){		return g + h;}
uint node::GetG(){		return g;}
uint node::GetH(){		return h;}
uint node::GetX() const {	return maptile->GetX();}
uint node::GetY() const {	return maptile->GetY();}
uint node::GetZ() const {	return maptile->GetZ();}
tile* node::GetTile(){		return maptile;}
node* node::GetParent(){	return parent;}
bool node::GetPass(){		return maptile->IsSpace();}

generator::generator(){
	directions = {
		{0,1,0},{1,0,0},{0,-1,0},{-1,0,0},
		{-1,-1,0},{1,1,0},{-1,1,0},{1,-1,0},
		{0,1,-1},{1,0,-1},{0,-1,-1},{-1,0,-1},
		{-1,-1,-1},{1,1,-1},{-1,1,-1},{1,-1,-1},
		{0,1,1},{1,0,1},{0,-1,1},{-1,0,1},
		{-1,-1,1},{1,1,1},{-1,1,1},{1,-1,1}
	};
}

node* generator::FindNodeOnList(nodeset& nodes, uint x, uint y, uint z){
	for(auto node:nodes){
		if(	node->GetX() == x &&
			node->GetY() == y &&
			node->GetZ() == z){
			return node;
		}
	}
	return nullptr;
}

bool generator::FindNodeOnList(nodeset& nodes, node target){
	for(auto node:nodes){
		if(	node->GetX() == target.GetX() &&
			node->GetY() == target.GetY() &&
			node->GetZ() == target.GetZ()){
			return true;
		}
	}
	return false;
}

uint generator::GetHeuristic(uint x, uint y, uint z, tile* finish){
	struct direction delta = {
		(x>finish->GetX())?x-finish->GetX():finish->GetX()-x,
		(y>finish->GetY())?y-finish->GetY():finish->GetY()-y,
		(z>finish->GetZ())?z-finish->GetZ():finish->GetZ()-z
	};
	return 10 * (delta.dx + delta.dy + delta.dz);
}

void generator::ReleaseNodes(nodeset& nodes){
	for(auto it = nodes.begin(); it != nodes.end();){
		delete *it;
		it = nodes.erase(it);
	}
}

std::vector<tilewspace*> generator::FindPath(map* tilesmap, tile* start, tile* finish){
	node* current = nullptr;
	nodeset openset, closedset;
	openset.insert(new node(start));

	while(!openset.empty()){
		current = *openset.begin();
		for(auto curnode:openset){
			if(curnode->GetScore() <= current->GetScore()){
				current = curnode;
			}
		}
		
		if(current->GetTile() == finish){
			break;
		}

		closedset.insert(current);
		openset.erase(std::find(openset.begin(),openset.end(),current));

		for(uint i=0; i<24; i++){
			node newnode = node(tilesmap->GetTile(
				current->GetX()+directions[i].dx,
				current->GetY()+directions[i].dy,
				current->GetZ()+directions[i].dz
			));
			if(	newnode.GetTile() == nullptr ||
				!newnode.GetPass() ||
				FindNodeOnList(closedset, newnode)){
				continue;
			}
			uint addcost = 	(i<4)?10:(
					(i<8)?14:(
					(i<12)?24:(
			  		(i<16)?28:(
					(i<20)?6:11))));
			uint totalcost = current->GetG() + addcost;

			node* successor = FindNodeOnList(openset,
							newnode.GetX(),
							newnode.GetY(),
							newnode.GetZ());
			if(successor == nullptr){
				successor = new node(tilesmap->GetTile(
						newnode.GetX(),
						newnode.GetY(),
						newnode.GetZ()
					),current);
				successor->SetG(totalcost);
				successor->SetH(GetHeuristic(
					successor->GetX(),
					successor->GetY(),
					successor->GetZ(),
					finish));
				openset.insert(successor);
			}else if(totalcost < successor->GetG()){
				successor->Reparent(current);
				successor->SetG(totalcost);
			}
		}
	}
	
	std::vector<tilewspace*> path;
	while(current!=nullptr){
		path.push_back((tilewspace*)current->GetTile());
		current = current->GetParent();
	}

	ReleaseNodes(openset);
	ReleaseNodes(closedset);

	return path;
}
