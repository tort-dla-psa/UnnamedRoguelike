#ifndef interface_h
#define interface_h
#include <string>
#include <vector>
#include <ncurses.h>
#include "tile.h"
#include "item.h"
class creature;

class myColor{
        short r,g,b,num;
        std::string name;
public:
        myColor(short r, short g, short b, std::string name, short num);
        myColor();
        ~myColor();
        std::string GetName();
        short GetNum();
        short GetR();
        short GetG();
        short GetB();
};

class myPair{
        myColor* fg;
        myColor* bg;
	short num;
public:
        myPair(myColor* fg, myColor* bg, short num);
        myPair();
        ~myPair();
	myColor* GetFg();
	myColor* GetBg();
};

class window{
protected:
	short width, height;
	bool updated;
	bool focused;
	WINDOW* win;
	myColor* fg;
public:
	window(short width, short height);
	~window();
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
	myPair* focusedpair;
	short x, y;
	bool luanch,
	     ldanch,
	     ruanch,
	     rdanch;
public:
	window_bordered(short x, short y, short width, short height);
	~window_bordered();
	void Draw()override;
	void Clear()override;
	void SetLUAnch(bool key);
	void SetLDAnch(bool key);
	void SetRUAnch(bool key);
	void SetRDAnch(bool key);
	short GetX();
	short GetY();
	void MoveAt(short dx, short dy);
	void MoveTo(short x, short y);
	bool GetLUAnch();
	bool GetLDAnch();
	bool GetRUAnch();
	bool GetRDAnch();
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

class listline{
	std::string mes;
	short num;
public:
	listline(std::string mes, short num);
	~listline();
	void ChangeNum(short num);
	short GetNum();
	std::string GetMes();
};

class list{
	std::vector<listline> lines;
	short focus;
public:
	list();
	~list();
	void Add(std::string mes);
	void Remove(short pos);
	std::string GetMes(short pos);
	short GetFocus();
	short GetSize();
};

class window_dialog:public window_bordered{
	list messages;
public:
	window_dialog();
	void AddOption(std::string opt);
	void RemoveOption(short place);
};

class interface{
	worldwindow* mainscr;
	window_chat* chat;
	window_playerstats* playerstat;
	std::vector<myColor*> colors;
	std::vector<myPair*> pairs;
	void ProcessWindowOnResize(window_bordered* win,short deltax, short deltay);
	void ResizeInterface(short newx, short newy);
	void DrawWin(window* win);
public:
	interface();
	~interface();
	void AssignPlayer(creature* player);
	void CheckResize();
	void Draw();
	void ClearMap();
	void DrawOnMap(short x,short y,char ch);
	void DrawOnMap(short x,short y,char ch, short color);
	void ShowPlayerStat();
	void WriteToChat(std::string text);
	void ClearChat();
	void ChatScrollUp();
	void ChatScrollDown();
	void SetChatFocus(bool key);
	bool GetChatFocus();
	void SetHighlight(short hlght);
	void SetInventoryFocus(bool key);
	void InventoryFocusLeft();
	void InventoryFocusRight();
	bool GetInventoryFocus();
	short GetWidth();
	short GetHeight();
	short GetHighlight();
};
#endif
