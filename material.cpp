#include "material.h"
material::material(	short density,
			short meltingpoint,
			std::string name):
	density(density),
	meltingpoint(meltingpoint),
	name(name)
{}

material::~material(){}

short material::GetDensity(){	return density;}
short material::GetMeltingPoint(){	return meltingpoint;}
bool material::GetPass(){	return false;}
std::string material::GetName(){return name;}

gasMAT::gasMAT(short density,std::string name):
	material(density,-1,name)
{}

gasMAT::~gasMAT(){}

bool gasMAT::GetPass(){	return true;}

stoneMAT::stoneMAT(short density,short meltingpoint,std::string name):
	material(density,meltingpoint,name)
{}

stoneMAT::~stoneMAT(){}
