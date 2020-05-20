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
	uint64_t a = 2097152;

	a= a - 1;
	uint64_t f = ceil((double)a / (512 * 8 - 1));
	f = (a - f) / 8;
	cout << f;
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
		//root.RootCreate((char*)"ab.iso");
		/*root.RootLoad((char*)"ab.iso");
		root.CreateVolume();
		Volume *a = root.GetVolume();
		a->FAT = new bool[a->FAT_len];
		for (int i = 0; i < a->FAT_len; i++)
			a->FAT[i] = 0;
		a->Import("E:\\ham", NULL);*/
		//root.CreateVolume();
		//root.status();
	}
	catch (exception& e)
	{
		cout << e.what();
	}
}
#endif // MAIN