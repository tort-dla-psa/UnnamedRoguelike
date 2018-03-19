#ifndef palette_h
#define palette_h
#include<vector>
class myColor;
class myPair;

class mypalette{
	myColor*** colors;
	myPair*** pairs;
	myColor* black;
	std::vector<myPair*> pairstorage;
	std::vector<myColor*> colorstorage;
	myPair** whites;
	short maxcolors;
	short maxpairs;
	short colorscount;
	short pairscount;
public:
	mypalette();
	~mypalette();
	myColor* GetColor(short x, short y);
	myColor* GetColor(short r, short g, short b);
	myPair* GetPair(short x,short y);
	myPair* GetPair(short r1, short g1, short b1, short r2, short g2, short b2);
	myColor* AddColor(short r, short g, short b);
	myPair* AddPair(myColor* c1, myColor* c2);
	myPair* AddToTable(myColor* c1, myColor* c2);
	myPair* GetDarker(myPair* target, short delta);
	myPair* GetWhite(short delta);
	short GetColorsCount();
	short GetPairsCount();
};
#endif
