#include <iostream>
#include <queue>
#include <windows.h>
#include <conio.h>

using namespace std;

const int LEBAR = 20;
const int TINGGI = 10;
const char PLAYER_CHAR = 'X';
const char PELURU_CHAR = '|';
const char KOSONG = ' ';
const int FPS = 100;

struct Posisi {
    int x, y;
};

Posisi player = {LEBAR / 2, TINGGI - 1};
queue<Posisi> peluruQueue;

void layarBersih() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD Pos = {0, 0};
    SetConsoleCursorPosition(hOut, Pos);
}

void render(queue<Posisi> peluruCopy) {
    vector<vector<char>> layar(TINGGI, vector<char>(LEBAR, KOSONG));

    while (!peluruCopy.empty()) {
        Posisi p = peluruCopy.front();
        peluruCopy.pop();
        if (p.y >= 0 && p.y < TINGGI)
            layar[p.y][p.x] = PELURU_CHAR;
    }

    layar[player.y][player.x] = PLAYER_CHAR;

    layarBersih();
    for (int y = 0; y < TINGGI; y++) {
        for (int x = 0; x < LEBAR; x++) {
            cout << layar[y][x];
        }
        cout << endl;
    }
}

void update() {
    int size = peluruQueue.size();
    queue<Posisi> updatedPeluru;

    for (int i = 0; i < size; i++) {
        Posisi p = peluruQueue.front();
        peluruQueue.pop();
        p.y--;

        if (p.y >= 0) {
            updatedPeluru.push(p);  // Masih dalam layar
        }
    }

    peluruQueue = updatedPeluru;
}

int main() {
    DWORD lastShoot = 0;

    while (true) {
        if (_kbhit()) {
            char ch = _getch();
            if (ch == 'a' && player.x > 0) player.x--;
            if (ch == 'd' && player.x < LEBAR - 1) player.x++;
            if (ch == 'q') break;
        }

        // Tembakan otomatis setiap 300ms
        DWORD now = GetTickCount();
        if (now - lastShoot >= 300) {
            peluruQueue.push({player.x, player.y - 1});
            lastShoot = now;
        }

        update();
        render(peluruQueue);
        Sleep(FPS);
    }

    return 0;
}
