#ifndef mat_h
#define mat_h
#include<string>
class material{
	short density;
	short value;
	short id;
	short meltingpoint;
	std::string name;
	char img;
public:
	material(char img,short density, short value, short meltingpoint, std::string name, short id);
	~material();
	short GetDensity();
	short GetValue();
	short GetId();
	short GetMeltingPoint();
	std::string GetName();
	virtual bool GetPass();
	char GetChar();
};
class gasMAT:public material{
public:
	gasMAT(char img,short density, short value, std::string name, short id);
	bool GetPass() override;
};
class stoneMAT:public material{
public:
	stoneMAT(char img,short density, short value, short meltingpoint, std::string name, short id);
};
class organicMAT:public material{
	organicMAT(short density, short value, short meltingpoint, std::string name, short id);
};
class metalMAT:public material{
	metalMAT(short density, short value, short meltingpoint, std::string name, short id);
};
#endif
