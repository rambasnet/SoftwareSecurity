#include <cstdio>
#include <sqlite3.h>
#include <string>
using namespace std;

// prototypes
static int callback_select(void *, int, char**, char**);
void execute_sql(sqlite3 *, string);


static int callback_select(void *data, int colCount, char *row[], char *colName[]) {
   for(int i=0; i<colCount; i++) {
      printf("%s = %s\n", colName[i], row[i] ? row[i] : "NULL");
   }
   printf("\n");
   return 0;
}

void execute_sql(sqlite3 *db, string sql, static int (* callback)(void*, int, char**, char**)) {
    char *errMsg;
    if (sqlite3_exec(db, sql.c_str(), callback, 0, &errMsg) == SQLITE_OK)
        cout << "sql statement executed successfully!\n";
    else
        printf("SQL error: %s...\n", errMsg);
}
