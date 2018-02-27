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
	InitKeys();
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
	CreateItem('2',materials[2],2,0,0);
	DoGravity();
	tile* destignation = mp->FindTileOnVertical(25,12);

	tile* start = (tile*)creatures[1]->GetPlace();
	std::vector<tilewspace*> path = pathfinder->FindPath(mp,start,destignation); 
	creatures[1]->SetPath(path);
}

engine::~engine(){
	delete mp;
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
void engine::DrawRecurse(ushort x, ushort y, ushort z, ushort iter, ushort max){
	tile* temp = mp->GetTile(x+win->GetCamOffsetX(),y+win->GetCamOffsetY(),z);
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
		DrawRecurse(x,y,z+1,iter+1,max);
	}else{
		win->DrawOnMap(x,y,temp->GetImg(),1+iter);
	}
}
void engine::DrawMap(){
	short camZ = win->GetCamZ();
	short size = player->GetSightSize();
	short mapX = mp->GetWidth();
	short mapY = mp->GetHeight();
	short X = win->GetCamOffsetX();
	short Y = win->GetCamOffsetY();
	short winx = win->GetWidth();
	short winy = win->GetHeight();
	for(short i=0; i<winx; i++){
		for(short j=0; j<winy; j++){
			if(j+Y<0||i+X<0||j+Y>=mapY||i+X>=mapX){
				win->DrawOnMap(i,j,' ', 1);
				continue;
			}
			DrawRecurse(i,j,camZ,0,4);
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
		}
		return temp;
	}
	return NULL;
}
void engine::MovePlayer(const int keycode){
	short* dirs = GetDir(keycode);
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
void engine::MoveCam(const int keycode){
	short* dirs = GetDir(keycode);
	short x = dirs[0], y = dirs[1], z = dirs[2];
	tile* temptile = mp->GetTile(x,y,z);
	win->CamFollow(temptile);
	delete[] dirs;
}
void engine::PerformAttack(const int keycode){
	short* dirs = GetDir(keycode);
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
			gameobjectmovable* temp = objects[0];
			if(!player->IsAttacked(temp)){
				win->ShowAttackDialog(objects);
				return;
			}
			if(temp!=player){
				short dp = player->Attack(temp);
				WriteLog(temp,dp);
				if(temp->GetHp()<1)
					DelObject(temp);
			}
		}else{
			win->ShowAttackDialog(objects);
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

void engine::PickUp(const int keycode){
	short* dirs = GetDir(keycode);
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

void engine::InitKeys(){
	win->AddKey('A',win->iKEY_ATTACK_LEFT);
	win->AddKey('D',win->iKEY_ATTACK_RIGHT);
	win->AddKey('S',win->iKEY_ATTACK_DOWN);
	win->AddKey('W',win->iKEY_ATTACK_UP);
	win->AddKey('a',win->iKEY_LEFT);
	win->AddKey('d',win->iKEY_RIGHT);
	win->AddKey('s',win->iKEY_DOWN);
	win->AddKey('w',win->iKEY_UP);
	win->AddKey(10,win->iKEY_ENTER); 
	win->AddKey(27,win->iKEY_ESC);
	win->AddKey('c',win->iKEY_OPEN_CHAT);
	win->AddKey('l',win->iKEY_CAMERA_FLY);
	win->AddKey('g',win->iKEY_PICK_UP);
	win->AddKey('h',win->iKEY_DROP);
	win->AddKey('i',win->iKEY_OPEN_INVENTORY);
}
void engine::HandleKey(char ch){
	const int keycode = win->HandleKey(ch);
	if(keycode==win->iKEYCODE_PLAYER_MOVE_UP||
			keycode==win->iKEYCODE_PLAYER_MOVE_DOWN||
			keycode==win->iKEYCODE_PLAYER_MOVE_LEFT||
			keycode==win->iKEYCODE_PLAYER_MOVE_RIGHT)
	{
		MovePlayer(keycode);
	}else if(keycode==win->iKEYCODE_PLAYER_ATTACK_UP||
			keycode==win->iKEYCODE_PLAYER_ATTACK_DOWN||
			keycode==win->iKEYCODE_PLAYER_ATTACK_LEFT||
			keycode==win->iKEYCODE_PLAYER_ATTACK_RIGHT)
	{
		PerformAttack(keycode);
	}else if(keycode==win->iKEYCODE_PLAYER_PICK_UP){
		PickUp(keycode);
	}else if(keycode==win->iKEYCODE_CAM_MOVE_UP||
			keycode==win->iKEYCODE_CAM_MOVE_DOWN||
			keycode==win->iKEYCODE_CAM_MOVE_LEFT||
			keycode==win->iKEYCODE_CAM_MOVE_RIGHT||
			keycode==win->iKEYCODE_CAM_MOVE_DOWNZ||
			keycode==win->iKEYCODE_CAM_MOVE_UPZ)
	{
		MoveCam(keycode);
	}
}

void engine::MainLoop(){
	while(player!=NULL){
		win->CheckResize();
		win->SetCamParameters(win->GetWidth(),win->GetHeight());
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

short* engine::GetDir(const int keycode){
	short x = win->GetCamX();
	short y = win->GetCamY();
	short z = win->GetCamZ();
	if(keycode==win->iKEYCODE_PLAYER_MOVE_UP||
		keycode==win->iKEYCODE_PLAYER_ATTACK_UP||
		keycode==win->iKEYCODE_CAM_MOVE_UP)
	{
		y-=1;
	}else if(keycode==win->iKEYCODE_PLAYER_MOVE_DOWN||
		keycode==win->iKEYCODE_PLAYER_ATTACK_DOWN||
		keycode==win->iKEYCODE_CAM_MOVE_DOWN)
	{
		y+=1;
	}else if(keycode==win->iKEYCODE_PLAYER_MOVE_LEFT||
		keycode==win->iKEYCODE_PLAYER_ATTACK_LEFT||
		keycode==win->iKEYCODE_CAM_MOVE_LEFT)
	{
		x-=1;
	}else if(keycode==win->iKEYCODE_PLAYER_MOVE_RIGHT||
		keycode==win->iKEYCODE_PLAYER_ATTACK_RIGHT||
		keycode==win->iKEYCODE_CAM_MOVE_RIGHT)
	{
		x+=1;
	}else if(keycode==win->iKEYCODE_PLAYER_ATTACK_DOWNZ||
		keycode==win->iKEYCODE_CAM_MOVE_DOWNZ)
	{
		z+=1;
	}else if(keycode==win->iKEYCODE_PLAYER_ATTACK_UPZ||
		keycode==win->iKEYCODE_CAM_MOVE_UPZ)
	{
		z-=1;
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
