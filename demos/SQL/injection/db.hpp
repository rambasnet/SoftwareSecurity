#include <cstdio>
#include <sqlite3.h>
#include <string>
#include <iostream>
#include <tuple>
using namespace std;

// prototypes
static int callback_select(void *, int, char**, char**);
void execute_sql(sqlite3 *, string, int (* )(void*, int, char**, char**));
void setup_db(sqlite3 *);


static int callback_select(void *data, int colCount, char *row[], char *colName[]) {
   for(int i=0; i<colCount; i++) {
      printf("%s = %s\n", colName[i], row[i] ? row[i] : "NULL");
   }
   printf("\n");
   return 0;
}

static int callback(void* data, int colCount, char**row, char** colName) {
    (vector< vector<string> >) data;
}

static int callback_dummy(void * data, int colCount, char *row[], char *colName[]) {
    return 0;
}

void execute_sql(sqlite3 *db, string sql, int (* callback)(void*, int, char**, char**), void * data) {
    char *errMsg;
    if (sqlite3_exec(db, sql.c_str(), callback, data, &errMsg) != SQLITE_OK)
        cerr << "SQL error: " << errMsg << "...\n";
}

void setup_db(sqlite3 *db) {
    string sql = "CREATE TABLE IF NOT EXISTS Account ( \
                uid TEXT, \
                username TEXT, \
                password TEXT, \
                PRIMARY KEY (uid) \
                );";
    execute_sql(db, sql, callback_dummy);

    sql = "CREATE TABLE IF NOT EXISTS Player ("
                "uid TEXT, "
                "credits INTEGER, "
                "name TEXT, "
                "PRIMARY KEY (uid) "
                ");";
    execute_sql(db, sql, callback_dummy);
    printf("Database setup complete...\n");
}

// This function reads the player data for the current uid
// from the file. It returns false if it is unable to find player
// data for the current uid.
bool read_player_data(sqlite3 *db, User &player) { 
    
    return false;
}

bool authenticate(sqlite3 *db, User &player) {
    string username, password, sql, uid;
    cout << "Username: ";
    getline(cin, username);
    cout << "Password: ";
    getline(cin, password);
    sql = "SELECT uid from Account WHERE username = '" + username 
            + "' and password = '" + password + "';";
    bool authenticated = false;
    // define lambda function with captures
    tuple<string, bool> data;
    auto callback = [](void * data, int colCount, char** row, char** colName) {
        for(int i=0; i<colCount; i++) {
            uid = row[i];
            authenticated = true;
        }
        return 0;
    };
    execute_sql(db, sql, int (* )callback);
}
