#include <cstdio>
#include <sqlite3.h>
#include <string>
#include <iostream>
#include <cstring>
#include <iomanip>

#include "security.hpp"
#include "db.hpp"

using namespace std;

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
    string sql, uid, password;
    sql = "CREATE TABLE IF NOT EXISTS Account ( "
            "uid TEXT, "
            "username TEXT, "
            "password TEXT, "
            "role TEXT, "
            "UNIQUE (username), "
            "PRIMARY KEY (uid)"
            ");";

    if (DEBUG)
        cout << "SQL: " << sql << endl;
    execute_sql(db, sql, callback_dummy, 0);

    sql = "CREATE TABLE IF NOT EXISTS Player ("
                "uid TEXT, "
                "credits INTEGER, "
                "name TEXT, "
                "PRIMARY KEY (uid) "
                ");";
    if (DEBUG)
        cout << "SQL: " << sql << endl;
    execute_sql(db, sql, callback_dummy, 0);
    uid = uuid();
    password= "P@$sw0rd!";
    //password = hash_password("P@$sw0rd!");
    sql = "INSERT INTO Account (uid, username, password, role) VALUES ('";
    sql += uid + "', 'admin', '" + password + "', 'admin');";
    if (DEBUG)
        cout << "SQL: " << sql << endl;
    if (execute_sql(db, sql, callback_dummy, 0)) {
        sql = "INSERT INTO Player (uid, credits, name) ";
        sql += "VALUES ('" + uid + "', 0, 'Admin Admin');";
        if (DEBUG)
            cout << "SQL: " << sql << endl;
        execute_sql(db, sql, callback_dummy, 0);
    }
    printf("Database setup complete...\n");
}

bool authenticate(sqlite3 *db, User &player) {
    string username, password, sql, uid;
    cout << "Username: ";
    getline(cin, username);
    cout << "Password: ";
    getline(cin, password);
    //password = hash_password(password);

    sql = "SELECT uid, password, role from Account WHERE username = '" + username 
            + "' and password = '" + password + "';";
    
    if (DEBUG)
        cout << "SQL: " << sql << endl;

    char * errMsg;
    // define lambda function for callback
    static auto get_uid = [](void* data, int colCount, char** row, char** colName) {
        //cout << "colCount = " << colCount << endl;
        User * p = (User *)data;
        // set the player's info if it's not set already!
        if (strcmp(p->uid, "") == 0) {
            strcpy(p->uid, row[0]);
            strcpy(p->role, row[1]);
            strcpy(p->role, row[2]);
        }
        return 0;
    };

    if (sqlite3_exec(db, sql.c_str(), get_uid, &player, &errMsg) != SQLITE_OK) {
        cerr << "SQL error: " << errMsg << "...\n";
        return false;
    }
    if (strcmp(player.uid, "") == 0) return false;

    sql = "SELECT credits, name from Player WHERE uid = '" + string(player.uid) + "';";
    if (DEBUG)
        cout << "SQL: " << sql << endl;

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
    //password = hash_password(password);
    string sql = "INSERT INTO Account (uid, username, password, role) VALUES ( ";
    sql += "'" + uid + "', '" + username + "', '" + password + "', 'user');";
    if (DEBUG)
        cout << "SQL: " << sql << endl;
    if (execute_sql(db, sql, callback_dummy, 0)) {
        sql = "INSERT INTO Player (uid, credits, name) VALUES (";
        sql += "'" + uid + "', '500', '" + name + "');";
        if (DEBUG)
            cout << "SQL: " << sql << endl;
        if(execute_sql(db, sql, callback_dummy, 0)) {
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

void update_player_credits(sqlite3 *db, User &player) {
    string sql = "UPDATE Player set credits = " + to_string(player.credits);
    sql += " WHERE uid = '" + string(player.uid) + "';";
    if (DEBUG)
        cout << "SQL: " << sql << endl;
    execute_sql(db, sql, callback_dummy, 0);
}

//TEST TEST', credits = credits+1000000 where uid = 'e30e64d5-0505-42ca-88f4-909a6867d663'; --

void update_name(sqlite3 *db, User &player) {
    string sql = "UPDATE Player set name = '" + string(player.name);
    sql += "' WHERE uid = '" + string(player.uid) + "';";
    if (DEBUG)
        cout << "SQL: " << sql << endl;
    execute_sql(db, sql, callback_dummy, 0);
}

void show_database(sqlite3 *db) {
    string sql = "SELECT A.username, A.password, A.role, P.name, P.credits "
        "FROM Account A "
        "LEFT JOIN Player P "
		"ON A.uid == P.uid "
		"ORDER BY P.name; ";
    cout << setfill(' ');
    cout << "\n            ~*~*~*~* User Account Details ~*~*~*\n";
    cout << left << setw(20) << "Username" << setw(20) << "Password" << setw(10) << "Role" 
        << setw(20) << "Full name" << setw(10) << "Credits" << endl;
    cout << setw(80) << setfill('-') << "" << endl;
    cout << setfill(' ');
    static auto call_select = [](void * data, int colLen, char** row, char** colNames) {
        cout << setw(20) << row[0];
        cout << setw(20) << row[1];
        cout << setw(10) << row[2];
        cout << setw(20) << row[3];
        cout << setw(10) << row[4];
        cout << endl;
        return 0;
    };
    execute_sql(db, sql, call_select, 0);
    cout << setw(80) << setfill('*') << "" << endl;
    cout << setfill(' ');
}

void get_credits(sqlite3 *db, User &player) {
    // SQLite3 prepared statement demo
    sqlite3_stmt *stmt = 0;
    const char query[] = "SELECT credits from Player where uid = ?";
    if (DEBUG)
        cout << "SQL: " << query << endl;
    sqlite3_prepare_v2(db, query, strlen(query), &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, player.uid, strlen(player.uid), SQLITE_STATIC);
    while (sqlite3_step(stmt) == SQLITE_ROW) { // While query has a row
        //colCount should be 1
        for(int colIndex = 0; colIndex < sqlite3_column_count(stmt); colIndex++) {
            player.credits = sqlite3_column_int(stmt, colIndex);
            //cout << "Credits = " << player.credits << endl;
        }
    }
}