#include <iostream>;
#include <string>;
#include <fstream>;

#include "SpoolStuff.h";

using namespace std;

int main(){

	SpoolStuff *spools = new SpoolStuff();

	spools->GetAllPrinters();
	spools->CreateFolder();
	spools->GetDefaultSpoolFileDirectory();
	spools->GetPOSTLink();
	spools->ChangeKAPD();
	spools->MoveFiles();
	spools->PostToLink();

	cin.get();

	return 0;
}