#pragma once

#include <vector>
#include "entry.h"
using namespace std;

class Volume
{
	/*
			recycle			10000000
			using			01000000
			hidden			00100000
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
	uint32_t startSector;
	vector<Entry> entry;
public:
	bool Create(Packg& scope, string fileName);
	Volume() {};
	~Volume() {};
	void setFlags();
	void addFile(Entry file, string path, Entry *&ViTriRDET); // ViTriRDET la vi tri bang RDET chua file do
	Entry * addFolder(Entry folder, string path, Entry *&ViTriRDET); // ViTriRDET nhu tren, ham tra ve bang RDET moi tao
	void ExportFiLe(string path, Entry *&file);
};