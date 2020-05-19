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
int BinarySearch(vector<uint32_t> arr, int n, int key) {
	int l = 0;
	int r = n - 1;
	int mid = (r - l) / 2 + l;

	while (l <= r) {
		if (key == arr[mid]) {
			return mid;
		}
		if (key < arr[mid]) {
			r = mid - 1;
			mid = (r - l) / 2 + l;
		}
		if (key > arr[mid]) {
			l = mid + 1;
			mid = (r - l) / 2 + l;
		}
	}
	return l;
}

void scopeAdd(uint32_t key, vector<uint32_t>& scope)
{
	if (scope.size() == 0) {
		scope.push_back(key);
	}
	else {
		int loc = BinarySearch(scope, scope.size(), key);
		if (loc > scope.size() - 1) {
			scope.push_back(key);
		}
		else {
			auto i = scope.begin();
			i += loc;
			scope.insert(i, key);
		}
	}
}
void main() {
	vector<uint32_t>scope;
	scopeAdd(3, scope);
	scopeAdd(3, scope);
	scopeAdd(3, scope);
	scopeAdd(9, scope);
	scopeAdd(7, scope);
	scopeAdd(1, scope);
	scopeAdd(0, scope);
	scopeAdd(2, scope);

	for (auto j : scope) {
		cout << j << " ";
	}
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
		//root.RootCreate((char*)"a.iso");
		root.RootLoad((char*)"a.iso");
		root.CreateVolume();
		//root.status();
	}
	catch (exception& e)
	{
		cout << e.what();
	}
}
#endif // MAIN