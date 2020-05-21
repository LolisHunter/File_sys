#pragma once
#include "img.h" 
#include <vector>
#include "entry.h"
#include "img.h"
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
	uint32_t StCluster = 0; // start cluster /
	uint64_t FAT_len; // Sc * Nc / 512
	bool* FAT;
	uint32_t startSector;
	vector<Entry> entry;

	string disk;
public:
	void _list(string tab);
	void Load(string fileName);
	bool Create(Packg& scope, string fileName, bool Vname[26]);
	Volume() {};
	~Volume() {};
	void setFlags();
	void ExportFiLe(string path, const Entry* file);
	void AddEntry(Entry& entry);
	void LoadFolder(string disk, Entry* entry);
	uint64_t ViTriCluster(int i);
	uint8_t CheckType(string type, vector<Type>& type_list);
	uint32_t FreeInFAT();
	uint32_t FreeInFAT(int i);
	void addEntrySt(Entry* file, Entry* ViTriRDET);
	seeker AddTable(seeker seek, bool End, int& i);
	void AddData(ifstream& file, Entry* f);
	bool Import(string pathFile, Entry* vitri, vector<Type>& type_list); // luc dau de vitri = NULL
	bool Export(string path, Entry* vitri); // luc dau de path = ""
};