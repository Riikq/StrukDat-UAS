#include <iostream>
#include <vector>
#include <conio.h>   // For _getch()
#include <windows.h> // For Sleep()
#include <cstdlib>   // For rand() and srand()
#include <ctime>     // For time()
#include <algorithm> // For remove_if

using namespace std;

// Konstanta game
const int LEBAR = 40;
const int TINGGI = 20;
const int FPS = 100; // milliseconds per frame

// Objek visual
const char player = 'X';
const char rintangan = '*';
const char kosong = ' ';

// Status
bool gameOver = false;
int score = 0;
int level = 1; // Tambahan: variabel global level

// Posisi player
struct Posisi {
    int x, y;
};

// Tree untuk menyimpan level
struct TreeNode {
    int level;
    TreeNode* left;
    TreeNode* right;

    TreeNode(int lvl) : level(lvl), left(nullptr), right(nullptr) {}
};

// Deklarasi global untuk tree level
TreeNode* root = new TreeNode(1); // Mulai dari level 1
TreeNode* currentLevelNode = root;

Posisi playerPos;
vector<Posisi> halangan;

// Fungsi membersihkan layar
void layarBersih() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD Posisi = {0, 0};
    SetConsoleCursorPosition(hOut, Posisi);
}

// Efek visual naik level
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

// Fungsi naik level
void naikLevel() {
    if (level < 3) {
        TreeNode* newLevelNode = new TreeNode(level + 1);
        currentLevelNode->left = newLevelNode;
        currentLevelNode = newLevelNode;
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

// Input keyboard
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

// Update game tiap frame
void update() {
    if (score > 0 && score % 200 == 0) {
        naikLevel();
    }

    int peluangMuncul = max(1, 5 - level); // Minimal peluang = 1
    if (rand() % peluangMuncul == 0) {
        Posisi h;
        h.x = rand() % LEBAR;
        h.y = 0;
        halangan.push_back(h);
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

// Render layar
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

// Main
int main() {
    cout << "Rocket Runner" << endl;
    cout << "Gunakan WASD untuk menggerakkan roketmu (X)" << endl;
    cout << "Hindari halangan (*)" << endl;
    cout << "Tekan tombol apapun untuk mulai..." << endl;
    _getch();

    init();

    while (!gameOver) {
        handleInput();
        update();
        render();
        Sleep(FPS);
    }

    cout << "\nGame Over! Skormu: " << score << endl;
    cout << "Tekan tombol apapun untuk keluar..." << endl;
    _getch();

    return 0;
}