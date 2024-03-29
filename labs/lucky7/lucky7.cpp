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
#include <filesystem> // file system specific permission

#include "lucky7.h"

using namespace std;
namespace fs = std::filesystem;

int get_choice(User &player) {
   int choice = 0;
   do{
      //system("clear");
      cout << "-=[ Lucky 7 Game Menu ]=-\n";
      cout << "1 - Play Lucky 7 game\n";
      cout << "2 - Play Lucky 777 game\n";
      cout << "3 - Play Lucky 77777 game\n";
      cout << "4 - View your total credits\n";
      cout << "5 - Change your user name\n";
      cout << "6 - Reset your account at 500 credits\n";
      cout << "7 - Quit\n";
      printf("[Name: %s]\n", player.name);
      printf("[You have %u credits] ->  ", player.credits);
      cout << "Enter your choice [1-7]: ";
      cin >> choice;
    
      if ((choice < 1) || (choice > 7))
         cerr << "The number " << choice << " is an invalid selection.\n\n";
      else 
         return choice;

   } while(true);
   
}

// This is the new user registration function.
// It will create a new player account and append it to the file
void register_new_player(char * data_file, User &player)  { 
    cout << "-=-={ New Player Registration }=-=-\n";
    cout << "Enter your name: ";
    mgets(player.name); // get player's name
    player.uid = getuid();
    player.credits = 500;

    ofstream fout(data_file, ios::app);
    if(not fout) {
        cerr << "Fatal error in register_new_player() while opening " << data_file << " file\n";
        exit(-1);
    }
    
    //fout << player.uid << " " << player.credits << " " << player.name << endl;
    fout << left << setw(10) << player.uid << setw(15) << player.credits << setw(100) << player.name << endl;
    fs::permissions(data_file, fs::perms::group_read|fs::perms::others_read, fs::perm_options::remove);
    fout.close();
    printf("\nWelcome to the Lucky 7 Game %s.\n", player.name);
    printf("You have been given %u credits.\n", player.credits);
}

// This function writes the current player data to the file.
// It is used primarily for updating the credits after games.
void update_player_data(char * data_file, User &player) {
    int read_uid, credits;
    string name;
    fstream file(data_file, ios::out|ios::in);
    streampos position;
    if (!file) {
        cerr << "Fatal error opening file " << data_file << endl;
        exit(-1);
    }

    while(file >> read_uid) {  // Loop until correct uid is found
        if (read_uid == player.uid) // found our line
        {
            file.seekg(-4, ios::cur);
            file << left << setw(10) << player.uid << setw(15) << player.credits << setw(100) << player.name << endl;
            break;
        }
        else {
            file >> credits;
            getline(file, name);
        }
    }
    file.close();
}

// This function reads the current player data from db file.
bool read_player_data(char * data_file, User &player) {
    int uid, read_uid, credits;
    string name;
    ifstream fin(data_file);

    uid = getuid(); // get loggedin user's id
    bool found = false;
    if (not fin) {
        cerr << "Database file doesn't exist: " << data_file << endl;
        return false;
    }

    while(fin >> read_uid >> credits and not found) {  // Loop until correct uid is found
        //cout << read_uid << " " << credits << endl;
        if (read_uid == uid) { // found our player's data
            player.uid = uid;
            player.credits = credits;
            fin >> ws; // read and discard trailing whitespaces
            getline(fin, name);
            strcpy(player.name, name.c_str());
            player.current_game = nullptr;
            found = true;
        }
        else {
            // read and discard player's full name
            //fin >> credits;
            getline(fin, name);
        }
    }
    fin.close();
    return found;
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

void show_credits(const User & player) {
    cout << setw(30) << setfill('=') << "\n";
    cout << "Name: " << player.name << endl;
    cout << "Credits: " << player.credits << endl;
    cout << setw(20) << "\n";
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
unsigned int lucky7() {
    cout << "the random number is: " << flush;
    int num = get_random_number(9);
    printNumber(num);
    cout << endl;
    if (num == 7) return 1; //win jackpot
    else return 0; // loss
}

// win jackpot of 100K for 3 777 numbers
unsigned int lucky777() {
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
unsigned int lucky77777() {
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

void reset_credit(char * datafile, User & player) {
   player.credits = 500;
   update_player_data(datafile, player);
}

unsigned int get_random_number(int max) {
    srand(time(0)); // Seed the randomizer with the current time.
    int num = rand()%max+1;
    return num;
}

void rstrip(string &line) {
    int last_space = line.length()-1;
    while(line[last_space] == ' ') --last_space;
    line.erase(line.begin()+last_space+1, line.end());
}