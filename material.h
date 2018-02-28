#ifndef mat_h
#define mat_h
#include<string>
class material{
	short density;
	short meltingpoint;
	std::string name;
public:
	material(short density, short meltingpoint, std::string name);
	virtual ~material();
	short GetDensity();
	short GetMeltingPoint();
	std::string GetName();
	virtual bool GetPass();
};
class gasMAT:public material{
public:
	gasMAT(short density, std::string name);
	~gasMAT();
	bool GetPass() override;
};
class stoneMAT:public material{
public:
	stoneMAT(short density, short meltingpoint, std::string name);
	~stoneMAT();
};
class organicMAT:public material{
	organicMAT(short density, short meltingpoint, std::string name);
	~organicMAT();
};
class metalMAT:public material{
	metalMAT(short density, short meltingpoint, std::string name);
	~metalMAT();
};
#endif
