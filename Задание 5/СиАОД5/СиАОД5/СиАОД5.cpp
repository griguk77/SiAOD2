#include <iostream>
#include <iomanip>

using namespace std;

const int maxV = 17;

class Edge {
public:
    int a = 0;
    int b = 0;
    int weight = 0;

    Edge* newEdge(int a, int b, int weight) {
        Edge* R = new Edge;
        R->a = a;
        R->b = b;
        R->weight = weight;
        return R;
    }
};

class Graph {
public:
    int V = 0;
    int R = 0;
    int** Matr = NULL;

    void GRAPHInit(int V, Graph& G) {
        G.V = V;
        G.Matr = new int* [V];
        for (int i = 0; i < V; i++) {
            G.Matr[i] = new int[V];
        }
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                G.Matr[i][j] = 0;
            }
        }
    }

    void GRAPHInsert(Graph& G, Edge* R) {
        int a = R->a, b = R->b;
        if (G.Matr[a][b] == 0) {
            G.R++;
        }
        G.Matr[a][b] = R->weight;
        G.Matr[b][a] = R->weight;
    }

    bool Deikstra(Graph G, int* ver, int& k, int begin_index, int end_index) {
        int d[maxV];
        int v[maxV];
        int temp, minindex, min;
        for (int i = 0; i < maxV; i++) {
            d[i] = 10000;
            v[i] = 1;
        }
        d[begin_index] = 0;
        do {
            minindex = 10000;
            min = 10000;
            for (int i = 1; i < maxV; i++) {
                if ((v[i] == 1) && (d[i] < min)) {
                    min = d[i];
                    minindex = i;
                }
            }
            if (minindex != 10000) {
                for (int i = 1; i < maxV; i++) {
                    if (G.Matr[minindex][i] > 0) {
                        temp = min + G.Matr[minindex][i];
                        if (temp < d[i]) {
                            d[i] = temp;
                        }
                    }
                }
                v[minindex] = 0;
            }
        } while (minindex < 10000);

        ver[0] = end_index;
        int weight = d[end_index];
        int kk = k;
        while (end_index != begin_index) {
            for (int i = 1; i < maxV; i++) {
                if (G.Matr[i][end_index] != 0) {
                    int temp = weight - G.Matr[i][end_index];
                    if (temp == d[i]) {
                        weight = temp;
                        end_index = i;
                        ver[k] = i;
                        k++;
                    }
                }
            }
            if (kk == k) {
                return false;
            }
            kk = k;
        }
        return true;
    }

    void PrintMatr(Graph G) {
        for (int i = 1; i < G.V; i++) {
            for (int j = 1; j < G.V; j++) {
                cout << setw(3) << G.Matr[i][j];
            }
            cout << '\n';
        }
    }

    void PrintEdges(Graph G) {
        int k = 0;
        int** b = NULL;
        b = new int* [G.V];
        for (int i = 0; i < G.V; i++) {
            b[i] = new int[G.V];
        }
        for (int i = 0; i < G.V; i++) {
            for (int j = 0; j < G.V; j++) {
                b[i][j] = 0;
            }
        }
        for (int i = 1; i < G.V; i++) {
            for (int j = 1; j < G.V; j++) {
                if (G.Matr[i][j] != 0 && b[i][j] != 1) {
                    cout << i << "-" << j << " | " << G.Matr[i][j] << endl;
                    k++;
                    if (k >= G.R) {
                        delete[] b;
                        return;
                    }
                    b[j][i] = 1;
                }
            }
        }
    }

    bool isConnected(Graph G, int index, int* a) {
        bool b = false;
        if (index == 1) {
            for (int i = 1; i < G.V; i++) {
                if (G.Matr[index][i] != 0) {
                    a[i] = 1;
                }
            }
            for (int i = index; i < G.V; i++) {
                if (G.Matr[index][i] == 0 && index != i) {
                    b = isConnected(G, i, a);
                    if (b == true) {
                        a[index] = 1;
                    }
                }
            }
            for (int i = 1; i < G.V; i++) {
                if (a[i] != 1) {
                    for (int j = 1; j < G.V; j++) {
                        if (G.Matr[i][j] != 0) {
                            return false;
                        }
                    }
                }
            }
            return true;
        }
        else {
            for (int i = 1; i < G.V; i++) {
                if (G.Matr[index][i] != 0 && a[i] == 1) {
                    a[index] = 1;
                    return true;
                }
            }
            for (int i = index; i < G.V; i++) {
                if (G.Matr[index][i] != 0 && index != i) {
                    b = isConnected(G, i, a);
                    if (b == true) {
                        a[index] = 1;
                        return true;
                    }
                }
            }
        }
        return false;
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    bool bb;
    int x[maxV], n, a, b, weight;
    Edge* R = nullptr;
    Graph G;
    G.GRAPHInit(maxV, G);
    int ver[maxV];
    int k = 1;
    int begin_index, end_index;

    int m = 1;
    while (m) {
        cout << "Выберете действие:\n1 - создать граф.\n2 - добавить 1 ребро в граф.\n3 - вывести граф.\n4 - вывести таблицу.\n5 - найти кратчайший путь методом Дейкстры.\n6 - проверить граф на связность.\n0 - завершить работу.\n";
        cin >> m;
        switch (m) {
        case 1:
            G.GRAPHInit(maxV, G);
            cout << "Введите количество рёбер\n";
            cin >> n;
            cout << "Введите " << n << " строк в таком порядке: 1 вершина, 2 вершина, вес ребра\n";
            for (int i = 0; i < n; i++) {
                cin >> a; cin >> b; cin >> weight;
                R = R->newEdge(a, b, weight); G.GRAPHInsert(G, R);
            }
            break;
        case 2:
            cout << "Введите строку в таком порядке: 1 вершина, 2 вершина, вес ребра\n";
                cin >> a; cin >> b; cin >> weight;
                R = R->newEdge(a, b, weight); G.GRAPHInsert(G, R);
            break;
        case 3:
            G.PrintEdges(G);
            break;
        case 4:
            G.PrintMatr(G);
            break;
        case 5:
            for (int i = 0; i < maxV; i++) {
                ver[i] = 0;
            }
            k = 1;
            cout << "Введите в строчку 1 и 2 вершины\n";
            cin >> begin_index; cin >> end_index;
            bb = G.Deikstra(G, ver, k, begin_index, end_index);
            if (bb) {
                cout << "Вывод кратчайшего пути:\n";
                for (int i = k - 1; i >= 0; i--) {
                    cout << setw(3) << ver[i];
                }
                cout << endl;
            }
            else {
                cout << "Нет пути\n";
            }
            break;
        case 6:
            for (int i = 0; i < maxV; i++) {
                x[i] = 0;
            }
            x[1] = 1;
            bb = G.isConnected(G, 1, x);
            if (bb) {
                cout << "Граф связный\n";
            }
            else {
                cout << "Граф несвязный\n";
            }
            break;
        }
    }
    cout << "Работа завершена\n";
}