#ifndef LOGIN_HPP
#define LOGIN_HPP

#include <string>
using namespace std;

bool akunTerdaftar(const string& username, const string& password);
bool usernameSudahAda(const string& username);
void registerAkun();
string login();

#endif
