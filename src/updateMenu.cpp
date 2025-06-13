#include <iostream>
#include <cstring>
#include <cstdlib>
using namespace std;

// Struktur node queue
struct Pemain {
    char nama[50];
    Pemain* next;
};

// Struktur queue
struct Queue {
    Pemain* front;
    Pemain* rear;

    Queue() {
        front = rear = nullptr;
    }

    void enqueue(const char* nama) {
        Pemain* baru = new Pemain;
        strcpy(baru->nama, nama);
        baru->next = nullptr;

        if (rear == nullptr) {
            front = rear = baru;
        } else {
            rear->next = baru;
            rear = baru;
        }
    }

    void tampilkan() {
        cout << "\n=== Riwayat Pemain ===\n";
        if (front == nullptr) {
            cout << "Belum ada pemain.\n";
        } else {
            Pemain* current = front;
            while (current != nullptr) {
                cout << "- " << current->nama << endl;
                current = current->next;
            }
        }
    }
};

void tampilPilihanGame() {
    cout << "\nPilih Mode Permainan:\n";
    cout << "1. Mode Tembak (dengan peluru & boss)\n";
    cout << "2. Mode Hindari (tanpa peluru)\n";
    cout << "Pilihan: ";
}

int main() {
    Queue pemainQueue;
    int pilihan = 0;

    while (pilihan != 3) {
        cout << "\n=== MENU ROCKET GAMES ===\n";
        cout << "1. Mulai Permainan\n"; //kalo pilih ini nanti pemain input nama pemain dan pilih mode permainan (pake peluru atau mode hindar doang)
        cout << "2. Lihat Skor dan Riwayat\n";
        cout << "3. Keluar\n";
        cout << "Pilih: ";
        cin >> pilihan;
        cin.ignore();

        if (pilihan == 1) {
            char nama[50];
            cout << "Masukkan nama pemain: ";
            cin.getline(nama, 50);
            pemainQueue.enqueue(nama);

            int mode;
            tampilPilihanGame();
            cin >> mode;

            if (mode == 1) {
                system("g++ rocket_shooter.cpp -o shooter && shooter");
            } else if (mode == 2) {
                system("g++ rocket_avoid.cpp -o avoid && avoid");
            } else {
                cout << "Mode tidak tersedia.\n";
            }
        }
        else if (pilihan == 2) {
            pemainQueue.tampilkan();
        }
        else if (pilihan == 3) {
            cout << "\nTerima kasih telah bermain.\n";
        }
        else {
            cout << "Pilihan tidak valid!\n";
        }
    }

    return 0;
}
