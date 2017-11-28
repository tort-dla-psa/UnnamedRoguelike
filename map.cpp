#include<ncurses.h>
#include"map.h"
#include"creature.h"
#include"FastNoise.h"

map::map(short a, short b, short c):
	width(a),height(b),depth(c)
{
	tiles = new tile***[c];
	for(short z=0; z<c; z++){
		tiles[z] = new tile**[a];
		for(short x=0; x<a; x++){
			tiles[z][x] = new tile*[b];
		}
	}
}
map::~map(){
	for(short i=0; i<depth; i++){
		for(short x=0; x<width; x++){
			for(short y=0; y<height ;y++){
				if(tiles[i][x][y]!=NULL)
					delete tiles[i][x][y];
			}
			delete[] tiles[i][x];
		}
		delete[] tiles[i];
	}
	delete[] tiles;
}
tile* map::GetTile(short a, short b, short c){
	if(c<0 || c>=depth)
		return NULL;
	return tiles[c][a][b];
}
void map::DelTile(tile*target){
	short x = target->GetX();
	short y = target->GetY();
	short z = target->GetZ();

	delete tiles[z][x][y];	
	tiles[z][x][y] = new tile(x,y,z,air);
}
void map::DelTile(short a, short b, short c){
	delete (tiles[c][a][b]);
	tiles[c][a][b] =  new tile(a,b,c,air);
}

short map::GetWidth(){	return width;}
short map::GetHeight(){	return height;}
short map::GetDepth(){	return depth;}

Perlin::Perlin(short w, short h, short d, std::vector<material*> mats):map(w,h,d){
	air = mats[0];
	if(!air->GetPass())
		return;
	surfstone = mats[1];
	midstone = mats[2];
	deepstone = mats[3];
	borderstone = mats[4];
	FastNoise noiseHeight,noiseBiome,noiseCavesMain,noiseCavesSecond;
	noiseHeight.SetNoiseType(FastNoise::PerlinFractal);
	noiseHeight.SetSeed(rand()%10000);
	noiseBiome.SetNoiseType(FastNoise::Perlin);
	noiseBiome.SetSeed(rand()%10000);
	noiseCavesMain.SetNoiseType(FastNoise::SimplexFractal);
	noiseCavesMain.SetSeed(rand()%10000);
	noiseCavesSecond.SetNoiseType(FastNoise::SimplexFractal);
	noiseCavesSecond.SetSeed(rand()%10000);
	float heightMap[w][h];
	//float biomeMap[w][h];
	short*** caveMap = new short**[d-1];
	float cavemainchance=-1.0+2.0*0.20;
	float cavesecondchance=-1.0+2.0*0.10;
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
			short middepth = 1+rand()%3;
			short bigdepth = middepth+d/3+rand()%3;
			for(short z=1; z<tiledepth-middepth-bigdepth; z++){
				tiles[d-1-z][x][y] = new tile(x,y,d-1-z,deepstone);
			}
			for(short z=tiledepth-middepth-bigdepth; z<tiledepth-middepth; z++){
				tiles[d-1-z][x][y] = new tile(x,y,d-1-z,midstone);
			}
			for(short z=tiledepth-middepth; z<tiledepth; z++){
				tiles[d-1-z][x][y] = new tile(x,y,d-1-z,surfstone);
			}
			for(short z=tiledepth; z<d; z++){
				tiles[d-1-z][x][y] = new tile(x,y,d-1-z,air);
			}
		}
	}
	for(short z=0;z<d-1;z++){
		caveMap[z] = new short*[w];
			for(short x=0;x<w;x++){
				caveMap[z][x] = new short[h];
					for (short y=0; y<h ;y++){
						mainNoise = noiseCavesMain.GetNoise(x,y,z);
						secondNoise = noiseCavesSecond.GetNoise(x,y,z);
						if(mainNoise<cavemainchance){
							caveMap[z][x][y] = 0;
							delete tiles[z][x][y];
							tiles[z][x][y] = new tile(x,y,z,air);
						}else if(secondNoise<cavesecondchance){
							caveMap[z][x][y] = 0;
							delete tiles[z][x][y];
							tiles[z][x][y] = new tile(x,y,z,air);
						}else{
							caveMap[z][x][y] = 1;
						}
					}
			}
	}
	
	for(short z=0; z<d-1; z++){
		for(short x=0; x<w; x++){
			delete[] caveMap[z][x];
		}
		delete[] caveMap[z];
	}
	delete[] caveMap;
}
camera::camera(creature* cr){
	this->cr = cr;
	place = NULL;
}
camera::camera(tile* place){
	this->cr = cr;
	cr = NULL;
}
camera::~camera(){
	cr=NULL;
	place=NULL;
}
void camera::SetParams(short width, short height){
	this->width=width;
	this->height=height;
}
short* camera::GetCoords(){
	if(cr)
		return new short[3]{cr->GetX(),cr->GetY(),cr->GetZ()};
	if(place)
		return new short[3]{place->GetX(),place->GetY(),place->GetZ()};
}
short camera::GetOffsetX(){
	return (cr)?	cr->GetX()-width/2:
			place->GetX()-width/2;
}
short camera::GetOffsetY(){
	return (cr)?	cr->GetY()-height/2:
			place->GetY()-height/2;
}
void camera::FollowCreature(creature* target){
	cr=target;
	place = NULL;
}
void camera::GoToPlace(tile* target){
	place = target;
	cr = NULL;
}
short camera::GetX(){	return (cr)?cr->GetX():place->GetX();}
short camera::GetY(){	return (cr)?cr->GetY():place->GetY();}
short camera::GetZ(){	return (cr)?cr->GetZ():place->GetZ();}
bool camera::Flying(){	return (cr)?false:true;}
