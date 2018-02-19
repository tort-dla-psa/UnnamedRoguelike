#include<ncurses.h>
#include<string>
#include"interface.h"
#include"MyColor.h"
#include"MyPair.h"
#include"lists.h"
#include"windows.h"

interface::~interface(){
	delete playerstat,
	       chat,
	       mainscr;
	for(auto c:colors){
		delete c;
	}
	for(auto p:pairs){
		delete p;
	}
	clear();
	endwin();
}

interface::interface(){
	initscr();
	noecho();
	curs_set(FALSE);
	start_color();
	colors.push_back(new myColor(0,0,0,"black",colors.size()));	//0
	colors.push_back(new myColor(1000,1000,1000,"white",colors.size()));	//1
	colors.push_back(new myColor(500,500,500,"gray",colors.size()));	//2
	colors.push_back(new myColor(1000,1000,200,"yelow",colors.size()));	//3
	colors.push_back(new myColor(1000,200,1000,"violet",colors.size()));	//4
	colors.push_back(new myColor(200,1000,1000,"cyan",colors.size())); 	//5
	colors.push_back(new myColor(1000,200,200,"red",colors.size()));	//6
	colors.push_back(new myColor(200,1000,200,"green",colors.size()));	//7
	colors.push_back(new myColor(200,200,1000,"blue",colors.size()));	//8
	colors.push_back(new myColor(800,800,800,"white2",colors.size()));	//9
	colors.push_back(new myColor(600,600,600,"white3",colors.size()));	//10
	colors.push_back(new myColor(400,400,400,"white4",colors.size()));	//11
	colors.push_back(new myColor(200,200,200,"white5",colors.size()));	//12
	pairs.push_back(new myPair(colors[1],colors[0],pairs.size()+1));	//1
	pairs.push_back(new myPair(colors[0],colors[1],pairs.size()+1));	//2
	pairs.push_back(new myPair(colors[2],colors[0],pairs.size()+1));	//3
	pairs.push_back(new myPair(colors[3],colors[0],pairs.size()+1));	//4
	pairs.push_back(new myPair(colors[4],colors[0],pairs.size()+1));	//5
	pairs.push_back(new myPair(colors[5],colors[0],pairs.size()+1));	//6
	pairs.push_back(new myPair(colors[6],colors[0],pairs.size()+1));	//7
	pairs.push_back(new myPair(colors[7],colors[0],pairs.size()+1));	//8
	pairs.push_back(new myPair(colors[8],colors[0],pairs.size()+1));	//9
	pairs.push_back(new myPair(colors[0],colors[9],pairs.size()+1));	//10
	pairs.push_back(new myPair(colors[0],colors[10],pairs.size()+1));	//11
	pairs.push_back(new myPair(colors[0],colors[11],pairs.size()+1));	//12
	pairs.push_back(new myPair(colors[0],colors[12],pairs.size()+1));	//13
	short width,height;
	getmaxyx(stdscr,height,width);
	keypad(stdscr,true);
	short rightoffset = 10;
	short leftoffset = 20;
	short rightstart = width-rightoffset;
	mainscr = new worldwindow(width,height+1);
	playerstat = new window_playerstats(rightstart, 0, rightoffset, 6);
	chat = new window_chat(0,height-12,leftoffset-1,12,50);
	playerstat->SetRUAnch(true);
	chat->SetLDAnch(true);
}

void interface::AssignPlayer(creature* player){
	playerstat->AssignPlayer(player);
}

void interface::Draw(){
	mainscr->Draw();
	playerstat->Draw();
	chat->Draw();
	doupdate();
	move(GetHeight()-1,GetWidth()-1);
}
void interface::CheckResize(){
	short tempwidth, tempheight;
	getmaxyx(stdscr,tempheight,tempwidth);
	if(tempheight!=mainscr->GetHeight() || tempwidth!=mainscr->GetWidth()){
		ResizeInterface(tempwidth, tempheight);
	}
}
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
		if(win->GetLDAnch() || win->GetRDAnch())
			win->MoveAt(0,deltay);
		if(win->GetRUAnch() || win->GetRDAnch())
			win->MoveAt(deltax,0);
	}
}
void interface::ShowPlayerStat(){
	playerstat->PrintStats();
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
void interface::SetChatFocus(bool key){	chat->SetFocused(key);}
bool interface::GetChatFocus(){		chat->IsFocused();}

void interface::DrawOnMap(short x, short y,char ch){
	mainscr->Draw(x,y,ch);
}
void interface::DrawOnMap(short x, short y,char ch, short color){
	if(color==1){
		mainscr->Draw(x,y,ch, 2);
	}else{
		mainscr->Draw(x,y,ch, 8+color);
	}
}
void interface::ClearMap(){
	mainscr->Clear();
}

void interface::SetInventoryFocus(bool key){	
	playerstat->SetFocused(key);
}
void interface::InventoryFocusLeft(){
	playerstat->FocusLeft();
}
void interface::InventoryFocusRight(){
	playerstat->FocusRight();
}
short interface::GetWidth(){			return mainscr->GetWidth();}
short interface::GetHeight(){			return mainscr->GetHeight();}
short interface::GetHighlight(){		return playerstat->GetHighlight();}
bool interface::GetInventoryFocus(){		return playerstat->IsFocused();}
