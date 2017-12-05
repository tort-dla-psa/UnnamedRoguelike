#include "material.h"
material::material(	short density,
			short value,
			short meltingpoint,
			std::string name):
	density(density),
	value(value),
	meltingpoint(meltingpoint),
	name(name)
{}

material::~material(){}

short material::GetDensity(){	return density;}
short material::GetValue(){	return value;}
short material::GetMeltingPoint(){	return meltingpoint;}
bool material::GetPass(){	return false;}
std::string material::GetName(){return name;}

gasMAT::gasMAT(		short density,
			short value,
			std::string name):
	material(	density,
			value,
			-1,
			name)
{}

bool gasMAT::GetPass(){	return true;}

stoneMAT::stoneMAT(	short density,
			short value,
			short meltingpoint,
			std::string name):
	material(	density,
			value,
			meltingpoint,
			name)
{}
