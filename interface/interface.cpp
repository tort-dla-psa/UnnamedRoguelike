#include<ncurses.h>
#include<string>
#include<algorithm>
#include"interface.h"
#include"MyColor.h"
#include"MyPair.h"
#include"windows.h"
#include"camera.h"
#include"../gameobject.h"
#include"../creature.h"
#include"../map.h"
#include"MyPalette.h"

interface::~interface(){
	delete playerstat,
	       chat,
	       mainscr;
	delete cam;
	delete palette;
	wins.clear();
	clear();
	endwin();
}

interface::interface(){
	initscr();
	noecho();
	curs_set(FALSE);
	if(has_colors()==false){
		endwin();
		exit(1);
	}
	start_color();
	palette = new mypalette();
	getmaxyx(stdscr,height,width);
	keypad(stdscr,true);
	pastkey = pastkeycode = 0;
	InitKeys();
	InitKeycodes();
	wrefresh(stdscr);
}

void interface::OpenGameInterface(){
	mainscr = new worldwindow(width,height+1);
	playerstat = new window_playerstats(width/6, height-10, width/6, 10, palette->GetPair(0,9),palette->GetPair(0,0));
	chat = new window_chat(0,height-10,width/6,10,50, palette->GetPair(0,9),palette->GetPair(0,0));
	attackdialog = nullptr;
	wins.push_back(mainscr);
	wins.push_back(playerstat);
	wins.push_back(chat);
	SetFocus(mainscr);
}
int interface::GetNextKey(){		return ++pastkey;}
int interface::GetNextKeycode(){	return --pastkeycode;}
iConstInt interface::GetKeycode(char ch){
	iConstInt code;
	try{
		code = keyties.at(ch);
	}catch(const std::out_of_range &e){
		return iKEYCODE_NOTHING;
	}
	return code.GetCode();
}
void interface::InitKeys(){
	iKEY_UP=	iConstInt(GetNextKey());
	iKEY_DOWN=	iConstInt(GetNextKey());
	iKEY_LEFT=	iConstInt(GetNextKey());
	iKEY_RIGHT=	iConstInt(GetNextKey());
	iKEY_UPZ=	iConstInt(GetNextKey());
	iKEY_DOWNZ=	iConstInt(GetNextKey());
	iKEY_UPLEFT=	iConstInt(GetNextKey());
	iKEY_UPRIGHT=	iConstInt(GetNextKey());
	iKEY_DOWNLEFT=	iConstInt(GetNextKey());
	iKEY_DOWNRIGHT=	iConstInt(GetNextKey());
	iKEY_ATTACK_UP=	iConstInt(GetNextKey());
	iKEY_ATTACK_DOWN=	iConstInt(GetNextKey());
	iKEY_ATTACK_LEFT=	iConstInt(GetNextKey());
	iKEY_ATTACK_RIGHT=	iConstInt(GetNextKey());
	iKEY_PICK_UP=	iConstInt(GetNextKey());
	iKEY_DROP=	iConstInt(GetNextKey());
	iKEY_OPEN_INVENTORY=	iConstInt(GetNextKey());
	iKEY_OPEN_CHAT=	iConstInt(GetNextKey());
	iKEY_CAMERA_FLY=	iConstInt(GetNextKey());
	iKEY_ESC=	iConstInt(GetNextKey());
	iKEY_ENTER=	iConstInt(GetNextKey());
	iKEY_PGUP=	iConstInt(GetNextKey());
	iKEY_PGDOWN=	iConstInt(GetNextKey());
}
void interface::InitKeycodes(){
	iKEYCODE_NOTHING=	iConstInt(GetNextKeycode());
	iKEYCODE_PLAYER_MOVE_UP=	iConstInt(GetNextKeycode());
	iKEYCODE_PLAYER_MOVE_DOWN=	iConstInt(GetNextKeycode());
	iKEYCODE_PLAYER_MOVE_LEFT=	iConstInt(GetNextKeycode());
	iKEYCODE_PLAYER_MOVE_RIGHT=	iConstInt(GetNextKeycode());
	iKEYCODE_PLAYER_MOVE_UPZ=	iConstInt(GetNextKeycode());
	iKEYCODE_PLAYER_MOVE_DOWNZ=	iConstInt(GetNextKeycode());
	iKEYCODE_PLAYER_MOVE_UPLEFT=	iConstInt(GetNextKeycode());
	iKEYCODE_PLAYER_MOVE_UPRIGHT=	iConstInt(GetNextKeycode());
	iKEYCODE_PLAYER_MOVE_DOWNLEFT=	iConstInt(GetNextKeycode());
	iKEYCODE_PLAYER_MOVE_DOWNRIGHT=	iConstInt(GetNextKeycode());
	iKEYCODE_PLAYER_ATTACK_UP=	iConstInt(GetNextKeycode());
	iKEYCODE_PLAYER_ATTACK_DOWN=	iConstInt(GetNextKeycode());
	iKEYCODE_PLAYER_ATTACK_LEFT=	iConstInt(GetNextKeycode());
	iKEYCODE_PLAYER_ATTACK_RIGHT=	iConstInt(GetNextKeycode());
	iKEYCODE_PLAYER_ATTACK_UPZ=	iConstInt(GetNextKeycode());
	iKEYCODE_PLAYER_ATTACK_DOWNZ=	iConstInt(GetNextKeycode());
	iKEYCODE_PLAYER_ATTACK_UPLEFT=	iConstInt(GetNextKeycode());
	iKEYCODE_PLAYER_ATTACK_UPRIGHT=	iConstInt(GetNextKeycode());
	iKEYCODE_PLAYER_ATTACK_DOWNLEFT=	iConstInt(GetNextKeycode());
	iKEYCODE_PLAYER_ATTACK_DOWNRIGHT=	iConstInt(GetNextKeycode());
	iKEYCODE_PLAYER_PICK_UP=	iConstInt(GetNextKeycode());
	iKEYCODE_PLAYER_DROP=	iConstInt(GetNextKeycode());
	iKEYCODE_CAM_MOVE_UP=	iConstInt(GetNextKeycode());
	iKEYCODE_CAM_MOVE_DOWN=	iConstInt(GetNextKeycode());
	iKEYCODE_CAM_MOVE_LEFT=	iConstInt(GetNextKeycode());
	iKEYCODE_CAM_MOVE_RIGHT=	iConstInt(GetNextKeycode());
	iKEYCODE_CAM_MOVE_UPZ=	iConstInt(GetNextKeycode());
	iKEYCODE_CAM_MOVE_DOWNZ=	iConstInt(GetNextKeycode());
	iKEYCODE_OPEN_INVENTORY=	iConstInt(GetNextKeycode());
	iKEYCODE_OPEN_CHAT=	iConstInt(GetNextKeycode());
	iKEYCODE_ATTACK_DIALOG_OK=	iConstInt(GetNextKeycode());
	iKEYCODE_ATTACK_DIALOG_CLOSE=	iConstInt(GetNextKeycode());
	iKEYCODE_MAINMENU_NEWGAME=	iConstInt(GetNextKeycode());
	iKEYCODE_MAINMENU_QUIT=	iConstInt(GetNextKeycode());
}
void interface::AddKey(char key, iConstInt keycode){
	keyties[key] = keycode;
}
iConstInt interface::HandleKey(char key){
	iConstInt code = GetKeycode(key);
	if(mainscr->IsFocused()){
		if(!cam->Flying()){
			if(code==iKEY_UP){
				return iKEYCODE_PLAYER_MOVE_UP;
			}else if(code==iKEY_DOWN){
				return iKEYCODE_PLAYER_MOVE_DOWN;
			}else if(code==iKEY_LEFT){
				return iKEYCODE_PLAYER_MOVE_LEFT;
			}else if(code==iKEY_RIGHT){
				return iKEYCODE_PLAYER_MOVE_RIGHT;
			}else if(code==iKEY_UPLEFT){
				return iKEYCODE_PLAYER_MOVE_UPLEFT;
			}else if(code==iKEY_UPRIGHT){
				return iKEYCODE_PLAYER_MOVE_UPRIGHT;
			}else if(code==iKEY_DOWNLEFT){
				return iKEYCODE_PLAYER_MOVE_DOWNLEFT;
			}else if(code==iKEY_DOWNRIGHT){
				return iKEYCODE_PLAYER_MOVE_DOWNRIGHT;
			}else if(code==iKEY_ATTACK_UP){
				return iKEYCODE_PLAYER_ATTACK_UP;
			}else if(code==iKEY_ATTACK_DOWN){
				return iKEYCODE_PLAYER_ATTACK_DOWN;
			}else if(code==iKEY_ATTACK_LEFT){
				return iKEYCODE_PLAYER_ATTACK_LEFT;
			}else if(code==iKEY_ATTACK_RIGHT){
				return iKEYCODE_PLAYER_ATTACK_RIGHT;
			}else if(code==iKEY_ATTACK_UPZ){
				return iKEYCODE_PLAYER_ATTACK_UPZ;
			}else if(code==iKEY_ATTACK_DOWNZ){
				return iKEYCODE_PLAYER_ATTACK_DOWNZ;
			}else if(code==iKEY_ATTACK_UPLEFT){
				return iKEYCODE_PLAYER_ATTACK_UPLEFT;
			}else if(code==iKEY_ATTACK_UPRIGHT){
				return iKEYCODE_PLAYER_ATTACK_UPRIGHT;
			}else if(code==iKEY_ATTACK_DOWNLEFT){
				return iKEYCODE_PLAYER_ATTACK_DOWNLEFT;
			}else if(code==iKEY_ATTACK_DOWNRIGHT){
				return iKEYCODE_PLAYER_ATTACK_DOWNRIGHT;
			}else if(code==iKEY_PICK_UP){
				return iKEYCODE_PLAYER_PICK_UP;
			}else if(code==iKEY_OPEN_INVENTORY){
				SetFocus(playerstat);
			}else if(code==iKEY_OPEN_CHAT){
				SetFocus(chat);
			}else if(code==iKEY_CAMERA_FLY){
				cam->Follow((gameobjectstatic*)player->GetPlace());
			}
		}else{
			if(code==iKEY_UP){
				return iKEYCODE_CAM_MOVE_UP;
			}else if(code==iKEY_DOWN){
				return iKEYCODE_CAM_MOVE_DOWN;
			}else if(code==iKEY_LEFT){
				return iKEYCODE_CAM_MOVE_LEFT;
			}else if(code==iKEY_RIGHT){
				return iKEYCODE_CAM_MOVE_RIGHT;
			}else if(code==iKEY_DOWNZ){
				return iKEYCODE_CAM_MOVE_DOWNZ;
			}else if(code==iKEY_UPZ){
				return iKEYCODE_CAM_MOVE_UPZ;
			}else if(code==iKEY_ESC){
				cam->Follow(player);
			}else if(code==iKEY_OPEN_INVENTORY){
				SetFocus(playerstat);
			}else if(code==iKEY_OPEN_CHAT){
				SetFocus(chat);
			}
		}
	}else if(attackdialog){
		if(code==iKEY_UP){
			AttackDialogUp();
		}else if(code==iKEY_DOWN){
			AttackDialogDown();
		}else if(code==iKEY_ENTER){
			player->Attack(GetAttackDialogTarget());
		}else if(code==iKEY_ESC){
			AttackDialogClose();
		}
	}else if(playerstat->IsFocused()){
		if(code==iKEY_ESC){
			SetFocus(mainscr);
		}else if(code==iKEY_DROP){
			player->Drop(GetHighlight());
			short oldhlght = GetHighlight();
			InventoryFocusRight();
			if(oldhlght==GetHighlight())
				InventoryFocusLeft();
		}else if(code==iKEY_LEFT){
			InventoryFocusLeft();
		}else if(code==iKEY_RIGHT){
			InventoryFocusRight();
		}else if(code==iKEY_PICK_UP){
			player->Take(GetHighlight());
		}
	}else if(chat->IsFocused()){
		if(code==iKEY_PGUP){
			ChatScrollUp();
		}else if(code==iKEY_PGDOWN){
			ChatScrollDown();
		}else if(code==iKEY_ESC){
			SetFocus(mainscr);
		}
	}
}

iConstInt interface::ShowMainMenu(){
	std::vector<mainmenuitem*> items;
	mainmenuitem* newgame = new mainmenuitem();
	newgame->code = iKEYCODE_MAINMENU_NEWGAME;
	newgame->str = "New Game";
	mainmenuitem* quit = new mainmenuitem();
	quit->code = iKEYCODE_MAINMENU_QUIT;
	quit->str = "Quit";
	items.push_back(newgame);
	items.push_back(quit);
	menu = new mainmenu(width,height,items);
	menu->Draw();
	doupdate();
	iConstInt key;
	while(!(key==iKEYCODE_MAINMENU_QUIT)&&
		!(key==iKEYCODE_MAINMENU_NEWGAME))
	{
		menu->Draw();
		doupdate();
		char c = getch();
		iConstInt code = GetKeycode(c);
		if(code==iKEY_UP){
			menu->FocusUp();
		}else if(code==iKEY_DOWN){
			menu->FocusDown();
		}else if(code==iKEY_ENTER){
			key = menu->GetFocused()->code;
		}
	}
	if(!(key==iKEYCODE_MAINMENU_QUIT)){
		OpenGameInterface();
	}
	delete newgame,quit,menu;
	items.clear();
	return key;
}
void interface::AssignPlayer(creature* player){
	playerstat->AssignPlayer(player);
	if(cam){
		delete cam;
	}
	cam = new camera(player);
	this->player = player;
}

void interface::CheckResize(){
	short tempwidth, tempheight;
	getmaxyx(stdscr,tempheight,tempwidth);
	if(tempheight!=mainscr->GetHeight() || tempwidth!=mainscr->GetWidth()){
		ResizeInterface(tempwidth, tempheight);
	}
}
void interface::Draw(){
	mainscr->Draw();
	chat->Draw();
	playerstat->PrintStats();
	playerstat->Draw();
	if(attackdialog)
		attackdialog->Draw();
	doupdate();
	move(GetHeight()-1,GetWidth()-1);
}
void interface::ClearMap(){
	mainscr->Clear();
}

void interface::DrawMap(map* tilemap){
	short camZ = GetCamZ();
	short size = player->GetSightSize();
	short mapX = tilemap->GetWidth();
	short mapY = tilemap->GetHeight();
	short X = GetCamOffsetX();
	short Y = GetCamOffsetY();
	short winx = GetWidth();
	short winy = GetHeight();
	
	for(short i=0; i<winx; i++){
		for(short j=0; j<winy; j++){
			if(j+Y<0||i+X<0||j+Y>=mapY||i+X>=mapX){
				DrawOnMap(i,j,' ');
				continue;
			}
			bool key=true;
			short iter=0;
			short max=4;
			while(key){
				tile* place = tilemap->GetTile(i+X,j+Y,camZ+iter);
				if(iter==max || !place){
					DrawOnMap(i,j,'.');
					key=false;
					continue;
				}
				if(place->IsSpace()){
					if(tilemap->GetVisible(place)){
						tilewspace* temp = (tilewspace*) place;
						if(temp->HasObjects()){
							DrawOnMap(i,j,temp->GetImg(),iter);
							key=false;
							continue;
						}
					}
					iter++;
				}else{
					if(tilemap->GetVisible(place)){
						DrawOnMap(i,j,place->GetImg(),place->GetColor(),iter);
					}else if(tilemap->GetRevealed(place)){
						DrawOnMap(i,j,place->GetImg(),iter);
					}else{
						DrawOnMap(i,j,'.');
					}
					key=false;
					continue;
				}
			}
		}
		addch('\n');
	}
}
void interface::DrawOnMap(short x, short y,char ch){
	mainscr->Draw(x,y,ch,palette->GetPair(1,4));
}
void interface::DrawOnMap(short x, short y,char ch,short delta){
	mainscr->Draw(x,y,ch,palette->GetWhite(delta));
}
void interface::DrawOnMap(short x, short y,char ch, myPair* color){
	mainscr->Draw(x,y,ch,color);
}
void interface::DrawOnMap(short x, short y,char ch, myPair* color,short delta){
	mainscr->Draw(x,y,ch,palette->GetDarker(color,delta));
}
void interface::ShowAttackDialog(std::vector<gameobjectmovable*> targets){
	short width,height;
	getmaxyx(stdscr,height,width);
	ushort winx = width/6*2;
	ushort winy = height-10;
	attackdialog = new attack_dialog(winx,winy,width/6,10,targets, palette->GetPair(0,9),palette->GetPair(0,0));
	wins.push_back(attackdialog);
	SetFocus(attackdialog);
}
void interface::AttackDialogUp(){
	attackdialog->FocusUp();
}
void interface::AttackDialogDown(){
	attackdialog->FocusDown();
}
gameobjectmovable* interface::GetAttackDialogTarget(){
	gameobjectmovable* target = attackdialog->GetFocused();
	AttackDialogClose();
	return target;
}
bool interface::GetAttackDialogFocus(){
	return (attackdialog);
}
void interface::AttackDialogClose(){
	wins.erase(std::find(wins.begin(),wins.end(),attackdialog),wins.end());
	delete attackdialog;
	attackdialog = nullptr;
	SetFocus(mainscr);
}
void interface::WriteToChat(std::string text){
	chat->AddMessage(text);
}
void interface::ClearChat(){
	chat->Clear();
}
void interface::ChatScrollUp(){
	chat->ScrollUp();
}
void interface::ChatScrollDown(){
	chat->ScrollDown();
}
void interface::SetFocus(window* win){
	for(auto w:wins){
		if(w==win){
			w->SetFocused(true);
		}else{
			w->SetFocused(false);
		}
	}
}

bool interface::GetChatFocus(){		chat->IsFocused();}

void interface::InventoryFocusLeft(){	playerstat->FocusLeft();}
void interface::InventoryFocusRight(){	playerstat->FocusRight();}
bool interface::GetInventoryFocus(){		return playerstat->IsFocused();}

short interface::GetWidth(){			return mainscr->GetWidth();}
short interface::GetHeight(){			return mainscr->GetHeight();}
short interface::GetHighlight(){		return playerstat->GetHighlight();}

void interface::ResizeInterface(short newx, short newy){
	short deltay = newy - mainscr->GetHeight();
	short deltax = newx - mainscr->GetWidth();

	mainscr->Resize(newx,newy);
	ProcessWindowOnResize(playerstat,deltax,deltay);
	ProcessWindowOnResize(chat,deltax, deltay);
	
	Draw();
}
void interface::ProcessWindowOnResize(window_bordered* win, short deltax, short deltay){
	if(win!=NULL){
		win->MoveAt(deltax,deltay);
	}
}

short interface::GetCamX(){	return cam->GetX();}
short interface::GetCamY(){	return cam->GetY();}
short interface::GetCamZ(){	return cam->GetZ();}
gameobjectstatic* interface::GetCamPlace(){	return cam->GetPlace();}
short interface::GetCamOffsetX(){	return cam->GetOffsetX();}
short interface::GetCamOffsetY(){	return cam->GetOffsetY();}
void interface::CamFollow(gameobjectmovable* target){	cam->Follow(target);}
void interface::CamFollow(gameobjectstatic* target){	cam->Follow(target);}
bool interface::CamFlying(){	return cam->Flying();}
void interface::SetCamParameters(short width, short height){	cam->SetParams(width,height);}

mypalette* interface::GetPalette(){	return palette;}
void interface::DrawPalette(){
	clear();
	refresh();
	int pairscount = palette->GetPairsCount();
	for(int i=0; i<pairscount; i++){
		for(int j=0; j<5; j++){
			mvwaddch(stdscr, j, i, 'C' | COLOR_PAIR(palette->GetPair(i,j)->GetNum()));
		}
		refresh();
		addch('\n');
	}
	refresh();
	getch();
}
