//#include "Volume.h"
//
//
//string Volume::getName16Char(const string& source)
//{
//	string name = source;
//	if (name.length() >= 16)
//	{
//		name = name.substr(0, 16);
//	}
//	else
//	{
//		for (int i = name.length(); i < 16; i++)
//		{
//			name += " ";
//		}
//	}
//
//	return name;
//}
//
//Volume::Volume(const string& name, const int& size, const int& startCluster, const int& number)
//{
//	this->name = this->getName16Char(name);
//	this->size = size;
//	this->remaining = size;
//	this->startCluster = startCluster;
//	this->number = number;
//}
//
//void Volume::InitVolume(const string& disk)
//{
//	fstream fout(disk, ios_base::binary | ios_base::out | ios_base::in);
//	if (fout.is_open() == false)
//	{
//		throw exception("Can't open disk. Can't init volume");
//	}
//
//	//Go to position at cluster 1
//	fout.seekp(8 * 512 + 4 * number, ios_base::beg);
//	fout.write((char*)&startCluster, 4);
//
//	//Go to this start cluster
//	fout.seekp(8 * 512 * startCluster, ios_base::beg);
//	fout << name;
//	fout.write((char*)&startCluster, 4);
//	fout.write((char*)&size, 4);
//	fout.write((char*)&remaining, 4);
//
//	fout.close();
//}
