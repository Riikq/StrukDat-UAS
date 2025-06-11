#include <iostream>
#include <vector>
#include <conio.h>  // For _getch() function to detect key presses
#include <windows.h> // For Sleep() function
#include <cstdlib>   // For rand() and srand()
#include <ctime>     // For time()
#include <algorithm> // For remove_if

using namespace std;

// Konstan yang dipakai dalam game
const int LEBAR = 40;
const int TINGGI = 20;
const int FPS = 100; // milliseconds between frames

// Obyek game
const char player = 'X';
const char rintangan = '*';
const char kosong = ' ';

// status game
bool gameOver = false;
int score = 0;

// Player position
struct Posisi {
    int x, y;
};

Posisi playerPos;
vector<Posisi> halangan;

// Fungsi untuk membersihkan layar (Hanya bisa di Windows)
void layarBersih() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD Posisi = {0, 0};
    SetConsoleCursorPosition(hOut, Posisi);
}

// Inisialisasi game
void init() {
    // Set random seed
    srand(static_cast<unsigned int>(time(nullptr)));
    
    // player spawn
    playerPos.x = LEBAR / 2;
    playerPos.y = TINGGI - 2;
    
    // menghapus halangan yang ada
    halangan.clear();
    
    // reset status game
    gameOver = false;
    score = 0;
}

// Input WASD
void handleInput() {
    if (_kbhit()) {
        char key = _getch();
        
        // tidak case-sensitive
        if (key >= 'A' && key <= 'Z') {
            key = key - 'A' + 'a';
        }
        
        // WASD
        switch (key) {
            case 'w': // Up
                if (playerPos.y > 0) playerPos.y--;
                break;
            case 'a': // Left
                if (playerPos.x > 0) playerPos.x--;
                break;
            case 's': // Down
                if (playerPos.y < TINGGI - 1) playerPos.y++;
                break;
            case 'd': // Right
                if (playerPos.x < LEBAR - 1) playerPos.x++;
                break;
            case 'q': // Quit
                gameOver = true;
                break;
        }
    }
}

// update status game
void update() {
    // memunculkan halangan random
    if (rand() % 5 == 0) {
        Posisi halanganBaru;
        halanganBaru.x = rand() % LEBAR;
        halanganBaru.y = 0;
        halangan.push_back(halanganBaru);
    }
    
    // halangan turun ke bawah
    for (auto& obs : halangan) {
        obs.y++;
    }
    
    // hapus halangan yang sudah sampai bawah
    halangan.erase(
        remove_if(halangan.begin(), halangan.end(), 
                  [](const Posisi& p) { return p.y >= TINGGI; }),
        halangan.end()
    );
    
    // cek tabrakan
    for (const auto& obs : halangan) {
        if (obs.x == playerPos.x && obs.y == playerPos.y) {
            gameOver = true;
            return;
        }
    }
    
    // tambah score per fps
    score++;
}

// Render game
void render() {
    // buffer
    vector<vector<char>> buffer(TINGGI, vector<char>(LEBAR, kosong));
    
    // buat halangan
    for (const auto& obs : halangan) {
        if (obs.y >= 0 && obs.y < TINGGI && obs.x >= 0 && obs.x < LEBAR) {
            buffer[obs.y][obs.x] = rintangan;
        }
    }
    
    // buat player
    buffer[playerPos.y][playerPos.x] = player;
    
    // bersihkan layar
    layarBersih();
    
    // buat score
    cout << "Score: " << score << " | Press Q to quit" << endl;
    for (int i = 0; i < LEBAR + 2; i++) cout << "#";
    cout << endl;
    
    // buat kotak game
    for (int y = 0; y < TINGGI; y++) {
        cout << "#";
        for (int x = 0; x < LEBAR; x++) {
            cout << buffer[y][x];
        }
        cout << "#" << endl;
    }
    
    // batas bawah
    for (int i = 0; i < LEBAR + 2; i++) cout << "#";
    cout << endl;
}

// loop semuanya
int main() {
    cout << "Rocket Runner" << endl;
    cout << "Gunakkan WASD untuk menggerakan roketmu (X)" << endl;
    cout << "Hindari halangan (*)" << endl;
    cout << "Tekan tombol apapun untuk mulai..." << endl;
    _getch();
    
    init();
    
    // Game loop
    while (!gameOver) {
        handleInput();
        update();
        render();
        Sleep(FPS);
    }
    
    // Pesan game over
    cout << "\nGame Over! Skormu: " << score << endl;
    cout << "Tekan tombol apapun untuk keluar..." << endl;
    _getch();
    
    return 0;
}