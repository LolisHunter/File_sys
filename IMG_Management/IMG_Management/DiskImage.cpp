#include "DiskImage.h"
#include <sstream>
#include "Volume.h"

//Master Cluster 0:
//offset 0 - 7 : Dấu hiệu nhận biết tự định nghĩa
//offset 8 - 11 : Kích thước đĩa(sector)
//offset 12 : Số lượng volume
//Để dành xài từ từ
//offset 511 - 512 : 0xAA55


DiskImage::DiskImage(const string& name, const int& size)
{
	this->name = name;
	this->size = size;
}

void DiskImage::InitDisk(const string& dir)
{
	string path;
	if (dir.empty())
	{
		path = name;
	}
	else
	{
		path += "\\" + name;
	}

	//CLUSTER 0
	ofstream fout(path, ios_base::out | ios_base::binary);

	if (fout.is_open() == false)
	{
		throw exception("Can't create file. Can't create disk!");
	}

	fout << "Oniisama";

	//-----Boot Sector begin-----
	fout.write((char*)&size, 4);

	int temp = 1;
	fout.write((char*)&temp, 1);

	for (int i = 12; i < 509; i++)
	{
		fout << 0;
	}

	int endSign = 0x55AA;
	fout.write((char*)&endSign, 2);
	//------Boot Sector end here-------

	//Fill the rest of 0th cluster
	for (int i = 512; i < 8 * 512; i++)
	{
		fout << 0;
	}

	//CLUSTER 1
	//Fill the rest of disk
	for (int i = 0; i < (size - 8) * 512; i++)
	{
		fout << 0;
	}

	fout.close();
}

bool CreateImageFile(string& name, uint64_t size) {
	ofstream fout(name, ios_base::out | ios_base::binary);
	if (!fout.is_open()) {
		return 1;
	}
	// template
	string temp = "hello world";
	for (auto c : temp) {
		fout << c;
	}

	fout.close();
	return 0;
}