#ifndef TEMBAK_OTOMATIS_HPP
#define TEMBAK_OTOMATIS_HPP

#include <queue>
using namespace std;

struct Posisi {
    int x, y;
};

extern const int LEBAR;
extern const int TINGGI;
extern const char PLAYER_CHAR;
extern const char PELURU_CHAR;
extern const char KOSONG;
extern const int FPS;

extern Posisi player;
extern queue<Posisi> peluruQueue;

void layarBersih();
void render(queue<Posisi> peluruCopy);
void update();

#endif
