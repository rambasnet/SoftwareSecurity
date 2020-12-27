#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.
#include <sstream>
#include <openssl/sha.h>
#include <iomanip>
#include <cassert>
#include <iostream>
#include "security.hpp"

using namespace std;

string sha256(const string str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str();
}

// randomanly generate a uuid and return it
string uuid() {
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    stringstream iss;
    iss << uuid;
    return iss.str();
}

string hash_password(const string password, string salt, const int rounds) {
    if (salt == "") 
        salt = uuid(); // randomanly generate salt
    string hashed_password = password;
    for(int i=0; i<rounds; i++) {
        hashed_password = sha256(hashed_password+salt);
    }
    return hashed_password + ":" + salt;
}

// check given plain text password against the hashed password with salt
bool check_password(const string password, const string hashedpd_with_salt) {
    size_t index = hashedpd_with_salt.find(":");
    if (index == string::npos) return false; // not hashed_password:salt format
    string salt = hashedpd_with_salt.substr(index+1);
    string hashed_pw = hash_password(password, salt);
    return (hashed_pw == hashedpd_with_salt) ? true: false;
}

void password_test() {
    string password = "password";
    string hashed_passwrd = hash_password(password);
    assert(check_password(password, hashed_passwrd));
    cerr << "password test 1: OK\n";
}