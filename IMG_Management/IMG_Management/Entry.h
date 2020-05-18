#pragma once
#include <bitset>
#include <time.h>
#include <vector>
#include <string>
using namespace std;

/*--------------------------------------------------------
Flags:
	delete		00000001	1
	hidden		00000010	2
	password	00000100	4
	Read-only	00001000	8
	sub-entry	00010000	16
	root		00100000	32
				01000000	64
				10000000	128
--------------------------------------------------------*/
#define NameS 20 // name size-1
struct Entry {
//  Define						Info						Byte
	uint8_t flags;			// flag ^						1
	uint16_t ctime;			// created time					2
	uint16_t mtime;			// last modifed time			2
	uint32_t StCluster;		// start cluster				4
	uint32_t size;			// size							4
	uint16_t TypeNum;		// type of data					2
	uint16_t ino;			// identify number				2
	
	// save at the first sector of the first cluster
	uint8_t eno;			// entry number
	uint8_t Namesize;		// Namesize						1
	string name;			// name							= NameSize
	// skip first sector 
	vector<Entry> list;
	Entry() {
		this->flags = 0;
		this->ctime = 0;
		this->mtime = 0;
		this->StCluster = 0;
		this->size = 0;
		this->TypeNum = 0;
		this->ino = 0;
		this->Namesize = 0;
		this->name = "";
	}
	tm cTime(); // out created time
	tm mTime(); // out last modified time ... 
};