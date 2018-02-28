#include<ncurses.h>
#include<random>
#include"map.h"
#include"creature.h"
#include"FastNoise.h"
#include"material.h"

map::map(ushort width, ushort height, ushort depth):
	width(width),height(height),depth(depth)
{
	tiles = new tile***[depth];
	for(ushort z=0; z<depth; z++){
		tiles[z] = new tile**[width];
		for(ushort x=0; x<width; x++){
			tiles[z][x] = new tile*[height];
		}
	}
}

map::~map(){
	for(ushort i=0; i<depth; i++){
		for(ushort x=0; x<width; x++){
			for(ushort y=0; y<height ;y++){
				if(tiles[i][x][y])
					delete tiles[i][x][y];
			}
			delete[] tiles[i][x];
		}
		delete[] tiles[i];
	}
	delete[] tiles;

	for(auto t:coverstone){
		delete t;
	}
	coverstone.clear();
	for(auto t:surfstone){
		delete t;
	}
	surfstone.clear();
	for(auto t:midstone){
		delete t;
	}
	midstone.clear();
	for(auto t:deepstone){
		delete t;
	}
	deepstone.clear();

	delete air;
	delete borderstone;
}

tile* map::GetTile(ushort x, ushort y, ushort z){
	if(	z<0 || z>=depth || 
		x<0 || x>=width ||
		y<0 || y>=height)
		return nullptr;
	if(tiles[z][x][y]->IsSpace())
		return (tilewspace*) tiles[z][x][y];
	return tiles[z][x][y];
}

void map::DelTile(tile* target){
	ushort x = target->GetX();
	ushort y = target->GetY();
	ushort z = target->GetZ();
	item* ore = target->GetOre();
	double dropchance = target->GetChance();
	delete target;
	tiles[z][x][y] = new tilewspace(x,y,z,(tilewspace*)air);
	tilewspace* temp = (tilewspace*)tiles[z][x][y];
	if(dropchance>=10.0/(10+rand()%90))
		temp->AddObject((gameobjectmovable*)ore);
}

void map::DelTile(ushort x, ushort y, ushort z){
	item* ore = tiles[z][x][y]->GetOre();
	double dropchance = tiles[z][x][y]->GetChance();
	delete tiles[z][x][y];
	tiles[z][x][y] = new tilewspace(x,y,z,(tilewspace*)air);
	tilewspace* temp = (tilewspace*)tiles[z][x][y];
	if(dropchance>=10.0/(10+rand()%90))
		temp->AddObject((gameobjectmovable*)ore);
}

tile* map::FindTileOnVertical(ushort x, ushort y){
	ushort z=0;
	tile* temp;
	while(z<depth){
		temp = GetTile(x,y,z);
		if(!temp->IsSpace()){
			return GetTile(x,y,z-1);
		}else{
			z++;
		}
	}
	return nullptr;
}

ushort map::GetWidth(){		return width;}
ushort map::GetHeight(){	return height;}
ushort map::GetDepth(){		return depth;}

tile* Perlin::FindTile(std::string name, std::vector<tile*>tiletypes){
	for(auto element:tiletypes){
		if(element->GetMat()->GetName()==name)
			return element;
	}
	return nullptr;
}

tile* Perlin::PickRand(std::vector<tile*> tiletypes){
	std::random_device rd;
	std::mt19937 eng(rd());
	std::uniform_int_distribution<> distr(0, tiletypes.size()-1);
	return tiletypes[distr(eng)];
}

tile* Perlin::PickCover(std::vector<tile*> tiletypes){
	std::vector<tile*> temptiles;
	for(auto t:tiletypes){
		if(t->GetDepthType()==tileenums::DepthType::cover){
			temptiles.push_back(t);
		}
	}
	return PickRand(temptiles);
}

tile* Perlin::PickSurface(std::vector<tile*> tiletypes){
	std::vector<tile*> temptiles;
	for(auto t:tiletypes){
		if(t->GetDepthType()==tileenums::DepthType::surface){
			temptiles.push_back(t);
		}
	}
	return PickRand(temptiles);
}

tile* Perlin::PickMiddle(std::vector<tile*> tiletypes){
	std::vector<tile*> temptiles;
	for(auto t:tiletypes){
		if(t->GetDepthType()==tileenums::DepthType::middle){
			temptiles.push_back(t);
		}
	}
	return PickRand(temptiles);
}

tile* Perlin::PickDeep(std::vector<tile*> tiletypes){
	std::vector<tile*> temptiles;
	for(auto t:tiletypes){
		if(t->GetDepthType()==tileenums::DepthType::deep){
			temptiles.push_back(t);
		}
	}
	return PickRand(temptiles);
}

Perlin::Perlin(ushort w, ushort h, ushort d, std::vector<tile*> tiletypes):map(w,h,d){
	air = FindTile("air",tiletypes);
	borderstone = FindTile("borderstone",tiletypes);
	coverstone.push_back(PickCover(tiletypes));
	surfstone.push_back(PickSurface(tiletypes));
	midstone.push_back(PickMiddle(tiletypes));
	deepstone.push_back(PickDeep(tiletypes));
	tile* temp = FindTile("dirt",tiletypes);
	FastNoise noiseHeight,noiseBiome,noiseCavesMain,noiseCavesSecond;
	noiseHeight.SetNoiseType(FastNoise::PerlinFractal);
	noiseHeight.SetSeed(rand()%65000);
	noiseBiome.SetNoiseType(FastNoise::Perlin);
	noiseBiome.SetSeed(rand()%65000);
	noiseCavesMain.SetNoiseType(FastNoise::SimplexFractal);
	noiseCavesMain.SetSeed(rand()%65000);
	noiseCavesSecond.SetNoiseType(FastNoise::SimplexFractal);
	noiseCavesSecond.SetSeed(rand()%65000);
	float heightMap[w][h];
	//float biomeMap[w][h];
	float cavemainchance=-0.5;
	float cavesecondchance=-0.6;
	float mainNoise, secondNoise;
	for (short x=0; x<w;x++){
		for (short y=0; y<h;y++){
			heightMap[x][y] = noiseHeight.GetNoise(x,y);
		//	biomeMap[x][y] = noiseBiome.GetNoise(x,y)
		}
	}
	for(short x=0; x<w; x++){
		for(short y=0; y<h; y++){
			short tiledepth = d-d/4+heightMap[x][y]*d/4;
			tiles[d-1][x][y] = new tile(x,y,d-1,borderstone);
			short surfdepth = 1+rand()%3;
			short middepth = (tiledepth-surfdepth)/3.0;
			short bigdepth = middepth+rand()%3;
			for(short z=1; z<tiledepth-surfdepth-middepth-bigdepth; z++){
				tiles[d-1-z][x][y] = new tile(x,y,d-1-z,deepstone[0]);
				mainNoise = noiseCavesMain.GetNoise(x,y,d-1-z);
				secondNoise = noiseCavesSecond.GetNoise(x,y,d-1-z);
				if(mainNoise<cavemainchance||secondNoise<cavesecondchance){
					delete tiles[d-1-z][x][y];
					tiles[d-1-z][x][y] = new tilewspace(x,y,d-1-z,(tilewspace*)air);
				}
			}
			for(short z=tiledepth-surfdepth-middepth-bigdepth; z<tiledepth-surfdepth-middepth; z++){
				tiles[d-1-z][x][y] = new tile(x,y,d-1-z,midstone[0]);
				mainNoise = noiseCavesMain.GetNoise(x,y,d-1-z);
				secondNoise = noiseCavesSecond.GetNoise(x,y,d-1-z);
				if(mainNoise<cavemainchance||secondNoise<cavesecondchance){
					delete tiles[d-1-z][x][y];
					tiles[d-1-z][x][y] = new tilewspace(x,y,d-1-z,(tilewspace*)air);
				}
			}
			for(short z=tiledepth-surfdepth-middepth; z<tiledepth-surfdepth; z++){
				tiles[d-1-z][x][y] = new tile(x,y,d-1-z,surfstone[0]);
				mainNoise = noiseCavesMain.GetNoise(x,y,d-1-z);
				secondNoise = noiseCavesSecond.GetNoise(x,y,d-1-z);
				if(mainNoise<cavemainchance||secondNoise<cavesecondchance){
					delete tiles[d-1-z][x][y];
					tiles[d-1-z][x][y] = new tilewspace(x,y,d-1-z,(tilewspace*)air);
				}
			}
			for(short z=tiledepth-surfdepth; z<tiledepth; z++){
				tiles[d-1-z][x][y] = new tile(x,y,d-1-z,coverstone[0]);
				mainNoise = noiseCavesMain.GetNoise(x,y,d-1-z);
				secondNoise = noiseCavesSecond.GetNoise(x,y,d-1-z);
				if(mainNoise<cavemainchance||secondNoise<cavesecondchance){
					delete tiles[d-1-z][x][y];
					tiles[d-1-z][x][y] = new tilewspace(x,y,d-1-z,(tilewspace*)air);
				}
			}
			for(short z=tiledepth; z<d; z++){
				tiles[d-1-z][x][y] = new tilewspace(x,y,d-1-z,(tilewspace*)air);
			}
		}
	}
}

Perlin::~Perlin(){}
