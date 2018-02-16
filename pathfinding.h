#ifndef path_h
#define path_h

#include<vector>
#include<functional>
#include<set>
#include"tile.h"
#include"map.h"

using uint = unsigned int;

struct direction{
	int dx, dy, dz;
};

class tile;
class node{
	tile* maptile;
	uint g,h;
	node* parent;
public:
	node(tile* maptile, node* parent = nullptr);
	~node();
	void Reparent(node* newparent);
	void SetG(uint newg);
	void SetH(uint newh);
	uint GetScore();
	uint GetG();
	uint GetH();
	uint GetX() const;
	uint GetY() const;
	uint GetZ() const;
	bool GetPass();
	tile* GetTile();
	node* GetParent();
};

using nodeset = std::set<node*>;

class generator{
	node* FindNodeOnList(nodeset& nodes, uint x, uint y, uint z);
	bool FindNodeOnList(nodeset& nodes, node target);
	void ReleaseNodes(nodeset& nodes);
	uint GetHeuristic(uint x, uint y, uint z, tile* finish);
	std::vector<direction> directions;
public:
	generator();
	std::vector<tilewspace*> FindPath(map* tilesmap, tile* start, tile* finish);
};

#endif
