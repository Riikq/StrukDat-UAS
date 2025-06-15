#include <iostream>
using namespace std;

const int lebar = 20;
const int tinggi = 10;

int roketX = lebar / 2;
int roketY = tinggi - 1;

int peluruX = -1, peluruY = -1;
bool peluruAktif = false;

void tampilkan() {
    system("clear"); 
    for (int y = 0; y < tinggi; y++) {
        for (int x = 0; x < lebar; x++) {
            if (x == roketX && y == roketY)
                cout << 'A';
            else if (peluruAktif && x == peluruX && y == peluruY)
                cout << '|';
            else
                cout << ' ';
        }
        cout << endl;
    }
}

void tembak() {
    if (!peluruAktif) {
        peluruX = roketX;
        peluruY = roketY - 1;
        peluruAktif = true;
    }
}

void updatePeluru() {
    if (peluruAktif) {
        peluruY--;
        if (peluruY < 0) peluruAktif = false;
    }
}

int main() {
    char input;
    while (true) {
        tampilkan();
        cin >> input;
        if (input == ' ') tembak();
        updatePeluru();
    }
    return 0;
}
