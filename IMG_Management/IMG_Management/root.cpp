#include "root.h"
#include "img.h"
#include <iostream>
#include <fstream>
#include "Debug.h"

using namespace std;
template<class T>
void SaveByte(ofstream& fout, T in) {
	char c;
	for (uint8_t i = 0; i < sizeof(T); i++) {
		c = in;
		fout << c;
		in = in >> 8;
	}
}

template<class T>
static void LoadByte(ifstream& fin, T& out) {
	out = 0;
	char c;
	for (uint8_t i = 0; i < sizeof(T); i++) {
		fin >> c;
		T temp = c;
		temp = temp << (8 * i);
		out += temp;
	}
}

Root::Root() {
}

void Root::RootCreate(char fileName[]) {
	// Boot
	this->fileName = fileName;
	ifstream fin(fileName);
	if (fin.is_open())
	{
		cout << "This file exist, you want overwrite" << endl;
		cout << "Input (y/n key):";
		char choose;
		choose = getchar();
		fin.close();
		if (choose != 'y')
			return;
	}
	ofstream fout(fileName,ios::binary);
	if (!fout.is_open()) { DEBUG_PRINT("Create fail"); }
	cout << "nhap kich thuoc: (GB)";
	float size = 0;
	cin >> size;
	uint64_t byte = size; byte = byte * 1024 * 1024 * 1024;
	this->Sv = byte / 512;
	this->Sb = 8;
	this->Ss = BUFFER;
	this->Se = 8;
	this->Name = '/';

	//this->Sf = 0;//ceil(Sc * Nc / 512) / Nf;
	SaveByte(fout, this->flags);
	SaveByte(fout, this->Name);
	SaveByte(fout, this->Ss);
	SaveByte(fout, this->Sb);
	SaveByte(fout, this->Sv);
	SaveByte(fout, this->Se);
	SaveByte(fout, this->pwd_sz);
	for (auto i : this->pwd) {
		SaveByte(fout, i);
	}
	
	int core = sizeof(this->flags)
		+ sizeof(this->Name)
		+ sizeof(this->Ss)
		+ sizeof(this->Sb)
		+ sizeof(this->Sv)
		+ sizeof(this->Se)
		+ sizeof(this->pwd_sz)
		+ sizeof(this->pwd);

	for (int i = 0; i < 512 - core; i++) {
		fout << (uint8_t)0;
	}

	// Type code here
	for (int i = 0; i < Ss * (Sb - 1);i++) {
		fout << (uint8_t)0;
	}

	DEBUG_PRINT("[CREATE BOOT] : done\n");
	// volume entry
	string s("This is entry...");
	for (auto i : s) {
		fout << i;
	}
	for (int i = 0; i < ((Se * Ss) - s.length());i++) {
		SaveByte(fout, (uint8_t)0x00);
	}
	DEBUG_PRINT("[CREATE ENTRY] : done\n");

	// DATA ... ...
	fout.close();
}

void Root::RootLoad(char fileName[])
{
	ifstream fin(fileName, ios::in | ios::binary);
	if (!fin.is_open()) {
		DEBUG_PRINT("[OPEN FAIL] \n");
		throw("create fail");
	}
	LoadByte(fin, this->flags);
}