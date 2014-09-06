#include "SpoolStuff.h"

using namespace std;

SpoolStuff::SpoolStuff(void){ //Default constructor
	Level = 2;
}


SpoolStuff::~SpoolStuff(void){ //Deconstructor
}

//Get all Printers from EnumPrinters.
void SpoolStuff::GetAllPrinters(){
	EnumPrinters(PRINTER_ENUM_LOCAL, NULL, Level, NULL, 0, &sz, &cnt );

	//---This Fills the PRINTER_INFO_2 list---

	if ((list = (PRINTER_INFO_2*) malloc(sz)) == 0 )   //No Idea what this does yet. Snipped online.
		return;
	
	if(!EnumPrinters(PRINTER_ENUM_LOCAL, NULL, Level, (LPBYTE)list, sz, &sz, &cnt)){free( list ); //Fills the list with all Printer Names?
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

	/*
	Orginally I was going to go through EnumPrinters to DevMode to DocumentSettings, in the end I was unable to find the option to actually change Keep All Printed Documents and
	moved to changing the Registery files dirrectly.
	*/
}

//Changing Keep All Printed Documents. --Note: Read message at bottom for the issue/solution I ran into--
void SpoolStuff::ChangeKAPD(){ //Registry hack to change all printers to KAPD

	string printerStartLocation = "SYSTEM\\CurrentControlSet\\Control\\Print\\Printers\\";
	string printerEndLocation = "\\DsSpooler";
	string dSpoolLocation;
	string printerLocation;
	HKEY hKey;

	for (int i = 0; i < sizeof(list) - 1; i++){

		//---------TEST SHIT PLEASE IGNORE-----------------
		GetKAPDValue(list[i].pPrinterName);
		//---------TEST SHIT PLEASE IGNORE-----------------


		dSpoolLocation = printerStartLocation + list[i].pPrinterName + printerEndLocation; //Sets the location for each printer's DsSpool.

		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,(dSpoolLocation.c_str()),0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS){ //Attempts to open the Registry Key location.
			TCHAR value[256];
			DWORD dwKeyDataType;
			UCHAR byteRegArray[] = {0x01}; //Sets the byte data to 1.

			RegSetValueEx(hKey, "printKeepPrintedJobs", 0, REG_BINARY, (BYTE *)byteRegArray, sizeof(byteRegArray)); //Sets the key to 1 in registry which should enable it. However, for some reason
																															 // While the changes take effect, they don't actually work even after a restart
		}
		
		RegCloseKey(hKey); //Closes the key.

		printerLocation = printerStartLocation + list[i].pPrinterName;


		/*
		Basically, at this point in time, what I need to do to get this to fully work is to check to see if DsSpooler\printKepepPrintedJobs is set to 1.
		If it isn't set to 1, I need to set it to 1, then go back to \\Printers\\PRINTERSNAME and change Attributes to add 256 Decimal places. This also requiers a restart or a restart of
		the Spooler service. Once I do that, it will end up working full and correctly.

		The main issue I was having was I couldn't find a way to correctly read, compare and use binary data from the Registry.
		*/
	}
}

//Gets the Default Spool File Directory from Regestry  --NOTE: I know each printer can have it's own Spool File, however this was ignored.--
void SpoolStuff::GetDefaultSpoolFileDirectory(){
	CString location;
	HKEY hKey;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,("SYSTEM\\CurrentControlSet\\Control\\Print\\Printers"),0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS){ //Attempts to open the Registry Key location.
		TCHAR value[256];
		DWORD dwKeyDataType;
		DWORD dwDataBufSize = 256;
		if (RegQueryValueEx(hKey,("DefaultSpoolDirectory"), NULL, &dwKeyDataType,(LPBYTE) &value, &dwDataBufSize) == ERROR_SUCCESS){ //Trys to get data from DefaultSpoolDirectory
			location = CString(value); //Converts it to CString
		}
		RegCloseKey(hKey); //Closes the key.
	}
	SpoolLocation = location; //Sets the SpoolLocation variable to the location.
}

//Creates the folder CachedFiles in the same location of the EXE
void SpoolStuff::CreateFolder(){
	mkdir("CachedFiles"); //Should automatically ignore if the file is already there.
}

//Moves all Files form Spools Dirrectory to CachedFiles
void SpoolStuff::MoveFiles(){
	HANDLE hFile;
	WIN32_FIND_DATA file;
	string AllFiles = SpoolLocation + "\\*.*";
	hFile = FindFirstFile(AllFiles.c_str(), &file); //Gets the first file in the folder.

	string startLocation;
	string endLocation;

	while (FindNextFile(hFile, &file) != 0){ //While Loop to find next file until no more files.
		cout << "File Name" << string(file.cFileName) << "\n"; //Currently outputting file names
		startLocation = SpoolLocation + "\\" + string(file.cFileName); //Sets the location for the starting file.
		endLocation = "CachedFiles\\" + string(file.cFileName); //Sets the locaiton for the end file.
		CopyFile(startLocation.c_str(),endLocation.c_str(),0); //Copys file from start to end.
	}
}

//Gets the post link
void SpoolStuff::GetPOSTLink(){
	string line;
	ifstream myfile ("SiteLocation.txt"); //Basic File IO. Do I need to explain?
	if (myfile.is_open()){
		while(getline (myfile,line) ){
			SiteLocation = line;
		}
		myfile.close();
	}
	else cout << "SiteLocation.txt not found"; 
}

//Posts it to the website Currently not coded.
void SpoolStuff::PostToLink(){

}

//Get KAPD Value from the Regestry, return true if its set to 1.
bool SpoolStuff::GetKAPDValue(string printerName){
	string printerStartLocation = "SYSTEM\\CurrentControlSet\\Control\\Print\\Printers\\";
	string printerEndLocation = "\\DsSpooler";
	string dSpoolLocation;

	HKEY hKey;

	string test;

	dSpoolLocation = printerStartLocation + printerName + printerEndLocation;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,(dSpoolLocation.c_str()),0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS){
		TCHAR value[256];
		DWORD dwKeyDataType;
		DWORD dwDataBufSize = 10;

		if (RegQueryValueEx(hKey,("printKeepPrintedJobs"), NULL, &dwKeyDataType,(LPBYTE) &value, &dwDataBufSize) == ERROR_SUCCESS){ //Trys to get data from DefaultSpoolDirectory
			cout << "Hello World " << value[0]; //Converts it to CString
		}

		/*
		if (RegQueryValueEx(hKey,("printKeepPrintedJobss"), NULL, &dwKeyDataType,(LPBYTE) &value, &dwDataBufSize) == ERROR_SUCCESS){ //Trys to get data from DefaultSpoolDirectory
			test = CString(value); //Converts it to CString
		}
		*/
	}


	return true;
}