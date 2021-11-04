#include <iostream>

using namespace std;

class IdealTree {
    char data;
    IdealTree* leftTree = nullptr;
    IdealTree* rightTree = nullptr;

public:
    IdealTree* createNode(char c) {
        IdealTree* node = new IdealTree;
        node->data = c;
        return node;
    }

    IdealTree* createTree(IdealTree*& T, int n) {
        int nl, nr, x;
        char c;
        if (n != 0) {
            T = new IdealTree;
            cout << "Введите информационную часть узла (символ)\n";
            cin >> c;
            T = createNode(c);
            nl = n / 2;
            nr = n - nl - 1;
            createTree(T->leftTree, nl);
            createTree(T->rightTree, nr);
        }
        return T;
    }

    bool isEmpty(IdealTree* T) {
        if (T == nullptr) {
            return true;
        }
        else {
            return false;
        }
    }

    void printTreeRightToLeft(IdealTree* T, int level, int l) {
        if (!isEmpty(T)) {
            printTreeRightToLeft(T->rightTree, level + 1, l);
            for (int i = 1; i <= level * l; i++) {
                cout << ' ';
            }
            cout << T->data << endl;
            printTreeRightToLeft(T->leftTree, level + 1, l);
        }
    }

    int findLevel(IdealTree* T, char c, int level) {
        int l = -1;
        if (!isEmpty(T)) {
            if (T->data == c) {
                l = level;
                return l;
            }
            l = findLevel(T->rightTree, c, level + 1);
            if (l != -1) {
                return l;
            }
            l = findLevel(T->leftTree, c, level + 1);
            return l;
        }
        return l;
    }

    int leftTreeCount(IdealTree* T, int level, int count) {
        if (!isEmpty(T)) {
            if('0' <= T->data && T->data <= '9' && level != 0) {
                count++;
            }
            count = leftTreeCount(T->leftTree, level + 1, count);
            if (level != 0) {
                count = leftTreeCount(T->rightTree, level + 1, count);
            }
        }
        return count;
    }

    void printTreeVertical(IdealTree* T, int level, char* c, int* indexLev, int& spaceCount, int* space) {
        if (!isEmpty(T)) {
            printTreeVertical(T->leftTree, level + 1, c, indexLev, spaceCount, space);
            c[(int)pow(2, level) + indexLev[level] - 1] = T->data;
            space[(int)pow(2, level) + indexLev[level] - 1] = spaceCount;
            spaceCount++;
            indexLev[level]++;
            printTreeVertical(T->rightTree, level + 1, c, indexLev, spaceCount, space);
            if (level == 0) {
                int index = 0;
                int step = -1;
                int maxLevel = 0;
                for (int i = 0; i < 20; i++) {
                    if (indexLev[i] == 0) {
                        maxLevel = i - 2;
                        break;
                    }
                }
                for (int i = 0; i <= maxLevel; i++) {
                    for (int j = 1; j <= (int)pow(2, i); j++) {
                        if (c[index] != ' ') {
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

    int countLeaves(IdealTree* T, char c) {
        if (isEmpty(T)) {
            return 0;
        }
        else {
            if (T->leftTree == nullptr && T->rightTree == nullptr && T->data > c) {
                return 1;
            }
            else {
                return (countLeaves(T->leftTree, c) + countLeaves(T->rightTree, c));
            }
        }
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    IdealTree* T = new IdealTree;
    T = nullptr;
    int n, m = 1, l, lev, count, countLeaves, spaceCount;
    char sym, symSr;
    char c[100];
    int space[100];
    int indexLev[20];
    while (m != 0) {
        cout << "Выберете действие:\n1 - создать дерево.\n2 - отобразить дерево на экране, повернув его справа налево.\n3 - определить уровень элемента.\n4 - определить количество цифр в левом поддереве исходного дерева.\n5 - вывести дерево, располагая элементы вертикально.\n6 - подсчитать количество листьев, больших заданного значения.\n0 - завершить работу.\n";
        cin >> m;
        switch (m) {
        case 1:
            cout << "Введите количество элементов\n";
            cin >> n;
            T = T->createTree(T, n);
            break;
        case 2:
            if (T->isEmpty(T)) {
                cout << "Дерево пусто\n";
            }
            else {
                cout << "Введите расстояние между уровнями\n";
                cin >> l;
                T->printTreeRightToLeft(T, 0, l);
            }
            break;
        case 3:
            if (T->isEmpty(T)) {
                cout << "Дерево пусто\n";
            }
            else {
                cout << "Введите искомый элемент\n";
                cin >> sym;
                lev = T->findLevel(T, sym, 0);
                if (lev == -1) {
                    cout << "Элемент отсутствует в дереве\n";
                }
                else {
                    cout << "Элемент располагается на " << lev << " уровне\n";
                }
            }
            break;
        case 4:
            if (T->isEmpty(T)) {
                cout << "Дерево пусто\n";
            }
            else {
                count = T->leftTreeCount(T, 0, 0);
                cout << "В левом поддереве исходного дерева находятся " << count << " цифр\n";
            }
            break;
        case 5:
            if (T->isEmpty(T)) {
                cout << "Дерево пусто\n";
            }
            else {
                for (int i = 0; i < 100; i++) {
                    c[i] = ' ';
                }
                for (int i = 0; i < 100; i++) {
                    space[i] = 0;
                }
                for (int i = 0; i < 20; i++) {
                    indexLev[i] = 0;
                }
                spaceCount = 0;
                T->printTreeVertical(T, 0, c, indexLev, spaceCount, space);
            }
            break;
        case 6:
            if (T->isEmpty(T)) {
                cout << "Дерево пусто\n";
            }
            else {
                cout << "Введите символ для сравнения\n";
                cin >> symSr;
                countLeaves = T->countLeaves(T, symSr);
                cout << "Количество листьев, удовлетворяющих условию: " << countLeaves << endl;
            }
            break;
        }
    }
    cout << "Работа завершена";
}