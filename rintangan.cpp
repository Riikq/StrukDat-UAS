#include <iostream>
using namespace std;

const int MAX_BINTANG = 100;
const int SCREEN_WIDTH = 20;
const int SCREEN_HEIGHT = 5;

struct Star {
    int x;
    int y;
    Star* next;
};

struct Queue {
    Star* front;
    Star* rear;
};

// Inisialisasi queue
void initQueue(Queue &q) {
    q.front = q.rear = NULL;
}

// Tambah bintang ke queue
void enqueue(Queue &q, int x, int y) {
    Star* newStar = new Star;
    newStar->x = x;
    newStar->y = y;
    newStar->next = NULL;

    if (q.rear == NULL) {
        q.front = q.rear = newStar;
    } else {
        q.rear->next = newStar;
        q.rear = newStar;
    }
}

// Hapus bintang yang sudah keluar dari layar
void dequeue(Queue &q) {
    if (q.front == NULL) return;

    Star* temp = q.front;
    q.front = q.front->next;

    if (q.front == NULL) q.rear = NULL;

    delete temp;
}

// Update posisi bintang, cek tabrakan
void updateBintang(Queue &q, int &nyawa, int roketY) {
    Star* curr = q.front;
    Star* prev = NULL;

    while (curr != NULL) {
        curr->x--;

        // Cek tabrakan
        if (curr->x == 1 && curr->y == roketY) {
            nyawa--;
            // Hapus bintang yang menabrak
            if (prev == NULL) {
                dequeue(q);
                curr = q.front;
                continue;
            } else {
                prev->next = curr->next;
                delete curr;
                break;
            }
        }

        // Hapus jika sudah keluar dari layar
        if (curr->x < 0) {
            if (prev == NULL) {
                dequeue(q);
                curr = q.front;
                continue;
            } else {
                prev->next = curr->next;
                delete curr;
                break;
            }
        }

        prev = curr;
        curr = curr->next;
    }
}

// Gambar permainan (tanpa library eksternal)
void tampilkanLayar(Queue q, int roketY, int nyawa) {
    char layar[SCREEN_HEIGHT][SCREEN_WIDTH];

    // Kosongkan layar
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            layar[i][j] = ' ';
        }
    }

    // Tempatkan roket
    layar[roketY][1] = '^';

    // Tempatkan bintang
    Star* curr = q.front;
    while (curr != NULL) {
        if (curr->x >= 0 && curr->x < SCREEN_WIDTH && curr->y >= 0 && curr->y < SCREEN_HEIGHT) {
            layar[curr->y][curr->x] = '*';
        }
        curr = curr->next;
    }

    // Tampilkan layar
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            cout << layar[i][j];
        }
        cout << endl;
    }
    cout << "Nyawa: " << nyawa << endl;
}

int main() {
    Queue rintangan;
    initQueue(rintangan);

    int roketY = SCREEN_HEIGHT / 2;
    int nyawa = 3;
    int langkah = 0;

    char input;
    do {
        // Setiap 3 langkah tambahkan bintang baru
        if (langkah % 3 == 0) {
            enqueue(rintangan, SCREEN_WIDTH - 1, rand() % SCREEN_HEIGHT);
        }

        tampilkanLayar(rintangan, roketY, nyawa);

        cout << "Gerak (w: atas, s: bawah, q: keluar): ";
        cin >> input;

        if (input == 'w' && roketY > 0) roketY--;
        else if (input == 's' && roketY < SCREEN_HEIGHT - 1) roketY++;

        updateBintang(rintangan, nyawa, roketY);

        langkah++;

    } while (nyawa > 0 && input != 'q');

    if (nyawa == 0)
        cout << "Game Over! Nyawa habis.\n";
    else
        cout << "Keluar dari permainan.\n";

    return 0;
}
