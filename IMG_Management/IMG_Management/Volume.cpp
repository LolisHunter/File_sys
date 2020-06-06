#include "Volume.h"
#include <iostream>
#include <fstream>
#include "img.h"
#include "Debug.h"
#include "mask.h"
using namespace std;

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
void Volume::_list(string tab) {
	cout << tab;
	cout << ">" << this->Name << endl;
	for (auto i : entry) {
		i._list(tab + "---");
	}
}

void Volume::GetEntryName(Entry *entry)
{
	ifstream fin(disk, ios::in || ios::binary);
	seeker point = ViTriCluster(entry->StCluster);
	fin.seekg(point, ios::beg);
	LoadByte(fin, entry->ino);
	uint32_t i;
	LoadByte(fin, i);
	LoadByte(fin, entry->Namesize);
	for (int i = 0; i < entry->Namesize; i++)
	{
		char a;
		LoadByte(fin, a);
		entry->name.push_back(a);
	}
	fin.close();
}
void Volume::ls()
{
	for (int i = 0; i < entry.size(); i += 3) {
		cout << entry[i].name << "\t\t";
		cout << ((i + 1 < entry.size()) ? entry[i + 1].name : "") << "\t\t";
		cout << ((i + 2 < entry.size()) ? entry[i + 2].name : "") << "\n";
	}
}
void Volume::LoadFolder(string disk, Entry *entry)
{
	ifstream fin(disk, ios::binary | ios::in);
	seeker point = ViTriCluster(entry->StCluster);
	fin.seekg(point, ios::beg);
	Entry a;
	do {
		LoadByte(fin, a.flags);
		if ((a.flags ^ END) < a.flags)
		{
			break;
		}
		if (a.flags == 0)
			return;
		if ( ((a.flags^DELETED) < a.flags))
		{
			fin.seekg(31, ios::cur);
			continue;
		}
		LoadByte(fin, a.ctime);
		if (a.ctime == 0)
			return;
		LoadByte(fin, a.mtime);
		LoadByte(fin, a.StCluster);
		LoadByte(fin, a.size);
		LoadByte(fin, a.TypeNum);
		LoadByte(fin, a.ino);
		LoadByte(fin, a.entryStSector);
		fin.seekg(4, ios::cur);
		GetEntryName(&a);
		if (a.size == 0)
			a.name = "Thu Muc";
		if ((a.flags ^ SUB_ENTRY) < a.flags)
		{
			fin.seekg(a.entryStSector, ios::cur);
		}
		entry->list.push_back(a);
		if (a.size == 0)
			LoadFolder(disk, &entry->list[entry->list.size() - 1]);
	} while (true);
	fin.close();
}

void Volume::Load(string fileName)
{
	ifstream fin(fileName, ios::in | ios::out | ios::binary);
	seeker point = startSector;
	point *= UNIT_SIZE;
	fin.seekg(point);

	LoadByte(fin, this->Ss);
	LoadByte(fin, this->Sc);
	LoadByte(fin, this->Sb);
	LoadByte(fin, this->Nf);
	LoadByte(fin, this->Sf);
	LoadByte(fin, this->Sv);
	LoadByte(fin, this->Nc);
	LoadByte(fin, this->StCluster);
	LoadByte(fin, this->FAT_len);
	// load FAT
	bool b;
	point = (uint64_t)startSector + Sb;
	point *= UNIT_SIZE;
	fin.seekg(point, ios::beg);
	for (int i = 0; i < FAT_len; i++)
	{
		LoadByte(fin, b);
		FAT.push_back(b);
	}
	// load entry
	point = (uint64_t)startSector + Sb + (uint64_t)Sf * Nf;
	point *= UNIT_SIZE;
	fin.seekg(point, ios::beg);
	Entry a;
	do {
		LoadByte(fin, a.flags);
		if ((a.flags ^ END) < a.flags)
		{
			break;
		}
		if ((a.flags^DELETED) < a.flags)
		{
			fin.seekg(31, ios::cur);
			continue;
		}
		LoadByte(fin, a.ctime);
		if (a.ctime == 0)
			return;
		LoadByte(fin, a.mtime);
		LoadByte(fin, a.StCluster);
		LoadByte(fin, a.size);
		LoadByte(fin, a.TypeNum);
		LoadByte(fin, a.ino);
		LoadByte(fin, a.entryStSector);
		fin.seekg(4, ios::cur);
		if (a.size == 0)
			a.name = "Thu Muc";
		if ((a.flags ^ SUB_ENTRY) < a.flags)
		{
			fin.seekg(a.entryStSector, ios::cur);
		}
		GetEntryName(&a);
		entry.push_back(a);
		if (a.size == 0)
			LoadFolder(disk, &entry[entry.size() - 1]);
	} while (true);
}

uint32_t ConvertTimeUnixToFAT(time_t a)
{
	tm *b = new tm;
	localtime_s(b, &a);
	char day = 0;
	day = day ^ b->tm_mday;
	day = day ^ ((b->tm_mon + 1) << 5);
	day = day ^ ((b->tm_year - 80) << 9);
	char time = 0;
	time = time ^ (b->tm_sec / 2);
	time = time ^ ((b->tm_min) << 5);
	time = time ^ ((b->tm_hour) << 11);
	delete b;
	return (uint32_t)day ^ (time << 16);
}

bool Volume::Create(Packg& scope,string fileName, bool Vname[26])
{
	uint64_t max_size = ((uint64_t)(scope.end - scope.strt) + 1);
	max_size *= 512;
	uint64_t byte;
	do
	{
		cout << "nhap kich thuoc: (MB)";
		cin >> byte;
		byte = byte * 1024 * 1024;
	} while (byte > max_size && cout << "Bruh!\n");

	this->Ss = UNIT_SIZE;
	this->Sc = 8;
	this->Sb = 1;
	this->Nf = 1;

	byte = byte/512 - this->Sb;
	this->Sf = ceil((double)byte / ((uint32_t)this->Ss * this->Sc - 1));
	this->Nc = (byte - this->Sf) / this->Sc;

	for (int i = 0; i < 26; i++) {
		if (!Vname[i]) {
			this->Name = 'A' + i;
			Vname[i] = true;
			break;
		}
	}
	this->setFlags();

	this->Sv = Sb + Sf*Nf + Nc*Sc;
	this->FAT_len = (Sv - Nc)/8;
	this->startSector = scope.strt;
	for (int i=0;i<FAT_len;i++)
		FAT.push_back(0);

	ofstream fout(fileName, ios::in | ios::out | ios::binary);
	if (!fout.is_open()) {
		return EXIT_FAILURE;
	}
	seeker pos = startSector; pos = pos * 512;
	fout.seekp(pos);
	// luu volume entry == > root luu entry neu create success
	//SaveByte(fout,this->Name);
	//SaveByte(fout, start);
	//SaveByte(fout, end);
	// luu volume
	// boot
	SaveByte(fout, this->Ss);
	SaveByte(fout, this->Sc);
	SaveByte(fout, this->Sb);
	SaveByte(fout, this->Nf);
	SaveByte(fout, this->Sf);
	SaveByte(fout, this->Sv);
	SaveByte(fout, this->Nc);
	SaveByte(fout, this->StCluster);
	SaveByte(fout, this->FAT_len);

	int core = sizeof(Ss) +
		sizeof(Sc) +
		sizeof(Sb) +
		sizeof(Nf) +
		sizeof(Sf) +
		sizeof(Sv) +
		sizeof(Nc) +
		sizeof(StCluster) +
		sizeof(FAT_len);

	fout.close();
	return EXIT_SUCCESS;
}

void Volume::setFlags()
{
	this->flags = USING;
}

uint32_t Volume::FreeInFAT() // tra ve don vi cluster
{
	int i;
	for (i = 0; i < FAT_len; i++)
	{
		if (FAT[i] == 0)
			return i;
	}
	throw ("Volume is full");
	return 0;
}
uint32_t Volume::FreeInFAT(int i) // tra ve don vi cluster
{
	for (; i < FAT_len; i++)
	{
		if (FAT[i] == 0)
			return i;
	}
	throw ("Volume [i,max] is full");
	return 0;
}

void Volume::addEntrySt(Entry* file, Entry* ViTriRDET)
{
	if (ViTriRDET == NULL)
	{
		file->entryStSector = (seeker)startSector + Sb + Sf * Nf; // vi tri RDET chinh
		entry.push_back(*file);
		AddEntry(*file);
	}
	else
	{
		ViTriRDET->list.push_back(*file);
		if (ViTriRDET->list.size() == int((8 * Ss) / 32) - 1)
		{
			Entry a;
			a.flags = END;
			ViTriRDET->list.push_back(a);
		}
		file->entryStSector = ViTriCluster(ViTriRDET->StCluster) / Ss;
		AddEntry(*file);
	}
}


seeker Volume::AddTable(seeker seek, bool End, int& i)
{
	ofstream fout(disk, ios_base::in | ios_base::out | ios_base::binary);
	if (fout.is_open() == false)
	{
		throw exception("Can't open disk. Can't add table!");
	}
	fout.seekp(seek, ios::beg);
	i = FreeInFAT();
	seeker sker = ((seeker)startSector + Sb + Sf * Nf) * Ss + i * Sc * Ss;
	if (End) // neu la flag end
	{
		SaveByte(fout, SUB_ENTRY);
		SaveByte(fout, (uint8_t)0);
		fout.seekp(19, ios::cur);
		SaveByte(fout, (uint64_t)sker);
	}
	fout.close();
	seek = i;
	return sker;
}

void Volume::ExportFiLe(string path, const Entry* file)
{
	ifstream fin(disk, ios::binary | ios::in);
	ofstream fout(path, ios::binary | ios::out);
	if (!fin.is_open())
		throw exception("Can't open disk when export file");
	uint32_t curCluster = file->StCluster;
	uint32_t nextCluster;
	uint32_t temp = curCluster;
	do
	{
		curCluster = temp;
		fin.seekg(ViTriCluster(curCluster) + 2, ios::beg);
		LoadByte(fin, nextCluster);
		fin.seekg(ViTriCluster(curCluster) + Ss, ios::beg);
		char* data = new char[(Sc - 1) * Ss + 1];
		uint32_t buffer = (Sc - 1) * Ss;
		fin.read(data, buffer);
		fout.write(data, buffer);
		temp = nextCluster;
	} while (nextCluster != curCluster); // dau hieu ket thuc
	fin.close();
	fout.close();
}

void Volume::AddEntry(Entry& entry)
{
	ifstream fin(disk, ios::in | ios::binary);


	if (fin.is_open() == false) {
		throw exception("Can't open disk. Can't add entry!");
	}

	seeker sker = entry.entryStSector;  //(entry.entryStCluster); sker *= this->Ss; sker *= this->Sc;
	sker *= UNIT_SIZE;
	int i = 0;
	uint8_t flag;
	do {
		fin.seekg(sker, ios::beg);
		LoadByte(fin, flag);
		//Check if(flag = END)
		if ((flag ^ END) < flag)
		{
			sker = AddTable(sker, NULL, i);
			break;
		}
		if (flag == 0 || (flag ^ DELETED) < flag)
		{
			break;
		}
		else {
			sker += 32;
		}
	} while (true);
	ofstream fout(disk, ios::in | ios::binary);
	fout.seekp(sker, ios::beg);
	SaveByte(fout, entry.flags);
	SaveByte(fout, entry.ctime);
	SaveByte(fout, entry.mtime);
	SaveByte(fout, entry.StCluster);
	SaveByte(fout, entry.size);
	SaveByte(fout, entry.TypeNum);
	SaveByte(fout, entry.ino);
	SaveByte(fout, entry.entryStSector);
	FAT[i] = 1;
	fout.close();
}


uint64_t Volume::ViTriCluster(int i) // tra ve vi tri byte trong disk
{
	return ((seeker)startSector + Sb + Sf * Nf + 8 + i * 8) * Ss;
}

uint8_t Volume::CheckType(string type, vector<Type>& type_list)
{
	if (type.size() > 15)
		return 0;
	for (auto it : type_list)
	{
		if (strcmp(it.extension, type.c_str()) == 0)
		{
			return it.code;
		}
	}
	Type a;
	a.code = type_list.size() + 1;
	for (int i = 0; i < type.size(); i++)
		a.extension[i] = type[i];
	type_list.push_back(a);
	ofstream fout(disk, ios::out | ios::binary | ios::in);
	seeker point = 3;
	point *= UNIT_SIZE;
	fout.seekp(point);
	SaveByte(fout, a.code);
	for (auto i : a.extension) {
		SaveByte(fout, i);
	}
	fout.close();
	return a.code;
}

string TakeType(string fileName)
{
	int i = 0;
	int temp = fileName.find('.', i);
	if (temp < 0)
		return "";
	while (temp > i)
	{
		i = fileName.find('.', i + 1);
		temp = fileName.find('.', i + 1);
	}
	return fileName.substr(i + 1, fileName.size() - i - 1);
}

void Volume::AddData(ifstream& file, Entry* f)
{
	ofstream Disk(disk, ios::out | ios::binary | ios::in);
	if (!Disk.is_open())
		throw exception("Can't open disk. Can't add data!");
	uint8_t temp;
	int i = FreeInFAT();
	f->StCluster = i;
	seeker vt1 = ViTriCluster(i);
	seeker vt2;
	Disk.seekp(vt1, ios::beg);
	seeker fileLength = 0;
	file.seekg(0, ios::end);
	fileLength = file.tellg();
	file.seekg(0);
	f->size = fileLength;
	while (!file.eof())
	{
		SetFAT(i, 1);
		SaveByte(Disk, f->ino);
		if (!file.eof())
			i = FreeInFAT(i);
		SaveByte(Disk, (uint32_t)i);
		SaveByte(Disk, f->Namesize);
		for (auto i : f->name)
			SaveByte(Disk, i);
		Disk.seekp(vt1 + Ss, ios::beg);
		seeker Length = ((seeker)Sc - 1) * Ss;
		if (fileLength <= (((seeker)Sc - 1) * Ss))
			Length = fileLength;
		else
		{
			Length = ((seeker)Sc - 1) * Ss;
			fileLength -= ((seeker)Sc - 1) * Ss;
		}
		for (int i = 0; i < Length; i++)
		{
			LoadByte(file, temp);
			SaveByte(Disk, temp);
		}
		if (Length == fileLength)
			file.get();
		Disk.seekp(vt1 + 512, ios::beg);
		vt2 = ViTriCluster(i);
		Disk.seekp(vt2 - vt1, ios::cur);
		vt1 = vt2;
	}
	file.close();
}

void Volume::SetFAT(uint32_t cluster, bool value)
{
	FAT[cluster] = 1;
	seeker temp = startSector + Sb + cluster;
	temp *= UNIT_SIZE;
	ofstream fout(disk, ios::out | ios::binary | ios::in);
	fout.seekp(temp);
	SaveByte(fout, value);
	fout.close();
}

bool Volume::Import(string pathFile, Entry* vitri, vector<Type>& type_list) //luc dau vitri = NULL
{
	ifstream fin(pathFile, ios_base::in | ios_base::binary);
	if (fin.is_open()) // la file
	{
		Entry a;
		int i = 0; // tim name trong path
		int temp = pathFile.find('\\', i);
		while (temp > i)
		{
			i = pathFile.find('\\', i + 1);
			temp = pathFile.find('\\', i + 1);
		}
		a.size = 1;
		a.name = pathFile.substr(i + 1, pathFile.size() - i - 1);
		a.Namesize = a.name.size();
		a.TypeNum = CheckType(TakeType(a.name), type_list);
		a.flags = ENTRYUSING;
		struct stat st;
		stat(pathFile.c_str(), &st);
		a.ctime = ConvertTimeUnixToFAT(st.st_ctime);
		a.mtime = ConvertTimeUnixToFAT(st.st_mtime);
		AddData(fin, &a);
		addEntrySt(&a, vitri);  // o dia
		fin.close();
	}
	else // la thu muc hoac khong ton tai
	{
		system(("dir /b/a-d-h " + pathFile + ">file.txt").c_str()); // doc cac file
		ifstream file("file.txt");
		system(("dir /b/ad-h " + pathFile + ">folder.txt").c_str()); // doc cac file
		ifstream folder("folder.txt");
		if (!file.is_open() || !folder.is_open())
			return false;
		if (file.get() != -1 && folder.get() != -1)
			return false;// file ko ton tai
		file.seekg(0, ios::beg);
		folder.seekg(0, ios::beg);
		Entry a;
		struct stat st;
		stat(pathFile.c_str(), &st);
		a.ctime = ConvertTimeUnixToFAT(st.st_ctime);
		a.mtime = ConvertTimeUnixToFAT(st.st_mtime);
		a.TypeNum = 0;
		int i = 0; // tim name trong path
		int temp = pathFile.find('\\', i);
		while (temp > i)
		{
			i = pathFile.find('\\', i + 1);
			temp = pathFile.find('\\', i + 1);
		}
		a.name = pathFile.substr(i + 1, pathFile.size() - i - 1);
		a.Namesize = a.name.size();
		a.size = 0;
		a.flags = ENTRYUSING;
		AddTable(0, 0, i);
		a.StCluster = i;
		SetFAT(i, 1);
		addEntrySt(&a, vitri); // o dia
		Entry* link;
		if (vitri)
		{
			vitri->list.push_back(a); //logic
			link = &vitri[vitri->list.size() - 1];
		}
		else
		{
			entry.push_back(a); // logic
			link = &entry[entry.size() - 1];
		}


		string dir;
		while (!file.eof())
		{
			getline(file, dir);
			if (dir.size() == 0)
				break;
			if (!Import(pathFile + "\\" + dir, link, type_list))
				return false;
		}
		file.close();

		while (!folder.eof())
		{
			getline(folder, dir);
			if (dir.size() == 0)
				break;
			if (!Import(pathFile + "\\" + dir, link, type_list))
				return false;
		}
		file.close();
		folder.close();
		fin.close();
	}
	return 1;
}

bool Volume::Export(string path, Entry* vitri)
{
	string temp;
	if (path.size() == 0)
		temp = vitri->name;
	else
		temp = path + "\\" + vitri->name;
	if (vitri->list.size() != 0)
	{
		_mkdir(temp.c_str());
		for (int i = 0; i < vitri->list.size(); i++)
		{
			Export(temp, &vitri->list[i]);
		}
	}
	else
	{
		ExportFiLe(temp, vitri);
	}
	return true;
}

Entry* Volume::getEntry(string Name)
{
	if (!entry.size()) {
		return NULL;
	}
	for (int i = 0; i < entry.size();i++) {
		if (entry[i].name == Name) {
			return &entry[i];
		}
	}
	return NULL;
}
