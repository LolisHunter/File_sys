#pragma once
#include <iostream>
#include <fstream>
#include "Volume.h"
#include <vector>
#define BUFFER 512
using namespace std;

struct Type {
	uint8_t code;
	char extension[17];
};

class Root : protected Volume
{
private:
	vector<Volume> list;
	vector<Type> type;
public:
	Root() {
		this->Name = '/';
		this->Ss = 1;
		this->Sc = 2;
		this->Sb = 3;
		this->Nf = 4;
		this->Sf = 5;
		this->Sv = 6;
		this->Nc = 7;
		this->StCluster = 8;
	}
	//~Root() {};
	void RootCreate(char fileName[]);
};

template<class T>
void SaveByte(ofstream& fout, T in) {
	char c;
	for (uint8_t i = 0; i < sizeof(T); i++) {
		c = in;
		fout << c;
		in = in >> 8;
	}
}

void Root::RootCreate(char fileName[]) {
	// Boot
	ofstream fout(fileName, ios::in | ios::out | ios::binary);
	if (!fout.is_open()) { DEBUG_PRINT("Create fail"); }
	SaveByte(fout, this->Ss);
	SaveByte(fout, this->Sc);
	SaveByte(fout, this->Sb);
	SaveByte(fout, this->Nf);
	SaveByte(fout, this->Sf);
	SaveByte(fout, this->Sv);
	SaveByte(fout, this->Nc);
	SaveByte(fout, this->StCluster);
	//SaveByte(fout, this->Name);
	string name = "abcdefgh";
	for (auto i : name) {
		SaveByte(fout, i);
	}
	SaveByte(fout, (short)0xA2F7); // end key

	for (int i = 0; i < 512 - 22 - name.length(); i++) {
		fout << (uint8_t)0;
	}
}