#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <queue>
#include <windows.h>
#include <conio.h>
#include <vector>
#include <cstring>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

using namespace std;

// ====== Login System ======
bool akunTerdaftar(const string& username, const string& password) {
    ifstream file("akun.txt");
    string user, pass;
    while (file >> user >> pass)
        if (user == username && pass == password)
            return true;
    return false;
}

bool usernameSudahAda(const string& username) {
    ifstream file("akun.txt");
    string user, pass;
    while (file >> user >> pass)
        if (user == username)
            return true;
    return false;
}

void registerAkun() {
    string username, password;
    cout << "\n=== REGISTRASI AKUN ===\nUsername: ";
    cin >> username;
    if (usernameSudahAda(username)) {
        cout << "Username sudah terdaftar.\n";
        return;
    }
    cout << "Password: ";
    cin >> password;
    ofstream file("akun.txt", ios::app);
    file << username << " " << password << endl;
    cout << "Akun berhasil dibuat!\n";
}

string login() {
    string username, password;
    int attempts = 3;
    cout << "\n=== LOGIN ===\n";
    while (attempts--) {
        cout << "Username: "; cin >> username;
        cout << "Password: "; cin >> password;
        if (akunTerdaftar(username, password)) {
            cout << "\nLogin berhasil!\n";
            return username;
        } else {
            cout << "Salah. Sisa: " << attempts << "\n";
        }
    }
    return "";
}

// ====== Sound Manager ======
void playShootSound() {
    PlaySound(TEXT("sound_effects/Shoot.wav"), NULL, SND_ASYNC | SND_FILENAME);
}
void playHitSound() {
    PlaySound(TEXT("sound_effects/Hit.wav"), NULL, SND_ASYNC | SND_FILENAME);
}
void playExplodeSound() {
    PlaySound(TEXT("sound_effects/Explode.wav"), NULL, SND_ASYNC | SND_FILENAME);
}

// ====== Skor Handling ======
const int MAKS_RIWAYAT = 10;
int riwayatSkor[MAKS_RIWAYAT];
int jumlahRiwayat = 0;

void simpanSkor(int skor) {
    if (jumlahRiwayat < MAKS_RIWAYAT)
        riwayatSkor[jumlahRiwayat++] = skor;
    else {
        for (int i = 1; i < MAKS_RIWAYAT; i++)
            riwayatSkor[i - 1] = riwayatSkor[i];
        riwayatSkor[MAKS_RIWAYAT - 1] = skor;
    }
}

void tampilkanRiwayat() {
    cout << "\n=== Riwayat Skor ===\n";
    for (int i = 0; i < jumlahRiwayat; i++)
        cout << "Permainan ke-" << (i + 1) << ": " << riwayatSkor[i] << endl;
}

// ====== Leaderboard BST ======
struct Node {
    string name;
    int score;
    Node* left, *right;
    Node(string n, int s) : name(n), score(s), left(NULL), right(NULL) {}
};

class Leaderboard {
private:
    Node* root;
    stack<pair<string, int>> scoreHistory;

    Node* insert(Node* node, string name, int score) {
        if (!node) return new Node(name, score);
        if (score < node->score)
            node->left = insert(node->left, name, score);
        else
            node->right = insert(node->right, name, score);
        return node;
    }

    void printInOrder(Node* node) {
        if (!node) return;
        printInOrder(node->right);
        cout << node->name << " - " << node->score << endl;
        printInOrder(node->left);
    }

public:
    Leaderboard() : root(NULL) {}

    void addScore(const string& name, int score) {
        root = insert(root, name, score);
        scoreHistory.push({name, score});
    }

    void showLeaderboard() {
        cout << "\n=== LEADERBOARD ===\n";
        printInOrder(root);
    }

    void showHistory() {
        cout << "\n=== RIWAYAT SKOR (Stack) ===\n";
        stack<pair<string, int>> temp = scoreHistory;
        while (!temp.empty()) {
            auto [n, s] = temp.top();
            cout << n << " - " << s << endl;
            temp.pop();
        }
    }
};

// ====== Permainan Rocket ======
const int LEBAR = 20, TINGGI = 10, FPS = 100;
const char PLAYER_CHAR = 'X', PELURU_CHAR = '|', KOSONG = ' ';

struct Posisi { int x, y; };
Posisi player = {LEBAR / 2, TINGGI - 1};
queue<Posisi> peluruQueue;

void layarBersih() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hOut, {0, 0});
}

void render(queue<Posisi> peluruCopy) {
    vector<vector<char>> layar(TINGGI, vector<char>(LEBAR, KOSONG));
    while (!peluruCopy.empty()) {
        Posisi p = peluruCopy.front(); peluruCopy.pop();
        if (p.y >= 0 && p.y < TINGGI) layar[p.y][p.x] = PELURU_CHAR;
    }
    layar[player.y][player.x] = PLAYER_CHAR;

    layarBersih();
    for (auto& row : layar) {
        for (char c : row) cout << c;
        cout << endl;
    }
}

void update() {
    queue<Posisi> updated;
    while (!peluruQueue.empty()) {
        Posisi p = peluruQueue.front(); peluruQueue.pop();
        p.y--;
        if (p.y >= 0) updated.push(p);
    }
    peluruQueue = updated;
}

// ====== Main Game Loop ======
void mulaiPermainan(const string& username, Leaderboard& lb) {
    int skor = 0;
    DWORD lastShoot = 0;

    while (true) {
        if (_kbhit()) {
            char ch = _getch();
            if (ch == 'a' && player.x > 0) player.x--;
            if (ch == 'd' && player.x < LEBAR - 1) player.x++;
            if (ch == ' ') {
                peluruQueue.push({player.x, player.y - 1});
                playShootSound();
            }
            if (ch == 'q') break;
        }

        DWORD now = GetTickCount();
        if (now - lastShoot >= 500) {
            peluruQueue.push({player.x, player.y - 1});
            lastShoot = now;
            skor++; // Skor bertambah otomatis tiap tembakan
        }

        update();
        render(peluruQueue);
        Sleep(FPS);
    }

    cout << "\nPermainan selesai. Skor Anda: " << skor << "\n";
    simpanSkor(skor);
    lb.addScore(username, skor);
}

// ====== Menu Utama ======
int main() {
    string username;
    Leaderboard leaderboard;
    int menu;

    cout << "=== ROCKET GAME ===\n1. Login\n2. Register\nPilih: ";
    cin >> menu;

    if (menu == 2) registerAkun();
    username = login();
    if (username == "") return 0;

    while (true) {
        cout << "\n1. Main Game\n2. Lihat Riwayat\n3. Leaderboard\n4. Keluar\nPilih: ";
        cin >> menu;
        if (menu == 1) mulaiPermainan(username, leaderboard);
        else if (menu == 2) tampilkanRiwayat();
        else if (menu == 3) leaderboard.showLeaderboard();
        else break;
    }

    cout << "Sampai jumpa!\n";
    return 0;
}
