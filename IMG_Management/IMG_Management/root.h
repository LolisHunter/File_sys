#pragma once

#include "Volume.h"
#include <vector>
#include "img.h"
#define BUFFER 512

using namespace std;

/*
	flags			bool			1
	root letter		/				1
	byte/sector		Ss				2
	sector/boot		Sb				2
	sector/volume	Sv				4
	sector/vol_etr	Se				2
	pwd_size		pwd_sz			2
	password		pwd				sizeof(pwd)

*/

class Root
{
private:
	uint8_t flags = 0; // flags ^
	uint8_t Name = 0; // volume letter
	uint16_t Ss = 0; // byte/sector
	uint16_t Sb = 0; // sector/boot
	uint32_t Sv = 0; // sector/volume
	uint16_t Se = 0; // sector/volume directory entry
	uint16_t pwd_sz = 0; //password size
	string pwd =""; // Vigenere custom
	string fileName;
	vector<Volume> list;
	vector<uint32_t> abc; // start end, start end // don vi la sector
public:
	Root();

	void RootCreate(char fileName[]);
	void RootLoad(char fileName[]);
};