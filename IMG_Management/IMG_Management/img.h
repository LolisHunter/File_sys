#pragma once
#include <fstream>
#include <string>
#include "Debug.h"
#define BUFFER 512
typedef uint64_t seeker;
using namespace std;

bool CreateImageFile(string& name, uint64_t size) {
	ofstream fout(name, ios_base::out | ios_base::binary);
	if (!fout.is_open()) {
		return 1;
	}
	// template
	string temp = "hello world";
	uint8_t buffer[BUFFER];
	for (uint8_t c : temp) {
		fout << c;
	}

	fout.close();
	return 0;
}

seeker getFileLength(char fileName[]) {
	std::ifstream in(fileName, std::ifstream::ate | std::ifstream::binary);
	return in.tellg();
}

ifstream InSector(seeker& pos, char fileName[]) {
	ifstream fin(fileName);
	bool iResult = false;
	if (pos * 512 < getFileLength(fileName)) {
		fin.seekg(pos * 512);
		DEBUG_PRINT("Seek fail");
	}
	return fin;
}