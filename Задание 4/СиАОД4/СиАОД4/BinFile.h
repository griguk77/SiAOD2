#pragma once
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

struct record {
	int key;
	char ISBN[20];
	char author[20];
	char name[20];
};
//1 создание бинарного файла из текстового
int CreateBinFile(string nametf, string namebf) {

	ifstream itf(nametf);
	if (itf) {
		fstream obf(namebf, ios::out | ios::binary);
		record r;
		int len = sizeof(record);
		while (!itf.eof()) {
			itf >> r.key;
			itf.get();
			itf.getline(r.ISBN, len, '\n');
			itf.getline(r.author, len, '\n');
			itf.getline(r.name, len, '\n');
			obf.write((char*)&r, sizeof(record));
		}
		itf.close();
		obf.close();
		return 0;
	}
	return 1;
}

//2 вывод бинарного файла
int outBinFile(string namebf) {
	fstream obf(namebf, ios::in | ios::binary);
	if (obf.is_open()) {
		record r;
		int len = sizeof(r);
		obf.read((char*)&r, len);
		while (!obf.eof()) {
			cout << r.key << "  " << r.ISBN << "  " << r.author << "  " << r.name << "\n";
			obf.read((char*)&r, sizeof(record));
		}
		obf.close();
		return 0;
	}
	return 1;
}
