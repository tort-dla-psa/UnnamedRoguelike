#ifndef interface_h
#define interface_h
#include <vector>
class window;
class window_bordered;
class worldwindow;
class window_chat;
class window_playerstats;
class myColor;
class myPair;
class creature;

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
	void ShowDialog(std::string mes, std::vector<std::string> variants);
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
