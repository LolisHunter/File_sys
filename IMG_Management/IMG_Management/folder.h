#pragma once
#include <vector>
#include "entry.h"
#include "file.h"
using namespace std;

class Folder {
private:
	Entry entry;
	vector<File> file;
public:
	Folder() {};
	~Folder() {};
};