#include "iConstInt.h"

iConstInt::iConstInt(int code):code(code){}
iConstInt::iConstInt():code(0){}
iConstInt::~iConstInt(){}
int iConstInt::GetCode()const{	return code;}
bool operator==(const int& c, const iConstInt& k){	return k.GetCode()==c;}
bool operator==(const iConstInt& k1, const iConstInt& k2){	return k1.GetCode()==k2.GetCode();}
