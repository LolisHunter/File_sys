﻿//#pragma once
//#include <string>
//#include <fstream>
//using namespace std;
//
////+Sector 0
////offset 00 - 15: Tên
////offset 16 - 19 : Cluster bắt đầu
////offset 20 - 23 : Kích thước tổng
////offset 24 - 27 : Kích thước còn lại
////+ Sector 1 : RDET và DATA
//
//class Volume
//{
//private:
//	string name;
//	int startCluster;
//	int size; //(cluster)
//	int remaining; //(cluster)
//	int number;
//
//	string getName16Char(const string& source);
//public:
//	Volume(const string& name, const int& size, const int& startCluster, const int& number = 0);
//	void InitVolume(const string& diskPath);
//};
#pragma once

#include <vector>
#include "entry.h"
using namespace std;

class Volume
{
	/*
			recycle			00000001
			hidden			00000010

	*/
public:
	uint8_t flags = 0;
	uint8_t Name = 0; // volume letter // ki tu hay chuoi?
	uint16_t Ss = 0; // byte/sector
	uint8_t Sc = 0; // sector/cluster
	uint16_t Sb = 0; // sector/boot
	uint8_t Nf = 0; // number of FATs // co the xoa
	uint16_t Sf = 0; // sector/FAT  Sf * Nf =  ceil (Sc * Nc / 512)
	uint32_t Sv = 0; // sector/volume
	uint32_t Nc = 0; // cluster/volume
	uint32_t StCluster = 0; // start cluster / first entry / entry = 2 cluster
	uint64_t FAT_len; // Sc * Nc / 512
	bool* FAT;
	uint64_t startSector;
	vector<Entry> entry;
public:
	bool Create(vector<uint32_t>& abc, string fileName);
	Volume() {};
	~Volume() {};
	void setFlags();
	void addFile(Entry file, string path, Entry *&ViTriRDET); // ViTriRDET la vi tri bang RDET chua file do
	Entry * addFolder(Entry folder, string path, Entry *&ViTriRDET); // ViTriRDET nhu tren, ham tra ve bang RDET moi tao
	void ExportFiLe(string path, Entry *&file);
};