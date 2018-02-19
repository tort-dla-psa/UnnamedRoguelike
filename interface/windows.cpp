#include<ncurses.h>
#include"../creature.h"
#include"windows.h"

window::window(short width, short height):
        width(width),height(height)
{
        win = newwin(height,width,0,0);
        updated=false;
        focused=false;
}
window::~window(){}

void window::Clear(){
        wclear(win);
        SetUpdated(true);
}

void window::Draw(){
        if(IsUpdated()){
                wnoutrefresh(win);
                SetUpdated(false);
        }
}

void window::Resize(short newx, short newy){
        wresize(win, newy, newx);
        width = newx;
        height = newy;
        SetUpdated(true);
}

void window::SetUpdated(bool key){      updated = key;}
void window::SetFocused(bool key){      focused = key;}
bool window::IsUpdated(){               return updated;}
bool window::IsFocused(){               return focused;}
short window::GetWidth(){               return width;}
short window::GetHeight(){              return height;}


worldwindow::worldwindow(short width, short height):
        window(width,height){}

worldwindow::~worldwindow(){}

void worldwindow::Draw(short x, short y, char ch){
        mvwaddch(win,y,x,ch);
        SetUpdated(true);
}
void worldwindow::Draw(short x, short y, char ch, short color){
        mvwaddch(win,y,x,ch | COLOR_PAIR(color));
        SetUpdated(true);
}
window_bordered::window_bordered(short x, short y, short width, short height):
        x(x),y(y),window(width,height)
{
        win = newwin(height, width, y, x);
        subwin = derwin(win,height-2, width -2, 1, 1);
        focused=updated=false;
        luanch=ldanch=ruanch=rdanch=false;
}
window_bordered::~window_bordered(){}

void window_bordered::Draw(){
        if(IsUpdated()){
                box(win,0,0);
                wnoutrefresh(win);
                wnoutrefresh(subwin);
                SetUpdated(false);
        }
}

void window_bordered::Clear(){
        wclear(subwin);
        SetUpdated(true);
}

void window_bordered::MoveAt(short dx, short dy){
        this->x+=x;
        this->y+=y;
        mvwin(win,this->y,this->x);
        SetUpdated(true);
}

void window_bordered::MoveTo(short x, short y){
        this->x=x;
        this->y=y;
        mvwin(win,this->y,this->x);
        SetUpdated(true);
}

void window_bordered::SetLUAnch(bool key){      luanch=key;}
void window_bordered::SetLDAnch(bool key){      ldanch=key;}
void window_bordered::SetRUAnch(bool key){      ruanch=key;}
void window_bordered::SetRDAnch(bool key){      rdanch=key;}
bool window_bordered::GetLUAnch(){              return luanch;}
bool window_bordered::GetLDAnch(){              return ldanch;}
bool window_bordered::GetRUAnch(){              return ruanch;}
bool window_bordered::GetRDAnch(){              return rdanch;}


window_chat::window_chat(short x, short y, short width, short height,short chatsize):window_bordered(x, y, width, height)
{
        this->chatsize = chatsize;
        mes = new std::string[chatsize];
        for(short i=0; i<chatsize; i++){
                mes[i] = "";
        }
        pointer = 0;
        mescount = 0;
        SetUpdated(true);
}

window_chat::~window_chat(){
        delete[] mes;
}

void window_chat::Clear(){
        for(short i=0; i<chatsize; i++){
                mes[i] = "";
        }
        werase(subwin);
        mescount=0;
        SetUpdated(true);
}

void window_chat::Draw(){
        box(win,0,0);
//      if(IsUpdated()){
        werase(subwin);
        for(short i=0; i<GetHeight()-2; i++){
                if(i+pointer<chatsize)
                        wprintw(subwin,"%s",mes[i+pointer].c_str());
        }
        wnoutrefresh(subwin);
        SetUpdated(false);
//      }
        wnoutrefresh(win);
}

void window_chat::AddMessage(std::string mes){
        short count=0;
        short linecount=0;
        short temppointer=0;
        std::string tempmes[chatsize];
        for(short i=0; i<chatsize; i++){
                tempmes[i]="";
        }
        for(short i=0; i<mes.length(); i++){
                tempmes[linecount]+=mes[i];
                count++;
                if(count>=GetWidth()-2){
                        count=0;
                        linecount++;
                        if(linecount>chatsize){
                                break;
                        }
                }
        }
        tempmes[linecount++]+='\n';
        if(mescount+linecount<chatsize){
                for(short i=mescount; i<=mescount+linecount; i++){
                        this->mes[i] = tempmes[temppointer++];
                        if(i>GetHeight()-3&&i<mescount+linecount){
                                ScrollDown();
                        }
                }
                mescount+=linecount;
        }else{
                short offset = mescount+linecount-chatsize;
                for(short i=0; i<mescount-offset; i++){
                        this->mes[i] = this->mes[i+offset];
                }
                mescount-=offset;
                for(short i=chatsize-linecount; i<chatsize; i++){
                        this->mes[i] = tempmes[temppointer++];
                }
                mescount+=linecount;
        }
        SetUpdated(true);
}

void window_chat::ScrollUp(){
        pointer=(pointer-1>-1)?pointer-1:0;
        SetUpdated(true);
}

void window_chat::ScrollDown(){
        short upperlimit = mescount-(GetHeight()-3);
        if(upperlimit<0){
                pointer = 0;
        }else if(pointer+1>upperlimit){
                pointer = upperlimit;
        }else
                pointer+=1;
        SetUpdated(true);
}


window_playerstats::window_playerstats(short x, short y, short width, short height):
        window_bordered(x, y, width, height)
{
        itemhlght=0;
}

window_playerstats::~window_playerstats(){
        player = NULL;
}

void window_playerstats::PrintStats(){
        werase(subwin);
        if(player==NULL)
                return;
        wprintw(subwin,"hp:%d\n",player->GetHp());
        if(player->GetInHand()!=NULL){
                std::string mes ="";
                mes+=player->GetInHand()->GetImg();
                mes+='\n';
                wprintw(subwin,"hand:%s\n",mes.c_str());
        }else{
                wprintw(subwin,"hand:none\n");
        }
        for(short i=0; i<player->GetInvSize(); i++){
                if(player->GetItem(i)!=NULL){
                        if(i==itemhlght){
                                waddch(subwin,(player->GetItem(i)->GetImg()|COLOR_PAIR(2)));
                        }else{
                                waddch(subwin,(player->GetItem(i)->GetImg()|COLOR_PAIR(1)));
                        }
                }else{
                        waddch(subwin,' ');
                }
        }
        SetUpdated(true);
}

void window_playerstats::AssignPlayer(creature* player){
        this->player = player;
}

void window_playerstats::FocusLeft(){
        for(short i=itemhlght-1; i>-1; i--){
                if(player->GetItem(i)!=NULL){
                        itemhlght=i;
                        break;
                }
        }
        SetUpdated(true);
}

void window_playerstats::FocusRight(){
        for(short i=itemhlght+1; i<player->GetInvSize(); i++){
                if(player->GetItem(i)!=NULL){
                        itemhlght=i;
                        break;
                }
        }
        SetUpdated(true);
}

short window_playerstats::GetHighlight(){       return itemhlght;}


window_dialog::window_dialog(short x, short y, short width, short height, std::string message):
        window_bordered(x,y,width,height), message(message){}

window_dialog::~window_dialog(){};

void window_dialog::AddOption(std::string opt){
        variants.Add(opt);
}

void window_dialog::RemoveOption(short place){
        variants.Remove(place);
}

void window_dialog::Draw(){

}

