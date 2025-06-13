// rocket_avoid.cpp - Mode Hindari Tanpa Menembak
#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <algorithm>
using namespace std;

const int WIDTH = 40;
const int HEIGHT = 20;

struct Posisi {
    int x, y;
};

vector<Posisi> rintangan;
Posisi roket = {1, HEIGHT / 2};
bool gameOver = false;
int frame = 0;

void clearScreen() {
    COORD pos = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void input() {
    if (_kbhit()) {
        char c = _getch();
        if (c == 'w' && roket.y > 0) roket.y--;
        else if (c == 's' && roket.y < HEIGHT - 1) roket.y++;
        else if (c == 'a' && roket.x > 0) roket.x--;
        else if (c == 'd' && roket.x < WIDTH - 1) roket.x++;
    }
}

void updateRintangan() {
    if (frame % 15 == 0) rintangan.push_back({WIDTH - 1, rand() % HEIGHT});
    for (auto& r : rintangan) r.x--;
    rintangan.erase(remove_if(rintangan.begin(), rintangan.end(), [](Posisi r){ return r.x <= 0; }), rintangan.end());
}

void cekTabrakan() {
    for (auto& r : rintangan)
        if (r.x == roket.x && r.y == roket.y)
            gameOver = true;
}

void render() {
    clearScreen();
    char layar[HEIGHT][WIDTH];
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            layar[i][j] = ' ';
    layar[roket.y][roket.x] = 'X';
    for (auto& r : rintangan) layar[r.y][r.x] = '*';
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) cout << layar[i][j];
        cout << endl;
    }
    cout << "Mode: Hindari Bintang" << endl;
}

int main() {
    srand(time(0));
    while (!gameOver) {
        input();
        updateRintangan();
        cekTabrakan();
        render();
        Sleep(100);
        frame++;
    }
    cout << "GAME OVER!\n";
    return 0;
}
