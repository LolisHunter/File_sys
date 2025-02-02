#include "root.h"
#include "img.h"
#include <iostream>
#include <fstream>
#include "Debug.h"
#include "mask.h"
#include "bitset"

using namespace std;
template<class T>
void SaveByte(ofstream& fout, T in) {
	uint8_t c;
	for (uint8_t i = 0; i < sizeof(T); i++) {
		c = in;
		fout << c;
		in = in >> 8;
	}
}

template<class T>
void LoadByte(ifstream& fin, T& out) {
	out = 0;
	char c;
	for (uint8_t i = 0; i < sizeof(T); i++) {
		fin.get(c);
		T temp = (uint8_t)c;
		temp = temp << (8 * i);
		out = out | temp;
	}
}

Root::Root() {
}

void Root::_list()
{
	cout << this->Name << endl;
	for (auto i : list) {
		i._list("---");
	}
}

void Root::ls()
{
	for (auto i : list) {
		if ((i.flags ^ HIDDEN) > i.flags) {
			cout << '\\' << i.Name << "  ";
		}
	}
	cout << endl;
}

void Root::RootCreate(string fileName) {
	// Checking
	this->fileName = fileName;
	ifstream fin(fileName);
	if (fin.is_open())
	{
		cout << "This file exist, you want overwrite" << endl;
		cout << "Input (y/n key):";
		char choose;
		choose = getchar();
		fin.close();
		if (choose != 'y')
			return;
	}

	ofstream fout(fileName,ios::binary);
	if (!fout.is_open()) { DEBUG_PRINT("Create fail"); }
	cout << "nhap kich thuoc: (GB)";
	float size = 0;
	cin >> size;
	uint64_t byte = size; byte = byte * 1024 * 1024 * 1024;
	this->Sb = 8;
	this->Ss = UNIT_SIZE;
	this->Se = 5;
	this->Sv = byte / 512;
	this->Name = '/';

	this->scopeUpdate();

	// Create iso
	uint64_t data_size = this->Sv;
	data_size *= UNIT_SIZE;
	char* buffer;
	buffer = (char*)calloc(data_size, sizeof(char));
	fout.write(buffer, data_size);
	free(buffer);
	fout.close();
	DEBUG_PRINT("[CREATE ISO] : done\n");

	// ---------------------------------------------------------
	//                  SAVE INFOMATION

	seeker point;

	fout.open(fileName, ios::in | ios::out | ios::binary);
// Boot
	SaveByte(fout, this->flags);
	SaveByte(fout, this->Name);
	SaveByte(fout, this->Ss);
	SaveByte(fout, this->Sb);
	SaveByte(fout, this->Sv);
	SaveByte(fout, this->Se);
	SaveByte(fout, this->pwd_sz);
	for (auto i : this->pwd) {
		SaveByte(fout, i);
	}

	// Type code here
	point = this->Sb - 5;
	point *= UNIT_SIZE;
	for (int i = 0; i < type_list.size();i++) {
		fout.seekp(point);
		SaveByte(fout,type_list[i].code);
		fout.write(type_list[i].extension, 15);
		point = point + 16;
	}

	DEBUG_PRINT("[CREATE BOOT] : done\n");

// volume entry
	point = this->Sb;
	point *= UNIT_SIZE;
	fout.seekp(point);
	for (int i = 0; i < (this->Se * UNIT_SIZE) / vENTRY_SIZE; i++) {
		fout << DEFAULT;
		point += 10;
		fout.seekp(point);
	}
	DEBUG_PRINT("[CREATE ENTRY] : done\n");

// DATA ... ...
	fout.close();
}

void Root::RootLoad(string fileName)
{
	ifstream fin(fileName, ios::in);
	if (!fin.is_open()) {
		DEBUG_PRINT("[OPEN FAIL] \n");
		throw exception("Load disk fail");
	}

	this->fileName = fileName;
	seeker point;
	fin.seekg(ios::beg);
	// boot load
	LoadByte(fin, this->flags);
	LoadByte(fin, this->Name);
	LoadByte(fin, this->Ss);
	LoadByte(fin, this->Sb);
	LoadByte(fin, this->Sv);
	LoadByte(fin, this->Se);
	LoadByte(fin, this->pwd_sz);
	for (int i = 0; i < this->pwd_sz; i++) {
		char c;
		fin >> c;
		this->pwd.push_back(c);
	}

	this->scopeUpdate();

	// type load
	Type Ttemp;
	point = this->Sb - 5;
	point *= UNIT_SIZE;
	fin.seekg(point);
	// 160 = 5 * UNIT_SIZE / 16
	for (int i = 0; i < 160; i++) {
		LoadByte(fin, Ttemp.code);
		if (Ttemp.code == 0) {
			break;
		}
		fin.read(Ttemp.extension, 15);
		type_list.push_back(Ttemp);
	}

	// load volume dir entry
	Volume Vtemp;
	Vtemp.disk = fileName;
	point = this->Sb;
	point *= UNIT_SIZE;
	// 256 = Se * UNIT_SIZE / vENTRY_SIZE
	for (int i = 0; i < 256; i++) {
		fin.seekg(point);
		LoadByte(fin, Vtemp.flags);
		if (i == 0 && Vtemp.flags == DEFAULT) {
			DEBUG_PRINT("[NO VOLUME FOUND]");
			break;
		}
		else if (Vtemp.flags == DEFAULT) {
			break;
		}
		else if ((Vtemp.flags ^ RECYCLE) > Vtemp.flags) {
			LoadByte(fin, Vtemp.Name);
			Vname[Vtemp.Name - 'A'] = 1;
			LoadByte(fin, Vtemp.startSector);
			uint32_t end;
			LoadByte(fin, end);
			Vtemp.Sv = end - Vtemp.startSector + 1;
			scopeAdd(Vtemp.startSector);
			scopeAdd(end);
			Vtemp.Load(fileName);
			list.push_back(Vtemp);
			list[list.size() - 1].entry = Vtemp.entry;
		}
		point += 10;
	}
}

void Root::AddVolumeEntry(Volume& v)
{
	seeker startIndex = (uint64_t)this->Ss * this->Sb;
	ofstream fout;
	ifstream fin;
	uint8_t key;
	uint64_t max_size = this->Ss; 
	max_size *= ((uint64_t)this->Sb + (uint64_t)this->Se);

	// start finding recycle entry
	do
	{
		fin.open(this->fileName);
		if (fin.is_open() == false) {
			throw exception("[Disk open fail]");
		}
		fin.seekg(startIndex);
		LoadByte(fin, key);
		fin.close();
	} while (startIndex < max_size
		&& ((key ^ RECYCLE) > key) && (startIndex += vENTRY_SIZE));
	//end ~> startIndex

	fout.open(this->fileName, ios_base::in | ios_base::out | ios_base::binary);
	fout.seekp(startIndex);
	
	v.setFlags();
	SaveByte(fout, v.flags);
	SaveByte(fout, v.Name);
	SaveByte(fout, v.startSector);
	uint32_t endSec = v.startSector + v.Sv -1;
	SaveByte(fout, endSec);

	fout.close();
}

void Root::scopeUpdate()
{
	scope.clear();
	scopeAdd(0);
	scopeAdd(this->Sv - 1);
}

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

void Root::scopeAdd(uint32_t key)
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

void Root::status()
{
	for (int i = 0; i < sizeof(flags) * 8; i++) {
		cout << bitset<sizeof(flags)*8>(flags)[sizeof(Name) * 8 -1-i];
	}
	cout <<  (uint8_t)flags << endl;
	cout  << (char)Name;
	for (int i = 0; i < sizeof(Name) * 8; i++) {
		cout << bitset<sizeof(Name) * 8>(Name)[sizeof(Name) * 8 -1 - i];
	}
	cout << endl << (int)Ss << endl;
	cout << (int)Sb << endl;
	cout << (int)Sv << endl;
	cout << (int)Se << endl;
	cout << (int)pwd_sz << endl;
	cout << (string)pwd << endl;
	cout << fileName << endl;
}


void Root::findUnlocated(vector<Packg>& list_unlocated) {
	uint32_t pointer = Sb + Se - 1;
	for (int i = 1; i < scope.size(); i = i + 2) {
		if (scope[i] - pointer > 1) {
			Packg temp;
			temp.strt = pointer + 1;
			temp.end = scope[i] - 1;
			list_unlocated.push_back(temp);
		}
		if (i + 1 > scope.size() - 1) {
			break;
		}
		pointer = scope[i + 1];
	}
}
void Root::CreateVolume()
{
	if (!checkPWD()) {
		return;
	}
	vector<Packg> list_unlocated;
	cout << this->Name << " " << (this->Sv /2 )/1024 << "MB\n";
	findUnlocated(list_unlocated);
	if (!list_unlocated.size()) {
		cout << "All memory had been alocated";
		DEBUG_PRINT("CREATE VOLUME FAIL");
	}
	else {
		for (int i = 0; i < list_unlocated.size(); i++) {
			cout << "[" << i + 1 << "] " << "EMPTY MEMORY : "
				<< (list_unlocated[i].end - list_unlocated[i].strt) / 2 / 1024 << "MB\n";
		}
	}
	int opt;
	do
	{
		cout << "Choose : ";
		cin >> opt;
	} while ((opt > list_unlocated.size() || opt < 1) && cout << "Bruh!\n");
	Volume temp;
	temp.disk = fileName;
	bool fail = temp.Create(list_unlocated[opt - 1], this->fileName, Vname);
	if (!fail)
	{
		list.push_back(temp);
		this->AddVolumeEntry(temp);
	}
}

void Root::DeleteVolume(char Name){
	if (!checkPWD()) {
		return;
	}
	for (int i = 0; i < list.size();i++) {
		if (list[i].Name == Name) {
			ifstream fin(this->fileName);
			if (!fin.is_open()) {
				DEBUG_PRINT("[CAN NOT OPEN DISK]");
			}
			seeker point = this->Sb;
			point *= UNIT_SIZE;
			point += 1;
			do
			{
				fin.seekg(point);
				char c;
				LoadByte(fin, c);
				if (c == Name) {
					point = point - 1;
					break;
				}
				point = point + 10;
			} while (point < (seeker)(13)*UNIT_SIZE);

			ofstream fout(fileName, ios::in | ios::out | ios::binary);
			fout.seekp(point);
			list[i].flags = list[i].flags ^ RECYCLE;
			SaveByte(fout, list[i].flags);
			list.erase(list.begin() + i);
			fout.close();
			return;
		}
	}
	cout << "VOLUME CAN NOT FOUND\n";
}

bool Root::is_open()
{
	if (!this->fileName.length()) {
		return 0;
	}
	return 1;
}

Volume* Root::getVolume(char Name)
{
	for (int i = 0; i < list.size();i++) {
		if (list[i].Name == Name) {
			return &list[i];
		}
	}
	return NULL;
}

void Root::createPassword()
{
	if (!checkPWD()) {
		return;
	}
start:;
	string pwd;
	cout << "Enter password : ";
	cin >> pwd;
	string temp;
	cout << "Confirm password : ";
	cin >> temp;
	if (sha1(pwd) == sha1(temp)) {
		this->flags = flags | PASSWORD;
		this->pwd = sha1(pwd);
		this->pwd_sz = this->pwd.size();
	}
	else {
		char c;
		cout << "Confirm password does not match!\n";
		cout << "Do you want to continue? <y/n>\n";
		c = getchar();
		if (c == 'y') {
			goto start;
		}
	}

	ofstream fout(this->fileName, ios::in | ios::out | ios::binary);
	fout.seekp(12);
	SaveByte(fout, this->pwd_sz);
	for (auto i : this->pwd) {
		SaveByte(fout, i);
	}
	fout.close();
}

bool Root::Auth()
{
	string temp;
	cout << "Enter password : ";
	cin >> temp;
	temp = sha1(temp);
	if (temp == this->pwd) {
		return true;
	}
	return false;
}

bool Root::checkPWD()
{
	if ((flags ^ PASSWORD) < flags) {
		char c = 0;
		do
		{
			if (!Auth()) {
				cout << "Wrong password\n";
				cout << "Do you want to continue? <y/n>\n";
				c = getchar();
			}
			else {
				return true;
			}
		} while (c == 'y');
		return false;
	}
	return true;
}

void Root::hide_show_Vol(char Name)
{
	if (!checkPWD()) {
		return;
	}
	for (int i = 0; i < list.size(); i++) {
		if (list[i].Name == Name) {
			ifstream fin(this->fileName);
			if (!fin.is_open()) {
				DEBUG_PRINT("[CAN NOT OPEN DISK]");
			}
			seeker point = this->Sb;
			point *= UNIT_SIZE;
			point += 1;
			do
			{
				fin.seekg(point);
				char c;
				LoadByte(fin, c);
				if (c == Name) {
					point = point - 1;
					break;
				}
				point = point + 10;
			} while (point < (seeker)(13) * UNIT_SIZE);

			ofstream fout(fileName, ios::in | ios::out | ios::binary);
			fout.seekp(point);
			list[i].flags = list[i].flags ^ HIDDEN;
			SaveByte(fout, list[i].flags);
			fout.close();
			return;
		}
	}
	cout << "VOLUME CAN NOT FOUND\n";
}
