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

