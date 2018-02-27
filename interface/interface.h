#ifndef interface_h
#define interface_h
#include <vector>
class window;
class window_bordered;
class worldwindow;
class window_chat;
class window_playerstats;
class attack_dialog;
class myColor;
class myPair;
class creature;
class gameobjectmovable;
class gameobjectstatic;
class keytie;
class camera;

class interface{
	std::vector<window*> wins;
	camera* cam;
	creature* player;
	worldwindow* mainscr;
	window_chat* chat;
	window_playerstats* playerstat;
	attack_dialog* attackdialog;
	std::vector<myColor*> colors;
	std::vector<myPair*> pairs;
	std::vector<keytie*> keys;
	void ProcessWindowOnResize(window_bordered* win,short deltax, short deltay);
	void ResizeInterface(short newx, short newy);
	void DrawWin(window* win);
public:
	const int iKEY_UP=	1;
	const int iKEY_DOWN =	2;
	const int iKEY_LEFT =	3;
	const int iKEY_RIGHT =	4;
	const int iKEY_UPZ =	5;
	const int iKEY_DOWNZ =	6;
	const int iKEY_ATTACK_UP =	7;
	const int iKEY_ATTACK_DOWN =	8;
	const int iKEY_ATTACK_LEFT =	9;
	const int iKEY_ATTACK_RIGHT =	10;
	const int iKEY_PICK_UP =	11;
	const int iKEY_DROP =		12;
	const int iKEY_OPEN_INVENTORY =	13;
	const int iKEY_OPEN_CHAT =	14;
	const int iKEY_CAMERA_FLY=	15;
	const int iKEY_ESC = 	16;
	const int iKEY_ENTER = 	17;
	const int iKEY_PGUP= 	18;
	const int iKEY_PGDOWN = 19;

	const int iKEYCODE_PLAYER_MOVE_UP =	-1;
	const int iKEYCODE_PLAYER_MOVE_DOWN =	-2;
	const int iKEYCODE_PLAYER_MOVE_LEFT =	-3;
	const int iKEYCODE_PLAYER_MOVE_RIGHT =	-4;
	const int iKEYCODE_PLAYER_ATTACK_UP =	-5;
	const int iKEYCODE_PLAYER_ATTACK_DOWN =	-6;
	const int iKEYCODE_PLAYER_ATTACK_LEFT =	-7;
	const int iKEYCODE_PLAYER_ATTACK_RIGHT =-8;
	const int iKEYCODE_PLAYER_ATTACK_UPZ =	-9;
	const int iKEYCODE_PLAYER_ATTACK_DOWNZ =-10;
	const int iKEYCODE_PLAYER_PICK_UP =	-11;
	const int iKEYCODE_PLAYER_DROP=		-12;
	const int iKEYCODE_CAM_MOVE_UP =	-13;
	const int iKEYCODE_CAM_MOVE_DOWN =	-14;
	const int iKEYCODE_CAM_MOVE_LEFT =	-15;
	const int iKEYCODE_CAM_MOVE_RIGHT =	-16;
	const int iKEYCODE_CAM_MOVE_UPZ =	-17;
	const int iKEYCODE_CAM_MOVE_DOWNZ =	-18;
	const int iKEYCODE_OPEN_INVENTORY =	-19;
	const int iKEYCODE_OPEN_CHAT =		-20;
	const int iKEYCODE_ATTACK_DIALOG_OK =	-21;
	const int iKEYCODE_ATTACK_DIALOG_CLOSE =-22;

	interface();
	~interface();
	void AddKey(char key, int keycode);
	int HandleKey(char key);
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
