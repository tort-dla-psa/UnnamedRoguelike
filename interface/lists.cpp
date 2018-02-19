#include"lists.h"

listline::listline(std::string mes, short num):
        mes(mes),num(num){}

listline::~listline(){}

void listline::ChangeNum(short num){    this->num = num;}
short listline::GetNum(){               return num;}
std::string listline::GetMes(){         return mes;}


list::list(){}

list::~list(){}

void list::Add(std::string mes){
        lines.push_back(listline(mes,lines.size()));
}

void list::Remove(short pos){
        if(pos<lines.size())
                lines.erase(lines.begin()+pos);
}

std::string list::GetMes(short pos){
        if(pos<lines.size() && pos>-1)
                return lines[pos].GetMes();
        return "NOP";
}

short list::GetFocus(){ return focus;}
short list::GetSize(){  return lines.size();}
