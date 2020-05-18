#include "Volume.h"
//
//
//string Volume::getName16Char(const string& source)
//{
//	string name = source;
//	if (name.length() >= 16)
//	{
//		name = name.substr(0, 16);
//	}
//	else
//	{
//		for (int i = name.length(); i < 16; i++)
//		{
//			name += " ";
//		}
//	}
//
//	return name;
//}
//
//Volume::Volume(const string& name, const int& size, const int& startCluster, const int& number)
//{
//	this->name = this->getName16Char(name);
//	this->size = size;
//	this->remaining = size;
//	this->startCluster = startCluster;
//	this->number = number;
//}
//
//void Volume::InitVolume(const string& disk)
//{
//	fstream fout(disk, ios_base::binary | ios_base::out | ios_base::in);
//	if (fout.is_open() == false)
//	{
//		throw exception("Can't open disk. Can't init volume");
//	}
//
//	//Go to position at cluster 1
//	fout.seekp(8 * 512 + 4 * number, ios_base::beg);
//	fout.write((char*)&startCluster, 4);
//
//	//Go to this start cluster
//	fout.seekp(8 * 512 * startCluster, ios_base::beg);
//	fout << name;
//	fout.write((char*)&startCluster, 4);
//	fout.write((char*)&size, 4);
//	fout.write((char*)&remaining, 4);
//
//	fout.close();
//}
#include <iostream>
#include <fstream>
#include "img.h"
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
bool Volume::Create(vector<uint32_t> &abc,string fileName)
{
	cout << "nhap kich thuoc: (GB)";
	int size;
	cin >> size;
	uint64_t byte = (uint64_t)size * 1024 * 1024 * 1024;
	cout << "Nhap volume name";
	cin.get();
	int i = 1;
	uint64_t between;
	uint32_t start{},end;
	while (i < abc.size())
	{
		between = (uint64_t)(abc[i + 1] - abc[i]) * 512;
		if (between >= byte)
		{
			start = abc[i] + 1;
			end = abc[i + 1] - 1;
			abc.push_back(start);
			abc.push_back(end);
			// sort lai

			break;
		}
		i += 2;
	}
	if (i > abc.size()) {
		DEBUG_PRINT("CAN NOT CREATE NEW VOLUME");
		return EXIT_FAILURE;
	}
	this->Sv = byte / 512;
	this->Sb = 1;
	this->Ss = 512;
	this->Nf = 1;
	this->Sc = 8;
	this->Nc = 1111111111111; // can tinh
	this->Sf = ceil((Sc * Nc) / 512) / Nf;
	this->FAT_len = Sc * Nc / 512;
	ofstream fout(fileName, ios::in | ios::out | ios::binary);
	seeker pos = start; pos = pos * 512;
	fout.seekp(pos);
	// luu volume entry == > root luu entry neu create success
	//SaveByte(fout,this->Name);
	//SaveByte(fout, start);
	//SaveByte(fout, end);
	// luu volume
	// boot
	SaveByte(fout, this->Ss);
	SaveByte(fout, this->Sc);
	SaveByte(fout, this->Sb);
	SaveByte(fout, this->Nf);
	SaveByte(fout, this->Sf);
	SaveByte(fout, this->Sv);
	SaveByte(fout, this->Nc);
	SaveByte(fout, this->StCluster);
	SaveByte(fout, this->FAT_len);
	SaveByte(fout, this->Name);
	SaveByte(fout, (short)0xA2F7); // end key

	int core = sizeof(Ss) +
		sizeof(Sc) +
		sizeof(Sb) +
		sizeof(Nf) +
		sizeof(Sf) +
		sizeof(Sv) +
		sizeof(Nc) +
		sizeof(StCluster) +
		sizeof(FAT_len) +
		sizeof(Name) + sizeof(short);
	for (int i = 0; i < 512 - core; i++) {
		SaveByte(fout, (uint8_t)0);
	}

	// FAT bool
	for (seeker i = 0; i < (seeker)this->Sf*512; i++)
	{
		SaveByte(fout, (uint8_t)0);
	}
	//for(uint64_t i=0;i<)

	// DATA
	// entry
	fout.close();
	return EXIT_SUCCESS;
}
