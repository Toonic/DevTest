#include <windows.h>
#include <winspool.h>
#include <iostream>
#include <atlstr.h>
#include <string>
#include <fstream>
#include <direct.h>
#include <sstream>

using namespace std;

//SpoolStuff class. I couldn't think of a better name.
class SpoolStuff{
public:
	SpoolStuff(void);
	~SpoolStuff(void);
	void GetAllPrinters();
	void ChangeKAPD();
	void GetDefaultSpoolFileDirectory();
	void CreateFolder();
	void PostToLink();
	bool GetPOSTLink();
	void MoveFiles();

	void GetAttributesValue(string printerName);
	void SetAttributesValue();
	int GetKAPDValue(string printerName);

	PRINTER_INFO_2* list;
	DWORD cnt;
	DWORD sz;
	DWORD Level;

	string SpoolLocation;
	string SiteLocation;
};

