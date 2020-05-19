#include <iostream>
#include <string>
//#include "DiskImage.h"
//#include "Volume.h"
#include "Debug.h"
#include "root.h"
#include "img.h"
using namespace std;
#include "mask.h"
//#define TEST

#ifdef TEST
void main() {
	ifstream fout("test.txt");
	char extension[15] = "abcdhajskwoakq";
	fout.read(extension, 15);
	cout << extension;
	char ab[15] = "efgh";
	fout.read(ab, 15);
	cout << ab;
	fout.close();
}
#endif // TEST


#ifndef TEST
int main(int agrc, char* agrv[]) 
{
	try
	{
		//DiskImage dsk("MyDisk.img", 64);
		//dsk.InitDisk("");

		//Volume vol("Vol1", 4, 2, 0);
		//vol.InitVolume("MyDisk.img");
		Root root;
		root.RootCreate((char*)"a.iso");
		root.RootLoad((char*)"a.iso");
		root.status();
	}
	catch (exception& e)
	{
		cout << e.what();
	}
}
#endif // MAIN