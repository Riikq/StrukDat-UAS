#include <iostream>
using namespace std;

int gameSelesai = 0;

void gameLoop() {
    while (!gameSelesai && !isGameOver()) {
        // ini desesuain lagi sama nama fungsinya
        draw(); // ini fungsi buat nampilin gamesnya
        updatePeluru(); // ini fungsi gerakan peluru ke kanan
        deleteStar(0); // gerakan bintang ke kiri + deteksi tabrakan

        char input;
        cin >> input;
        gerakRoket(input);

        if (bintang.top == 0) {
            cout << "Semua bintang telah dihancurkan!" << endl;
            gameSelesai = 1;
        }
    }

    cout << "Game Over! Skor kamu: " << score << endl;
    pushSkor(score); // fungsi skor
}

// nanti dia akhir mau ada ini juga boleh di fungsi main
while (true) {
    if (nyawa <= 0) {
        cout << "Kamu kehabisan nyawa!" << endl;
        break; // keluar dari game loop
    } else if (bintang.top == 0) {
        cout << "Semua bintang berhasil dihancurkan!" << endl;
        break; // keluar dari game loop
    }
}

