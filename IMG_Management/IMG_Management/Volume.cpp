#include "Volume.h"
#include <iostream>
#include <fstream>
#include "img.h"
#include "Debug.h"
#include "mask.h"
using namespace std;

template<class T>
void SaveByte(ofstream& fout, T in) {
	uint8_t c =0;
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
		T temp = c;
		temp = temp << (8 * i);
		out += temp;
	}
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

void Volume::_list(string tab)
{
	cout << tab;
	cout << ">" << this->Name << endl;
	for (auto i : entry) {
		i._list(tab + "   ");
	}
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
	this->Sf = ceil((double)byte / (this->Ss * this->Sc - 1));
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
	this->StCluster = this->Sb + this->Sf * this->Nf + scope.strt;
	this->FAT_len = (Sv - Nc)/8;
	this->startSector = scope.strt;

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

void Volume::addEntrySt(Entry *file,Entry *ViTriRDET)
{
	if (ViTriRDET == NULL)
	{
		file->entryStCluster = (startSector + Sb + Sf * Nf) / 8; // vi tri RDET chinh
		AddEntry(*file);
	}
	else
	{
		ViTriRDET->list.push_back(*file);
		file->entryStCluster = ViTriRDET->StCluster;
		AddEntry(*file);
	}
}


seeker Volume::AddTable(seeker seek, bool End, int&i)
{
	ofstream fout(disk, ios_base::in | ios_base::out | ios_base::binary);
	if (fout.is_open() == false)
	{
		throw exception("Can't open disk. Can't add table!");
	}
	fout.seekp(seek, ios::beg);
	i = FreeInFAT();
	seeker sker = (startSector + Sb + Sf * Nf) * Ss + i * Sc * Ss;
	if (End) // neu la flag end
	{
		SaveByte(fout, SUB_ENTRY);
		SaveByte(fout, (uint8_t)0);
		fout.seekp(16, ios::cur);
		SaveByte(fout, (uint64_t)sker);
	}
	fout.close();
	seek = i;
	return sker;
}

void Volume::ExportFiLe(string path,const Entry * file)
{
	ifstream fin(disk, ios::binary | ios::in);
	ofstream fout(path,ios::binary | ios::out);
	if (!fin.is_open())
		throw exception("Can't open disk when export file");
	uint32_t curCluster = file->StCluster;
	uint32_t nextCluster;
	do	
	{
			fin.seekg(ViTriCluster(curCluster) + 2, ios::beg);
			LoadByte(fin, nextCluster);
			fin.seekg(ViTriCluster(curCluster) + Ss, ios::beg);
			char * data = new char[(Sc - 1) * Ss + 1];
			uint32_t buffer = (Sc - 1) * Ss;
			fin.read(data, buffer);
			fout.write(data, buffer);
			curCluster = nextCluster;
	} while (nextCluster != 123456); // dau hieu ket thuc
	fin.close();
	fout.close();
}

void Volume::AddEntry(Entry& entry)
{
	ifstream fin;
	ofstream fout;

	fin.open(disk);
	if(fin.is_open() == false){
		throw exception("Can't open disk. Can't add entry!");
	}

	seeker sker = StCluster;
	sker *= UNIT_SIZE;
	fin.seekg(sker, ios::beg);

	uint8_t flag;
	do{
		LoadByte(fin, flag);
		//Check if(flag = END)
		if((flag ^ END) < flag)
		{
			int i = 0;
			sker = AddTable(sker,NULL, i);
			break; 
		}
		if(flag == 0 || (flag^DELETED) < flag)
		{
			break;
		}
		else{
			sker += 32;
		}
	} while(true);

	fout.open(disk, ios::in | ios::out | ios::binary);
	fout.seekp(sker);
	SaveByte(fout,entry.flags);
	SaveByte(fout,entry.ctime);
	SaveByte(fout,entry.mtime);
	SaveByte(fout,entry.StCluster);
	SaveByte(fout,entry.size);
	SaveByte(fout,entry.TypeNum);
	SaveByte(fout,entry.ino);
	SaveByte(fout,entry.entryStCluster);


	fout.close();
}

uint64_t Volume::ViTriCluster(int i) // tra ve vi tri byte trong disk
{
	return (startSector + Sb + Sf * Nf + 8 + i * 8) * Ss;
}

void Volume::AddData(fstream &file, Entry *f)
{
	ofstream log("log.txt");
	ifstream log1("log.txt");
	ofstream Disk(disk);
	if(!Disk.is_open())
		throw exception("Can't open disk. Can't add data!");
	char *temp = new char[(Sc - 1) * Ss + 1];
	int i = FreeInFAT();
	log << i;
	seeker vt1 = ViTriCluster(i);
	seeker vt2;
	Disk.seekp(vt1, ios::beg);
	while (file.getline(temp, (Sc - 1) * Ss))
	{
		SaveByte(Disk, f->ino);
		i = FreeInFAT(i);
		SaveByte(Disk,(uint32_t) i);
		SaveByte(Disk, f->Namesize);
		Disk.write(f->name.c_str(), sizeof(f->name.c_str()));
		Disk.seekp(vt1 + 512, ios::beg);
		Disk.write(temp, (Sc - 1) * Ss);
		vt2 = ViTriCluster(i);
		log << i;
		log << " ";
		Disk.seekp(vt2 - vt1, ios::cur);
		vt1 = vt2;
	}
	delete []temp;
	log.close();
	log1 >> i;
	f->StCluster = i;
	FAT[i] = 1;
	while (log1 >> i)
	{
		log1.get();
		FAT[i] = 1;
	}
	log1.close();
}

bool Volume::Import(string pathFile, Entry *vitri) //luc dau vitri = NULL
{
	fstream fin(pathFile, ios_base::in | ios_base::binary);
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
		a.flags = 0;
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
		file.seekg(0,ios::beg);
		folder.seekg(0,ios::beg);
		Entry a;
		struct stat st;
		stat(pathFile.c_str(), &st);
		a.ctime = ConvertTimeUnixToFAT(st.st_ctime);
		a.mtime = ConvertTimeUnixToFAT(st.st_mtime);
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
		a.StCluster = AddTable(0, 0, i);
		FAT[i] = 1;
		addEntrySt(&a, vitri); // o dia
		Entry *link;
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
			if (!Import(pathFile + "\\" + dir, link))
				return false;
		}
		file.close();

		while (!folder.eof())
		{
			getline(folder, dir);
			if (dir.size() == 0)
				break;
			if (!Import(pathFile + "\\" + dir, link))
				return false;
		}
		file.close();
		folder.close();
	}
	return 1;
}

bool Volume::Export(string path, Entry *vitri)
{
	string temp;
	if (path.size() == 0)
		temp = vitri->name;
	else
		temp = path + "\\" + vitri->name;
	if (vitri->list.size() != 0)
	{
		if (_mkdir(temp.c_str()) != 0)
		{
			return false; // tao thu muc khong thanh cong
		}
		for (int i = 0; i < vitri->list.size(); i++)
		{
			Export(temp, &vitri->list[i]);
		}
	}
	else
	{
		ExportFiLe(path, vitri);
	}
	return true;
}
