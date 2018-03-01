#ifndef iconst_h
#define iconst_h

class iConstInt{
int code;
public:
	iConstInt(int code);
	iConstInt();
	~iConstInt();
	int GetCode()const;
};

bool operator==(const int& c, const iConstInt& k);
bool operator==(const iConstInt& k1, const iConstInt& k2);

#endif
