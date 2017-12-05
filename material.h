#ifndef mat_h
#define mat_h
#include<string>
class material{
	short density;
	short value;
	short meltingpoint;
	std::string name;
public:
	material(short density, short value, short meltingpoint, std::string name);
	~material();
	short GetDensity();
	short GetValue();
	short GetMeltingPoint();
	std::string GetName();
	virtual bool GetPass();
};
class gasMAT:public material{
public:
	gasMAT(short density, short value, std::string name);
	bool GetPass() override;
};
class stoneMAT:public material{
public:
	stoneMAT(short density, short value, short meltingpoint, std::string name);
};
class organicMAT:public material{
	organicMAT(short density, short value, short meltingpoint, std::string name);
};
class metalMAT:public material{
	metalMAT(short density, short value, short meltingpoint, std::string name);
};
#endif
