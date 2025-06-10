#include <iostream>
using namespace std;

const int width = 20;
const int height = 10;
char screen[height][width];

int x = height / 2; // posisi vertikal roket
int score = 0;
int nyawa = 3;

const int ukuran = 50;

struct QueueStar{
    int top;
    int posisiX[ukuran]; //posisi horizontal
    int posisiY[ukuran]; // posisi vertikal
}bintang;

struct stackPeluru{
    int top;
    int posisiX[ukuran];
    int posisiY[ukuran];
} peluru;

void createStar(){
    bintang.top = 0;
}

int isEmpty(){
    if (bintang.top == 0){
        return 1;
    }
    else{
        return 0;
    }
}

int isFull(){
    if (bintang.top >= ukuran){
        return 1;
    }
    else{
        return 0;
    }
}

void insertBintang(int y) //karena array struct nya itu string
{
    if (isFull()){
        bintang.posisiX[bintang.top] = width - 1; // dari kanan
        bintang.posisiY[bintang.top] = y; // posisi acak/ditentukan
        bintang.top++;
    }
}

void deleteStar(){
    // gerak bintang ke kiri
    for (int i = 0; i < bintang.top; i++) {
        bintang.posisiX[i]--;

        // Cek tabrakan dengan peluru
        for (int j = 0; j < peluru.top; j++) {
            if (bintang.posisiX[i] == peluru.posisiX[j] && bintang.posisiY[i] == peluru.posisiY[j]) {
                score += 10;

                // hapus bintang
                for (int k = i; k < bintang.top - 1; k++) {
                    bintang.posisiX[k] = bintang.posisiX[k + 1];
                    bintang.posisiY[k] = bintang.posisiY[k + 1];
                }
                bintang.top--;

                // hapus peluru
                for (int k = j; k < peluru.top - 1; k++) {
                    peluru.posisiX[k] = peluru.posisiX[k + 1];
                    peluru.posisiY[k] = peluru.posisiY[k + 1];
                }
                peluru.top--;

                break;
            }
        }

        // Mengecek tabrakan dengan roket
        if (bintang.posisiX[i] == 1 && bintang.posisiY[i] == x) {
            nyawa--;
            cout << "Anda menaabrak roket! Nyawa sekarang: " << nyawa << endl;

            for (int j = i; j < bintang.top - 1; j++) {
                bintang.posisiX[j] = bintang.posisiX[j + 1];
                bintang.posisiY[j] = bintang.posisiY[j + 1];
            }
            bintang.top--;
            break;
        }

        // Bintang keluar dari kiri
        if (bintang.posisiX[i] < 0) {
            for (int j = i; j < bintang.top - 1; j++) {
                bintang.posisiX[j] = bintang.posisiX[j + 1];
                bintang.posisiY[j] = bintang.posisiY[j + 1];
            }
            bintang.top--;
            break;
        }
    }
}


void displayStar() {
    system("cls");

    // mengosongkan screen
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            screen[i][j] = ' ';

    // Menampilkan roket
    screen[x][1] = 'R';

    // Menampilkan bintang
    for (int i = 0; i < bintang.top; i++)
        screen[bintang.posisiY[i]][bintang.posisiX[i]] = '*';

    // cetak layar
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++)
            cout << screen[i][j];
        cout << endl;
    }
    cout << "Score: " << score << endl;
}
