#include <iostream>
#include <string>
#include <conio.h>
#include <windows.h>
#include "login.hpp"
#include "menu.hpp"
#include "skor.hpp"
#include "leaderboard.hpp"
#include "soundtembakan.hpp"
#include "spacebar.hpp"
#include "tembakOtomatis.hpp"

using namespace std;

int main() {
    string userLogin;
    int pilihan;
    Queue pemainQueue;
    Leaderboard leaderboard;

    cout << "=== SELAMAT DATANG DI ROCKET GAMES ===\n";
    cout << "1. Login\n2. Register Akun\nPilih: ";
    cin >> pilihan;

    if (pilihan == 2) {
        registerAkun();
    }

    userLogin = login();
    if (userLogin == "") return 0;

    pemainQueue.enqueue(userLogin.c_str());

    do {
        cout << "\n=== MENU UTAMA ===\n";
        cout << "1. Mulai Permainan\n";
        cout << "2. Lihat Riwayat Pemain\n";
        cout << "3. Lihat Riwayat Skor\n";
        cout << "4. Lihat Leaderboard\n";
        cout << "5. Keluar\n";
        cout << "Pilih: ";
        cin >> pilihan;

        if (pilihan == 1) {
            cout << "\nGame dimulai... Gunakan A/D untuk gerak, Q untuk keluar\n";
            DWORD lastShoot = 0;
            int skor = 0;
            player = {LEBAR / 2, TINGGI - 1};
            while (true) {
                if (_kbhit()) {
                    char ch = _getch();
                    if (ch == 'a' && player.x > 0) player.x--;
                    if (ch == 'd' && player.x < LEBAR - 1) player.x++;
                    if (ch == 'q') break;
                }
                DWORD now = GetTickCount();
                if (now - lastShoot >= 300) {
                    peluruQueue.push({player.x, player.y - 1});
                    playShootSound();
                    lastShoot = now;
                    skor++;
                }
                update();
                render(peluruQueue);
                Sleep(FPS);
            }

            cout << "Permainan selesai. Skor Anda: " << skor << endl;
            simpanSkor(skor);
            leaderboard.addScore(userLogin, skor);

        } else if (pilihan == 2) {
            pemainQueue.tampilkan();

        } else if (pilihan == 3) {
            tampilkanRiwayat();

        } else if (pilihan == 4) {
            leaderboard.showLeaderboard();
            leaderboard.showHistory();

        } else if (pilihan == 5) {
            cout << "\nTerima kasih telah bermain!\n";

        } else {
            cout << "Pilihan tidak valid.\n";
        }

    } while (pilihan != 5);

    return 0;
}
