#include <iostream>
#include <fstream>
#include <string>
using namespace std;

bool akunTerdaftar(const string& username, const string& password) {
    ifstream file("akun.txt");
    string user, pass;

    while (file >> user >> pass) {
        if (user == username && pass == password) {
            return true;
        }
    }

    return false;
}

bool usernameSudahAda(const string& username) {
    ifstream file("akun.txt");
    string user, pass;

    while (file >> user >> pass) {
        if (user == username) {
            return true;
        }
    }

    return false;
}

void registerAkun() {
    string username, password;

    cout << "\n=== REGISTRASI AKUN ===" << endl;
    cout << "Masukkan username baru: ";
    cin >> username;

    if (usernameSudahAda(username)) {
        cout << "Username sudah terdaftar. Silakan coba login.\n" << endl;
        return;
    }

    cout << "Masukkan password: ";
    cin >> password;

    ofstream file("akun.txt", ios::app); // tambah data ke akhir file
    file << username << " " << password << endl;

    cout << "Akun berhasil dibuat!\n" << endl;
}

string login() {
    string username, password;
    int attempts = 3;

    cout << "\n=== LOGIN AKUN ===" << endl;

    while (attempts > 0) {
        cout << "Username: ";
        cin >> username;
        cout << "Password: ";
        cin >> password;

        if (akunTerdaftar(username, password)) {
            cout << "\nLogin berhasil! Selamat datang, " << username << "!\n" << endl;
            return username;
        } else {
            attempts--;
            cout << "Username atau Password salah. Sisa percobaan: " << attempts << "\n" << endl;
        }
    }

    cout << "Login gagal 3 kali. Program keluar.\n" << endl;
    return "";
}

int main() {
    int pilihan;
    string userLogin;

    cout << "=== SELAMAT DATANG DI ROCKET GAMES ===" << endl;
    cout << "1. Login\n2. Register Akun\nPilih: ";
    cin >> pilihan;

    if (pilihan == 2) {
        registerAkun();
    }

    userLogin = login();

    if (userLogin != "") {
        // lanjut ke menu utama game
        cout << "Masuk ke game sebagai: " << userLogin << endl;
        // nanti userLogin bisa disimpan ke leaderboard
    } else {
        return 0;
    }

    return 0;
}
