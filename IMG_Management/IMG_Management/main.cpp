#include <iostream>
#include <string>
//#include "DiskImage.h"
//#include "Volume.h"
#include "Debug.h"
#include "root.h"
#include "img.h"
using namespace std;
#include "mask.h"
#define TEST

#ifdef TEST
void main() {
	Root root;
	root.RootCreate("ab.iso");
	root.RootLoad("ab.iso");
	cout << root.scope.size();
	root.CreateVolume();
}
#endif // TEST


#ifndef TEST
void tree(Root& root);
Root open(string fileName);
Root Create(string fileName);
void CreateVolume(Root& root);
int main(int agrc, char* agrv[]) 
{
	string cmd;
	Root root;
	do
	{
		cout << "\n>";
		getline(cin, cmd);
		try
		{

			size_t found = cmd.find(" ");
			string  prefx;
			if (found == string::npos) {
				prefx = cmd.substr(0);
			}
			else {
				prefx = cmd.substr(0, found);
			}
			
			if ( prefx == "open") {
				found++;
				string file = cmd.substr(found);
				root = open(file);
			}
			else if (prefx == "tree") {
				if (root.is_open()) {
					tree(root);
				}
				else {
					cout << "You did not open disk or create yet." << endl;
				}
			}
			else if (prefx == "create") {
				found++;
				string file = cmd.substr(found);
				root = Create(file);
			}
			else if (prefx == "createVol") {
				if (root.is_open()) {
					CreateVolume(root);
				}
				else {
					cout << "You did not open disk or create yet." << endl;
				}
			}
		}
		catch (exception& e)
		{
			cout << e.what() << endl;
		}
	} while (cmd != "exit");

}

void tree(Root& root) {
	root._list();
}
Root open(string fileName) {
	Root root;
	root.RootLoad(fileName);
	return root;
}
Root Create(string fileName) {
	Root root;
	root.RootCreate(fileName);
	return root;
}
void CreateVolume(Root& root) {
	root.CreateVolume();
}
void DeleteVolume(Root& root) {
	
}
#endif // MAIN