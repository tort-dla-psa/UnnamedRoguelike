#include<ncurses.h>
#include"MyColor.h"
#include"MyPair.h"

myPair::myPair(myColor* fg, myColor* bg, short num):
        fg(fg),bg(bg),num(num)
{
        init_pair(num,fg->GetNum(),bg->GetNum());
}
myPair::myPair(){}
myPair::~myPair(){
        fg=NULL;
        bg=NULL;
}

myColor* myPair::GetFg(){       return fg;}
myColor* myPair::GetBg(){       return bg;}

