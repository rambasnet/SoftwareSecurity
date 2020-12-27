#include <cstdio>
#include <sqlite3.h>
#include <string>
#include <iostream>
#include <tuple>
#include <vector>
#include <cstring>

#include "security.hpp"

using namespace std;

// prototypes
void execute_sql(sqlite3 *, string, int (* )(void*, int, char**, char**));
void setup_db(sqlite3 *);
bool register_player(sqlite3 *, User &);
void update_player_data(sqlite3 *, User &);
char *mgets(char* src);
void change_username();

static int callback(void* data, int colCount, char**row, char** colName) {
    vector<string> record;
    for(int i=0; i<colCount; i++) {
        record.push_back(row[i]);
    }
    ((vector< vector<string> > *)data)->push_back(record);
    return 0;
}

static int callback_dummy(void * data, int colCount, char *row[], char *colName[]) {
    return 0;
}

bool execute_sql(sqlite3 *db, string sql, int (* callback)(void*, int, char**, char**), void * data) {
    char *errMsg;
    if (sqlite3_exec(db, sql.c_str(), callback, data, &errMsg) != SQLITE_OK) {
        cerr << "SQL error: " << errMsg << "...\n";
        return false;
    }
    return true;
}

void setup_db(sqlite3 *db) {
    string sql = "CREATE TABLE IF NOT EXISTS Account ( \
                uid TEXT, \
                username TEXT, \
                password TEXT, \
                PRIMARY KEY (uid) \
                );";
    execute_sql(db, sql, callback, 0);

    sql = "CREATE TABLE IF NOT EXISTS Player ("
                "uid TEXT, "
                "credits INTEGER, "
                "name TEXT, "
                "PRIMARY KEY (uid) "
                ");";
    execute_sql(db, sql, callback, 0);
    printf("Database setup complete...\n");
}

bool authenticate(sqlite3 *db, User &player) {
    string username, password, sql, uid;
    cout << "Username: ";
    getline(cin, username);
    cout << "Password: ";
    getline(cin, password);
    sql = "SELECT uid from Account WHERE username = '" + username 
            + "' and password = '" + password + "';";
    // define lambda function with captures
    // cout << sql << endl;
    vector<vector<string> > data;
    char * errMsg;
    static auto get_uid = [](void* data, int colCount, char** row, char** colName) {
        for(int i=0; i<colCount; i++)
            *(string *)data = string(row[0]);
        
        return 0;
    };

    if (sqlite3_exec(db, sql.c_str(), get_uid, &uid, &errMsg) != SQLITE_OK) {
        cerr << "SQL error: " << errMsg << "...\n";
        return false;
    }

    sql = "SELECT credits, name from Player WHERE uid = '" + uid + "';";
    static auto get_player = [](void * data, int colCount, char** row, char** rowName) {
        User * player = (User *)data;
        player->credits = atoi(row[0]);
        strcpy(player->name, row[1]);
        return 0;
    };

    if (sqlite3_exec(db, sql.c_str(), get_player, &player, &errMsg) != SQLITE_OK) {
        cerr << "SQL error: " << errMsg << "...\n";
        return false;
    }
    return true;
}

bool register_player(sqlite3 *db, User &player) {
    string uid = uuid();
    string username, password, name;
    cout << "-=-={ New Player Registration }=-=-\n";
    cout << "Username: ";
    getline(cin, username);
    cout << "Password: ";
    getline(cin, password);
    cout << "Full name: ";
    getline(cin, name);
    string hashed_password = hash_password(password);
    string sql = "INSERT INTO Account (uid, username, password) VALUES ( ";
    sql += "'" + uid + "', '" + username + "', '" + password + "');";
    if (execute_sql(db, sql, callback, 0)) {
        sql = "INSERT INTO Player (uid, credits, name) VALUES (";
        sql += "'" + uid + "', '500', '" + name + "');";
        if(execute_sql(db, sql, callback, 0)) {
            strcpy(player.uid, uid.c_str());
            player.credits = 500;
            strcpy(player.name, name.c_str());
            return true;
        }
    }
    return false;
}

char * mgets(char *dst) {
    char *ptr = dst;
    int ch; 
	/* skip leading white spaces */ 
    while (true) {
        ch = getchar();
        if (ch == ' ' or ch == '\t' or ch == '\n') continue;
        else break;
    }

    /* now read the rest until \n or EOF */ 
    while (true) {
        *(ptr++) = ch; 
        ch = getchar();
        if (ch == '\n' or ch == EOF) break;
    }
    *(ptr) = 0;
    return dst;
}

void update_player_data(sqlite3 *db, User &player) {
    string sql = "UPDATE Account set name = '" + string(player.name) + "', ";
    sql += "credits = " + to_string(player.credits) + ";";
    execute_sql(db, sql, callback_dummy, 0);
}