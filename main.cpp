#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <fstream>
#include <string>

using namespace std;

// Konstanta
const int LEBAR = 40;
const int TINGGI = 20;
const int FPS = 100;

const char PLAYER_CHAR = 'X';
const char RINTANGAN_CHAR = '*';
const char PELURU_CHAR = '|';
const char KOSONG = ' ';

// Status
bool gameOver = false;
int score = 0;
int level = 1;

// Posisi
struct Posisi {
    int x, y;
};

// Level tree
struct TreeNode {
    int level;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int lvl) : level(lvl), left(nullptr), right(nullptr) {}
};

TreeNode* root = new TreeNode(1);
TreeNode* currentLevelNode = root;

Posisi playerPos;
vector<Posisi> halangan;
vector<Posisi> peluru;

// Fungsi membersihkan layar
void layarBersih() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD Pos = {0, 0};
    SetConsoleCursorPosition(hOut, Pos);
}

// Efek naik level
void efekLevelUp(int levelBaru) {
    layarBersih();
    string pesan = "=== LEVEL " + to_string(levelBaru) + " ===";
    string bintang(pesan.length(), '*');

    for (int i = 0; i < 3; i++) {
        layarBersih();
        cout << "\n\n\n";
        cout << string((LEBAR - pesan.length()) / 2, ' ') << bintang << endl;
        cout << string((LEBAR - pesan.length()) / 2, ' ') << pesan << endl;
        cout << string((LEBAR - pesan.length()) / 2, ' ') << bintang << endl;
        Sleep(400);
        layarBersih();
        Sleep(200);
    }
}

// Naik level
void naikLevel() {
    if (level < 3) {
        TreeNode* newNode = new TreeNode(level + 1);
        currentLevelNode->left = newNode;
        currentLevelNode = newNode;
        level++;
        efekLevelUp(level);
    }
}

// Inisialisasi game
void init() {
    srand(static_cast<unsigned int>(time(nullptr)));
    playerPos.x = LEBAR / 2;
    playerPos.y = TINGGI - 2;
    halangan.clear();
    peluru.clear();
    gameOver = false;
    score = 0;
    level = 1;
    root = new TreeNode(1);
    currentLevelNode = root;
}

// Input
void handleInput() {
    if (_kbhit()) {
        char key = _getch();
        if (key >= 'A' && key <= 'Z') key = key - 'A' + 'a';

        switch (key) {
            case 'w':
                if (playerPos.y > 0) playerPos.y--;
                break;
            case 'a':
                if (playerPos.x > 0) playerPos.x--;
                break;
            case 's':
                if (playerPos.y < TINGGI - 1) playerPos.y++;
                break;
            case 'd':
                if (playerPos.x < LEBAR - 1) playerPos.x++;
                break;
            case ' ':
                peluru.push_back({playerPos.x, playerPos.y - 1});
                break;
            case 'q':
                gameOver = true;
                break;
        }
    }
}

// Update status game
void update() {
    if (score > 0 && score % 200 == 0) {
        naikLevel();
    }

    // Tambah halangan
    int peluang = max(1, 5 - level);
    if (rand() % peluang == 0) {
        halangan.push_back({rand() % LEBAR, 0});
    }

    // Update halangan
    for (auto& h : halangan) h.y++;
    halangan.erase(remove_if(halangan.begin(), halangan.end(),
                             [](const Posisi& p) { return p.y >= TINGGI; }),
                   halangan.end());

    // Update peluru (naik ke atas)
    for (auto& p : peluru) p.y--;
    peluru.erase(remove_if(peluru.begin(), peluru.end(),
                           [](const Posisi& p) { return p.y < 0; }),
                 peluru.end());

    // Cek tabrakan peluru dengan halangan
    vector<Posisi> halanganBaru;
    vector<Posisi> peluruBaru;

    for (auto& h : halangan) {
        bool tertembak = false;
        for (auto& p : peluru) {
            if (h.x == p.x && h.y == p.y) {
                tertembak = true;
                score += 5;
                break;
            }
        }
        if (!tertembak)
            halanganBaru.push_back(h);
    }

    for (auto& p : peluru) {
        bool kena = false;
        for (auto& h : halangan) {
            if (h.x == p.x && h.y == p.y) {
                kena = true;
                break;
            }
        }
        if (!kena)
            peluruBaru.push_back(p);
    }

    halangan = halanganBaru;
    peluru = peluruBaru;

    // Cek tabrakan dengan player
    for (const auto& h : halangan) {
        if (h.x == playerPos.x && h.y == playerPos.y) {
            gameOver = true;
            return;
        }
    }

    score++;
}

// Render
void render() {
    vector<vector<char>> buffer(TINGGI, vector<char>(LEBAR, KOSONG));

    for (const auto& h : halangan) {
        if (h.x >= 0 && h.x < LEBAR && h.y >= 0 && h.y < TINGGI)
            buffer[h.y][h.x] = RINTANGAN_CHAR;
    }

    for (const auto& p : peluru) {
        if (p.x >= 0 && p.x < LEBAR && p.y >= 0 && p.y < TINGGI)
            buffer[p.y][p.x] = PELURU_CHAR;
    }

    buffer[playerPos.y][playerPos.x] = PLAYER_CHAR;
    layarBersih();

    cout << "Score: " << score << " | Level: " << level << " | Tekan Q keluar, Spasi tembak" << endl;
    for (int i = 0; i < LEBAR + 2; i++) cout << "#";
    cout << endl;

    for (int y = 0; y < TINGGI; y++) {
        cout << "#";
        for (int x = 0; x < LEBAR; x++) cout << buffer[y][x];
        cout << "#" << endl;
    }

    for (int i = 0; i < LEBAR + 2; i++) cout << "#";
    cout << endl;
}

// Simpan skor ke file
void simpanRiwayatSkor(int skor) {
    ofstream file("riwayat_skor.txt", ios::app);
    if (file.is_open()) {
        time_t now = time(0);
        char* dt = ctime(&now);
        dt[strlen(dt) - 1] = '\0';
        file << dt << " | Skor: " << skor << endl;
        file.close();
    }
}

// Tampilkan riwayat
void tampilkanRiwayatSkor() {
    ifstream file("riwayat_skor.txt");
    string line;
    cout << "\n=== Riwayat Skor ===" << endl;
    if (file.is_open()) {
        while (getline(file, line)) cout << line << endl;
        file.close();
    } else {
        cout << "Belum ada riwayat." << endl;
    }
}

// Main game loop
int main() {
    while (true) {
        cout << "Rocket Runner" << endl;
        cout << "Gunakan WASD untuk gerak | Spasi untuk menembak | Q keluar" << endl;
        tampilkanRiwayatSkor();
        cout << "Tekan tombol apapun untuk mulai..." << endl;
        _getch();

        init();

        while (!gameOver) {
            handleInput();
            update();
            render();
            Sleep(FPS);
        }

        simpanRiwayatSkor(score);
        cout << "\nGame Over! Skormu: " << score << endl;
        cout << "Tekan R untuk main lagi atau lainnya untuk keluar..." << endl;

        char pilihan = _getch();
        if (pilihan != 'r' && pilihan != 'R') break;
        gameOver = false;
    }

    return 0;
}
