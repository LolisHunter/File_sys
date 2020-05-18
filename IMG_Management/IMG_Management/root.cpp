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
Root::Root() {
	this->Name = '/';
}

void Root::RootCreate(char fileName[]) {
	// Boot
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
	//cin >> size;
	uint64_t byte = size; byte = byte * 1024 * 1024 * 1024;
	this->Sv = byte / 512;
	this->Sb = 8;
	this->Ss = BUFFER;
	this->Nf = 0;
	this->Sc = 8;
	this->Nc = 1111111111111; // can tinh
	this->Sf = 0;//ceil(Sc * Nc / 512) / Nf;
	SaveByte(fout, this->Ss);
	SaveByte(fout, this->Sc);
	SaveByte(fout, this->Sb);
	SaveByte(fout, this->Nf);
	SaveByte(fout, this->Sf);
	SaveByte(fout, this->Sv);
	SaveByte(fout, this->Nc);
	SaveByte(fout, this->StCluster);
	SaveByte(fout, this->Name);
	//string name = "abcdefgh";
	//for (auto i : name) {
	//	SaveByte(fout, i);
	//}
	SaveByte(fout, (short)0xA2F7); // end key

	for (int i = 0; i < 512 - 23; i++) {
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
	for (int i = 0; i < ((8 * Ss) - s.length());i++) {
		SaveByte(fout, (uint8_t)0x00);
	}
	DEBUG_PRINT("[CREATE ENTRY] : done\n");

	// DATA ... ...
	fout.close();
}

void Root::RootLoad(char fileName[])
{

}