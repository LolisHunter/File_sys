#pragma once

#include "Volume.h"
#include <vector>
#define BUFFER 512

using namespace std;

struct Type {
	uint8_t code;
	char extension[17];
};

class Root : public Volume
{
private:
	string fileName;
	vector<Volume> list;
	vector<Type> type;
	vector<uint32_t> abc; // start end, start end // don vi la sector
public:
	Root();

	void RootCreate(char fileName[]);
	void RootLoad(char fileName[]);
	void AddVolume(const Volume& v);
};