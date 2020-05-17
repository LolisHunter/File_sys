#include <iostream>
#include <string>
#include "DiskImage.h"
#include "Volume.h"

using namespace std;
#define DEBUG
#define MAIN

#if defined (DEBUG)
static void DebugPrint(string pszMsg);
#define DEBUG_PRINT(pszMsg) DebugPrint(pszMsg)
#else
#define DEBUG_PRINT(pszMsg) NULL
#endif

#ifdef TEST
void main() {

}
#endif // TEST


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
#endif
