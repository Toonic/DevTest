#include <iostream>;
#include <string>;
#include <fstream>;
#include <thread>

#include "SpoolStuff.h";

using namespace std;

SpoolStuff *spools = new SpoolStuff();

void MoveFileTask();
void PostToLinkTask();

int main(){

	cout << "Getting a list of all printers.\n";
	spools->GetAllPrinters();

	cout << "Checking and Creating the CachedFiles folder.\n";
	spools->CreateFolder();

	cout << "Getting the default spool dirrectory.\n";
	spools->GetDefaultSpoolFileDirectory();

	cout << "Changing all printers to 'Keep Printed Documents'.\n";
	spools->ChangeKAPD();

	cout << "Attempting to get POST link.\n";
	if(spools->GetPOSTLink()){
		cout << "Link found, moving files and POSTing them to the site.\n";
		thread t1(MoveFileTask);
		thread t2(PostToLinkTask);

		t1.join();
		t2.join();
	}
	else{
		cout << "Unable to find POST Link. Please make sure SiteLocation.txt exisits.";
	}
	

	cin.get();

	return 0;
}

void MoveFileTask(){
	spools->MoveFiles();
}

void PostToLinkTask(){
	spools->PostToLink();
}