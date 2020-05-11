#include <iostream>
#include <string>
#include "img.h"
using namespace std;

#define MAIN
#define DEBUG

void pLog(string& plain_text);

#ifdef MAIN
int main(int agrc, char* agrv[]) {
	string a = "a.img";
	bool temp = CreateImageFile(a, 20);
	return temp;
}


#endif // main

#ifdef DEBUG
void pLog(string& plain_text) {
	cout << "[Log] : " << plain_text << endl;
}
#endif // debug

#ifndef DEBUG
void pLog(string& plain_text) {}
#endif // !debug
