#include "KeyTie.h"

keytie::keytie(char key, int returncode):
	key(key),returncode(returncode){}

keytie::~keytie(){}

void keytie::SetKey(char key){	this->key = key;}
void keytie::SetCode(int code){	this->returncode = code;}
char keytie::GetChar(){	return key;}
int keytie::GetCode(){	return returncode;}
