
class keytie{
	char key;
	int returncode;
public:
	keytie(char key, int returncode);
	~keytie();
	void SetKey(char key);
	void SetCode(int code);
	char GetChar();
	int GetCode();
};
