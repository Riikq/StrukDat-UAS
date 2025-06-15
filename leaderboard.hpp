#ifndef LEADERBOARD_HPP
#define LEADERBOARD_HPP

#include <iostream>
#include <stack>
#include <string>
using namespace std;

struct Node {
    string name;
    int score;
    Node* left;
    Node* right;

    Node(string n, int s);
};

class Leaderboard {
private:
    Node* root;
    stack<pair<string, int>> scoreHistory;
    Node* insert(Node* node, string name, int score);
    void printInOrder(Node* node);

public:
    Leaderboard();
    void addScore(const string& name, int score);
    void showLeaderboard();
    void showHistory();
};

#endif