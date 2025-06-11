#include <iostream>
#include <conio.h>      // for _kbhit() and _getch()
#include <windows.h>    // for Sleep()
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

const int width = 40;
const int height = 20;
const int maxStars = 10;

struct Entity {
    int x, y;
    Entity(int _x, int _y) : x(_x), y(_y) {}
};

vector<Entity> stars;
vector<Entity> bullets;
Entity rocket(5, height / 2);
bool gameOver = false;
int score = 0;
time_t startTime;

void draw() {
    system("cls");
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i == rocket.y && j == rocket.x)
                cout << 'R';
            else {
                bool printed = false;
                for (auto& s : stars)
                    if (s.x == j && s.y == i) {
                        cout << '*';
                        printed = true;
                        break;
                    }
                for (auto& b : bullets)
                    if (b.x == j && b.y == i) {
                        cout << '|';
                        printed = true;
                        break;
                    }
                if (!printed)
                    cout << ' ';
            }
        }
        cout << endl;
    }
    cout << "Score: " << score << endl;
    cout << "Stars left: " << stars.size() << endl;
}

void input() {
    if (_kbhit()) {
        char ch = _getch();
        switch (ch) {
        case 'w': if (rocket.y > 0) rocket.y--; break;
        case 's': if (rocket.y < height - 1) rocket.y++; break;
        case 'a': if (rocket.x > 0) rocket.x--; break;
        case 'd': if (rocket.x < width - 1) rocket.x++; break;
        case ' ': bullets.push_back(Entity(rocket.x + 1, rocket.y)); break;
        }
    }
}

void logic() {
    // Move bullets
    for (int i = 0; i < bullets.size(); i++) {
        bullets[i].x++;
        if (bullets[i].x >= width) {
            bullets.erase(bullets.begin() + i);
            i--;
        }
    }

    // Move stars
    for (int i = 0; i < stars.size(); i++) {
        stars[i].x--;
        if (stars[i].x == rocket.x && stars[i].y == rocket.y) {
            gameOver = true;
            return;
        }
        if (stars[i].x < 0) {
            stars.erase(stars.begin() + i);
            i--;
        }
    }

    // Bullet-Star collision
    for (int i = 0; i < bullets.size(); i++) {
        for (int j = 0; j < stars.size(); j++) {
            if (bullets[i].x == stars[j].x && bullets[i].y == stars[j].y) {
                bullets.erase(bullets.begin() + i);
                stars.erase(stars.begin() + j);
                score += 10;
                i = -1;
                break;
            }
        }
    }

    // Time-based score
    score = (int)(difftime(time(0), startTime)) * 2 + score;

    if (stars.empty())
        gameOver = true;
}

void initStars() {
    srand(time(0));
    for (int i = 0; i < maxStars; i++) {
        int x = rand() % (width / 2) + width / 2;
        int y = rand() % height;
        stars.push_back(Entity(x, y));
    }
}

int main() {
    startTime = time(0);
    initStars();
    while (!gameOver) {
        draw();
        input();
        logic();
        Sleep(100);
    }
    system("cls");
    cout << "Game Over! Final Score: " << score << endl;
    return 0;
}
