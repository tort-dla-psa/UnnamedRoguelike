#ifndef color_h
#define color_h

class myColor{
        short r,g,b,num;
public:
        myColor(short r, short g, short b, short num);
        myColor();
        ~myColor();
        short GetNum();
        short GetR();
        short GetG();
        short GetB();
};
#endif
