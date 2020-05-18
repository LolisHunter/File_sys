#pragma once
#include <fstream>
#include <string>
#include "Debug.h"
#define BUFFER 512
typedef uint64_t seeker;
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

bool CreateImageFile(string& name, uint64_t size) {
	ofstream fout(name, ios_base::out | ios_base::binary);
	if (!fout.is_open()) {
		return 1;
	}
	// template
	string temp = "hello world";
	uint8_t buffer[BUFFER];
	for (uint8_t c : temp) {
		fout << c;
	}

	fout.close();
	return 0;
}

seeker getFileLength(char fileName[]) {
	std::ifstream in(fileName, std::ifstream::ate | std::ifstream::binary);
	seeker a = in.tellg();
	in.close();
	return a;
}

ifstream InSector(seeker& pos, char fileName[]) {
	ifstream fin(fileName);
	bool iResult = false;
	if (pos * 512 < getFileLength(fileName)) {
		fin.seekg(pos * 512);
		DEBUG_PRINT("Seek fail");
	}
	return fin;
}

int FindFree(fstream &Disk, int STTvolume, int &vtDau, int &vtCuoi)
{
	
}

//bool Import(string disk, int STTvolume, string path, int vitri)
//{
//	ifstream fin(path, ios_base::in | ios_base::binary);
//	if (fin.is_open()) // la file
//	{
//		int FreeVolumeSize; //free volume size
//		struct stat st;
//		stat(path.c_str(), &st);
//		if (st.st_size > FreeVolumeSize)
//			return 0;
//		fstream Disk(disk, ios_base::in | ios_base::out | ios_base::binary);
//		int vtDau, vtCuoi;
//		int next;
//		char data[4092];
//		string b;
//		while (!fin.eof())
//		{
//			FindFree(Disk, STTvolume, vtDau, vtCuoi); // tim toi vi tri tron
//			int flag = 1;
//			while (vtDau != vtCuoi && flag)
//			{
//				Disk.seekp(vtDau, Disk.beg());
//				fin.getline(data, 4092);
//				b = data;
//				if (!fin.eof())
//					Disk.write((char*)&next, sizeof(int));
//				else
//				{
//					next = -1;
//					Disk.write((char*)&next, sizeof(int));
//					flag = 0;
//				}
//				Disk << b;
//			}
//		}
//	}
//	else // la thu muc hoac khong ton tai
//	{
//		system(("dir" + path + ">file.txt").c_str());
//		ifstream file("file.txt");
//		if (!file.is_open())
//			return false;
//		string dir;
//		for (int i = 0; i < 7; i++)
//			getline(file, dir);
//		if (file.eof())
//			return false; // khong ton tai
//		vector<string> folder;
//		int number = 0;
//		// tao entry table roi add vo (tai a neu a khac 0)
//		fstream Disk(disk, ios_base::in | ios_base::out | ios_base::binary);
//		int vtDau, vtCuoi;
//		while (getline(file, dir) && dir[0] != ' ')
//		{
//			string ngay = dir.substr(0, 10);
//			string gio = dir.substr(12, 20);
//			string name = dir.substr(38, dir.size());
//			int a = FindFree(Disk, STTvolume, vtDau, vtCuoi);
//			// add vo entry table
//			number++;
//			// neu number == max entry table thi tao entry table phu
//			Import(disk, STTvolume, path + "/" + name, a);
//		}
//	}
//	return 1;
//}

//bool Export(string Disk, int STTvolume, string FileName)
//{
//	// time volume do
//	// tim file name trong volume
//	ifstream fin(Disk, ios_base::in | ios_base::binary);
//	if (!fin.is_open())
//		return false;
//	// di toi entry tim file name
//	int vt;
//	int size; // neu la file
//	fin.seekg(vt, fin.beg());
//	// neu la thu muc
//
//	// neu la file
//	int next;
//	char data[4092];
//	ofstream fout(FileName, ios_base::out | ios_base::binary);
//	do
//	{
//		fin.read((char*)&next, sizeof(int));
//		fin.getline(data, 4092, 'F'); // chuoi ket thuc
//		string b = data;
//		fout.write((char*))
//			fout << b.c_str();
//		fin.seekg(next, fin.beg()); // di toi cluster tiep theo
//	} while (next != 0);
//	fout.close();
//}