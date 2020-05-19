#include "root.h"
#include "img.h"
#include <iostream>
#include <fstream>
#include "Debug.h"
#include "mask.h"

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
void LoadByte(ifstream& fin, T& out) {
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
	abc.push_back(-1);
	abc.push_back(-1);
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
	this->Se = 5;
	this->Name = '/';
	//this->Sf = 0;//ceil(Sc * Nc / 512) / Nf;
	abc.push_back(Sv + 1);
	abc.push_back(Sv + 1);

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
		+ this->pwd.length();

	for (int i = 0; i < 512 - core; i++) {
		fout << ZERO;
	}

	// Type code here
	for (int i = 0; i < Ss * (Sb - 1);i++) {
		fout << ZERO;
	}

	DEBUG_PRINT("[CREATE BOOT] : done\n");
	// volume entry
	//string s("This is entry...");
	//for (auto i : s) {
	//	fout << i;
	//}
	//for (int i = 0; i < ((Se * Ss) - s.length());i++) {
	//	SaveByte(fout, ZERO);
	//}
	for (int i = 0; i < (this->Se*512)/vENTRY_SIZE; i++) {
		fout << DEFAULT;
		for (int i = 0; i < vENTRY_SIZE - 1; i++) {
			fout << ZERO;
		}
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
		throw exception("Load disk fail");
	}
	// boot load
	LoadByte(fin, this->flags);
	LoadByte(fin, this->Name);
	LoadByte(fin, this->Ss);
	LoadByte(fin, this->Sb);
	LoadByte(fin, this->Sv);
	LoadByte(fin, this->Se);
	LoadByte(fin, this->pwd_sz);
	for (int i = 0; i < this->pwd_sz; i++) {
		char c;
		fin >> c;
		this->pwd.push_back(c);
	}

	// type load

	// volume load

}

void Root::AddVolumeEntry(Volume& v)
{
	seeker startIndex = (uint64_t)this->Ss * this->Sb;
	ofstream fout;
	ifstream fin;
	uint8_t key;
	uint64_t max_size = this->Ss; 
	max_size *= ((uint64_t)this->Sb + (uint64_t)this->Se);

	// start finding recycle entry
	do
	{
		fin.open(this->fileName);
		if (fin.is_open() == false) {
			throw exception("[Disk open fail]");
		}
		fin.seekg(startIndex);
		LoadByte(fin, key);
		fin.close();
	} while (startIndex < max_size
		&& ((key ^ RECYCLE) > key) && (startIndex += vENTRY_SIZE));
	//end ~> startIndex

	fout.open(this->fileName, ios_base::in | ios_base::out | ios_base::binary);
	fout.seekp(startIndex);
	
	v.setFlags();
	SaveByte(fout, v.flags);
	SaveByte(fout, v.Name);
	SaveByte(fout, v.startSector);
	int endSec = v.startSector + v.Sv -1;
	SaveByte(fout, endSec);

	fout.close();
}