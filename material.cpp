#include "material.h"
material::material(	char img,
			short density,
			short value,
			short meltingpoint,
			std::string name,
			short id):
	img(img),
	density(density),
	value(value),
	meltingpoint(meltingpoint),
	name(name),
	id(id)
{}

material::~material(){}

short material::GetDensity(){	return density;}
short material::GetValue(){	return value;}
short material::GetId(){	return id;}
short material::GetMeltingPoint(){	return meltingpoint;}
bool material::GetPass(){	return false;}
std::string material::GetName(){return name;}
char material::GetChar(){	return img;}

gasMAT::gasMAT(		char img,
			short density,
			short value,
			std::string name,
			short id):
	material(	img,
			density,
			value,
			-1,
			name,
			id)
{}

bool gasMAT::GetPass(){	return true;}

stoneMAT::stoneMAT(	char img,
			short density,
			short value,
			short meltingpoint,
			std::string name,
			short id):
	material(	img,
			density,
			value,
			meltingpoint,
			name,
			id)
{}
