#pragma once

#include <sqlite3.h>
#include "lucky7.hpp"

#define DEBUG true

using namespace std;

// prototypes
void execute_sql(sqlite3 *, string, int (* )(void*, int, char**, char**));
void setup_db(sqlite3 *);
bool register_player(sqlite3 *, User &);
bool authenticate(sqlite3 *, User &);
void update_player_credits(sqlite3 *, User &);
char *mgets(char* src);
void update_name(sqlite3 *, User &);
void show_database(sqlite3 *);
void get_credits(sqlite3 *, User &);