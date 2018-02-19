#ifndef mypair_h
#define mypair_h
class myColor;

class myPair{
        myColor* fg;
        myColor* bg;
        short num;
public:
        myPair(myColor* fg, myColor* bg, short num);
        myPair();
        ~myPair();
        myColor* GetFg();
        myColor* GetBg();
};
#endif
