#ifndef color_h
#define color_h
#include<string>

class myColor{
        short r,g,b,num;
        std::string name;
public:
        myColor(short r, short g, short b, std::string name, short num);
        myColor();
        ~myColor();
        std::string GetName();
        short GetNum();
        short GetR();
        short GetG();
        short GetB();
};
#endif
