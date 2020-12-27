#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h> //getuid()
#include <sys/types.h> // getuid()
#include <iostream>
#include <fstream>
#include <iomanip>
//#include <filesystem> // file system specific permission
#include <sqlite3.h>

#include "lucky7.hpp"
#include "security.hpp"
#include "db.hpp"

using namespace std;
//namespace fs = std::filesystem;

int get_menu_choice() {
    int choice = 0;
    do {
        cout << setw(40) << setfill('=') << "" << endl;
        cout << "-=[ Main Menu Options ]=-\n"
            << "1 - Login \n"
            << "2 - Register New Player\n"
            << "3 - Exit\n"
            << "Enter your choice [1-3]: ";
        cin >> choice;
        cin.ignore(1000, '\n');
        if(cin.fail())
            cin.clear();

        if ((choice < 1) || (choice > 3))
            cerr << "The number " << choice << " is an invalid selection.\n\n";
        else 
            return choice;

    }while(true);

}

int get_game_choice(User &player) {
   size_t choice = 0;
   do{
      //system("clear");
      cout << setw(50) << setfill('=') << "" << endl;
      cout << "-=[ Lucky 7 Game Menu ]=-\n";
      cout << "1 - Play Lucky 7 game\n";
      cout << "2 - Play Lucky 777 game\n";
      cout << "3 - Play Lucky 77777 game\n";
      cout << "4 - View your total credits\n";
      cout << "5 - Change name\n";
      cout << "6 - Reset account at 500 credits\n";
      cout << "7 - Logout\n";
      cout << setw(50) << setfill('~') << "" << endl; 
      printf("[Name: %s]\n", player.name);
      printf("[You have %u credits]\n", player.credits);
      cout << setw(50) << setfill('=') << "" << endl; 
      cout << "Enter your choice [1-7]: ";
      cin >> choice;
      cin.ignore(1000, '\n');
      if(cin.fail())
         cin.clear();

      if ((choice < 1) || (choice > 7))
         cerr << "The number " << choice << " is an invalid selection.\n\n";
      else 
         return choice;
   } while(true);
}

void show_credits(sqlite3 *db, User &player) {
    get_credits(db, player);
    cout << setw(50) << setfill('=') << "\n";
    cout << "Name: " << player.name << endl;
    cout << "Credits: " << player.credits << endl;
    cout << setw(50) << "\n";
    cout << setfill(' ');
}

void printNumber(int randNumber) {
    int maxRoll = 10000;
    int num;
    for(int i=0; i<maxRoll; i++) {
        num = i%9+1;
        cout << num << flush;
        sleep(0.25);
        cout << "\b" << flush;
    }
    cout << randNumber << " " << flush;
        
}

// win jackpot of 10K if random number 7 is generated
int lucky7() {
    cout << "the random number is: " << flush;
    int num = get_random_number(9);
    printNumber(num);
    cout << endl;
    if (num == 7) return 1; //win jackpot
    else return 0; // loss
}

// win jackpot of 100K for 3 777 numbers
int lucky777() {
    cout << "3 random numers are: " << flush;
    int num1 = get_random_number(9);
    printNumber(num1);

    int num2 = get_random_number(9);
    printNumber(num2);

    int num3 = get_random_number(9);
    printNumber(num3);

    cout << endl;
    if (num1 == 7 and num2 == 7 and num3 == 7) return 2; // jackpot
    else if (num1 == num2 and num2 == num3) return 1; // normal win
    else return 0;
}

// win Jackpot of 1M if all 5 random numbers are 77777
int lucky77777() {
    cout << "5 random numers are: " << flush;
    int num1 = get_random_number(9);
    printNumber(num1);

    int num2 = get_random_number(9);
    printNumber(num2);

    int num3 = get_random_number(9);
    printNumber(num3);

    int num4 = get_random_number(9);
    printNumber(num4);

    int num5 = get_random_number(9);
    printNumber(num5);
    cout << endl;
    if (num1 == 7 and num2 == 7 and num3 == 7 and num4 == 7 and num5 ==7) return 3;
    else if (num1 == num2 and num2 ==  num3 and num3 == num4 and num4 == num5) return 2;
    else return 0;
}

unsigned int get_random_number(int max) {
    srand(time(0)); // Seed the randomizer with the current time.
    int num = rand()%max+1;
    return num;
}