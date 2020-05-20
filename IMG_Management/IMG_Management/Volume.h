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
	bool Create(Packg& scope, string fileName, bool Vname[26]);
	Volume() {};
	~Volume() {};
	void setFlags();
	void addFile(Entry file, string path, Entry *&ViTriRDET); // ViTriRDET la vi tri bang RDET chua file do
	Entry * addFolder(Entry folder, string path, Entry *&ViTriRDET); // ViTriRDET nhu tren, ham tra ve bang RDET moi tao
	void ExportFiLe(string path, Entry *&file);
	void AddEntry(const Entry& entry, const string& disk);
	uint64_t ViTriCluster(int i);
	uint32_t FreeInFAT();
	uint32_t FreeInFAT(int i);
	void addEntrySt(Entry &file, Entry *ViTriRDET); 
	seeker AddTable(seeker seek, bool End);
	void AddEntry(Entry& entry);
	void AddData(fstream &file, Entry *&f);
	void ExportFiLe(string path,const Entry *file);
	bool Import(string pathFile, Entry *vitri); // luc dau de vitri = NULL
	bool Export(string path, Entry *vitri); // luc dau de path = ""
};