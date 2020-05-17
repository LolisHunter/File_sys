#include <iostream>
#include <string>
//#include "DiskImage.h"
//#include "Volume.h"
#include "Debug.h"
#include "root.h"
#include "img.h"
using namespace std;
//#define TEST

#ifdef TEST
void main() {

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
	}
	catch (exception& e)
	{
		cout << e.what();
	}
}
#endif // MAIN