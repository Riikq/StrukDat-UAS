#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <queue>
#include <vector>
#include <windows.h>
#include <conio.h>
#include <mmsystem.h>
#include <algorithm>
#include <ctime>
#pragma comment(lib, "winmm.lib")

using namespace std;

// ================= CONFIG =================
const int LEBAR = 50;
const int TINGGI = 25;
const int FPS = 50;
const char PLAYER_CHAR = 'A';
const char PELURU_CHAR = '|';
const char STAR_CHAR = '*';
const char POWERUP_CHAR = 'P';
const char EMPTY = ' ';

// ================ SOUND ===================
void playSound(const string& file) {
    string fullPath = "sound_effects/" + file;
    PlaySoundA(fullPath.c_str(), NULL, SND_ASYNC | SND_FILENAME);
}

void shootSound()   { playSound("Shoot.wav"); }
void explodeSound() { playSound("Explode.wav"); }
void hitSound()     { playSound("Hit.wav"); }
void powerupSound() { playSound("PowerUp.wav"); }

// ================ POSITIONS ===============
struct Posisi { int x, y; };
Posisi player = { LEBAR / 2, TINGGI - 1 };
queue<Posisi> peluruQueue;
vector<Posisi> stars;
vector<Posisi> powerUps;

// ================ GAME STATE ==============
int skor = 0;
bool gameOver = false;
bool powerUpActive = false;
DWORD powerUpStart = 0;
const int POWERUP_DURATION = 5000;

// ================ UTILITY =================
void layarBersih() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hOut, { 0, 0 });
}

bool isCollide(Posisi a, Posisi b) {
    return a.x == b.x && a.y == b.y;
}

void spawnStar() {
    stars.push_back({ rand() % LEBAR, 0 });
}

void spawnPowerUp() {
    powerUps.push_back({ rand() % LEBAR, 0 });
}

// ================ RENDER ==================
void render() {
    vector<vector<char>> layar(TINGGI, vector<char>(LEBAR, EMPTY));

    // Draw stars
    for (auto& s : stars)
        if (s.y >= 0 && s.y < TINGGI)
            layar[s.y][s.x] = STAR_CHAR;

    // Draw powerUps
    for (auto& p : powerUps)
        if (p.y >= 0 && p.y < TINGGI)
            layar[p.y][p.x] = POWERUP_CHAR;

    // Draw bullets
    queue<Posisi> copy = peluruQueue;
    while (!copy.empty()) {
        Posisi p = copy.front(); copy.pop();
        if (p.y >= 0 && p.y < TINGGI)
            layar[p.y][p.x] = PELURU_CHAR;
    }

    // Draw player
    layar[player.y][player.x] = PLAYER_CHAR;

    layarBersih();
    for (int y = 0; y < TINGGI; y++) {
        for (int x = 0; x < LEBAR; x++)
            cout << layar[y][x];
        cout << endl;
    }

    cout << "Score: " << skor;
    if (powerUpActive) cout << "  [POWER-UP ACTIVE]";
    cout << endl;
}

// ================ UPDATE ==================
void updatePeluru() {
    queue<Posisi> updated;
    while (!peluruQueue.empty()) {
        Posisi p = peluruQueue.front(); peluruQueue.pop();
        p.y--;
        if (p.y >= 0) updated.push(p);
    }
    peluruQueue = updated;
}

void updateStars() {
    vector<Posisi> newStars;
    for (auto& s : stars) {
        s.y++;
        if (isCollide(s, player)) {
            explodeSound();
            gameOver = true;
        } else if (s.y < TINGGI) {
            newStars.push_back(s);
        }
    }
    stars = newStars;
}

void updatePowerUps() {
    vector<Posisi> newPowerUps;
    for (auto& p : powerUps) {
        p.y++;
        if (isCollide(p, player)) {
            powerUpActive = true;
            powerUpStart = GetTickCount();
            powerupSound();
        } else if (p.y < TINGGI) {
            newPowerUps.push_back(p);
        }
    }
    powerUps = newPowerUps;
}

void checkBulletCollision() {
    vector<Posisi> newStars;
    while (!peluruQueue.empty()) {
        Posisi bullet = peluruQueue.front(); peluruQueue.pop();
        bool hit = false;
        for (size_t i = 0; i < stars.size(); i++) {
            if (isCollide(bullet, stars[i])) {
                hitSound();
                skor += 10;
                stars.erase(stars.begin() + i);
                hit = true;
                break;
            }
        }
        if (!hit && bullet.y >= 0)
            peluruQueue.push(bullet);
    }
}

// ================ SCORE SYSTEM ============
void simpanScore(const string& username, int score) {
    ofstream file("scoreboard.txt", ios::app);
    file << username << " " << score << endl;
}

void tampilkanLeaderboard() {
    ifstream file("scoreboard.txt");
    vector<pair<string, int>> scores;
    string name; int val;

    while (file >> name >> val)
        scores.push_back({ name, val });

    sort(scores.begin(), scores.end(), [](auto& a, auto& b) {
        return a.second > b.second;
    });

    cout << "\n=== LEADERBOARD ===\n";
    for (auto& [n, s] : scores)
        cout << n << " - " << s << endl;
    cout << "===================\n";
}

// ================ GAME LOOP ==============
void mulaiGame(const string& username) {
    srand(time(0));
    skor = 0;
    gameOver = false;
    peluruQueue = {};
    stars.clear();
    powerUps.clear();
    powerUpActive = false;

    DWORD lastStar = 0, lastPower = 0, lastShoot = 0;

    while (!gameOver) {
        if (_kbhit()) {
            char ch = _getch();
            if (ch == 'a' && player.x > 0) player.x--;
            if (ch == 'd' && player.x < LEBAR - 1) player.x++;
            if (ch == ' ' || ch == 'w') {
                peluruQueue.push({ player.x, player.y - 1 });
                if (powerUpActive) {
                    if (player.x > 0) peluruQueue.push({ player.x - 1, player.y - 1 });
                    if (player.x < LEBAR - 1) peluruQueue.push({ player.x + 1, player.y - 1 });
                }
                shootSound();
            }
            if (ch == 'q') break;
        }

        DWORD now = GetTickCount();
        if (now - lastStar > 700) {
            spawnStar();
            lastStar = now;
        }

        if (now - lastPower > 5000) {
            spawnPowerUp();
            lastPower = now;
        }

        if (powerUpActive && now - powerUpStart > POWERUP_DURATION) {
            powerUpActive = false;
        }

        updatePeluru();
        updateStars();
        updatePowerUps();
        checkBulletCollision();
        render();
        Sleep(FPS);
    }

    simpanScore(username, skor);
    cout << "\nGame Over. Skor Anda: " << skor << "\n";
}

// ================ LOGIN & MENU ============
bool akunTerdaftar(const string& u, const string& p) {
    ifstream f("akun.txt"); string x, y;
    while (f >> x >> y) if (x == u && y == p) return true;
    return false;
}

bool usernameSudahAda(const string& u) {
    ifstream f("akun.txt"); string x, y;
    while (f >> x >> y) if (x == u) return true;
    return false;
}

void registerAkun() {
    string u, p;
    cout << "\nUsername Baru: "; cin >> u;
    if (usernameSudahAda(u)) { cout << "Sudah Ada!\n"; return; }
    cout << "Password: "; cin >> p;
    ofstream f("akun.txt", ios::app); f << u << " " << p << endl;
    cout << "Berhasil!\n";
}

string login() {
    string u, p; int attempts = 3;
    while (attempts--) {
        cout << "\nUsername: "; cin >> u;
        cout << "Password: "; cin >> p;
        if (akunTerdaftar(u, p)) return u;
        cout << "Salah. Sisa: " << attempts << endl;
    }
    return "";
}

// ================ MAIN ====================
int main() {
    int menu;
    string user;

    cout << "=== ROCKET GAME ===\n1. Login\n2. Register\nPilih: ";
    cin >> menu;
    if (menu == 2) registerAkun();
    user = login();
    if (user == "") return 0;

    while (true) {
        cout << "\n1. Main\n2. Leaderboard\n3. Keluar\nPilih: ";
        cin >> menu;
        if (menu == 1) mulaiGame(user);
        else if (menu == 2) tampilkanLeaderboard();
        else break;
    }

    return 0;
}
