#include <iostream>
#include <chrono>
#include <thread>

void bersihkanLayar() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int main() {
    const int lebarLayar = 20; 
    const int posisiRocket = 0; 
    int posisiBintang = lebarLayar - 1; 

    int nyawaRocket = 3;
    while (nyawaRocket > 0) {
        bersihkanLayar();

        std::cout << "Nyawa Roket: " << nyawaRocket << std::endl;

        for (int i = 0; i < lebarLayar; i++) {
            if (i == posisiRocket) {
                std::cout << "A";
            } else if (i == posisiBintang) {
                std::cout << "*";
            } else {
                std::cout << " "; 
            }
        }
        std::cout << std::endl;

        if (posisiBintang == posisiRocket) {
            nyawaRocket--; 
            posisiBintang = lebarLayar - 1; 
        } else {
            posisiBintang--; 
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

    bersihkanLayar();
    std::cout << "Game Over! Nyawa Roket habis T_T." << std::endl;

    return 0;
}
