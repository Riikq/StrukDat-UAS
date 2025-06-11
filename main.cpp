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

// Konstanta game
const int LEBAR = 40;
const int TINGGI = 20;
const int FPS = 100;

// Objek game
const char player = 'X';
const char rintangan = '*';
const char kosong = ' ';

// Status
bool gameOver = false;
int score = 0;
int level = 1;

// Posisi pemain
struct Posisi {
    int x, y;
};

// Node untuk level
struct TreeNode {
    int level;
    TreeNode* left;
    TreeNode* right;

    TreeNode(int lvl) : level(lvl), left(nullptr), right(nullptr) {}
};

// Tree level
TreeNode* root = new TreeNode(1);
TreeNode* currentLevelNode = root;

Posisi playerPos;
vector<Posisi> halangan;

// Fungsi membersihkan layar
void layarBersih() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD Posisi = {0, 0};
    SetConsoleCursorPosition(hOut, Posisi);
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
    gameOver = false;
    score = 0;
    level = 1;

    root = new TreeNode(1);
    currentLevelNode = root;
}

// Input WASD
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
            case 'q':
                gameOver = true;
                break;
        }
    }
}

// Update game
void update() {
    if (score > 0 && score % 200 == 0) {
        naikLevel();
    }

    int peluang = max(1, 5 - level);
    if (rand() % peluang == 0) {
        Posisi obs;
        obs.x = rand() % LEBAR;
        obs.y = 0;
        halangan.push_back(obs);
    }

    for (auto& h : halangan) h.y++;

    halangan.erase(
        remove_if(halangan.begin(), halangan.end(),
            [](const Posisi& p) { return p.y >= TINGGI; }),
        halangan.end()
    );

    for (const auto& h : halangan) {
        if (h.x == playerPos.x && h.y == playerPos.y) {
            gameOver = true;
            return;
        }
    }

    score++;
}

// Tampilkan ke layar
void render() {
    vector<vector<char>> buffer(TINGGI, vector<char>(LEBAR, kosong));

    for (const auto& h : halangan) {
        if (h.y >= 0 && h.y < TINGGI && h.x >= 0 && h.x < LEBAR) {
            buffer[h.y][h.x] = rintangan;
        }
    }

    buffer[playerPos.y][playerPos.x] = player;
    layarBersih();

    cout << "Score: " << score << " | Level: " << level << " | Tekan Q untuk keluar..." << endl;
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
        dt[strlen(dt) - 1] = '\0'; // hilangkan newline
        file << dt << " | Skor: " << skor << endl;
        file.close();
    }
}

// Tampilkan skor sebelumnya
void tampilkanRiwayatSkor() {
    ifstream file("riwayat_skor.txt");
    string line;
    cout << "\n=== Riwayat Skor Sebelumnya ===" << endl;
    if (file.is_open()) {
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    } else {
        cout << "Belum ada riwayat skor." << endl;
    }
}

// Main
int main() {
    while (true) {
        cout << "Rocket Runner" << endl;
        cout << "Gunakan WASD untuk menggerakkan roketmu (X)" << endl;
        cout << "Hindari halangan (*)" << endl;
        cout << "Tekan tombol apapun untuk mulai..." << endl;
        tampilkanRiwayatSkor();
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
        cout << "Tekan R untuk main lagi atau tombol lain untuk keluar..." << endl;

        char pilihan = _getch();
        if (pilihan != 'r' && pilihan != 'R') break;

        gameOver = false;
    }

    return 0;
}
