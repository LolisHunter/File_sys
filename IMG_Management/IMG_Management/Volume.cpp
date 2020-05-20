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
bool Volume::Create(Packg& scope,string fileName)
{
	uint64_t max_size = scope.end - scope.strt + 1;
	max_size *= 512;
	uint64_t byte;
	do
	{
		cout << "nhap kich thuoc: (MB)";
		cin >> byte;
		byte = byte * 1024 * 1024;
	} while (byte > max_size && cout << "Bruh!\n");
	cout << "Nhap volume name";
	this->Name = getchar();
	this->setFlags();
	this->Ss = UNIT_SIZE;
	this->Sc = 8;
	this->Sb = 1;
	this->Nf = 1;
	this->Nc = 1111111111111; // can tinh
	this->Sf = ceil((Sc * Nc) / 512) / Nf;
	this->Sv = byte;
	this->FAT_len = Nc;
	this->startSector = scope.strt;

	ofstream fout(fileName, ios::in | ios::out | ios::binary);
	if (!fout.is_open()) {
		return EXIT_FAILURE;
	}
	seeker pos = startSector; pos = pos * 512;
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

	int core = sizeof(Ss) +
		sizeof(Sc) +
		sizeof(Sb) +
		sizeof(Nf) +
		sizeof(Sf) +
		sizeof(Sv) +
		sizeof(Nc) +
		sizeof(StCluster) +
		sizeof(FAT_len);

	fout.close();
	return EXIT_SUCCESS;
}

void Volume::setFlags()
{
}

void Volume::addFile(Entry file,string path, Entry *&ViTriRDET)
{
	// Neu ViTriRDET = -1 thi them tai bang RDET chinh
	// them file vao bang RDET do
	// save xuong file img
}

Entry * Volume::addFolder(Entry folder, string path, Entry *&ViTriRDET)
{
	// Neu ViTriRDET = -1 thi them tai bang RDET chinh
	// tao bang RDET roi return con tro den RDET do
	// save xuong file img
	return &Entry();
}

void Volume::ExportFiLe(string path, Entry *& file)
{
	// tao ra file do
	// toi cho data cua file do
	// luu ra
	// dong file
}

void Volume::AddEntry(const Entry& entry, const string& disk)
{
	fstream fout(disk, ios_base:: in | ios_base::out | ios_base:: binary);
	if(fout.is_open() == false)
	{
		throw exception("Can't open disk. Can't add entry!");
	}

	seeker sker = entry.entryStCluster * this->Sc * this->Ss;
	fout.seekp(sker, ios::beg);

	uint8_t flag;
	do
	{
		LoadByte(fout, flag);
		//Check if(flag = END)
		if(flag ^ END < flag)
		{
			//sker = SomeFuntion(sker);
			break; 
		}

		if(flag == 0 || (flag^DELETED < flag))
		{
			break;
		}
		else
		{
			sker += 32;
		}
	} while(true);
	
	fout.seekp(sker);
	fout.write((char*)&entry.flags, 1);
	fout.write((char*)&entry.ctime, 2);
	fout.write((char*)&entry.mtime, 2);
	fout.write((char*)&entry.StCluster, 4);
	fout.write((char*)&entry.size, 4);
	fout.write((char*)&entry.TypeNum, 2);
	fout.write((char*)&entry.ino, 2);
	fout.write((char*)&entry.entryStCluster, 4);

	fout.close();
}

