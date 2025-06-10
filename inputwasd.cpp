#include <iostream>
using namespace std;

struct position{
    int x,y;
};

// === Konstanta dan Variabel Global ===
const int width = 20;
const int height = 10;
char screen[height][width];
position rocket = {1, height / 2}; // posisi roket

// buat peta kosong & posisi awal roket
void buatScreen() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i == rocket.y && j == rocket.x)
                screen[i][j] = 'R'; // roket
            else
                screen[i][j] = '.';
        }
    }
}

// tampilkan peta
void tampilScreen() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            cout << screen[i][j] << " ";
        }
        cout << endl;
    }
}

void gerakRoket(char input) {
    if (input == 'w' && rocket.y > 0)
        rocket.y--;
    else if (input == 's' && rocket.y < height - 1)
        rocket.y++;
    else if (input == 'a' && rocket.x > 0)
        rocket.x--;
    else if (input == 'd' && rocket.x < width - 1)
        rocket.x++;
}

