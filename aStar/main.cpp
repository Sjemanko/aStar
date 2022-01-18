#include <iostream>
#include <fstream>
#include <math.h>
#include <windows.h>

using namespace std;

#define END_POZX 19
#define END_POZY 0

#define START_POZX 0
#define START_POZY 19

#define OBSTACLE 5
struct Node {
    int value;
    int poz_x;
    int poz_y;
    int parent_pozX;
    int parent_pozY;
    int step;
    double weight;
};

// Inicjalizacja listy otwartej i zamknietej
bool openList[20][20] = { false };
bool closedList[20][20] = { false };

// Obliczanie heurystyki od danego miejsca na siatce
double heuristic_calculations(Node n) {
    return sqrt(pow((n.poz_x - END_POZX), 2) + pow((n.poz_y - END_POZY), 2)) + n.step;
}

// Sprawdzenie, czy obecny Node nie ma wartosci przeszkody i czy nie ma go w zamknietej liscie
bool isValid(Node n) {
    return (n.value != OBSTACLE && !closedList[n.poz_y][n.poz_x]) ? true : false;
}

// Ustawienie kolorow
void Color(int color = 15) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Czytanie siatki z pliku, algorytm wyszukiwania
int aStar_alg(){
        fstream grid;
        grid.open("grid.txt", ios::in);
        if (!grid.good()) {
            cout << "Nie mozna otworzyc pliku\n";
        exit(1);
    }

    int p, nextX, nextY;
    double nextWeight;
    Node n[20][20];

    // Inicjalizacja struktur
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            grid >> p;
            n[i][j].poz_x = j;
            n[i][j].poz_y = i;
            n[i][j].value = p;
            n[i][j].parent_pozX = -1;
            n[i][j].parent_pozY = -1;
            n[i][j].weight = -1;
            n[i][j].step = -1;
        }
    }
    grid.close();

    // Przekazanie wartosci o punkcie startowym
    int x = START_POZX; // 0
    int y = START_POZY; // 19

    // Ustawienie punktu startowego, dodanie go do listy zamknietej
    n[y][x].step = 0;
    closedList[y][x] = true;

    // Obliczenia dla heurystyk, sprawdzenie walidacji pozycji Node'a,  wyznaczenie rodzicow oraz sciezki do punktu koncowego
    while (!closedList[END_POZY][END_POZX]) {
        if (x != 0) {
            if (isValid(n[y][x - 1])) {
                if(heuristic_calculations(n[y][x - 1]) < heuristic_calculations(n[y][x])){
                    n[y][x - 1].step = n[y][x].step + 1;
                    n[y][x - 1].weight = heuristic_calculations(n[y][x - 1]);
                    n[y][x - 1].parent_pozX = x;
                    n[y][x - 1].parent_pozY = y;
                    openList[y][x - 1] = true;
                }
            }
        }
        // Badanie zakresow tablicy 20x20
        if (x != 19) {
            if (isValid(n[y][x + 1])) {
                if(heuristic_calculations(n[y][x + 1]) < heuristic_calculations(n[y][x])){
                    n[y][x + 1].step = n[y][x].step + 1;
                    n[y][x + 1].weight = heuristic_calculations(n[y][x + 1]);
                    n[y][x + 1].parent_pozX = x;
                    n[y][x + 1].parent_pozY = y;
                    openList[y][x + 1] = true;
                  }
            }
        }

        if (y != 0) {
            if (isValid(n[y - 1][x])) {
                 if(heuristic_calculations(n[y - 1][x]) < heuristic_calculations(n[y][x])){
                    n[y - 1][x].step = n[y][x].step + 1;
                    n[y - 1][x].weight = heuristic_calculations(n[y - 1][x]);
                    n[y - 1][x].parent_pozX = x;
                    n[y - 1][x].parent_pozY = y;
                    openList[y - 1][x] = true;
                }
            }
        }

        if (y != 19) {
            if (isValid(n[y + 1][x])) {
                if(heuristic_calculations(n[y + 1][x]) < heuristic_calculations(n[y][x])){
                    n[y + 1][x].step = n[y][x].step + 1;
                    n[y + 1][x].weight = heuristic_calculations(n[y + 1][x]);
                    n[y + 1][x].parent_pozX = x;
                    n[y + 1][x].parent_pozY = y;
                    openList[y + 1][x] = true;
                }
            }
        }
        //Ustawienie kolejnych node'ów
        nextX = -1;
        nextY = -1;
        nextWeight = -1;

        for (int i = 0; i < 20; i++) {
            for (int j = 0; j < 20; j++) {
                if (openList[i][j]) {
                    if (nextWeight == -1 || n[i][j].weight <= nextWeight) {
                        nextX = j;
                        nextY = i;
                        nextWeight = n[i][j].weight;
                    }
                }
            }
        }

        if (nextX == -1) {
            cout << "Nie mozna znalezc drogi do celu" << endl;
            exit(2);
        }

        x = nextX;
        y = nextY;
        openList[y][x] = false;
        closedList[y][x] = true;
    }

    x = END_POZX;
    y = END_POZY;

    // Powrot do punktu startowego
    while (!(x == START_POZX && y == START_POZY)) {
        n[y][x].value = 3;
        nextX = n[y][x].parent_pozX;
        nextY = n[y][x].parent_pozY;
        x = nextX;
        y = nextY;
    }
    n[y][x].value = 3;

    // wyœwietlanie trasy
    cout << "Trasa: \n";
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            if (n[i][j].value == 5) {
                Color(4);
                cout << "5 ";
                Color();
            }
            else if (n[i][j].value == 3) {
                Color(18);
                cout << "3 ";
                Color();
            }
            else
                cout << "0 ";
        }
        cout << endl;
    }
}

int main() {
    aStar_alg();
    return 0;
}
