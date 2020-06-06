#include <iostream>
#include <string>
//#include "DiskImage.h"
//#include "Volume.h"
#include "Debug.h"
#include "root.h"
#include "img.h"
using namespace std;
#include "mask.h"
//#define TEST

#ifdef TEST
void main() {
	/*Root root;
	root.RootCreate("ab.iso");
	cout << root.scope.size();
	root.CreateVolume();
	Volume *a = &root.list[0];
	a->Import("E:\\Ham",NULL,root.type_list);*/
	Root root1;
	root1.RootLoad("ab.iso");
	Volume *a = &root1.list[0];
	a->Export("", &a->entry[0]);
}
#endif // TEST


#ifndef TEST
void tree(Root& root);
Root open(string fileName);
Root Create(string fileName);
void CreateVolume(Root& root);
void DeleteVolume(Root& root, char Name);
void help();
void setPwd(Root& root);
void Import(Root& root,Entry * temp,Volume *temp1);
void Export(Root& root, Entry * temp, Volume *temp1);
void hide_show_Vol(Root& root, char Name);
void createP(Root& root);

int main(int agrc, char* agrv[]) 
{
	string cmd;
	Root root;
	Volume* vol = NULL;
	vector<Entry*> entr;
	int here = -1;
	do
	{
		if (here == -1) {
			cout << "~";
		}
		else if (here == 0) {
			cout << "/";
		}
		else if (here == 1) {
			cout << (char)vol->Name;
		}
		else {
			cout << entr[here - 2]->name;
		}
		cout << " > ";
		do
		{
			getline(cin, cmd);
		} while (cmd == "");
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

			if (prefx == "open") {
				found++;
				string file = cmd.substr(found);
				root = open(file);
				here = 0;
			}
			else if (prefx == "help" || prefx == "?") {
				help();
			}
			else if (prefx == "cls") {
				system("cls");
			}
			else if (prefx == "tree") {
				if (root.is_open()) {
					tree(root);
				}
				else {
					cout << "You did not open disk or create yet." << endl;
				}
			}
			else if (prefx == "createRoot") {
				if (here > 0) {
					here = 0;
					vol = NULL;
					entr.clear();
				}
				if (found == string::npos) {
					help();
				}
				else {
					found++;
					string file = cmd.substr(found);
					root = Create(file);
					here = 0;
				}
			}
			else if (prefx == "createVol") {
				if (here > 0) {
					here = 0;
					vol = NULL;
					entr.clear();
				}
				if (root.is_open()) {
					CreateVolume(root);
				}
				else {
					cout << "You did not open disk or create yet." << endl;
				}
			}
			else if (prefx == "ls") {
				if (here == 0) {
					root.ls();
				}
				else if (here == 1) {
					vol->ls();
				}
				else {
					entr[here - 2]->ls();
				}
			}
			else if (prefx == "cd") {
				if (found == string::npos) {
				}
				else {
					found++;
					string dir = cmd.substr(found);
					if (dir == "..") {
						if (here > 0) {
							if (here == 1) {
								vol = NULL;
								here--;
							}
							else {
								entr.pop_back();
								here--;
							}
						}
					}
					if (here == 0) {
						vol = root.getVolume(dir[0]);
						if (vol) { here++; }
					}
					else if (here == 1) {
						Entry* temp = vol->getEntry(dir);
						if (temp) {
							if (!temp->isFolder()) {
								cout << "  [This is a file]\n";
							}
							else {
								entr.push_back(temp);
								here++;
							}
						}
					}
					else {
						Entry* temp = entr[here - 2]->getEntry(dir);
						if (temp) {
							if (!temp->isFolder()) {
								cout << "  [This is a file]\n";
							}
							else {
								entr.push_back(temp);
								here++;
							}
						}
					}
				}
			}
			else if (prefx == "home") {
				here = 0;
				vol = NULL;
				entr.clear();
			}
			else if (prefx == "delVol") {
				if (found == string::npos) {
					help();
				}
				else {
					found++;
					string v = cmd.substr(found);
					DeleteVolume(root, v[0]);
					here = 0;
					vol = NULL;
					entr.clear();
				}
			}
			else if (prefx == "import") {
				if (here == 1)
				{
					Import(root, NULL, vol);
				}
				else
				{
					Entry* temp;
					if (here - 2 < 0)
						cout << "Vui long cd" << endl;
					temp = entr[here - 2];
					if (temp->size == 0)
						Import(root, temp,vol);
					else
						cout << "Khong the import vao file" << endl;
				}
			}
			else if (prefx == "export") {
				if (here == 1)
				{
					string a;
					cout << "Nhap ten file:";
					getline(cin, a);
					Entry* temp = vol->getEntry(a);
					if (temp)
						Export(root, temp, vol);
					else
						cout << "Khong thay entry do" << endl;
				}
				else
				{
					Entry* temp = entr[here - 2];
					Export(root, temp, vol);
				}
			}
			else if (prefx == "pwd") {
				createP(root);
			}
			else if (prefx == "hide" || prefx == "show") {
				if (found == string::npos) {
					help();
				}
				else {
					found++;
					string v = cmd.substr(found);
					hide_show_Vol(root, v[0]);
					here = 0;
					vol = NULL;
					entr.clear();
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
void DeleteVolume(Root& root, char Name) {
	root.DeleteVolume(Name);
}
void help() {
	cout << "type help/? to see this doc\n";
	cout << " open			open disk\n";
	cout << " createRoot	create iso disk\n";
	cout << " createVol		create volume\n";
	cout << " delVol		delete volume\n";
	cout << " cd			change directory\n";
	cout << " ls			listing the contents of a directory\n";
	cout << " tree			print volume tree\n";
	cout << " import		import file\n";
	cout << " export		export file\n";
	cout << " hide/show		hide or show a volume\n";
	cout << " home			go to root\n";
	cout << " pwd			set password\n";
}
void setPwd(Root& root) {
	root.createPassword();
}
void Import(Root& root,Entry * temp, Volume * temp1) {
	string s;
	root.ls();
	if (s == "exit") {
		return;
	}
	if (temp1) {
		cout << "Enter file path"; // NULL neu duong dan tuong doi
		getline(cin, s);
		temp1->Import(s, temp, root.type_list);
	}
}
void Export(Root& root,Entry * temp,Volume * temp1) {
	string s;
	root.ls();
	if (temp1) {
		cout << "Enter file path"; // nhap duong dan muon chua file (duong dan tuyet doi) hoac "" (duong dan tuong doi)
		getline(cin,s);
		temp1->Export(s, temp);
	}
}
void hide_show_Vol(Root& root, char Name) {
	root.hide_show_Vol(Name);
}
void createP(Root& root) {
	root.createPassword();
}
#endif // MAIN