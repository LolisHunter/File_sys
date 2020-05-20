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
		root.RootCreate((char*)"ab.iso");
		root.RootLoad((char*)"ab.iso");
		root.CreateVolume();

		//Volume a;
		//a.Ss = 512;
		//a.Sb = 1;
		//a.Sv = 100;
		//a.Sc = 8;
		//a.Nc = (a.Sv - a.Sb - a.Sf * a.Nf) / a.Sc; 
		//a.Sf = ceil((a.Sc * a.Nc) / 512) / a.Nf;
		//root.CreateVolume();
		//root.status();
	}
	catch (exception& e)
	{
		cout << e.what();
	}
}
#endif // MAIN