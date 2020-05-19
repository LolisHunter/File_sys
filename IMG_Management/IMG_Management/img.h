#pragma once
#ifndef _IMG_H
#define _IMG_H
#endif // _IMG_H

#ifdef _IMG_H
#include <string>
#include <fstream>
#include <vector>
#define BUFFER 512
typedef uint64_t seeker;
using namespace std;

struct Type {
	uint8_t code;
	char extension[16]; // 15 char + \0
};
static vector<Type> type_list;
#define TL type_list

template<class T>
static void SaveByte(ofstream& fout, T in);
template<class T>
static void LoadByte(ifstream& fin, T& out);
static bool CreateImageFile(string& name, uint64_t size);
static seeker getFileLength(char fileName[]);
static ifstream InSector(seeker& pos, char fileName[]);

//bool Import(string disk, Volume &volume, string path, int vitri)
//{
//	ifstream fin(path, ios_base::in | ios_base::binary);
//	if (fin.is_open()) // la file
//	{
//		struct stat st;
//		stat(path.c_str(), &st);
//		Entry a;
//		a.ctime = st.st_ctime;
//		a.mtime = st.st_mtime;
//		int i = 0; // tim name trong path
//		int temp = path.find('\\', i);
//		while (temp > i)
//		{
//			i = path.find('\\', i + 1);
//			temp = path.find('\\', i + 1);
//		}
//		a.name = path.substr(i + 1, path.size() - i - 1);
//		a.flags = 0;
//		fstream Disk(disk, ios_base::in | ios_base::out | ios_base::binary);
//		int vtDau, vtCuoi;
//		int next;
//		char data[4092];
//		//toi vi tri directory trong
//		string b;
//		while (!fin.eof())
//		{
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
//		system(("dir" + path + ">folder.txt").c_str());
//		ifstream file("folder.txt");
//		if (!file.is_open())
//			return false;
//		string dir;
//		for (int i = 0; i < 7; i++)
//			getline(file, dir);
//		if (file.eof())
//			return false; // khong ton tai
//
//		// tao entry table roi add vo (tai a neu a khac 0)
//		fstream Disk(disk, ios_base::in | ios_base::out | ios_base::binary);
//		int vtDau, vtCuoi;
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
#endif // _IMG_H