//#pragma once
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
#include <iostream>
#include <fstream>
#include <vector>
#include "folder.h"
#include "file.h"
#include "img.h"
using namespace std;

class Volume
{
protected:
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
	bool * FAT;
	vector<File> file;
	vector<Folder> folder;
public:
	void Create(vector<uint64_t> &abc,string fileName);
	Volume() {};
	~Volume() {};
};