#pragma once

#include <string>
#include <sqlite3.h>

using namespace std;

// Custom user struct to store information about users
struct User {
   char uid[100];
   char role[20]; //admin or player
   int credits;
   char name[100];
   int (*current_game) ();
};

// function prototypes
int get_menu_choice();
int get_game_choice(User &);
void show_credits(sqlite3 *, User &);
void jackpot();
void jackpot777();
void jackpot77777();
void play_the_game();
int lucky7();
int lucky777();
int lucky77777();
void fatal(char *);
void reset_credit(char *, User &);
unsigned int get_random_number(int max);