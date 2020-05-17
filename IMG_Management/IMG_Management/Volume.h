#pragma once
#include <string>
#include <fstream>
using namespace std;

//+Sector 0
//offset 00 - 15: Tên
//offset 16 - 19 : Cluster bắt đầu
//offset 20 - 23 : Kích thước tổng
//offset 24 - 27 : Kích thước còn lại
//+ Sector 1 : RDET và DATA

class Volume
{
private:
	string name;
	int startCluster;
	int size; //(cluster)
	int remaining; //(cluster)
	int number;

	string getName16Char(const string& source);
public:
	Volume(const string& name, const int& size, const int& startCluster, const int& number = 0);
	void InitVolume(const string& diskPath);
};
