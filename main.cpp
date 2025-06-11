#include <iostream>
#include <vector>
#include <conio.h>  // For _getch() function to detect key presses
#include <windows.h> // For Sleep() function
#include <cstdlib>   // For rand() and srand()
#include <ctime>     // For time()
#include <algorithm> // For remove_if

using namespace std;

// Game constants
const int WIDTH = 40;
const int HEIGHT = 20;
const int GAME_SPEED = 100; // milliseconds between frames

// Game objects
const char playerChar = 'X';
const char obstacleChar = '*';
const char emptyChar = ' ';

// Game state
bool gameOver = false;
int score = 0;

// Player position
struct Position {
    int x, y;
};

Position playerPos;
vector<Position> obstacles;

// Function to clear console screen (Windows-specific)
void clearScreen() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD Position = {0, 0};
    SetConsoleCursorPosition(hOut, Position);
}

// Initialize game
void init() {
    // Set random seed
    srand(static_cast<unsigned int>(time(nullptr)));
    
    // Set player initial position
    playerPos.x = WIDTH / 2;
    playerPos.y = HEIGHT - 2;
    
    // Clear any existing obstacles
    obstacles.clear();
    
    // Reset game state
    gameOver = false;
    score = 0;
}

// Handle player input
void handleInput() {
    if (_kbhit()) {
        char key = _getch();
        
        // Convert uppercase to lowercase for consistency
        if (key >= 'A' && key <= 'Z') {
            key = key - 'A' + 'a';
        }
        
        // WASD controls
        switch (key) {
            case 'w': // Up
                if (playerPos.y > 0) playerPos.y--;
                break;
            case 'a': // Left
                if (playerPos.x > 0) playerPos.x--;
                break;
            case 's': // Down
                if (playerPos.y < HEIGHT - 1) playerPos.y++;
                break;
            case 'd': // Right
                if (playerPos.x < WIDTH - 1) playerPos.x++;
                break;
            case 'q': // Quit
                gameOver = true;
                break;
        }
    }
}

// Update game state
void update() {
    // Generate new obstacles at the top randomly
    if (rand() % 5 == 0) { // 20% chance each frame
        Position newObstacle;
        newObstacle.x = rand() % WIDTH;
        newObstacle.y = 0;
        obstacles.push_back(newObstacle);
    }
    
    // Move obstacles down
    for (auto& obs : obstacles) {
        obs.y++;
    }
    
    // Remove obstacles that went off screen
    obstacles.erase(
        remove_if(obstacles.begin(), obstacles.end(), 
                  [](const Position& p) { return p.y >= HEIGHT; }),
        obstacles.end()
    );
    
    // Check for collisions
    for (const auto& obs : obstacles) {
        if (obs.x == playerPos.x && obs.y == playerPos.y) {
            gameOver = true;
            return;
        }
    }
    
    // Increase score
    score++;
}

// Render game
void render() {
    // Create a buffer for the game field
    vector<vector<char>> buffer(HEIGHT, vector<char>(WIDTH, emptyChar));
    
    // Draw obstacles
    for (const auto& obs : obstacles) {
        if (obs.y >= 0 && obs.y < HEIGHT && obs.x >= 0 && obs.x < WIDTH) {
            buffer[obs.y][obs.x] = obstacleChar;
        }
    }
    
    // Draw player
    buffer[playerPos.y][playerPos.x] = playerChar;
    
    // Clear screen
    clearScreen();
    
    // Draw frame
    cout << "Score: " << score << " | Press Q to quit" << endl;
    for (int i = 0; i < WIDTH + 2; i++) cout << "#";
    cout << endl;
    
    // Draw game field
    for (int y = 0; y < HEIGHT; y++) {
        cout << "#";
        for (int x = 0; x < WIDTH; x++) {
            cout << buffer[y][x];
        }
        cout << "#" << endl;
    }
    
    // Draw bottom border
    for (int i = 0; i < WIDTH + 2; i++) cout << "#";
    cout << endl;
}

// Main game loop
int main() {
    cout << "Simple Rocket Game" << endl;
    cout << "Use WASD keys to move your rocket (^)" << endl;
    cout << "Avoid obstacles (*)" << endl;
    cout << "Press any key to start..." << endl;
    _getch();
    
    init();
    
    // Game loop
    while (!gameOver) {
        handleInput();
        update();
        render();
        Sleep(GAME_SPEED);
    }
    
    // Game over message
    cout << "\nGame Over! Your score: " << score << endl;
    cout << "Press any key to exit..." << endl;
    _getch();
    
    return 0;
}