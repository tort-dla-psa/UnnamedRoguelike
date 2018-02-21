#include<algorithm>
#include"engine.h"
#include"FileOperations.cpp"

void ShowTiletypes(std::vector<tile*> tiletypes){
	for(auto it:tiletypes){
		printw("Char:%c \n",it->GetImg()); refresh();
		printw("Name:%s \n",it->GetName().c_str()); refresh();
		printw("Material:%s \n",it->GetMat()->GetName().c_str()); refresh();
		printw("IsSpace:%d \n",(it->IsSpace())?1:0); refresh();
		printw("Ore:%s \n\n",it->GetOre()->GetName().c_str()); refresh();
	}
}

engine::engine(){
	win = new interface();
	if(has_colors()==false){
		endwin();
		exit(1);
	}
	srand(time(NULL));

	materials = FileOperations::LoadMaterials();
	oreideas = FileOperations::LoadOres(materials);
	tileideas = FileOperations::LoadTiles(oreideas);
	ShowTiletypes(tileideas);
	getch();
	mp = new Perlin(100,100,50,tileideas);
	pathfinder = new generator();
	AddCreature("Adam",'T',10,1,0,1,0);
	AddCreature("Eve",'i',10,1,1,1,0);
	CreateItem('0',materials[0],1,0,0);
	CreateItem('1',materials[1],2,0,0);
	DoGravity();
	tile* destignation = mp->FindTileOnVertical(25,12);

	tile* start = (tile*)creatures[1]->GetPlace();
	std::vector<tilewspace*> path = pathfinder->FindPath(mp,start,destignation); 
	creatures[1]->SetPath(path);
}

engine::~engine(){
	delete mp;
	delete cam;
	delete win;
	for(short i=0; i<creatures.size(); i++)
			delete creatures[i];
	creatures.clear();
	
	for(short i=0; i<items.size(); i++)
			delete items[i];
	items.clear();
	
	for(short i=0; i<materials.size(); i++)
			delete materials[i];
	materials.clear();
}
void engine::drawrecurse(ushort x, ushort y, ushort z, ushort iter, ushort max){
	tile* temp = mp->GetTile(x+cam->GetOffsetX(),y+cam->GetOffsetY(),z);
	if(iter>=max||!temp){
		win->DrawOnMap(x,y,'.',1+iter);
		return;
	}
	if(temp->IsSpace()){
		tilewspace* temp2 = (tilewspace*) temp;
		if(temp2->HasObjects()){
			win->DrawOnMap(x,y,temp->GetImg(),1+iter);
			return;
		}
		drawrecurse(x,y,z+1,iter+1,max);
	}else{
		win->DrawOnMap(x,y,temp->GetImg(),1+iter);
	}
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
	for(auto it:items){
		it->GetName();
	}
}

void engine::DoGravity(){
	for (auto i:creatures){
		if(i->GetPlace()==NULL)
			continue;
		ushort x = i->GetX();
		ushort y = i->GetY();
		ushort z = i->GetZ()+1;
		tile* tilebelow = mp->GetTile(x,y,z);
		if(!tilebelow->IsSpace())
			return;
		while(tilebelow && tilebelow->IsSpace()){
			tilebelow = mp->GetTile(x,y,++z);
		}
		tilebelow = mp->GetTile(x,y,--z);
		i->Move((tilewspace*)tilebelow);
	}
	for (auto i:items){
		if(i->GetPlace()==NULL)
			continue;
		short x = i->GetX();
		short y = i->GetY();
		short z = i->GetZ()+1;
		tile* tilebelow = mp->GetTile(x,y,z);
		if(!tilebelow->IsSpace())
			return;
		while(tilebelow && tilebelow->IsSpace()){
			tilebelow = mp->GetTile(x,y,++z);
		}
		tilebelow = mp->GetTile(x,y,--z);
		i->Move((tilewspace*)tilebelow);
	}
}

creature* engine::AddCreature(std::string name, char img, ushort hp, ushort dp, ushort x, ushort y, ushort z){
	tile* temptile = mp->GetTile(x,y,z);
	if(temptile->IsSpace()){
		creature* temp = new creature(name, img, hp, dp);
		temp->Move((tilewspace*)temptile);
		creatures.push_back(temp);
		if(creatures.size()==1){
			player = temp;
			win->AssignPlayer(player);
			cam = new camera(player);
		}
		return temp;
	}
	return NULL;
}
void engine::MovePlayer(char ch){
	short* dirs = GetDir(ch);
	short x = dirs[0], y = dirs[1], z = dirs[2];
	tile* temptile = mp->GetTile(x,y,z);
	if(!temptile)	
		return;
	if(temptile->IsSpace()){
		player->Move((tilewspace*)temptile);
	}else{
		tile* temptile = mp->GetTile(x,y,z-1);
		if(temptile && temptile->IsSpace()){
			player->Move((tilewspace*)temptile);
		}
	}
	delete[] dirs;
}
void engine::MoveCam(char ch){
	short* dirs = GetDir(ch);
	short x = dirs[0], y = dirs[1], z = dirs[2];
	tile* temptile = mp->GetTile(x,y,z);
	cam->Follow(temptile);
	delete[] dirs;
}
void engine::PerformAttack(char ch){
	short* dirs = GetDir(ch);
	short x = dirs[0], y = dirs[1], z = dirs[2];
	tile* temptile = mp->GetTile(x,y,z);
	if(temptile->IsSpace()){
		std::vector<gameobjectmovable*> objects =((tilewspace*)temptile)->GetObjects();
		ushort objectssize = objects.size();
		if(objectssize==0){
			temptile = mp->GetTile(x,y,z+1);
			if(!temptile->IsSpace() && temptile->GetName()!="borderstone"){
				short dp = player->Attack(temptile);
				WriteLog(temptile, dp);
				if(temptile->GetHp()<1)
					mp->DelTile(temptile);
			}
		}else if(objectssize==1){
			//Add dialog window
			gameobjectmovable* temp = objects[0];
			if(temp!=player){
				short dp = player->Attack(temp);
				WriteLog(temp,dp);
				if(temp->GetHp()<1)
					DelObject(temp);
			}
		}else{
			//Add dialog window with list
		}
	}else if(!temptile->IsSpace() && temptile->GetName()!="borderstone"){
		short dp = player->Attack(temptile);
		WriteLog(temptile, dp);
		if(temptile->GetHp()<1){
			mp->DelTile(temptile);
		}
	}
	delete[] dirs;
}

item* engine::CreateItem(char img, material* materia){
	item* temp = new item("generic",img,materia);
	items.push_back(temp);
	return temp;
}
item* engine::CreateItem(char img, material* materia,  ushort x, ushort y, ushort z){
	tile* temptile = mp->GetTile(x,y,z);
	if(temptile && temptile->IsSpace()){
		item* tempitem = CreateItem(img,materia);
		tempitem->Move((tilewspace*)temptile);
		return tempitem;
	}
	return NULL;
}

void engine::PickUp(char ch){
	short* dirs = GetDir(ch);
	short x = dirs[0], y = dirs[1], z = dirs[2];
	tile* temptile = mp->GetTile(x,y,z);
	if(temptile->IsSpace()){
		tilewspace* temptile2 = (tilewspace*) temptile;
		gameobjectmovable* tempitem = (temptile2->GetObjects())[0];
		if(tempitem!=NULL){
			tempitem->Move(player);
			//full inventory
		}
	}
	delete[] dirs;
}

void engine::DelObject(gameobjectmovable* it){
	for(auto i:items){
		if(it==i){
			it->GetPlace()->RemoveObject(it);
			items.erase(std::find(items.begin(),items.end(),i),items.end());
			delete it;
			return;
		}
	}
	for(auto c:creatures){
		if(it==c){
			it->GetPlace()->RemoveObject(it);
			creatures.erase(std::find(creatures.begin(),creatures.end(),c),creatures.end());
			delete it;
			return;
		}
	}
}

void engine::HandleKey(char ch){
	if(ch=='0'){
		win->SetInventoryFocus(false);
		cam->Follow(player);
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
			cam->Follow(player);
		}else if(ch=='C'){
			win->SetChatFocus(true);
		}else if(ch=='l'){
			cam->Follow(player->GetPlace());
		}
	}else{
		if(ch=='w'||ch=='a'||ch=='s'||ch=='d'||ch=='>'||ch=='<'){
			MoveCam(ch);
		}else if(ch==27||ch=='l'){
			cam->Follow(player);
		}else if(ch=='i'){
			win->SetInventoryFocus(true);
			cam->Follow(player);			
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
		win->Draw();
		char ch = getch();
		HandleKey(ch);
		creatures[1]->FollowPath();
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
void engine::WriteLog(gameobject* target, ushort dp){
	std::string mes = "You hit "+target->GetName()+"("+target->GetImg()+") , "+std::to_string(dp)+" dp";
	win->WriteToChat(mes);
}
