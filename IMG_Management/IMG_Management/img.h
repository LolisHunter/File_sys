#pragma once
#ifndef _IMG_H
#define _IMG_H
#endif // _IMG_H

#ifdef _IMG_H
#include <string>
#include <fstream>
#include <vector>
#include <direct.h>
#define BUFFER 512
typedef uint64_t seeker;
using namespace std;

struct Packg {
	uint32_t strt;
	uint32_t end;
};

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

// dung de volume o day ==! 

//bool Import(Volume &volume, string path, Entry *vitri) //luc dau vitri  = -1
//{
//	ifstream fin(path, ios_base::in | ios_base::binary);
//	if (fin.is_open()) // la file
//	{
//		Entry a;
//		int i = 0; // tim name trong path
//		int temp = path.find('\\', i);
//		while (temp > i)
//		{
//			i = path.find('\\', i + 1);
//			temp = path.find('\\', i + 1);
//		}
//		a.name = path.substr(i + 1, path.size() - i - 1);
//		a.Namesize = a.name.size();
//		a.flags = 0;
//		volume.addFile(a, path , vitri);
//	}
//	else // la thu muc hoac khong ton tai
//	{
//		system(("dir /b/a-d-h" + path + ">file.txt").c_str()); // doc cac file
//		ifstream file("file.txt");
//		system(("dir /b/ad-h" + path + ">folder.txt").c_str()); // doc cac file
//		ifstream folder("folder.txt");
//		if (!file.is_open() || !folder.is_open())
//			return false;
//
//		if (file.eof() && folder.eof())
//			return false; // khong ton tai hoac thu muc trong nen khong tao
//
//		Entry a;
//		int i = 0; // tim name trong path
//		int temp = path.find('\\', i);
//		while (temp > i)
//		{
//			i = path.find('\\', i + 1);
//			temp = path.find('\\', i + 1);
//		}
//		a.name = path.substr(i + 1, path.size() - i - 1);
//		a.Namesize = a.name.size();
//		Entry *link = volume.addFolder(a, path, vitri);
//
//		string dir;
//		while (!file.eof())
//		{
//			getline(file, dir);
//			if (!Import(volume, path + "\\" + dir, link))
//				return false;
//		}
//		file.close();
//
//		while (!folder.eof())
//		{
//			getline(folder, dir);
//			if (!Import(volume, path + "\\" + dir, link))
//				return false;
//		}
//		file.close();
//		folder.close();
//	}
//	return 1;
//}
//
//bool Export(Volume &volume,string path, Entry *vitri)
//{
//	string temp;
//	if (path.size() == 0)
//		temp = vitri->name;
//	else
//		temp = path + "\\" + vitri->name;
//	if (vitri->list.size() != 0)
//	{
//		if (_mkdir(temp.c_str()) != 0)
//		{
//			return false; // tao thu muc khong thanh cong
//		}
//		for (int i = 0; i < vitri->list.size(); i++)
//		{
//			Export(volume, temp, &vitri->list[i]);
//		}
//	}
//	else
//	{
//		volume.ExportFiLe(path, vitri);
//	}
//	return true;
//}
#endif // _IMG_H