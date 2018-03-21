#include<ncurses.h>
#include<random>
#include"map.h"
#include"creature.h"
#include"FastNoise.h"
#include"material.h"

tilearray::tilearray(ushort width, ushort height, ushort depth):
	width(width),height(height),depth(depth)
{
	size = width*height*depth;
	tiles = new tile*[size];
	for(unsigned int i=0;i<size;i++){
		tiles[i] = nullptr;
	}
}

tilearray::~tilearray(){
	for(unsigned int i=0; i<size; i++){
		delete tiles[i];
	}
	delete[] tiles;
}
void tilearray::SetTile(ushort x, ushort y, ushort z, tile* target){
	tiles[x+y*width+z*width*height] = target;
}
tile* tilearray::GetTile(ushort x, ushort y, ushort z){
	if(	z<0 || z>=depth || 
		x<0 || x>=width ||
		y<0 || y>=height)
		return nullptr;
	return tiles[x+y*width+z*width*height];
}
tile* tilearray::GetTileDangerous(ushort x, ushort y, ushort z){
	return tiles[x+y*width+z*width*height];
}
tile* tilearray::GetTileDangerous(unsigned int i){
	return tiles[i];
}
void tilearray::DelTile(tile* target, tilewspace* replacement){
	ushort x = target->GetX();
	ushort y = target->GetY();
	ushort z = target->GetZ();
	item* ore = target->GetOre();
	double dropchance = target->GetChance();
	delete target;
	SetTile(x,y,z, new tilewspace(x,y,z,replacement));
	tilewspace* temp = (tilewspace*)GetTile(x,y,z);
	if(dropchance>=10.0/(10+rand()%90))
		temp->AddObject((gameobjectmovable*)ore);
}
void tilearray::DelTile(ushort x, ushort y, ushort z, tilewspace* replacement){
	tile* temp = GetTile(x,y,z);
	DelTile(temp, replacement);
}
ushort tilearray::GetWidth(){		return width;}
ushort tilearray::GetHeight(){		return height;}
ushort tilearray::GetDepth(){		return depth;}

map::map(ushort width, ushort height, ushort depth):
	tilearray(width,height,depth)
{
	revealed = new bool[size];
	visible = new bool[size];
	for(unsigned int i=0;i<size;i++){
		revealed[i] = false;
		visible[i] = false;
	}
}

map::~map(){
	delete[] revealed;
	delete[] visible;
}
bool map::GetRevealed(tile* place){
	return revealed[place->GetX()+
			place->GetY()*width+
			place->GetZ()*width*height];
}
bool map::GetVisible(tile* place){
	return visible[place->GetX()+
			place->GetY()*width+
			place->GetZ()*width*height];
}
void map::SetRevealed(tile* place, bool key){
	revealed[place->GetX()+
			place->GetY()*width+
			place->GetZ()*width*height] = key;
}
void map::SetVisible(tile* place, bool key){
	visible[place->GetX()+
			place->GetY()*width+
			place->GetZ()*width*height] = key;
}
void map::SetRevealed(tilearray* sphere,bool key){
	ushort w = sphere->GetWidth();
	ushort h = sphere->GetHeight();
	ushort d = sphere->GetDepth();
	unsigned int size = w*h*d;
	for(unsigned int i=0; i<size; i++){
		tile* temp = sphere->GetTileDangerous(i);
		if(!temp){
			continue;
		}
		SetRevealed(temp,key);
	}
	SetVisible(sphere, key);
}
void map::SetVisible(tilearray* sphere,bool key){
	ushort w = sphere->GetWidth();
	ushort h = sphere->GetHeight();
	ushort d = sphere->GetDepth();
	unsigned int size = w*h*d;
	for(unsigned int i=0; i<size; i++){
		tile* temp = sphere->GetTileDangerous(i);
		if(!temp){
			continue;
		}
		SetVisible(temp,key);
	}
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


tile* map::CastRay(tile* start, tile* end){
	short x0 = start->GetX();
	short y0 = start->GetY();
	short z0 = start->GetZ();
	short x1 = end->GetX();
	short y1 = end->GetY();
	short z1 = end->GetZ();

	short dx = abs(x0-x1),
		sx = x0<x1?1:x0==x1?0:-1;
	short dy = abs(y0-y1),
		sy = y0<y1?1:y0==y1?0:-1;
	short dz = abs(z0-z1),
		sz = z0<z1?1:z0==z1?0:-1;

	short dx2 = dx<<1;
	short dy2 = dy<<1;
	short dz2 = dz<<1;

	short err1, err2;
	short d = std::max(std::max(dx,dy),dz);

	tile* temptile = start;
	if(dx==d){
		err1 = dy2 - dx;
		err2 = dz2 - dx;
		for(int i=0; i<dx; i++){
			temptile = GetTileDangerous(x0,y0,z0);
			if(!temptile->IsSpace()){
				return temptile;
			}
			if(err1>0){
				y0+=sy;
				err1-=dx2;
			}
			if(err2>0){
				z0+=sz;
				err2-=dx2;
			}
			err1+=dy2;
			err2+=dz2;
			x0+=sx;
		}
	}else if(dy==d){
		err1 = dx2 - dy;
		err2 = dz2 - dy;
		for(int i=0; i<dy; i++){
			temptile = GetTileDangerous(x0,y0,z0);
			if(!temptile->IsSpace()){
				return temptile;
			}
			if(err1>0){
				x0+=sx;
				err1-=dy2;
			}
			if(err2>0){
				z0+=sz;
				err2-=dy2;
			}
			err1+=dx2;
			err2+=dz2;
			y0+=sy;
		}
	}else{
		err1 = dy2 - dz;
		err2 = dx2 - dz;
		for(int i=0; i<dz; i++){
			temptile = GetTileDangerous(x0,y0,z0);
			if(!temptile->IsSpace()){
				return temptile;
			}
			if(err1>0){
				y0+=sy;
				err1-=dz2;
			}
			if(err2>0){
				x0+=sx;
				err2-=dz2;
			}
			err1+=dy2;
			err2+=dx2;
			z0+=sz;
		}
	}
	return end;
}

tilearray* map::GetSphere(tile* center, ushort radius){
	ushort cx = center->GetX();
	ushort cy = center->GetY();
	ushort cz = center->GetZ();
	ushort x_positive_dist = (cx+radius>=width)? width-1: cx+radius,
	       x_negative_dist = (cx-radius<0)? 0: cx-radius, 
	       y_positive_dist = (cy+radius>=height)? height-1: cy+radius,
	       y_negative_dist = (cy-radius<0)? 0: cy-radius,
	       z_positive_dist = (cz+5>=depth)? depth-1: cz+5,
	       z_negative_dist = (cz-5<0)? 0: cz-5;
	tilearray* sphere = new tilearray(abs(x_positive_dist-x_negative_dist),
				abs(y_positive_dist-y_negative_dist),
				abs(z_positive_dist-z_negative_dist));
	sphere->SetTile(cx-x_negative_dist,cy-y_negative_dist,cz-z_negative_dist,center);
	for(int i=x_negative_dist; i<x_positive_dist; i++){
		for(int j=y_negative_dist; j<y_positive_dist; j++){
			for(int k=z_negative_dist; k<z_positive_dist; k++){
				short dx = i-cx;
				short dy = j-cy;
				short dz = k-cz;
				short distxy = sqrt(dx*dx+dy*dy);
				short dist = sqrt(distxy*distxy+dz*dz);
				if(dist>radius)	continue;
				tile* t = GetTileDangerous(i,j,k);
				t = CastRay(center,t);
				short tx = t->GetX();
				short ty = t->GetY();
				short tz = t->GetZ();
				if(!t->IsSpace()||(((tilewspace*)t)->HasObjects())){
					sphere->SetTile(tx-x_negative_dist,
						ty-y_negative_dist,
						tz-z_negative_dist,
						t);
					continue;
				}
				if(k+1>=z_positive_dist){
					continue;
				}
				tile* t2 = GetTileDangerous(tx,ty,tz+1);
				if(t2->IsSpace()){
					continue;
				}
				sphere->SetTile(tx-x_negative_dist,
					ty-y_negative_dist,
					tz+1-z_negative_dist,
					t2);
			}
		}
	}
	return sphere;
}
void map::DelTile(tile* target){
	tilearray::DelTile(target, (tilewspace*)air);
}
void map::DelTile(ushort x, ushort y, ushort z){
	tile* temp = GetTile(x,y,z);
	tilearray::DelTile(temp, (tilewspace*) air);
}

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
			SetTile(x,y,d-1,new tile(x,y,d-1,borderstone));
			short surfdepth = 1+rand()%3;
			short middepth = (tiledepth-surfdepth)/3.0;
			short bigdepth = middepth+rand()%3;
			for(short z=1; z<tiledepth-surfdepth-middepth-bigdepth; z++){
				SetTile(x,y,d-1-z,new tile(x,y,d-1-z,deepstone[0]));
				mainNoise = noiseCavesMain.GetNoise(x,y,d-1-z);
				secondNoise = noiseCavesSecond.GetNoise(x,y,d-1-z);
				if(mainNoise<cavemainchance||secondNoise<cavesecondchance){
					delete GetTile(x,y,d-1-z);
					SetTile(x,y,d-1-z,new tilewspace(x,y,d-1-z,(tilewspace*)air));
				}
			}
			for(short z=tiledepth-surfdepth-middepth-bigdepth; z<tiledepth-surfdepth-middepth; z++){
				SetTile(x,y,d-1-z,new tile(x,y,d-1-z,midstone[0]));
				mainNoise = noiseCavesMain.GetNoise(x,y,d-1-z);
				secondNoise = noiseCavesSecond.GetNoise(x,y,d-1-z);
				if(mainNoise<cavemainchance||secondNoise<cavesecondchance){
					delete GetTile(x,y,d-1-z);
					SetTile(x,y,d-1-z,new tilewspace(x,y,d-1-z,(tilewspace*)air));
				}
			}
			for(short z=tiledepth-surfdepth-middepth; z<tiledepth-surfdepth; z++){
				SetTile(x,y,d-1-z,new tile(x,y,d-1-z,surfstone[0]));
				mainNoise = noiseCavesMain.GetNoise(x,y,d-1-z);
				secondNoise = noiseCavesSecond.GetNoise(x,y,d-1-z);
				if(mainNoise<cavemainchance||secondNoise<cavesecondchance){
					delete GetTile(x,y,d-1-z);
					SetTile(x,y,d-1-z,new tilewspace(x,y,d-1-z,(tilewspace*)air));
				}
			}
			for(short z=tiledepth-surfdepth; z<tiledepth; z++){
				SetTile(x,y,d-1-z,new tile(x,y,d-1-z,coverstone[0]));
				mainNoise = noiseCavesMain.GetNoise(x,y,d-1-z);
				secondNoise = noiseCavesSecond.GetNoise(x,y,d-1-z);
				if(mainNoise<cavemainchance||secondNoise<cavesecondchance){
					delete GetTile(x,y,d-1-z);
					SetTile(x,y,d-1-z,new tilewspace(x,y,d-1-z,(tilewspace*)air));
				}
			}
			for(short z=tiledepth; z<d; z++){
				SetTile(x,y,d-1-z,new tilewspace(x,y,d-1-z,(tilewspace*)air));
			}
		}
	}
}

Perlin::~Perlin(){
	/*
	for(auto t:coverstone){
		if(t)
		delete t;
		t=nullptr;
	}
	coverstone.clear();
	for(auto t:surfstone){
		delete t;
		t=nullptr;
	}
	surfstone.clear();
	for(auto t:midstone){
		delete t;
		t=nullptr;
	}
	midstone.clear();
	for(auto t:deepstone){
		delete t;
		t=nullptr;
	}
	deepstone.clear();
	*/
}
