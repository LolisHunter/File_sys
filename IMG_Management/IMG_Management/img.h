#pragma once
#include <fstream>
#include <string>
using namespace std;

bool CreateImageFile(string& name, uint64_t size) {
	ofstream fout(name, ios_base::out | ios_base::binary);
	if (!fout.is_open()) {
		return 1;
	}
	// template
	string temp = "hello world";
	for (auto c : temp) {
		fout << c;
	}
	
	fout.close();
	return 0;
}