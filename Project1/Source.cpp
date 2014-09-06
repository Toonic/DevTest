#include <iostream>;
#include <string>;
#include <fstream>;
#include <thread>

#include "SpoolStuff.h";

using namespace std;

SpoolStuff *spools = new SpoolStuff();

void task1();

int main(){

	spools->GetAllPrinters();
	spools->CreateFolder();
	spools->GetDefaultSpoolFileDirectory();
	spools->GetPOSTLink();
	spools->ChangeKAPD();

	thread t1(MoveFileTask);
	thread t2(PostToLinkTask);

	t1.join();
	t2.join();

	cin.get();

	return 0;
}

void MoveFileTask(){
	spools->MoveFiles();
}

void PostToLinkTask(){
	spools->PostToLink();
}