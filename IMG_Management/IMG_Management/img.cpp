#include "img.h"
#include "Debug.h"
#include <fstream>
using namespace std;
#define BUFFER 512
typedef uint64_t seeker;


template<class T>
static void SaveByte(ofstream& fout, T in) {
	char c;
	for (uint8_t i = 0; i < sizeof(T); i++) {
		c = in;
		fout << c;
		in = in >> 8;
	}
}
template<class T>
static void LoadByte(ifstream& fin, T& out) {
	out = 0;
	char c;
	for (uint8_t i = 0; i < sizeof(T); i++) {
		fin.read(c, 1);
		T temp = c;
		temp = temp << (8 * i);
		out += temp;
	}
}
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
	seeker a = in.tellg();
	in.close();
	return a;
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