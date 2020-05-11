#pragma once
#include <fstream>
#include <string>
#define BUFFER 512
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