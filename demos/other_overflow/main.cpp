#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include "lucky7.h"
#include <unistd.h> //getuid()
#include <sys/types.h> // getuid()
#include <iostream>

char DATAFILE[] = "/var/lucky7.data"; // File to store players data

using namespace std;

// Global variables
User player;      // Player struct

int main(int argc, char* argv[]) {
   int choice;

   if(not read_player_data(DATAFILE,  player)) // Try to read player data from file.
      register_new_player(DATAFILE, player);    // If there is no data, register a new player.

   bool quit = false;
   do {
      choice = get_choice(player);
      switch(choice) {
         case 1:
            player.current_game = pick_a_number;
            play_the_game();   // Play the game.
            break;
         case 2:               
            player.current_game = lucky777;
            play_the_game();   // Play the game.
            break;
         case 3:
            player.current_game = lucky77777;
            play_the_game();   // Play the game.
            break;
         case 4:
            show_credits(player);
            break;
         case 5:
            change_username();
            update_player_data(DATAFILE, player);
            printf("Your name has been changed.\n\n");
            break;
         case 6:
            reset_credit(DATAFILE, player);
            break;
         default: // must be case 7
            quit = true;
      }
      //cin.get();
   } while(not quit);
   
   printf("\nThanks for playing! Bye.\n");
}


// This function simply awards the jackpot for the Pick a Number game
void jackpot() {
   printf("*+*+*+*+*+* JACKPOT *+*+*+*+*+*\n");
   printf("Congratulations!\n You have won the jackpot of 100 credits!\n");
   player.credits += 100;
}

// This function simply awards the jackpot for the lucky 777 game
void jackpot777() {
   printf("*+*+*+*+*+* JACKPOT 777 *+*+*+*+*+*\n");
   printf("Congratulations!!!!\n You have won the 777 jackpot of 10000 (10K) credits!\n");
   player.credits += 10000;
}

// This function simply awards the jackpot for the lucky 77777 game
void jackpot77777() {
   printf("*+*+*+*+*+* JACKPOT 77777 *+*+*+*+*+*\n");
   printf("!!!!Congratulations!!!!You have won the 77777 jackpot of 100000 (1M) credits!\n");
   player.credits += 1000000;
}

// This function contains a loop to allow the current game to be
// played again. It also writes the new credit totals to file
// after each game is played.
void play_the_game() { 
   char again;
   do {
      if (player.credits <=0 ) {
         player.credits = 0;
         update_player_data(DATAFILE, player); // Write the new credit total to file.
         cout << "Sorry, you're out of credit.\nReset your credit to 100 to play again\n";
         break;
      }
      player.credits -= 10;
      printf("\n[DEBUG] current_game pointer 0x%08x\n", player.current_game);
      if(player.current_game()) { // if won, give jackport
         if (player.current_game == pick_a_number)
            jackpot();
         else if (player.current_game == lucky777)
            jackpot777();
         else
            jackpot77777();
         printf("\nYou now have %u credits\n", player.credits);
      }
      else
      {
         cout << "Sorry... Better luck next time!!\n";
      }
   
      update_player_data(DATAFILE, player); // Write the new credit total to file.
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