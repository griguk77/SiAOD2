#pragma once
#include "BinFile.h"
#include "BinSearchTree.h"
using namespace std;
typedef node* Tbin;
Tbin createTreefromBinFile(string namebf) {
	fstream bf(namebf, ios::binary | ios::in);
	Tbin Root = NULL;
	int i;
	if (bf.is_open()) {
		record r;
		i = 0;
		bf.read((char*)&r, sizeof(r));
		while (!bf.eof()) {
			Insert(Root, r.key, i);
			i++; bf.read((char*)&r, sizeof(record));

		}
		bf.close();
		return Root;
	}
	return NULL;
}
//доступ к записи в файле по ее номеру
//Результат: 1) прочитанная запись из файла - параметр r 
//           2) коды завершения: 0 запись найдена и прочитана ;
//                               1 номер записи вне файла
//                             2 запись с ключом key не найдена в дереве Т
int readRecordInFile(fstream& bf, int key, Tbin T, record& r) {
	Tbin node = Search(T, key);
	if (node != NULL) {
		int offset = node->numRecordInBinFile * sizeof(record);
		bf.seekg(offset, ios::beg);
		if (!bf.bad()) {
			bf.read((char*)&r, sizeof(record));
			return 0;
		}
		else {
			return 1;
		}
	}
	else {
		return 2;
	}
}

/*int readRecord(string namebf, int key, Tbin T) {
	fstream bf(namebf, ios::binary | ios::in);



}
*/