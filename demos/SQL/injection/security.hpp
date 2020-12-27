#pragma once

#include <string>

using namespace std;

string sha256(const string);
string uuid();
string hash_password(const string pw, string salt="", const int round=10000);
bool check_password(const string password, const string hashedpd_with_salt);
void password_test();
