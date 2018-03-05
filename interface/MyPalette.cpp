#include"MyPalette.h"
#include"MyPair.h"
#include"MyColor.h"
#include<ncurses.h>

mypalette::mypalette(){
	struct vec3i{
		short r,g,b;
	}samples[8];
	samples[0].r=1000;	samples[0].g=1000;	samples[0].b=1000;
	samples[1].r=1000; 	samples[1].g=0;		samples[1].b=0;
	samples[2].r=1000; 	samples[2].g=1000;	samples[2].b=0;
	samples[3].r=0; 	samples[3].g=1000;	samples[3].b=0;
	samples[4].r=0; 	samples[4].g=1000;	samples[4].b=1000;
	samples[5].r=0; 	samples[5].g=0;		samples[5].b=1000;
	samples[6].r=1000; 	samples[6].g=0;		samples[6].b=1000;
	maxcolors = 20;	
	maxpairs = 40;
	pairscount = 0;
	colorscount = 0;
	black = AddColor(0,0,0);
	colors = new myColor**[maxcolors];
	for(short i=0;i<7;i++){
		colors[i] = new myColor*[5];
		for(short j=0;j<5;j++){
			colors[i][j] = AddColor(samples[i].r/5.0*(5-j), samples[i].g/5.0*(5-j), samples[i].b/5.0*(5-j));
		}
	}
	pairs = new myPair**[maxpairs];
	for(short i=0;i<14;i++){
		pairs[i] = new myPair*[5];
		pairscount++;
		if(i%2==0){
			for(short j=0;j<5;j++){
				pairs[i][j] = AddPair(colors[i/2][j], black);
			}
		}else{
			for(short j=0;j<5;j++){
				pairs[i][j] = AddPair(black, colors[i/2][j]);
			}
		}
	}
}

mypalette::~mypalette(){
	for(short i=0; i<colorscount; i++){
		for(short j=0; j<5; j++){
			delete colors[i][j];
		}
		delete[] colors[i];
	}
	delete[] colors;
	for(short i=0; i<pairscount; i++){
		for(short j=0; j<5; j++){
			delete pairs[i][j];
		}
		delete[] pairs[i];
	}
	delete[] pairs;
	reset_color_pairs();
}

myColor* mypalette::GetColor(short x, short y){
	if(	x>-1&&
		x<colorscount&&
		y>-1&&
		y<5)
	{
		return colors[x][y];
	}else{
		return colors[0][0];
	}
}

myColor* mypalette::GetColor(short r, short g, short b){
	for(auto c:colorstorage){
		if(c->GetR()==r&&
			c->GetG()==g&&
			c->GetB()==b)
		{
			return c;
		}
	}
	return nullptr;
}

myPair* mypalette::GetPair(short x, short y){
	if(	x>-1&&
		x<pairscount&&
		y>-1&&
		y<5)
	{
		return pairs[x][y];
	}else{
		return pairs[0][0];
	}
}

myPair* mypalette::GetPair(short r1, short g1, short b1,short r2, short g2, short b2){
	for(auto p:pairstorage){
		if(p->GetFg()->GetR()==r1&&
			p->GetFg()->GetG()==g1&&
			p->GetFg()->GetB()==b1&&
			p->GetBg()->GetR()==r2&&
			p->GetBg()->GetG()==g2&&
			p->GetBg()->GetB()==b2)
		{
			return p;
		}
	}
	return nullptr;
}

myColor* mypalette::AddColor(short r, short g, short b){
	myColor* temp = GetColor(r,g,b);
	if(temp){
		return temp;
	}else{
		colorstorage.push_back(new myColor(r,g,b,colorstorage.size()+8));
		return colorstorage.back();
	}
}

myPair* mypalette::AddPair(myColor* c1, myColor* c2){
	myPair* temp = GetPair(c1->GetR(),c1->GetG(),c1->GetB(),
				c2->GetR(),c2->GetG(),c2->GetB());
	if(temp){
		return temp;
	}else{
		pairstorage.push_back(new myPair(c1,c2,pairstorage.size()+1));
		return pairstorage.back();
	}
}

myPair* mypalette::AddToTable(myColor* c1, myColor* c2){
	myPair* temp = AddPair(c1,c2);
	for(int i=0; i<pairscount-1; i++){
		for(int j=0; j<5; j++){
			if(pairs[i][j]==temp){
				return temp;
			}
		}
	}
	short r1 = c1->GetR();
	short g1 = c1->GetG();
	short b1 = c1->GetB();
	short r2 = c2->GetR();
	short g2 = c2->GetG();
	short b2 = c2->GetB();
	pairs[pairscount] = new myPair*[5];
	for(int i=0; i<5; i++){
		myColor* newc1 = AddColor(r1/5.0*(5.0-i),g1/5.0*(5.0-i),b1/5.0*(5.0-i));
		myColor* newc2 = AddColor(r2/5.0*(5.0-i),g2/5.0*(5.0-i),b2/5.0*(5.0-i));
		pairs[pairscount][i] = AddPair(newc1,newc2);
	}
	pairscount++;
	return pairs[pairscount-1][0];
}

myPair* mypalette::GetDarker(myPair* target, short delta){
	if(delta>5||delta<0){
		return target;
	}
	for(int i=0; i<pairscount; i++){
		for(int j=0; j<5; j++){
			if(pairs[i][j]==target){
				return pairs[i][j+delta];
			}
		}
	}
}

short mypalette::GetColorsCount(){	return colorscount;}
short mypalette::GetPairsCount(){	return pairscount;}
