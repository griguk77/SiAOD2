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

	node* newNode(int key, int numRecord) {
		node* Node = new node;
		Node->key = key;
		Node->numRecordInBinFile = numRecord;
		Node->left = nullptr;
		Node->right = nullptr;
		return Node;
	}

	node* rightRotate(node* Root) {
		node* T = Root->left;
		Root->left = T->right;
		T->right = Root;
		return T;
	}

	node* leftRotate(node* Root) {
		node* T = Root->right;
		Root->right = T->left;
		T->left = Root;
		return T;
	}

	node* splaySearch(node* Root, int key) {
		if (Root == nullptr || Root->key == key) {
			return Root;
		}
		if (Root->key > key) {
			if (Root->left == nullptr) {
				return Root;
			}
			if (Root->left->key > key) {
				Root->left->left = splaySearch(Root->left->left, key);
				Root = rightRotate(Root);
			}
			else {
				if (Root->left->key < key) {
					Root->left->right = splaySearch(Root->left->right, key);
					if (Root->left->right != nullptr) {
						Root->left = leftRotate(Root->left);
					}
				}
			}
			if (Root->left == nullptr) {
				return Root;
			}
			else {
				return rightRotate(Root);
			}
		}
		else {
			if (Root->right == nullptr) {
				return Root;
			}
			if (Root->right->key > key) {
				Root->right->left = splaySearch(Root->right->left, key);
				if (Root->right->left != nullptr) {
					Root->right = rightRotate(Root->right);
				}
			}
			else {
				if (Root->right->key < key) {
					Root->right->right = splaySearch(Root->right->right, key);
					Root = leftRotate(Root);
				}
			}
			if (Root->right == nullptr) {
				return Root;
			}
			else {
				return leftRotate(Root);
			}
		}
	}

	node* insert(node* root, int key, int numRecord) {
		if (root == nullptr) {
			return newNode(key, numRecord);
		}
		root = splaySearch(root, key);
		if (root->key == key) {
			return root;
		}
		node* nNode = newNode(key, numRecord);
		if (root->key > key) {
			nNode->right = root;
			nNode->left = root->left;
			root->left = nullptr;
		}
		else {
			nNode->left = root;
			nNode->right = root->right;
			root->right = nullptr;
		}
		return nNode;
	}

	node* Delete(int key, node* root, int& delPr) {
		if (!root) {
			delPr = 1;
			return nullptr;
		}
		node* temp = new node;
		root = splaySearch(root, key);
		if (key != root->key) {
			delPr = 1;
			return root;
		}
		else {
			if (!root->left) {
				temp = root;
				root = root->right;
			}
			else {
				temp = root;
				root = splaySearch(root->left, key);
				root->right = temp->right;
			}
			delete temp;
			delPr = 0;
			return root;
		}
	}

	void printT(node* T, int l, int k) {
		if (T != nullptr) {
			printT(T->right, l + 1, k);
			for (int i = 1; i <= l * k; i++) {
				cout << ' ';
			}
			cout << T->key << "\n";
			printT(T->left, l + 1, k);
		}
	}



	void printTreeVertical(node* T, int level, int* c, int* indexLev, int& spaceCount, int* space) {
		if (T != nullptr) {
			printTreeVertical(T->left, level + 1, c, indexLev, spaceCount, space);
			c[(int)pow(2, level) + indexLev[level] - 1] = T->key;
			space[(int)pow(2, level) + indexLev[level] - 1] = spaceCount;
			spaceCount++;
			indexLev[level]++;
			printTreeVertical(T->right, level + 1, c, indexLev, spaceCount, space);
			if (level == 0) {
				int index = 0;
				int step = -1;
				int maxLevel = 0;
				for (int i = 0; i < 500; i++) {
					if (indexLev[i] == 0) {
						maxLevel = i - 2;
						break;
					}
				}
				for (int i = 0; i <= maxLevel; i++) {
					for (int j = 1; j <= (int)pow(2, i); j++) {
						if (c[index] != -1) {
							for (int k = 0; k < space[index] - step; k++) {
								cout << ' ';
							}
							step = space[index] + 1;
							cout << c[index];
						}
						index++;
					}
					cout << endl;
					step = -1;
				}
			}
		}
		else {
			indexLev[level]++;
		}
	}

	node* createTreefromBinFile(string namebf, int& numRecord) {
		fstream bf(namebf, ios::binary | ios::in);
		node* Root = nullptr;
		if (bf.is_open()) {
			record r;
			numRecord = 0;
			bf.read((char*)&r, sizeof(r));
			while (!bf.eof()) {
				Root = insert(Root, r.key, numRecord);
				numRecord++;
				bf.read((char*)&r, sizeof(record));
			}
			bf.close();
			return Root;
		}
		return nullptr;
	}

	node* readRecordInFile(fstream& bf, int key, node* T, record& r, int& f) {
		T = splaySearch(T, key);
		if (T == nullptr) {
			f = 2;
			return T;
		}
		if (T->key != key) {
			f = 2;
			return T;
		}
		else {
			if (T != nullptr) {
				if (T->numRecordInBinFile == -1) {
					f = 1;
					return T;
				}
				int offset = T->numRecordInBinFile * sizeof(record);
				bf.seekg(offset, ios::beg);
				if (!bf.bad()) {
					bf.read((char*)&r, sizeof(record));
					cout << T->key << endl;
					f = 0;
					return T;
				}
				else {
					f = 1;
					return T;
				}
			}
			else {
				f = 2;
				return T;
			}
		}
	}
};

int main()
{
	setlocale(LC_ALL, "Russian");
	int indexLev[1111];
	int c[1111];
	int space[1111];
	int spaceCount;
	record r;
	int numRecord = 0, key, delPr, L = 5, f, pr = 0;
	int keys[20];
	for (int i = 0; i < 20; i++) {
		keys[i] = 0;
	}
	int kod = r.CreateBinFile("A.txt", "Bin.dat", keys);
	node* Root = nullptr, search;
	if (kod != 0)
		cout << "file not found";
	else
		r.outBinFile("Bin.dat");
	int m = 1;
	fstream bf("Bin.dat", ios::binary | ios::in);
	while (m != 0) {
		cout << "Выберете действие:\n";
		cout << " 1.Включение элемента в дерево.\n 2.Поиск ключа в дереве.\n 3.Удаление ключа из дерева.\n 4.Отображение дерева (вертикально).\n 5.Создание дерева на основе двоичного файла.\n 6.Поиск записи в файле с использованием БДП.\n 7.Вывод двоичного файла.\n 8.Отображение дерева (боком).\n 9.Заполнение дерева 1000 узлами.\n 0.Завершение работы.\n";
		cin >> m;
		switch (m) {
		case 1:
			pr = 0;
			cout << "Введите ключ для узла: ";
			cin >> key;
			for (int i = 0; i < 20; i++) {
				if (key == keys[i]) {
					Root = Root->insert(Root, key, i);
					pr = -1;
					break;
				}
			}
			if (pr != -1) {
				Root = Root->insert(Root, key, -1);
			}
			break;
		case 2:
			cout << "Введите ключ для поиска: ";
			cin >> key;
			Root = Root->splaySearch(Root, key);
			cout << Root << endl;
			break;
		case 3:
			cout << "Введите ключ узла для удаления: ";
			cin >> key;
			Root = Root->Delete(key, Root, delPr);
			if (delPr == 0) {
				cout << "Узел удалён\n";
			}
			else {
				cout << "Узел с таким ключём отсутствует\n";
			}
			break;
		case 4:
			if (Root == NULL) {
				cout << "Дерево пусто\n";
			}
			else {
				for (int i = 0; i < 100; i++) {
					c[i] = -1;
				}
				for (int i = 0; i < 100; i++) {
					space[i] = 0;
				}
				for (int i = 0; i < 20; i++) {
					indexLev[i] = 0;
				}
				spaceCount = 0;
				Root->printTreeVertical(Root, 0, c, indexLev, spaceCount, space);
			}
			break;
		case 5:
			Root = Root->createTreefromBinFile("Bin.dat", numRecord);
			break;
		case 6:
			cout << "Введите ключ: ";
			cin >> key;
			Root = Root->readRecordInFile(bf, key, Root, r, f);
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
			cout << "Введите расстояние между узлами: ";
			cin >> L;
			Root->printT(Root, 0, L);
			break;
		case 9:
			for (int i = 0; i < 1000; i++) {
				Root = Root->insert(Root, i, i);
			}
			break;
		}
	}
	int skey = 0;
	clock_t start1 = clock();
	Root = Root->splaySearch(Root, skey);
	clock_t end1 = clock();
	cout << endl << "Поиск нулевого элемента: " << ((double)end1 - start1) / ((double)CLOCKS_PER_SEC) << endl << Root << endl << endl;
	skey = 999;
	clock_t start2 = clock();
	Root = Root->splaySearch(Root, skey);
	clock_t end2 = clock();
	cout << "Поиск последнегого элемента: " << ((double)end2 - start2) / ((double)CLOCKS_PER_SEC) << endl << Root << endl << endl;
	skey = rand() % 1000;
	clock_t start3 = clock();
	Root = Root->splaySearch(Root, skey);
	clock_t end3 = clock();
	cout << "Поиск случайного элемента: " << ((double)end3 - start3) / ((double)CLOCKS_PER_SEC) << endl << Root << endl << endl;
	cout << "Завершение работы";
}