#ifndef interface_h
#define interface_h
#include <vector>
#include"iConstInt.h"
class window;
class window_bordered;
class mainmenu;
class worldwindow;
class window_chat;
class window_playerstats;
class attack_dialog;
class myColor;
class myPair;
class creature;
class gameobjectmovable;
class gameobjectstatic;
class camera;

class keytie{
	char key;
	iConstInt returncode;
public:
	keytie(char key, iConstInt returncode);
	~keytie();
	void SetKey(char key);
	void SetCode(iConstInt code);
	char GetChar();
	iConstInt GetCode();
};

class interface{
	std::vector<window*> wins;
	mainmenu* menu;
	camera* cam;
	creature* player;
	worldwindow* mainscr;
	window_chat* chat;
	window_playerstats* playerstat;
	attack_dialog* attackdialog;
	std::vector<myColor*> colors;
	std::vector<myPair*> pairs;
	std::vector<keytie*> keys;
	short width,height;
	int pastkey;
	int pastkeycode;
	void ProcessWindowOnResize(window_bordered* win,short deltax, short deltay);
	void ResizeInterface(short newx, short newy);
	void InitKeys();
	void InitKeycodes();
	int GetNextKey();
	int GetNextKeycode();
	void OpenGameInterface();
public:
	iConstInt iKEY_UP;
	iConstInt iKEY_DOWN;
	iConstInt iKEY_LEFT;
	iConstInt iKEY_RIGHT;
	iConstInt iKEY_UPZ;
	iConstInt iKEY_DOWNZ;
	iConstInt iKEY_ATTACK_UP;
	iConstInt iKEY_ATTACK_DOWN;
	iConstInt iKEY_ATTACK_LEFT;
	iConstInt iKEY_ATTACK_RIGHT;
	iConstInt iKEY_PICK_UP;
	iConstInt iKEY_DROP;
	iConstInt iKEY_OPEN_INVENTORY;
	iConstInt iKEY_OPEN_CHAT;
	iConstInt iKEY_CAMERA_FLY;
	iConstInt iKEY_ESC;
	iConstInt iKEY_ENTER;
	iConstInt iKEY_PGUP;
	iConstInt iKEY_PGDOWN;

	iConstInt iKEYCODE_PLAYER_MOVE_UP;
	iConstInt iKEYCODE_PLAYER_MOVE_DOWN;
	iConstInt iKEYCODE_PLAYER_MOVE_LEFT;
	iConstInt iKEYCODE_PLAYER_MOVE_RIGHT;
	iConstInt iKEYCODE_PLAYER_ATTACK_UP;
	iConstInt iKEYCODE_PLAYER_ATTACK_DOWN;
	iConstInt iKEYCODE_PLAYER_ATTACK_LEFT;
	iConstInt iKEYCODE_PLAYER_ATTACK_RIGHT;
	iConstInt iKEYCODE_PLAYER_ATTACK_UPZ;
	iConstInt iKEYCODE_PLAYER_ATTACK_DOWNZ;
	iConstInt iKEYCODE_PLAYER_PICK_UP;
	iConstInt iKEYCODE_PLAYER_DROP;
	iConstInt iKEYCODE_CAM_MOVE_UP;
	iConstInt iKEYCODE_CAM_MOVE_DOWN;
	iConstInt iKEYCODE_CAM_MOVE_LEFT;
	iConstInt iKEYCODE_CAM_MOVE_RIGHT;
	iConstInt iKEYCODE_CAM_MOVE_UPZ;
	iConstInt iKEYCODE_CAM_MOVE_DOWNZ;
	iConstInt iKEYCODE_OPEN_INVENTORY;
	iConstInt iKEYCODE_OPEN_CHAT;
	iConstInt iKEYCODE_ATTACK_DIALOG_OK;
	iConstInt iKEYCODE_ATTACK_DIALOG_CLOSE;
	iConstInt iKEYCODE_MAINMENU_NEWGAME;
	iConstInt iKEYCODE_MAINMENU_QUIT;

	interface();
	~interface();
	iConstInt ShowMainMenu();
	void AddKey(char key, iConstInt keycode);
	iConstInt HandleKey(char key);
	void AssignPlayer(creature* player);
	void CheckResize();
	void Draw();
	void ClearMap();
	void DrawOnMap(short x,short y,char ch);
	void DrawOnMap(short x,short y,char ch, short color);
	void SetFocus(window* win);
	
	void ShowAttackDialog(std::vector<gameobjectmovable*> targets);
	bool GetAttackDialogFocus();
	void AttackDialogUp();
	void AttackDialogDown();
	gameobjectmovable* GetAttackDialogTarget();
	void AttackDialogClose();

	void WriteToChat(std::string text);
	void ClearChat();
	void ChatScrollUp();
	void ChatScrollDown();
	bool GetChatFocus();
	void SetHighlight(short hlght);
	void InventoryFocusLeft();
	void InventoryFocusRight();
	bool GetInventoryFocus();
	short GetWidth();
	short GetHeight();
	short GetHighlight();

	short GetCamX();
	short GetCamY();
	short GetCamZ();
	short GetCamOffsetX();
	short GetCamOffsetY();
	void CamFollow(gameobjectmovable* target);
	void CamFollow(gameobjectstatic* target);
	bool CamFlying();
	void SetCamParameters(short width, short height);
};
#endif
