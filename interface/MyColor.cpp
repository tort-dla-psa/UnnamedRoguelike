#include<ncurses.h>
#include"MyColor.h"

myColor::myColor(short r, short g, short b, short num):
        r(r),g(g),b(b),num(num)
{
        init_color(num,r,g,b);
}
myColor::myColor(){}
myColor::~myColor(){}

short myColor::GetR(){          return r;}
short myColor::GetG(){          return g;}
short myColor::GetB(){          return b;}
short myColor::GetNum(){        return num;}

