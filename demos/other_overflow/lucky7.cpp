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
   size_t choice = 0;
   do{
      //system("clear");
      cout << "-=[ Lucky 777 Game Menu ]=-\n";
      cout << "1 - Play Pick a Number game\n";
      cout << "2 - Play Lucky 777 game\n";
      cout << "3 - Play Lucky 77777 game\n";
      cout << "4 - View your total credits\n";
      cout << "5 - Change your user name\n";
      cout << "6 - Reset your account at 100 credits\n";
      cout << "7 - Quit\n";
      printf("[Name: %s]\n", player.name);
      printf("[You have %u credits] ->  ", player.credits);
      cout << "Enter you choice [1-7]: ";
      cin >> choice;
      if(cin.fail())
         cin.clear();

      if ((choice < 1) || (choice > 7))
         cerr << "The number " << choice << " is an invalid selection.\n\n";
      else 
         return choice;
   } while(true);
}

// This function reads the player data for the current uid
// from the file. It returns false if it is unable to find player
// data for the current uid.
bool read_player_data(char *data_file, User &player) { 
    int user_id;

    user_id = getuid();
    int uid, credits;
    string name;
    ifstream fin(data_file);
    if (!fin)
        return false;
    
    bool found = false;
    while(fin >> uid and not found) {
        fin >> credits;
        fin >> ws;
        getline(fin, name);
        if (uid == user_id) {
            player.uid = uid;
            player.credits = credits;
            rstrip(name);
            strcpy(player.name, name.c_str());
            found = true;
        }
    }
    fin.close();
    return found;
}

// This is the new user registration function.
// It will create a new player account and append it to the file
void register_new_player(char * data_file, User &player)  { 
    cout << "-=-={ New Player Registration }=-=-\n";
    cout << "Enter your name: ";
    mgets(player.name);
    player.uid = getuid();
    player.credits = 100;

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

bool pick_a_number() {
    size_t guess = 0;
    cout << "Pick a number between 1 and 10: ";
    cin >> guess;
    cin.ignore(1000, '\n');
    srand(time(0)); // Seed the randomizer with the current time.
    size_t hidden = rand()%10+1;
    if (guess == hidden) return true;
    else return false;
}

void show_credits(const User & player) {
    cout << "Name: " << player.name << endl;
    cout << "Credits: " << player.credits << endl;
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

bool lucky777() {
    cout << "3 random numers are: " << flush;
    int num1 = get_random_number(9);
    printNumber(num1);

    int num2 = get_random_number(9);
    printNumber(num2);

    int num3 = get_random_number(9);
    printNumber(num3);

    cout << endl;
    if (num1 == num2 and num2 ==  num3) return true;
    else return false;
}

// win Jackpot of 1M if all 5 random numbers match
bool lucky77777() {
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
    if (num1 == num2 and num2 ==  num3 and num3 == num4 and num4 == num5) return true;
    else return false;
}

void reset_credit(char * datafile, User & player) {
   player.credits = 100;
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