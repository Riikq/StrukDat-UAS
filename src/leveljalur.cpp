#include <iostream>
using namespace std;

struct tree{
    string level;
    tree *left, *right;
};

tree *pohon, *root;

void deklarasi(){
    pohon = NULL;
}

void insertLevel(tree **root, string level)
{
    tree *new_node;
    if ((*root) == NULL) //jika kondisi kosong, maka akan membuat node baru
    {
        new_node = new tree;
        new_node->level = level;
        new_node->left = new_node->right = NULL;

        (*root) = new_node;
        (*root)->level = level;
        (*root)->left = (*root)->right = NULL;
    }
    else if ((level < (*root)->level))
    {
        insertLevel(&(*root)->left, level);
    }
    else{
        insertLevel(&(*root)->right, level);
    }
}

//menampilkan secara inorder agar level dari easy terlebih dahulu
void tampilanLevelInOrder(tree *root){
    if (root != NULL){
        tampilanLevelInOrder(root->left);
        cout << (root->level) << " ";
        tampilanLevelInOrder(root->right);
    }
}

// Pilih level berdasarkan input pengguna
void pilihLevel(tree* root) {
    string input;
    tree* current = root;

    while (true) {
        cout << "\nLevel Sekarang: " << current->level << endl;
        if (current->left)
            cout << "Ketik 'left' untuk turun ke level " << current->left->level << endl;
        if (current->right)
            cout << "Ketik 'right' untuk naik ke level " << current->right->level << endl;
        cout << "Ketik 'start' untuk mulai game di level ini" << endl;

        cout << ">> ";
        cin >> input;

        if (input == "left" && current->left) {
            current = current->left;
        } else if (input == "right" && current->right) {
            current = current->right;
        } else if (input == "start") {
            cout << "\nGame dimulai pada level: " << current->level << "!\n";
            break;
        } else {
            cout << "Input tidak valid!\n";
        }
    }
}

// Program utama
int main() {
    tree* root = NULL;

    insertLevel(&root, "Medium");
    insertLevel(&root, "Easy");
    insertLevel(&root, "Hard");

    cout << "📋 Daftar Level Tersedia (Inorder):\n";
    tampilanLevelInOrder(root);

    pilihLevel(root);

    return 0;
}