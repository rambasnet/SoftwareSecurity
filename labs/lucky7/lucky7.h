#pragma once

#include <string>
using namespace std;

// Custom user struct to store information about users
struct User {
   int uid;
   int credits;
   char name[150];
   unsigned int (*current_game) ();
};

// function prototypes
bool read_player_data(char *, User &);
void register_new_player(char *, User &);
void update_player_data(char *data_file, User &player);
void show_credits(const User &);
void jackpot();
void jackpot777();
void jackpot77777();
void play_the_game();
unsigned int lucky7();
unsigned int lucky777();
unsigned int lucky77777();
void fatal(char *);
int get_choice(User &);
char *mgets(char* src);
void change_username();
void reset_credit(char *, User &);
unsigned int get_random_number(int max);
void rstrip(string &line);