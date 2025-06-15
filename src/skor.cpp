#include <iostream>
using namespace std;

const int MAKS_RIWAYAT = 10;

int riwayatSkor[MAKS_RIWAYAT];
int jumlahRiwayat = 0;

void simpanSkor(int skor) {
    if (jumlahRiwayat < MAKS_RIWAYAT) {
        riwayatSkor[jumlahRiwayat] = skor;
        jumlahRiwayat++;
    } else {
        // Jika sudah penuh, geser ke kiri dan masukkan yang baru di akhir
        for (int i = 1; i < MAKS_RIWAYAT; i++) {
            riwayatSkor[i - 1] = riwayatSkor[i];
        }
        riwayatSkor[MAKS_RIWAYAT - 1] = skor;
    }
}

void tampilkanRiwayat() {
    cout << "\n=== Riwayat Skor ===\n";
    for (int i = 0; i < jumlahRiwayat; i++) {
        cout << "Permainan ke-" << (i + 1) << ": " << riwayatSkor[i] << endl;
    }
    cout << "====================\n";
}

