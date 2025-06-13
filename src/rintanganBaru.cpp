// Rocket Runner - Queue Manual + Boss Zigzag Level 3
#include <iostream>
#include <conio.h>
#include <windows.h>
using namespace std;

const int WIDTH = 40;
const int HEIGHT = 20;
const int MAKS_RINTANGAN = 5;

struct Posisi {
    int x, y;
};

struct AntrianRintangan {
    int top;
    Posisi isi[MAKS_RINTANGAN];
} antrian;

struct BossStar {
    int x, y;
    int hp;
    bool arahNaik;
    bool aktif;
} boss = {WIDTH - 5, HEIGHT / 2, 3, false, false};

Posisi roket = {1, HEIGHT / 2};
bool gameOver = false;
int frame = 0;
int level = 1;

void createQueue() {
    antrian.top = 0;
}

bool isEmpty() {
    return (antrian.top == 0);
}

bool isFull() {
    return (antrian.top >= MAKS_RINTANGAN);
}

void insertQueue(Posisi data) {
    if (!isFull()) {
        antrian.isi[antrian.top] = data;
        antrian.top++;
    }
}

void updateRintangan() {
    for (int i = 0; i < antrian.top; i++) {
        antrian.isi[i].x--;
    }
    if (frame % 15 == 0 && !isFull()) {
        Posisi baru = {WIDTH - 1, rand() % HEIGHT};
        insertQueue(baru);
    }
    while (antrian.top > 0 && antrian.isi[0].x < 0) {
        for (int i = 1; i < antrian.top; i++) {
            antrian.isi[i - 1] = antrian.isi[i];
        }
        antrian.top--;
    }
}

void updateBoss() {
    if (level == 3 && !boss.aktif) {
        boss.aktif = true;
        boss.hp = 3;
        boss.x = WIDTH - 5;
        boss.y = HEIGHT / 2;
    }
    if (boss.aktif) {
        boss.x--;
        boss.y += boss.arahNaik ? -1 : 1;
        if (boss.y <= 0 || boss.y >= HEIGHT - 1) boss.arahNaik = !boss.arahNaik;
    }
}

void render() {
    COORD pos = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);

    char layar[HEIGHT][WIDTH];
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            layar[i][j] = ' ';

    layar[roket.y][roket.x] = 'X';
    for (int i = 0; i < antrian.top; i++) {
        Posisi r = antrian.isi[i];
        if (r.y >= 0 && r.y < HEIGHT && r.x >= 0 && r.x < WIDTH)
            layar[r.y][r.x] = '*';
    }

    if (boss.aktif && boss.x >= 0 && boss.x < WIDTH && boss.y >= 0 && boss.y < HEIGHT)
        layar[boss.y][boss.x] = 'B';

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) cout << layar[i][j];
        cout << endl;
    }
    cout << "Level: " << level;
    if (boss.aktif) cout << "  Boss HP: " << boss.hp;
    cout << endl;
}

void updateInput() {
    if (_kbhit()) {
        char input = _getch();
        if (input == 'w' && roket.y > 0) roket.y--;
        if (input == 's' && roket.y < HEIGHT - 1) roket.y++;
    }
}

void cekTabrakan() {
    for (int i = 0; i < antrian.top; i++) {
        if (antrian.isi[i].x == roket.x && antrian.isi[i].y == roket.y)
            gameOver = true;
    }
    if (boss.aktif && boss.x == roket.x && boss.y == roket.y)
        gameOver = true;
}

void tembakBossOtomatis() {
    if (boss.aktif && boss.x < WIDTH - 10) {
        if (frame % 10 == 0) {
            boss.hp--;
            if (boss.hp <= 0) boss.aktif = false;
        }
    }
}

int main() {
    srand(time(0));
    createQueue();

    while (!gameOver) {
        updateInput();
        updateRintangan();
        updateBoss();
        tembakBossOtomatis();
        cekTabrakan();
        render();
        Sleep(100);
        frame++;
        if (frame % 200 == 0) level++;
    }

    cout << "GAME OVER!\n";
    return 0;
}
