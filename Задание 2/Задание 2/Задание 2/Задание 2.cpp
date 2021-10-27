/*#include <iostream>
#include <fstream>

using namespace std;

struct Tbooks {
	char TISBN[21]; // ключ
	char Author[40]; // автор книги
	char Name[40]; // название книги
	bool Pins; // признак свободной для вставки ячейки
	bool Pdel; // признак удаления
};

int main()
{
	setlocale(LC_ALL, "Russian");
	ofstream fout;
	fout.open("DD.dat", ios::binary | ios::trunc);
	if (!fout)
	{
		cout << "file not open";
		return 1;
	}
	int x[3] = { 1,2,3 };
	//пишем массив из трех чисел
	fout.write((char*)x, 3 * sizeof(int));
	int y[3] = { 5,6,7 };
	fout.write((char*)y, 3 * sizeof(int));
	if (!fout.good())//контроль ошибок ввода-вывода
	{
		cout << "Error vvoda";
		return 1;
	}
	fout.close();
	ifstream fio("DD.dat", ios::in | ios::binary);//открыли для чтения
	while (!fio.eof())
	{ //читаем массив из трех чисел
		fio.read((char*)x, 3 * sizeof(int));
		if (!fio.eof()) {
			for (int i = 0; i < 3; i++) cout << x[i] << " Я x" << endl;
		}
	}
	fio.close();
	int a;
	ifstream fii;
	fii.open("DD.dat", ios::in | ios::binary);
	while (!fii.eof())
	{ //читаем по одному числу
		fii.read((char*)&a, sizeof(int));
		if (!fii.eof())
		{
			cout << a << " А я a" << endl;
		}
	}
	fii.close();
	return 0;
}*/

// ConsoleApplication27.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int hesh(unsigned long long a, int b) {
    return a % b;
}
struct Tbooks {
    unsigned long long ISBN; // ключ
    string Author; // автор книги
    string Name; // название книги
    bool Pins; // признак свободной для вставки ячейки
    bool Pdel; // признак удаления
};

struct Table {
    int m = 10; // длина таблицы
    int n = 0; // кол-во закрытых адресов
    int v = 0; // кол-во удалённых
    Tbooks* elements = new Tbooks[m]; // пока переменная m не инициализирована, не получится создать дин. массив размером m сама таблица, если переменная "объёмная", лучше делать массив из указателей

};

void add(Tbooks tov, Table& T) {
    FILE* f1;
    int index = hesh(tov.ISBN, T.m);
    while ((!T.elements[index].Pins) and (!T.elements[index].Pdel))
        index++;
    if (index < T.m) {
        T.elements[index] = tov;
        T.n = T.n + 1;
        T.elements[index].Pins = false;
        f1 = fopen("C:\\Users\\user\\source\\repos\\ConsoleApplication27\\pr2.dat", "rb+");
        fseek(f1, index * sizeof(Tbooks), SEEK_SET);
        fwrite(&T.elements[index], sizeof(Tbooks), 1, f1);
        fclose(f1);
    }
}
void vyvod(Table T) {
    int i = 0;
    cout << "Индекс \t" << "Артикул\t" << "Название товара\t" << "Количество товара\t" << "Признак открытого адреса\t" << "Признак удаления\t" << endl;
    for (i = 0; i < T.m; i++)
    {
        if ((T.elements[i].Pins == true) and (T.elements[i].Pdel == false))
            cout << i << "\t" << "-\t" << "-\t" << "-\t" << T.elements[i].Pins << "\t" << T.elements[i].Pdel << "\t" << endl;
        else
            if ((T.elements[i].Pins == true) and (T.elements[i].Pdel == true))
                cout << i << "\t" << T.elements[i].ISBN << "\t" << T.elements[i].Name << "\t" << T.elements[i].Author << "\t" << T.elements[i].Pins << "\t" << T.elements[i].Pdel << "\t" << endl;
            else {
                cout << i << "\t" << T.elements[i].ISBN << "\t" << T.elements[i].Name << "\t" << T.elements[i].Author << "\t" << T.elements[i].Pins << "\t" << T.elements[i].Pdel << "\t" << endl;
            }
    }
}


void findElement(Table T, unsigned long long ISBN)
{
    FILE* f1;
    int i;
    i = hesh(ISBN, T.m);
    while (ISBN != T.elements[i].ISBN) {
        if ((T.elements[i].Pins == true) and (T.elements[i].Pdel == false)) {
            cout << "Товара с таким артикулом никогда не было в хеш-таблице и в файле" << endl;
            break;
        }
        if (i = (T.m - 1)) {
            cout << "Товара с таким артикулом никогда не было в хеш-таблице и в файле" << endl;
            break;
        }
        i++;

    }
    if (ISBN == T.elements[i].ISBN)
    {
        if (T.elements[i].Pdel == true) cout << "Индекс в хеш-таблице был: " << i << ", но данные из ячейки и файла были удалены" << endl;
        if (T.elements[i].Pdel == false) {
            cout << "Индекс в хеш-таблице и в файле: " << i << endl;
            Tbooks g;
            f1 = fopen("C:\\Users\\user\\source\\repos\\ConsoleApplication27\\pr2.dat", "rb");
            fseek(f1, i * sizeof(Tbooks), SEEK_SET);
            fread(&g, sizeof(Tbooks), 1, f1);
            fclose(f1);
            cout << "Ключ товара: " << g.ISBN << endl;
            cout << "Название товара: " << g.Name << endl;
            cout << "Количество товара на складе: " << g.Author << endl;
        }

    }
}
void del(unsigned long long ISBN, Table& T) {
    FILE* f1;
    int i = hesh(ISBN, T.m);
    while (ISBN != T.elements[i].ISBN) {
        if ((T.elements[i].Pins == true) and (T.elements[i].Pdel == false)) {
            cout << "Товара с таким артикулом никогда не было в файле и хеш-таблице" << endl;
            break;
        }
        if (i = (T.m - 1)) {
            cout << "Товара с таким артикулом никогда не было в файле и хеш-таблице" << endl;
            break;
        }
        i++;

    }
    if (ISBN == T.elements[i].ISBN)
    {
        if (T.elements[i].Pdel == true) cout << "Данные по этому артикулу уже были удалены из ячейки" << endl;
        else
        {
            T.elements[i].Pdel = true;
            T.elements[i].Pins = true;
            int k = 0;
            T.n = T.n - 1;
            T.v = T.v + 1;
            f1 = fopen("C:\\Users\\user\\source\\repos\\ConsoleApplication27\\pr2.dat", "wb");
            while (k != T.m) {
                if (k == i) {
                    k = k + 1;
                    continue;
                }
                if (T.elements[k].Pins == true)
                {
                    k = k + 1;
                    continue;
                }
                fseek(f1, k * sizeof(Tbooks), SEEK_SET);
                fwrite(&T.elements[k], sizeof(Tbooks), 1, f1);
                k = k + 1;
            }
            fclose(f1);

        }
    }
}
int main()
{
    setlocale(LC_ALL, "rus");
    Tbooks a;
    Table tab;
    Table& tab1 = tab;
    FILE* f2;
    int e = 1;
    f2 = fopen("C:\\Users\\user\\source\\repos\\ConsoleApplication27\\pr2.dat", "wb");
    fclose(f2);
    while (e == 1)
    {
        cout << "Вводим новый товар? Если да, напишите цифру 1. Если нет - любую другую цифру: ";
        cin >> e;
        cout << endl;
        if (e != 1) break;
        cout << "Введите артикул товара: ";
        cin >> a.ISBN;
        cout << endl;
        cout << "Введите название товара: ";
        cin >> a.Name;
        cout << endl;
        cout << "Введите количество товара: ";
        cin >> a.Author;
        cout << endl;
        add(a, tab1);
    }
    findElement(tab, 2325);
    findElement(tab, 2324);
    del(2324, tab1);
    findElement(tab, 2324);
}