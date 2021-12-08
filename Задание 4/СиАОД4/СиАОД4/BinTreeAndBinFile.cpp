#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class record {
public:
	int key;	// ключ (построенный по ISBN)
	char ISBN[13];	// номер ISBN
	char author[20];	// автор книги
	char name[20];	//название книги

	int CreateBinFile(string nametf, string namebf, int* keys) {
		int i = 0;
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
				keys[i] = r.key;
				i++;
			}
			itf.close();
			obf.close();
			return 0;
		}
		return 1;
	}

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
};

class node {
public:
	int key = 0;	// ключ
	int numRecordInBinFile = 0;	// позиция элемента в файле
	node* left = nullptr;	// ссылка на левое поддерево
	node* right = nullptr;	// ссылка на правое поддерево


	typedef node* Tbin;

	void Insert(Tbin& T, int key, int numRecord) {
		if (T == NULL)
		{
			T = new node;
			T->key = key;
			T->numRecordInBinFile = numRecord;
		}
		else
		{
			if (key < T->key)
				Insert(T->left, key, numRecord);
			else
				if (key > T->key)
					Insert(T->right, key, numRecord);
		}
	}

	Tbin Search(Tbin T, int key) {
		if (T == NULL)
			return NULL;
		else
		{
			if (T->key == key)
				return T;
			else
			{
				if (key < T->key)
					return Search(T->left, key);
				else
					return Search(T->right, key);
			}
		}
	}

	void printT(Tbin T, int l, int k) {
		if (T != NULL)
		{
			printT(T->right, l + 1, k);
			for (int i = 1; i <= l * k; i++) {
				cout << ' ';
			}
			cout << T->key << "\n";
			printT(T->left, l + 1, k);
		}
	}

	void del(Tbin& qleft, Tbin& q) {
		if (qleft->right) {
			del(qleft->right, q);
		}
		else {
			q->key = qleft->key;
			q->numRecordInBinFile = qleft->numRecordInBinFile;
			q = qleft;
			qleft = qleft->left; 
		}
	}


	int delTree(int key, Tbin& T) {
		int i = 1;
		Tbin q = NULL;
		if (T == NULL) {
			return 1;
		}
		if (T) {
			if (key < T->key) {
				i = delTree(key, T->left);
			}
			else {
				if (key > T->key) {
					i = delTree(key, T->right);
				}
				else {
					i = 0;
					q = T;
					if (q->right == NULL) {
						T = q->left;
					}
					else {
						if (q->left == NULL) {
							T = q->right;
						}
						else {
							del(q->left, q);
						}
					}
					delete q;
					return i;
				}
				delete q;
				return i;
			}
			delete q;
			return i;
		}
	}

	Tbin createTreefromBinFile(string namebf, int& numRecord) {
		fstream bf(namebf, ios::binary | ios::in);
		Tbin Root = NULL;
		if (bf.is_open()) {
			record r;
			numRecord = 0;
			bf.read((char*)&r, sizeof(r));
			while (!bf.eof()) {
				Insert(Root, r.key, numRecord);
				numRecord++;
				bf.read((char*)&r, sizeof(record));
			}
			bf.close();
			return Root;
		}
		return NULL;
	}

	int readRecordInFile(fstream& bf, int key, Tbin T, record& r) {
		Tbin node = Search(T, key);
		if (node != NULL) {
			if (node->numRecordInBinFile == -1) {
				return 1;
			}
			int offset = node->numRecordInBinFile * sizeof(record);
			bf.seekg(offset, ios::beg);
			if (!bf.bad()) {
				bf.read((char*)&r, sizeof(record));
				cout << node->key << endl;
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
};

int main() {
	setlocale(LC_ALL, "Russian");
	typedef node* Tbin;
	record r;
	int numRecord = 0, key, delPr, L = 5, f, pr = 0;
	int keys[20];
	for (int i = 0; i < 20; i++) {
		keys[i] = 0;
	}
	int kod = r.CreateBinFile("A.txt", "Bin.dat", keys);
	Tbin Root = NULL, search;
	if (kod != 0) {
		cout << "file not found";
	}
	else {
		r.outBinFile("Bin.dat");
	}
	int m = 1;
	fstream bf("Bin.dat", ios::binary | ios::in);
	while (m != 0) {
		cout << "Выберете действие:\n";
		cout << " 1.Включение элемента в дерево.\n 2.Поиск ключа в дереве.\n 3.Удаление ключа из дерева.\n 4.Отображение дерева.\n 5.Создание дерева на основе двоичного файла.\n 6.Поиск записи в файле с использованием БДП.\n 7.Вывод двоичного файла.\n 8.Заполнение дерева 1000 узлами.\n 0.Завершение работы.\n";
		cin >> m;
		switch (m) {
		case 1:
			pr = 0;
			cout << "Введите ключ для узла: ";
			cin >> key;
			for (int i = 0; i < 20; i++) {
				if (key == keys[i]) {
					Root->Insert(Root, key, i);
					pr = -1;
					break;
				}
			}
			if (pr != -1) {
				Root->Insert(Root, key, -1);
			}
			break;
		case 2:
			cout << "Введите ключ для поиска: ";
			cin >> key;
			search = Root->Search(Root, key);
			cout << search << endl;
			break;
		case 3:
			cout << "Введите ключ узла для удаления: ";
			cin >> key;
			delPr = Root->delTree(key, Root);
			if (delPr == 0) {
				cout << "Узел удалён\n";
			}
			else {
				cout << "Узел с таким ключём отсутствует\n";
			}
			break;
		case 4:
			cout << "Введите расстояние между узлами: ";
			cin >> L;
			Root->printT(Root, 0, L);
			break;
		case 5:
			Root = Root->createTreefromBinFile("Bin.dat", numRecord);
			break;
		case 6:
			cout << "Введите ключ: ";
			cin >> key;
			f = Root->readRecordInFile(bf, key, Root, r);
			if (f == 0) {
				cout << "Запись найдена и прочитана\n";
				cout << "Вывод информации о найденной записи:\n";
				cout << "ISBN: " << r.ISBN << "; Автор книги: " << r.author << "; Название книги: " << r.name << endl;
			}
			if (f == 1) {
				cout << "Номер записи вне файла\n";
			}
			if (f == 2) {
				cout << "Узел с введённым ключом не найден в дереве\n";
			}
			break;
		case 7:
			r.outBinFile("Bin.dat");
			break;
		case 8:
			for (int i = 0; i < 1000; i++) {
				Root->Insert(Root, i, i);
			}
			break;
		}
	}
	int skey = 0;
	Tbin test;
	clock_t start1 = clock();
	test = Root->Search(Root, skey);
	clock_t end1 = clock();
	cout << endl << "Поиск нулевого элемента: " << ((double)end1 - start1) / ((double)CLOCKS_PER_SEC) << endl << test << endl << endl;
	skey = 500;
	clock_t start2 = clock();
	test = Root->Search(Root, skey);
	clock_t end2 = clock();
	cout << "Поиск среднего элемента: " << ((double)end2 - start2) / ((double)CLOCKS_PER_SEC) << endl << test << endl << endl;
	skey = 999;
	clock_t start3 = clock();
	test = Root->Search(Root, skey);
	clock_t end3 = clock();
	cout << "Поиск последнегого элемента: " << ((double)end3 - start3) / ((double)CLOCKS_PER_SEC) << endl << test << endl << endl;
	cout << "Завершение работы";
}