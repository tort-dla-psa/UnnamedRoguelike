#ifndef lists_h
#define lists_h
#include<string>
#include<vector>

class listline{
        std::string mes;
        short num;
public:
        listline(std::string mes, short num);
        ~listline();
        void ChangeNum(short num);
        short GetNum();
        std::string GetMes();
};

class list{
        std::vector<listline> lines;
        short focus;
public:
        list();
        ~list();
        void Add(std::string mes);
        void Remove(short pos);
        std::string GetMes(short pos);
        short GetFocus();
        short GetSize();
};
#endif
