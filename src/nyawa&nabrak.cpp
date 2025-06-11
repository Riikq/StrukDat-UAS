#include <iostream>
#include <queue>
#include <stack>
#include <cstdlib>
#include <ctime>

using namespace std;

struct Bintang {
    int position;
};

int main() {
    queue<Bintang> bintangQueue;
    stack<int> skorHistory;

    const int maxPos = 10;
    int roketPos = 0;
    int nyawa = 3;
    int skor = 0;
    int jumlahBintang = 5;

    srand(time(0));

    cout << "== GAME ROCKET vs BINTANG ==\n";
    cout << "Kontrol: [w] atas, [s] bawah, [a] kiri, [d] kanan, [spasi = 0] tembak\n\n";

    // Inisialisasi bintang
    for (int i = 0; i < jumlahBintang; ++i) {
        bintangQueue.push({maxPos - i * 2});
    }

    while (nyawa > 0 && !bintangQueue.empty()) {
        // Tampilkan status
        cout << "\nRoket di posisi: " << roketPos << " | Nyawa: " << nyawa << " | Skor: " << skor << "\n";
        cout << "Bintang di posisi: ";
        queue<Bintang> temp = bintangQueue;
        while (!temp.empty()) {
            cout << temp.front().position << " ";
            temp.pop();
        }
        cout << "\n\nAksi: ";
        
        string aksi;
        cin >> aksi;

        if (aksi == "a") roketPos = max(0, roketPos - 1);
        else if (aksi == "d") roketPos = min(maxPos, roketPos + 1);
        else if (aksi == "0") {
            // Tembak: hancurkan bintang paling depan jika dekat
            if (!bintangQueue.empty() && abs(bintangQueue.front().position - roketPos) <= 1) {
                cout << ">> Bintang dihancurkan!\n";
                bintangQueue.pop();
                skor += 10;
            } else {
                cout << ">> Tembakan meleset.\n";
            }
        }

        // Geser bintang ke kiri
        int size = bintangQueue.size();
        for (int i = 0; i < size; ++i) {
            Bintang b = bintangQueue.front(); bintangQueue.pop();
            b.position--;

            if (b.position == roketPos) {
                cout << ">> Tabrakan! Nyawa berkurang.\n";
                nyawa--;
                if (nyawa == 0) break;
                continue; // bintang tidak dimasukkan kembali
            }

            if (b.position > 0) bintangQueue.push(b);
        }
    }

    cout << "\n== GAME OVER ==\n";
    if (nyawa == 0) cout << "Roket kehabisan nyawa!\n";
    else if (bintangQueue.empty()) cout << "Semua bintang berhasil dihancurkan!\n";

    cout << "Skor akhir: " << skor << "\n";

    // Simpan skor ke histori
    skorHistory.push(skor);

    // Tampilkan histori
    cout << "\nRiwayat Skor:\n";
    while (!skorHistory.empty()) {
        cout << skorHistory.top() << "\n";
        skorHistory.pop();
    }

    return 0;
}