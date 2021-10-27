#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <ctime>

using namespace std;

int toInt(string s) {
    char c[6];
    c[0] = s[s.length() - 5];
    c[1] = s[s.length() - 4];
    c[2] = s[s.length() - 3];
    c[3] = s[s.length() - 2];
    c[4] = s[s.length() - 1];
    int res = atoi(c);
    return res;
}

struct Book {
    string ISBN; // ISBN
    string author; // автор книги
    string name; // название книги
    bool pins = true; // признак свободной ячейки
    bool pdel = false; // признак удалённой ячейки
};

class Table {
public:
    int m = 5; // размер таблицы
    int n = 0; // кол-во закрытых адресов
    int d = 0; // кол-во удалённых элементов
    Book* elements = new Book[m];

    int hesh(string ISBN, int m) {
        int a = toInt(ISBN);
        return a % m;
    }

    int hesh2(string ISBN, int m) {
        int a = toInt(ISBN);
        return a / m;
    }

    void rehesh(Book*& elements) {
        cout << "Производится рехеширование\n";
        FILE* f1;
        f1 = fopen("pr2.dat", "wb");
        fclose(f1);
        int k = 0;
        Book* newElements = new Book[m];
        while (k < m) {
            if (elements[k].pdel == true) {
                k++;
                continue;
            }
            if (elements[k].pins == true) {
                k++;
                continue;
            }
            int index = hesh(elements[k].ISBN, m);
            int step = hesh2(elements[k].ISBN, m);
            while ((!newElements[index].pins) and (!newElements[index].pdel)) {
                if (step % m == 0) {
                    index++;
                    if (index == m) {
                        index = 0;
                    }
                }
                else {
                    index = (index + step) % m;
                }
            }
            if (index < m) {
                newElements[index] = elements[k];
                newElements[index].pins = false;
                f1 = fopen("pr2.dat", "rb+");
                fseek(f1, index * sizeof(Book), SEEK_SET);
                fwrite(&newElements[index], sizeof(Book), 1, f1);
                fclose(f1);
            }
            k++;
        }
        delete[] elements;
        elements = newElements;
        d = 0;
    }

    void add(Book book, Table& T) {
        FILE* f1;
        if ((T.n + T.d > T.m * 0.75) and (T.d > 0)) {
            T.rehesh(T.elements);
        }
        int index = hesh(book.ISBN, T.m);
        int step = hesh2(book.ISBN, T.m);
        while ((!T.elements[index].pins) and (!T.elements[index].pdel)) {
            if (step % T.m == 0) {
                index++;
                if (index == T.m) {
                    index = 0;
                }
            }
            else {
                index = (index + step) % T.m;
            }
        }
        if (index < T.m) {
            T.elements[index] = book;
            T.n++;
            T.elements[index].pins = false;
            f1 = fopen("pr2.dat", "rb+");
            fseek(f1, index * sizeof(Book), SEEK_SET); // смещение начинается с позиции SEEK_SET (начало файла) на index * sizeof(Book) байт
            fwrite(&T.elements[index], sizeof(Book), 1, f1); // записывает 1 запись размером sizeof(Book) в файл f1 
            fclose(f1);
        }
        else {
            cout << "Не удалось вставить элемент в хеш-таблицу\n";
        }
    }

    void printTable(Table T) {
        cout << "Индекс \t" << "ISBN\t" << "Название книги\t" << "Автор книги\t" << "Признак открытого адреса\t" << "Признак удаления\t" << endl;
        for (int i = 0; i < T.m; i++) {
            if ((T.elements[i].pins == true) and (T.elements[i].pdel == false)) {
                cout << i << "\t" << "-\t" << "-\t" << "-\t" << T.elements[i].pins << "\t" << T.elements[i].pdel << "\t" << endl;
            }
            else {
                cout << i << "\t" << T.elements[i].ISBN << "\t" << T.elements[i].name << "\t" << T.elements[i].author << "\t" << T.elements[i].pins << "\t" << T.elements[i].pdel << "\t" << endl;
            }
        }
    }

    int search(Table& T, string ISBN) {
        FILE* f1;
        int index = hesh(ISBN, T.m);
        int firstIndex = index;
        int step = hesh2(ISBN, m);
        while (ISBN != T.elements[index].ISBN) {
            if ((T.elements[index].pins == true) and (T.elements[index].pdel == false)) {
                cout << "Данная книга отсутствует\n";
                return -1;
                break;
            }
            if (step % T.m == 0) {
                index++;
                if (index == T.m) {
                    index = 0;
                }
            }
            else {
                index = (index + step) % T.m;
            }
            if (index == firstIndex) {
                cout << "Данная книга отсутствует\n";
                return -1;
                break;
            }
        }
        if (ISBN == T.elements[index].ISBN)
        {
            if (T.elements[index].pdel == true) cout << "Элемент удалён\n";
            if (T.elements[index].pdel == false) {
                cout << "Индекс: " << index << endl;
                f1 = fopen("pr2.dat", "rb+");
                fseek(f1, index * sizeof(Book), SEEK_SET);
                fread(&T.elements[index], sizeof(Book), 1, f1);
                fclose(f1);
                cout << "ISBN: " << T.elements[index].ISBN << endl;
                cout << "Название книги: " << T.elements[index].name << endl;
                cout << "Автор книги: " << T.elements[index].author << endl;
                return index;
            }
        }
    }

    void del(string ISBN, Table& T) {
        FILE* f1;
        int index = hesh(ISBN, T.m);
        int firstIndex = index;
        int step = hesh2(ISBN, T.m);
        while (ISBN != T.elements[index].ISBN) {
            if ((T.elements[index].pins == true) and (T.elements[index].pdel == false)) {
                cout << "Данная книга отсутствует\n";
                break;
            }
            if (step % T.m == 0) {
                index++;
                if (index == T.m) {
                    index = 0;
                }
            }
            else {
                index = (index + step) % T.m;
            }
            if (index == firstIndex) {
                cout << "Данная книга отсутствует\n";
                break;
            }
        }
        if (ISBN == T.elements[index].ISBN) {
            if (T.elements[index].pdel == true) {
                cout << "Элемент удалён\n";
            }
            else {
                T.elements[index].pdel = true;
                T.elements[index].pins = true;
                int k = 0;
                T.n--;
                T.d++;
                f1 = fopen("pr2.dat", "wb");
                fclose(f1);
                while (k != T.m) {
                    if (k == index) {
                        k++;
                        continue;
                    }
                    if (T.elements[k].pins == true) {
                        k++;
                        continue;
                    }
                    f1 = fopen("pr2.dat", "rb+");
                    fseek(f1, k * sizeof(Book), SEEK_SET);
                    fwrite(&T.elements[k], sizeof(Book), 1, f1);
                    fclose(f1);
                    k++;
                }
            }
        }
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    Book a;
    Table tab;
    string ISBN;
    FILE* f2;
    int e = 1;
    int i;
    f2 = fopen("pr2.dat", "wb");
    fclose(f2);
    while (e != -1) {
        cout << "Для завершения введите -1. Для добавления элемента введите 1. Для вывода таблицы введите 2. Для поиска элемента введите 3. Для удаления элемента введите 4.\n";
        cin >> e;
        switch (e) {
        case 1:
            if (tab.n == tab.m) {
                cout << "В таблице нет свободного места для записи. Для записи придётся освободить место (удалить элемент).\n";
            }
            else {
                cout << "ISBN: ";
                cin >> a.ISBN;
                cout << "Название книги: ";
                cin >> a.name;
                cout << "Автор книги: ";
                cin >> a.author;
                tab.add(a, tab);
            }
            break;
        case 2:
            tab.printTable(tab);
            break;
        case 3:
            cout << "Введите искомый ISBN\n";
            cin >> ISBN;
            i = tab.search(tab, ISBN);
            break;
        case 4:
            cout << "Введите удаляемый ISBN\n";
            cin >> ISBN;
            tab.del(ISBN, tab);
            break;
        }
    }
    ISBN = tab.elements[0].ISBN;
    clock_t start1 = clock();
    i = tab.search(tab, ISBN);
    clock_t end1 = clock();
    cout << endl << "Поиск нулевого элемента: " << ((double)end1 - start1) / ((double)CLOCKS_PER_SEC) << endl << endl;
    ISBN = tab.elements[tab.m / 2].ISBN;
    clock_t start2 = clock();
    i = tab.search(tab, ISBN);
    clock_t end2 = clock();
    cout << "Поиск среднего элемента: " << ((double)end2 - start2) / ((double)CLOCKS_PER_SEC) << endl << endl;
    ISBN = tab.elements[tab.m - 1].ISBN;
    clock_t start3 = clock();
    i = tab.search(tab, ISBN);
    clock_t end3 = clock();
    cout << "Поиск последнегого элемента: " << ((double)end3 - start3) / ((double)CLOCKS_PER_SEC) << endl << endl;
    cout << "Программа завершена\n";
}