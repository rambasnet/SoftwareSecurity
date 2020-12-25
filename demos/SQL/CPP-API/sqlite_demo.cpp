#include <cstdio>
#include <sqlite3.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <unistd.h> //getuid()
#include <sys/types.h> // getuid()
#include <cstring>

using namespace std;

/*
data - 4th argument in sqlite3_exec function
argc - no. of columns in each row
argv[] - data retrieved for each row
colName[] - column names for each row
*/ 
static int callback(void *data, int argc, char *argv[], char *colName[]) {
   // cout << *(int *)data << endl;

   for(int i=0; i<argc; i++) {
      printf("%s = %s\n", colName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

void dropTable(sqlite3 *db) {
    char *errMsg;
    char sql[] = "DROP TABLE if exists myTable;";
    if (sqlite3_exec(db, sql, callback, 0, &errMsg) == SQLITE_OK)
        cout << "Table dropped successfully...\n";
    else
        printf("SQL error: %s\n", errMsg);
}

void createTable(sqlite3 *db) {
    char *errMsg;
    char sql[] = "CREATE TABLE if not exists myTable ( \
        uid int NOT NULL, \
        username TEXT NOT NULL, \
        password TEXT NOT NULL) ";

    if (sqlite3_exec(db, sql, callback, 0, &errMsg) == SQLITE_OK)
        cout << "Table created successfully...\n";
    else
        printf("SQL error: %s\n", errMsg);
}

void insertRecords(sqlite3 *db) {
    char *errMsg;
    int userid = getuid(); // get system userid

    string sql = "INSERT INTO myTable (uid, username, password) VALUES (" + 
        to_string(userid) + ", 'abc@gmail.com', 'SomeUniquePassword!')";

    if (sqlite3_exec(db, sql.c_str(), callback, 0, &errMsg) == SQLITE_OK)
        cout << "Row inserted successfully...\n";
    else
        printf("SQL error: %s\n", errMsg);

    sql = "INSERT INTO myTable (uid, username, password) VALUES (" + 
        to_string(userid) + ", 'abc@gmail.com', 'AnotherUniquePassword!')";
    if (sqlite3_exec(db, sql.c_str(), callback, 0, &errMsg) == SQLITE_OK)
        cout << "Row inserted successfully...\n";
    else
        printf("SQL error: %s\n", errMsg);
}

void readRecords(sqlite3 *db) {
    char *errMsg;
    int userid = getuid();
    string sql = "SELECT * FROM myTable WHERE uid = " + to_string(userid) + ";";
    int * num = new int;
    *num = 10;
    if (sqlite3_exec(db, sql.c_str(), callback, num, &errMsg) == SQLITE_OK)
        cout << "Records retrievied successfully...\n";
    else
        printf("SQL error: %s\n", errMsg);
}

int main() {
    sqlite3 *db;

    if (sqlite3_open("test.db", &db) == SQLITE_OK)
        cout << "Database opened successfully...\n";
    else {
        cerr << "Failed to open database!\n";
        exit(1);
    }
    dropTable(db);
    createTable(db);
    insertRecords(db);
    readRecords(db);
    sqlite3_close(db);
    return 0;
}

