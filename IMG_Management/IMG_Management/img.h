#pragma once
#ifndef _IMG_H
#define _IMG_H
#endif // _IMG_H

#ifdef _IMG_H
#include <string>
#include <fstream>
#include <vector>
#include <direct.h>
#define BUFFER 512
typedef uint64_t seeker;
using namespace std;

struct Packg {
	uint32_t strt;
	uint32_t end;
};

struct Type {
	uint8_t code;
	char extension[16]; // 15 char + \0
};
static vector<Type> type_list;
static bool Vname[26] = {0};
#define TL type_list

template<class T>
static void SaveByte(ofstream& fout, T in);
template<class T>
static void LoadByte(ifstream& fin, T& out);
static bool CreateImageFile(string& name, uint64_t size);
static seeker getFileLength(char fileName[]);
static ifstream InSector(seeker& pos, char fileName[]);

#endif // _IMG_H
