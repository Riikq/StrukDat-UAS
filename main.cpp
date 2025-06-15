#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <windows.h>
#include <conio.h>
#include <ctime>
#include <algorithm>

using namespace std;

// Konfigurasi game
const int LEBAR = 50;
const int TINGGI = 25;
const int FPS = 50;
const char PLAYER_CHAR = 'A';
const char PELURU_CHAR = '|';
const char STAR_CHAR = '*';
const char POWERUP_CHAR = 'P';
const char EMPTY = ' ';

// Struktur posisi objek
struct Posisi { int x, y; };

// Fungsi membersihkan layar
void layarBersih() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { 0, 0 };
    SetConsoleCursorPosition(hOut, pos);
}

bool isCollide(Posisi a, Posisi b) {
    return a.x == b.x && a.y == b.y;
}

void simpanScore(const string& username, int score) {
    ofstream file("scoreboard.txt", ios::app);
    file << username << " " << score << endl;
}

void tampilkanLeaderboard() {
    ifstream file("scoreboard.txt");
    vector<pair<string, int>> scores;
    string name; int val;

    while (file >> name >> val) {
        scores.push_back({ name, val });
    }

    sort(scores.begin(), scores.end(), [](auto& a, auto& b) {
        return a.second > b.second;
    });

    cout << "\n=== LEADERBOARD ===\n";
    for (auto& [n, s] : scores)
        cout << n << " - " << s << endl;
    cout << "===================\n";
}

// Mode menembak
void modeMenembak(const string& username) {
    Posisi player = { LEBAR / 2, TINGGI - 1 };
    queue<Posisi> peluruQueue;
    vector<Posisi> stars;
    vector<Posisi> powerUps;
    int skor = 0;
    bool gameOver = false;
    bool powerUpAktif = false;
    DWORD powerUpMulai = 0;
    const int DURASI_POWERUP = 5000;
    DWORD lastStar = 0, lastShoot = 0, lastPower = 0;
    srand((unsigned)time(0));

    while (!gameOver) {
        if (_kbhit()) {
            char ch = _getch();
            if (ch == 'a' && player.x > 0) player.x--;
            if (ch == 'd' && player.x < LEBAR - 1) player.x++;
            if (ch == 'q') break;
        }

        DWORD now = GetTickCount();

        // Tembakan otomatis
        if (now - lastShoot > 300) {
            peluruQueue.push({ player.x, player.y - 1 });
            if (powerUpAktif) {
                if (player.x > 0)
                    peluruQueue.push({ player.x - 1, player.y - 1 });
                if (player.x < LEBAR - 1)
                    peluruQueue.push({ player.x + 1, player.y - 1 });
            }
            lastShoot = now;
        }

        // Spawn bintang
        if (now - lastStar > 100) {
            for (int i = 0; i < 3; i++)
                stars.push_back({ rand() % LEBAR, 0 });
            lastStar = now;
        }

        // Spawn power-up
        if (now - lastPower > 6000) {
            powerUps.push_back({ rand() % LEBAR, 0 });
            lastPower = now;
        }

        // Cek tabrakan peluru vs bintang
        queue<Posisi> sisaPeluru;
        vector<Posisi> sisaStars;

        while (!peluruQueue.empty()) {
            Posisi peluru = peluruQueue.front(); peluruQueue.pop();
            peluru.y--;

            bool kena = false;
            for (auto& star : stars) {
                if (isCollide(peluru, star)) {
                    skor += 10;
                    star.x = -1;
                    kena = true;
                    break;
                }
            }

            if (!kena && peluru.y >= 0)
                sisaPeluru.push(peluru);
        }

        for (auto& s : stars) {
            s.y++;
            if (s.x == -1) continue;
            if (isCollide(s, player)) {
                gameOver = true;
            } else if (s.y < TINGGI) {
                sisaStars.push_back(s);
            }
        }

        // Update power-up
        vector<Posisi> sisaPowerUp;
        for (auto& p : powerUps) {
            p.y++;
            if (isCollide(p, player)) {
                powerUpAktif = true;
                powerUpMulai = now;
            } else if (p.y < TINGGI) {
                sisaPowerUp.push_back(p);
            }
        }

        // Matikan power-up jika sudah habis waktu
        if (powerUpAktif && now - powerUpMulai > DURASI_POWERUP) {
            powerUpAktif = false;
        }

        peluruQueue = sisaPeluru;
        stars = sisaStars;
        powerUps = sisaPowerUp;

        // Render
        vector<vector<char>> layar(TINGGI, vector<char>(LEBAR, EMPTY));
        for (auto& s : stars) layar[s.y][s.x] = STAR_CHAR;
        for (auto& p : powerUps) layar[p.y][p.x] = POWERUP_CHAR;
        queue<Posisi> peluruCopy = peluruQueue;
        while (!peluruCopy.empty()) {
            Posisi p = peluruCopy.front(); peluruCopy.pop();
            layar[p.y][p.x] = PELURU_CHAR;
        }
        layar[player.y][player.x] = PLAYER_CHAR;

        layarBersih();
        for (auto& row : layar) {
            for (char c : row) cout << c;
            cout << '\n';
        }
        cout << "Skor: " << skor;
        if (powerUpAktif) cout << "  [POWER-UP ACTIVE]";
        cout << endl;

        Sleep(FPS);
    }

    simpanScore(username, skor);
    cout << "\nGame Over. Skor Anda: " << skor << "\n";
}

// Mode menghindar
void modeMenghindar(const string& username) {
    Posisi player = { LEBAR / 2, TINGGI - 1 };
    vector<Posisi> stars;
    int skor = 0;
    bool gameOver = false;
    DWORD lastStar = 0;
    srand((unsigned)time(0));

    while (!gameOver) {
        if (_kbhit()) {
            char ch = _getch();
            if (ch == 'a' && player.x > 0) player.x--;
            if (ch == 'd' && player.x < LEBAR - 1) player.x++;
            if (ch == 'q') break;
        }

        DWORD now = GetTickCount();

        if (now - lastStar > 200) {
            stars.push_back({ rand() % LEBAR, 0 });
            lastStar = now;
        }

        vector<Posisi> sisaStars;
        for (auto& s : stars) {
            s.y++;
            if (isCollide(s, player)) {
                gameOver = true;
            } else if (s.y < TINGGI) {
                sisaStars.push_back(s);
            }
        }
        stars = sisaStars;
        skor++;

        // Render
        vector<vector<char>> layar(TINGGI, vector<char>(LEBAR, EMPTY));
        for (auto& s : stars) layar[s.y][s.x] = STAR_CHAR;
        layar[player.y][player.x] = PLAYER_CHAR;

        layarBersih();
        for (auto& row : layar) {
            for (char c : row) cout << c;
            cout << '\n';
        }
        cout << "Survive Time (Skor): " << skor << endl;
        Sleep(FPS);
    }

    simpanScore(username, skor);
    cout << "\nGame Over. Skor Anda: " << skor << "\n";
}

// Sistem login
bool akunTerdaftar(const string& u, const string& p) {
    ifstream f("akun.txt"); string x, y;
    while (f >> x >> y)
        if (x == u && y == p) return true;
    return false;
}

bool usernameSudahAda(const string& u) {
    ifstream f("akun.txt"); string x, y;
    while (f >> x >> y)
        if (x == u) return true;
    return false;
}

void registerAkun() {
    string u, p;
    cout << "\nUsername Baru: "; cin >> u;
    if (usernameSudahAda(u)) {
        cout << "Username sudah digunakan.\n"; return;
    }
    cout << "Password: "; cin >> p;
    ofstream f("akun.txt", ios::app); f << u << " " << p << endl;
    cout << "Akun berhasil dibuat!\n";
}

string login() {
    string u, p; int attempts = 3;
    while (attempts--) {
        cout << "\nUsername: "; cin >> u;
        cout << "Password: "; cin >> p;
        if (akunTerdaftar(u, p)) return u;
        cout << "Salah. Sisa kesempatan: " << attempts << "\n";
    }
    return "";
}

// Main menu
int main() {
    int menu;
    string user;

    cout << "=== ROCKET GAME ===\n1. Login\n2. Register\nPilih: ";
    cin >> menu;
    if (menu == 2) registerAkun();
    user = login();
    if (user == "") return 0;

    while (true) {
        cout << "\n=== MENU GAME ===\n";
        cout << "1. Mode Menembak\n";
        cout << "2. Mode Menghindar\n";
        cout << "3. Lihat Leaderboard\n";
        cout << "4. Keluar\nPilih: ";
        cin >> menu;

        if (menu == 1) modeMenembak(user);
        else if (menu == 2) modeMenghindar(user);
        else if (menu == 3) tampilkanLeaderboard();
        else break;
    }

    return 0;
}
