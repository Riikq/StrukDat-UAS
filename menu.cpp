#include <iostream>
#include <cstring>
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

// Program utama
int main() {
    Queue pemainQueue;
    int pilihan = 0;

    while (pilihan != 3) {
        cout << "\n=== MENU ROCKET GAMES ===\n";
        cout << "1. Mulai Permainan\n";
        cout << "2. Lihat Skor dan Riwayat\n";
        cout << "3. Keluar\n";
        cout << "Pilih: ";
        cin >> pilihan;
        cin.ignore();

        if (pilihan == 1) {
            char nama[50];
            cout << "\nGame dimulai...\n";
            cout << "Masukkan nama pemain: ";
            cin.getline(nama, 50);
            pemainQueue.enqueue(nama);
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
