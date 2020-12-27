#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h> //getuid()
#include <sys/types.h> // getuid()
#include <iostream>
#include <sqlite3.h>

#include "lucky7.hpp"
#include "security.hpp"
#include "db.hpp"

char DB_NAME[] = "/var/lucky7.db"; // File to store players data

bool AUTHENTICATED = false;

using namespace std;

// Global variables
User player;      // Player struct
sqlite3 *db;

void game();
void init();

int main(int argc, char* argv[]) {
   int option;
   
   if (argc == 2 and string(argv[1]) == "test") {
      password_test();
      return 0;
   }
   // initialize database
   init();

   do {
      option = get_menu_choice();
      if (option == 1) {
         if (authenticate(db, player))
            game();
         else
            cerr << "Login failed.\nUsername or password not correct.\n";
      }
      else if (option == 2) {
         if (register_player(db, player)) {
            cerr << "Registration successful!\n"
               << "Login to start playing...\n";
         }
      }
      else break;
   }while (true);
   printf("\nThanks for playing! Good Bye.\n");
}


// This function simply awards the jackpot for the Pick a Number game
void jackpot10K() {
   printf("*+*+*+*+*+* JACKPOT 10 THOUSAND *+*+*+*+*+*\n");
   printf("Congratulations!\n You have won the jackpot of 10000 (10K) credits!\n");
   player.credits += 10000;
}

// This function simply awards the jackpot for the lucky 777 game
void jackpot100K() {
   printf("*+*+*+*+*+* JACKPOT 100 THOSAND *+*+*+*+*+*\n");
   printf("Congratulations!!!!\n You have won the jackpot of 100000 (100K) credits!\n");
   player.credits += 100000;
}

// This function simply awards the jackpot for the lucky 77777 game
void jackpot1M() {
   printf("*+*+*+*+*+* JACKPOT 1 MILLION *+*+*+*+*+*\n");
   printf("!!!!Congratulations!!!!You have won the jackpot of 100000 (1M) credits!\n");
   player.credits += 1000000;
}

bool has_credits() {
   if (player.current_game == lucky7 and player.credits >= 10) {
      player.credits -= 10;
      return true;
   }

   if (player.current_game == lucky777 and player.credits >= 50) {
      player.credits -= 50;
      return true;
   }

   if (player.current_game == lucky77777 and player.credits >= 100) {
      player.credits -= 100;
      return true;
   }

   return false;
}

// This function contains a loop to allow the current game to be
// played again. It also writes the new credit totals to file
// after each game is played.
void play_the_game() {
   char again;
   int result;
   do {
      if (not has_credits()) {
         cout << "Sorry, you're out of credit.\nReset your credit to 500 to play again\n";
         break;
      }
      printf("\n[DEBUG] current_game pointer 0x%08x\n", player.current_game);
      result = player.current_game();
      if( result == 1) // if won, give jackport
         jackpot10K();
      else if (result == 2)
         jackpot100K();
      else if (result == 3)
         jackpot1M();
      else
         cout << "Sorry! Better luck next time...\n";

      printf("\nYou have %u credits\n", player.credits);
      //update_player_data(DATAFILE, player); // Write the new credit total to file.
      printf("Would you like to play again? [y/n]: ");
      cin >> again;
      cin.ignore(100, '\n');
   } while(again == 'y' or again == 'Y');
}

void change_username() {
   printf("\nChange user name\n");
   cout << "Enter your new name:\n";
   mgets(player.name);
}

void game() {
   int choice, last_game;
   do {
      choice = get_game_choice(player);
      if (choice < 4) {
         if (choice != last_game) {
            switch(choice) {
               case 1:
                  cout << "\n~*~*~ Lucky 7 ~*~*~\nCosts 10 credits to play this game.\n"
                  << "Machine will generate 1 random numbers each between 1 and 9.\n"
                  << "If the number is 7, you win a jackpot of 10 THOUSAND\n"
                  << "Otherwise, you lose.\n" << flush << endl;
                  player.current_game = lucky7;
                  break;
               case 2:
                  cout << "\n~*~*~ Lucky 777 ~*~*~\nCosts 50 credits to play this game.\n"
                  << "Machine will generate 3 random numbers each between 1 and 9.\n"
                  << "If all 3 numbers are 7, you win a jackpot of 100 THOUSAND\n"
                  << "If all 3 numbers match, you win 10 THOUSAND\n"
                  << "Otherwise, you lose.\n Enter to continue..." << flush << endl;               
                  player.current_game = lucky777;
                  break;
               case 3:
                  cout << "\n~*~*~ Lucky 77777 ~*~*~\nCosts 100 credits to play this game.\n"
                  << "Machine will generate 5 random numbers each between 1 and 9.\n"
                  << "If all 5 numbers are 7, you win a jackpot of 1 MILLION\n"
                  << "If all 5 numbers match, you win 100 THOUSAND\n"
                  << "Otherwise, you lose.\n Enter to continue..." << flush << endl;
                  player.current_game = lucky77777;
                  break;
            }
            last_game = choice;
         }  
         play_the_game();
      }
      else if (choice == 4)
         show_credits(player);
      else if (choice == 5)
         change_username();
      else if (choice == 6)
         player.credits = 500;

      update_player_data(db, player);
      //cin.get();
   } while(choice !=7 );
}

void init() {
   if (sqlite3_open(DB_NAME, &db) == SQLITE_OK)
        cerr << "Database opened successfully...\n";
    else {
        cerr << "Failed to open database!\n"
            << "Make sure program is root setuid.\n";
        exit(1);
   }
   setup_db(db);
}