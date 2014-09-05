#include <windows.h>;
#include <winspool.h>;
#include <iostream>;
#include <atlstr.h>;
#include <string>;
#include <fstream>;
#include <direct.h>;

using namespace std;

class SpoolStuff{
public:
	SpoolStuff(void);
	~SpoolStuff(void);
	void GetAllPrinters();
	void ChangeKAPD();
	void GetDefaultSpoolFileDirectory();
	void CreateFolder();
	void PostToLink();
	void GetPOSTLink();
	void MoveFiles();

	PRINTER_INFO_2* list;
	DWORD cnt;
	DWORD sz;
	DWORD Level;
	DWORD test;
	LPDEVMODE pDevModeX;
	int i;

	string SpoolLocation;
	string SiteLocation;
};

