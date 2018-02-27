#ifndef gameobject_h
#define gameobject_h
#include<string>

class tilewspace;
class creature;
class material;

using ushort = unsigned short;

struct vect3i{
	ushort x,y,z;
};

class gameobject{
protected:
	vect3i sizes;
	std::string name;
        char img;
	ushort hp;
	creature* owner;
	material* mat;
public:
	gameobject();
	~gameobject();

	void SetName(std::string name);
	void SetImg(char img);
	void SetMat(material* mat);
	void SetSizes(vect3i sizes);
	void GetDamage(ushort dmg);
	void SetHp(ushort hp);

	ushort GetXsize();
	ushort GetYsize();
	ushort GetZsize();
	ushort GetVolume();
	ushort GetWeight();
	ushort GetHp();
        creature* GetOwner();
	std::string GetName();
        virtual char GetImg();
	material* GetMat();
	virtual bool IsMovable();
};

class gameobjectmovable:public gameobject{
protected:
	creature* holder;
	tilewspace* place;
public:
	gameobjectmovable();
	gameobjectmovable(creature* holder);
	gameobjectmovable(tilewspace* place);
	~gameobjectmovable();

	ushort GetX();
	ushort GetY();
	ushort GetZ();
	void Move(creature* newholder);
	void Move(tilewspace* newplace);
        creature* GetHolder();
        tilewspace* GetPlace();
	bool IsMovable() override;
};

class gameobjectstatic:public gameobject{
protected:
	vect3i coords;
public:
	gameobjectstatic();
	gameobjectstatic(vect3i coords);
	gameobjectstatic(ushort x, ushort y, ushort z);
	~gameobjectstatic();

	ushort GetX();
	ushort GetY();
	ushort GetZ();
};
#endif
