#include"engine.h"
#include"FileOperations.cpp"

engine::engine(){
	win = new interface();
	if(has_colors()==false){
		endwin();
		exit(1);
	}
	srand(time(NULL));
	numofcreatures = 0;

	maxnumofcreatures = 100;
	creatures = new creature* [maxnumofcreatures];
	for(short i=0; i<maxnumofcreatures; i++)
		creatures[i] = NULL;
	
	numofmaterials = 0;
	maxnumofmaterials = 50;
	
	numofitems = 0;
	maxnumofitems = 200;
	items = new item* [maxnumofitems];
	for(short i=0; i<maxnumofitems; i++)
		items[i] = NULL;
	
	materials = FileOperations::LoadMaterials(maxnumofmaterials, numofmaterials);
	CreatePerlinMap();
	AddCreature("Adam",'T',10,1,0,0,0);
	AddCreature("Eve",'i',10,1,1,1,1);
	refresh();getch();
	CreateItem('0',1,1, materials[0],1,0,0);
	CreateItem('1',2,2, materials[1],2,0,0);
}

engine::~engine(){
	delete mp;
	delete cam;
	delete win;
	for(short i=0; i<maxnumofcreatures; i++)
		if(creatures[i]!=NULL)
			delete creatures[i];
	delete[] creatures;
	
	for(short i=0; i<maxnumofitems; i++)
		if(items[i]!=NULL)
			delete items[i];
	delete[] items;
	
	for(short i=0; i<maxnumofmaterials; i++)
		if(materials[i]!=NULL)
			delete materials[i];
	delete[] materials;
}

//void engine::CreatePerlinMap(){	mp = new Perlin(100,100,50);}
void engine::CreatePerlinMap(){	mp = new Perlin(100,100,50,materials);}
void engine::drawrecurse(short x, short y, short z, short iter, short max){
	tile* temp = mp->GetTile(x+cam->GetOffsetX(),y+cam->GetOffsetY(),z);
	if(iter>=max){
		win->DrawOnMap(x,y,'.',1+iter);
		return;
	}
	if(!temp->IsSpace()||temp->GetCreature()||temp->GetItem()){
		win->DrawOnMap(x,y,temp->GetChar(),1+iter);
	}else
		drawrecurse(x,y,z+1,iter+1,max);
}
void engine::DrawMap(){
	short camZ = cam->GetZ();
	short size = player->GetSightSize();
	short mapX = mp->GetWidth();
	short mapY = mp->GetHeight();
	short X = cam->GetOffsetX();
	short Y = cam->GetOffsetY();
	short winx = win->GetWidth();
	short winy = win->GetHeight();
	for(short i=0; i<winx; i++){
		for(short j=0; j<winy; j++){
			if(j+Y<0||i+X<0||j+Y>=mapY||i+X>=mapX){
				win->DrawOnMap(i,j,' ', 1);
				continue;
			}
			drawrecurse(i,j,camZ,0,4);
		}
		addch('\n');
	}
}

void engine::DoGravity(){
	for(short i=0; i<numofcreatures; i++){
		short x = creatures[i]->GetX();
		short y = creatures[i]->GetY();
		short z = creatures[i]->GetZ()+1;
		tile* tilebelow = mp->GetTile(x,y,z);
		while(tilebelow->IsSpace()){
			if(tilebelow->GetCreature()==NULL){
				creatures[i]->Move(tilebelow);
				tilebelow = mp->GetTile(x,y,++z);
			}else break;
		}
	}
	for(short i=0; i<numofitems; i++){
		if(items[i]->GetPlace()==NULL)
			continue;
		short x = items[i]->GetX();
		short y = items[i]->GetY();
		short z = items[i]->GetZ()+1;
		tile* tilebelow = mp->GetTile(x,y,z);
		while(tilebelow->IsSpace()){
			items[i]->Move(tilebelow);
			tilebelow = mp->GetTile(x,y,++z);
		}
	}
}

creature* engine::AddCreature(std::string name, char img, short hp, short dp, short x, short y, short z){
	if(numofcreatures<maxnumofcreatures && mp->GetTile(x,y,z)->IsSpace()){
		for(short i=0; i<maxnumofcreatures; i++){
			if(creatures[i]==NULL){
				creatures[i] = new creature(name, img, hp, dp, i);
				tile* temptile = mp->GetTile(x,y,z);
				creatures[i]->Move(temptile);
				if(numofcreatures==0){
					player = creatures[i];
					win->AssignPlayer(player);
					cam = new camera(player);
				}
				numofcreatures++;
				return creatures[i];
				}
			}
		}
	return NULL;
}
void engine::DelCreature(creature* cr){
	creatures[cr->GetId()]=NULL;
	numofcreatures--;
	delete cr;
}
void engine::MovePlayer(char ch){
	short* dirs = GetDir(ch);
	short x = dirs[0], y = dirs[1], z = dirs[2];
	if(mp->GetTile(x,y,z)->IsSpace()){
		tile* temptile = mp->GetTile(x,y,z);
		if(temptile->GetCreature()==NULL)
			player->Move(temptile);
	}else{
		tile* temptile = mp->GetTile(x,y,z-1);
		if(temptile!=NULL&&temptile->IsSpace()){
			if(temptile->GetCreature()==NULL)
				player->Move(temptile);
		}
	}
	delete[] dirs;
}
void engine::MoveCam(char ch){
	short* dirs = GetDir(ch);
	short x = dirs[0], y = dirs[1], z = dirs[2];
	tile* temptile = mp->GetTile(x,y,z);
	cam->GoToPlace(temptile);
	delete[] dirs;
}
void engine::PerformAttack(char ch){
	short* dirs = GetDir(ch);
	short x = dirs[0], y = dirs[1], z = dirs[2];
	tile* temptile = mp->GetTile(x,y,z);
	if(temptile->IsSpace()){
		creature* temp = temptile->GetCreature();
		if(temp!=NULL){
			if(temp!=player){
				short dp = player->Attack(temp);
				WriteLog(temp,dp);
				if(temp->GetHp()<1)
					DelCreature(temp);
			}
		}else{
			tile* temptile = mp->GetTile(x,y,z+1);
			if(!temptile->IsSpace() && temptile->GetHp()>-1){
				short dp = player->Dig(temptile);
				WriteLog(temptile, dp);
				if(temptile->GetHp()<1)
					mp->DelTile(temptile);
			}
		}
	}else if(!temptile->IsSpace() && temptile->GetHp()!=-10){
		short dp = player->Dig(temptile);
		WriteLog(temptile, dp);
		if(temptile->GetHp()<1)
			mp->DelTile(temptile);
	}
	delete[] dirs;
}

item* engine::CreateItem(char img, short volume, short sharpness, material* materia){
	if(numofitems<maxnumofitems){
		for(short i=0; i<maxnumofitems; i++){
			if(items[i]==NULL){
				items[i]=new item(img,volume,sharpness,materia,i);
				numofitems++;
				return items[i];
			}
		}
	}
	return NULL;
}
item* engine::CreateItem(char img, short volume, short sharpness, material* materia,  short x, short y, short z){
	tile* temptile = mp->GetTile(x,y,z);
	if(temptile->IsSpace()){
		item* tempitem = CreateItem(img,volume,sharpness,materia);
		tempitem->Move(temptile);
		return tempitem;
	}
	return NULL;
}

void engine::PickUp(char ch){
	short* dirs = GetDir(ch);
	short x = dirs[0], y = dirs[1], z = dirs[2];
	tile* temptile = mp->GetTile(x,y,z);
	if(temptile->IsSpace()){
		item* tempitem = temptile->GetItem();
		if(tempitem!=NULL){
			tempitem->Move(player);
			if(!player->PickUp(tempitem))
				WriteLog("Inventory full");
		}
	}
	delete[] dirs;
}

void engine::Drop(){
	player->Drop(0,player->GetPlace());
}

void engine::DelItem(item* target){
	items[target->GetId()]=NULL;
	numofitems--;
}

void engine::HandleKey(char ch){
	if(ch=='0'){
		win->SetInventoryFocus(false);
		cam->FollowCreature(player);
		return;
	}
	if(win->GetInventoryFocus()){
		if(ch=='I'||ch==27){
			win->SetInventoryFocus(false);
		}else if(ch=='g'){
			short choose = win->GetHighlight();
			player->Drop(choose);
			short items = player->GetItemsCount();
			short oldhlght = win->GetHighlight();
			win->InventoryFocusRight();
			if(oldhlght==win->GetHighlight())
				win->InventoryFocusLeft();
		}else if(ch=='h'){
			player->Take(win->GetHighlight());
		}else if(ch=='a'){
			win->InventoryFocusLeft();
		}else if(ch=='d'){
			win->InventoryFocusRight();
		}
		return;
	}
	if(win->GetChatFocus()){
		if(ch=='2'){
			win->ChatScrollUp();
		}else if(ch=='1'){
			win->ChatScrollDown();
		}else if(ch=='C'||ch==27){
			win->SetChatFocus(false);
		}
		return;
	}
	if(!cam->Flying()){
		if(ch=='w'||ch=='a'||ch=='s'||ch=='d'){
			MovePlayer(ch);
		}else if(ch=='W'||ch=='A'||ch=='S'||ch=='D'||ch=='>'||ch=='<'){
			PerformAttack(ch);
		}else if(ch=='g'){
			PickUp(ch);
		}else if(ch=='I'){
			win->SetInventoryFocus(true);
			cam->FollowCreature(player);
		}else if(ch=='C'){
			win->SetChatFocus(true);
		}else if(ch=='l'){
			cam->GoToPlace(player->GetPlace());
		}
	}else{
		if(ch=='w'||ch=='a'||ch=='s'||ch=='d'||ch=='>'||ch=='<'){
			MoveCam(ch);
		}else if(ch==27||ch=='l'){
			cam->FollowCreature(player);
		}else if(ch=='i'){
			win->SetInventoryFocus(true);
			cam->FollowCreature(player);			
		}else if(ch=='C'){
			win->SetChatFocus(true);
		}
		return;
	}
}

void engine::MainLoop(){
	while(player!=NULL){
		win->CheckResize();
		cam->SetParams(win->GetWidth(),win->GetHeight());
		DrawMap();
		win->ShowPlayerStat();
		win->Draw();
		char ch = getch();
		HandleKey(ch);
		if(ch=='0')
			break;
		DoGravity();
	}
}

short* engine::GetDir(char ch){
	short x = cam->GetX();
	short y = cam->GetY();
	short z = cam->GetZ();	
	if(ch=='w'||ch=='W'){
		y=(y-1<0)?0:y-1;
	}else if(ch=='s'||ch=='S'){
		short h = mp->GetHeight();
		y=(y+1==h)?h-1:y+1;
	}else if(ch=='a'||ch=='A'){
		x=(x-1<0)?0:x-1;
	}else if(ch=='d'||ch=='D'){
		short w = mp->GetWidth();
		x=(x+1==w)?w-1:x+1;
	}else if(ch=='>'){
		short d = mp->GetDepth();
		z=(z+1==d)?d-1:z+1;
	}else if(ch=='<'){
		z=(z-1<0)?0:z-1;
	}
	return new short[3]{x,y,z};
}
void engine::WriteLog(std::string mes){
	win->WriteToChat(mes);
}
void engine::WriteLog(tile* place, short dp){
	std::string mes = "You hit "+place->GetName()+", "+std::to_string(dp)+" dp";
	win->WriteToChat(mes);
}
void engine::WriteLog(creature* target, short dp){
	std::string mes = "You hit "+target->GetName()+"("+target->GetImg()+") , "+std::to_string(dp)+" dp";
	win->WriteToChat(mes);
}
