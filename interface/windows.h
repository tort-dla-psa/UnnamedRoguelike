#ifndef windows_h
#define windows_h
#include<string>
#include<ncurses.h>
#include<menu.h>

class myColor;
class myPair;
class creature;
class gameobjectmovable;

class window{
protected:
        short width, height;
        bool updated;
        bool focused;
        WINDOW* win;
	myColor* fg;
public:
	window(short width, short height);
	virtual ~window();
        virtual void Clear();
	virtual void Draw();
        void Resize(short newx, short newy);
        void SetUpdated(bool key);
	void SetFocused(bool key);
        bool IsUpdated();
        bool IsFocused();
        short GetWidth();
        short GetHeight();
};

class worldwindow:public window{
public:
        worldwindow(short width, short heigth);
        ~worldwindow();
        using window::Draw;
        void Draw(short x, short y, char ch);
        void Draw(short x, short y, char ch, short color);
};

class window_bordered:public window{
protected:
        WINDOW* subwin;
        myPair* pair;
        short x, y;
public:
        window_bordered(short x, short y, short width, short height);
        virtual ~window_bordered();
        void Draw()override;
        void Clear()override;
        short GetX();
        short GetY();
        void MoveAt(short dx, short dy);
        void MoveTo(short x, short y);
};

class window_chat:public window_bordered{
        std::string *mes;
        short pointer;
        short chatsize;
        short mescount;
public:
        window_chat(short x, short y, short width, short height, short chatsize);
        ~window_chat();
        void Clear() override;
        void Draw() override;
        void AddMessage(std::string mes);
        void ScrollUp();
        void ScrollDown();
};

class window_playerstats:public window_bordered{
        short itemhlght;
        creature* player;
public:
        window_playerstats(short x, short y, short width, short height);
        ~window_playerstats();
        void AssignPlayer(creature* player);
        void FocusLeft();
        void FocusRight();
        void PrintStats();
        short GetHighlight();
};

class attack_dialog:public window_bordered{
        std::string message;
	std::vector<gameobjectmovable*>* targetspointer;
	ITEM** items;
	ushort itemscount;
	ITEM* focused_item;
	std::string* chars;
        std::string* strings;
	MENU* mymenu;
public:
        attack_dialog(short x, short y, short width, short height, std::vector<gameobjectmovable*> targets);
        ~attack_dialog();
        void Draw() override;
	void FocusUp();
	void FocusDown();
	gameobjectmovable* GetFocused();
};

#endif
