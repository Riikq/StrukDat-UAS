// leaderboard.hpp
#ifndef LEADERBOARD_HPP
#define LEADERBOARD_HPP

#include <iostream>
#include <stack>
#include <string>
using namespace std;

// Node untuk tree (BST)
struct Node {
    string name;
    int score;
    Node* left;
    Node* right;

    Node(string n, int s) : name(n), score(s), left(NULL), right(NULL) {}
};

class Leaderboard {
private:
    Node* root;
    stack<pair<string, int>> scoreHistory; // Stack untuk riwayat skor

    // Insert ke dalam Binary Search Tree (berdasarkan skor)
    Node* insert(Node* node, string name, int score) {
        if (!node) return new Node(name, score);
        if (score < node->score)
            node->left = insert(node->left, name, score);
        else
            node->right = insert(node->right, name, score);
        return node;
    }

    // Tampilkan leaderboard dari skor tertinggi ke terendah (in-order terbalik)
    void printInOrder(Node* node) {
        if (!node) return;
        printInOrder(node->right);
        cout << node->name << " - " << node->score << endl;
        printInOrder(node->left);
    }

public:
    Leaderboard() : root(NULL) {}

    void addScore(const string& name, int score) {
        root = insert(root, name, score);
        scoreHistory.push({name, score});
    }

    void showLeaderboard() {
        cout << "\n=== LEADERBOARD ===\n";
        printInOrder(root);
    }

    void showHistory() {
        cout << "\n=== RIWAYAT SKOR (Stack) ===\n";
        stack<pair<string, int>> temp = scoreHistory;
        while (!temp.empty()) {
            auto [n, s] = temp.top();
            cout << n << " - " << s << endl;
            temp.pop();
        }
    }
};

#endif