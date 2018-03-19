class gameobjectmovable;
class gameobjectstatic;

using ushort = unsigned short;

class camera{
        gameobjectmovable* targetmov;
        gameobjectstatic* targetstat;
        ushort width, height;
public:
        camera(gameobjectmovable* target);
        camera(gameobjectstatic* target);
        ~camera();
        bool Flying();
        void Follow(gameobjectmovable* newtarget);
        void Follow(gameobjectstatic* newtarget);
        void SetParams(ushort width, ushort height);
        ushort GetX();
        ushort GetY();
        ushort GetZ();
	gameobjectstatic* GetPlace();
        ushort GetOffsetX();
        ushort GetOffsetY();
};

