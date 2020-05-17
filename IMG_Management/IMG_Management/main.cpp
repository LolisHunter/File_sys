#include <iostream>
#include <string>
#include "DiskImage.h"
#include "Volume.h"

using namespace std;

#define MAIN
#define DEBUG

void pLog(string& plain_text);

#ifdef MAIN
int main(int agrc, char* agrv[]) 
{
	try
	{
		DiskImage dsk("MyDisk.img", 64);
		dsk.InitDisk("");

		Volume vol("Vol1", 4, 2, 0);
		vol.InitVolume("MyDisk.img");
	}
	catch (exception& e)
	{
		cout << e.what();
	}
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
