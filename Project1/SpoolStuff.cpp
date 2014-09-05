#include "SpoolStuff.h"

using namespace std;

SpoolStuff::SpoolStuff(void){ //Default constructor
	Level = 2;
}


SpoolStuff::~SpoolStuff(void){ //Deconstructor
}

void SpoolStuff::GetAllPrinters(){
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

void SpoolStuff::ChangeKAPD(){ //Registry hack to change all printers to KAPD

	string printerStartLocation = "SYSTEM\\CurrentControlSet\\Control\\Print\\Printers\\";
	string printerEndLocation = "\\DsSpooler";
	string fullLocation;
	HKEY hKey;

	for (int i = 0; i < sizeof(list) - 1; i++){
		fullLocation = printerStartLocation + list[i].pPrinterName + printerEndLocation;

		//Test: "SYSTEM\\CurrentControlSet\\Control\\Print\\Printers\\Foxit Reader PDF Printer\\DsSpooler"

		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,(fullLocation.c_str()),0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS){ //Attempts to open the Registry Key location.
			TCHAR value[256];
			DWORD dwKeyDataType;
			UCHAR byteRegArray[] = {0x01};

			RegSetValueEx(hKey, "printKeepPrintedJobs", 0, REG_BINARY, (BYTE *)byteRegArray, sizeof(byteRegArray)); //Sets the key to 1 (Currently not working)
		}
			RegCloseKey(hKey); //Closes the key.
	}
}

void SpoolStuff::GetDefaultSpoolFileDirectory(){
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

void SpoolStuff::CreateFolder(){
	mkdir("CachedFiles"); //Should automatically ignore if the file is already there.
}

void SpoolStuff::MoveFiles(){
	HANDLE hFile;
	WIN32_FIND_DATA file;
	string AllFiles = SpoolLocation + "\\*.*";
	hFile = FindFirstFile(AllFiles.c_str(), &file);

	string startLocation;
	string endLocation;

	while (FindNextFile(hFile, &file) != 0){
		cout << "File Name" << string(file.cFileName) << "\n";
		startLocation = SpoolLocation + "\\" + string(file.cFileName);
		endLocation = "C:\\Users\\Jash\\Documents\\Visual Studio 2012\\Projects\\Project1\\Project1\\CachedFiles\\" + string(file.cFileName);
		bool b = CopyFile(startLocation.c_str(),endLocation.c_str(),0);
		if (!b){
			cout << "Error: " << GetLastError() << "\n";
		}
		else{
			cout << "OKay! \n";
		}
	}
}

void SpoolStuff::GetPOSTLink(){
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

void SpoolStuff::PostToLink(){

}