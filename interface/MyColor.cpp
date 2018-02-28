#include<ncurses.h>
#include"MyColor.h"

myColor::myColor(short r, short g, short b, std::string name, short num):
        r(r),g(g),b(b),name(name),num(num)
{
        init_color(num,r,g,b);
}
myColor::myColor(){}
myColor::~myColor(){}

short myColor::GetR(){          return r;}
short myColor::GetG(){          return g;}
short myColor::GetB(){          return b;}
short myColor::GetNum(){        return num;}
std::string myColor::GetName(){ return name;}

