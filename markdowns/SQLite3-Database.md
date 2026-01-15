# SQLite Database

- SQLite is an open-source software library that implements a self-contained, serverless, zero-configuration, transactional SQL database engine
- SQLite is the most widely deployed SQL database engine in the world
- for more on SQLite database, visit [https://sqlite.org/index.html](https://sqlite.org/index.html)

## Installation
- SQLite is cross-platform
- on Linux install sqlite3 (core SQL enigine), libsqlite3-dev (developer API), and sqlitebrowser (GUI sqlite3 db editor)


```c++
! echo kali | sudo -S apt install sqlite3
```

    [sudo] password for kali: 
    WARNING: apt does not have a stable CLI interface. Use with caution in scripts.
    
    Reading package lists...
    Building dependency tree...
    Reading state information...
    sqlite3 is already the newest version (3.40.1-1).
    The following packages were automatically installed and are no longer required:
      catfish dh-elpa-helper docutils-common gir1.2-xfconf-0 libcfitsio9 libgdal31
      libmpdec3 libnginx-mod-http-geoip libnginx-mod-http-image-filter
      libnginx-mod-http-xslt-filter libnginx-mod-mail libnginx-mod-stream
      libnginx-mod-stream-geoip libpoppler123 libprotobuf23 libpython3.10
      libpython3.10-dev libpython3.10-minimal libpython3.10-stdlib libtiff5
      libzxingcore1 nginx-common nginx-core php8.1-mysql python-pastedeploy-tpl
      python3-alabaster python3-commonmark python3-docutils python3-imagesize
      python3-roman python3-snowballstemmer python3-speaklater python3-sphinx
      python3.10 python3.10-dev python3.10-minimal ruby3.0 ruby3.0-dev ruby3.0-doc
      sphinx-common
    Use 'sudo apt autoremove' to remove them.
    0 upgraded, 0 newly installed, 0 to remove and 5 not upgraded.



```c++
! echo kali | sudo -S apt install libsqlite3-dev
```

    [sudo] password for kali: 
    WARNING: apt does not have a stable CLI interface. Use with caution in scripts.
    
    Reading package lists...
    Building dependency tree...
    Reading state information...
    libsqlite3-dev is already the newest version (3.40.1-1).
    The following packages were automatically installed and are no longer required:
      catfish dh-elpa-helper docutils-common gir1.2-xfconf-0 libcfitsio9 libgdal31
      libmpdec3 libnginx-mod-http-geoip libnginx-mod-http-image-filter
      libnginx-mod-http-xslt-filter libnginx-mod-mail libnginx-mod-stream
      libnginx-mod-stream-geoip libpoppler123 libprotobuf23 libpython3.10
      libpython3.10-dev libpython3.10-minimal libpython3.10-stdlib libtiff5
      libzxingcore1 nginx-common nginx-core php8.1-mysql python-pastedeploy-tpl
      python3-alabaster python3-commonmark python3-docutils python3-imagesize
      python3-roman python3-snowballstemmer python3-speaklater python3-sphinx
      python3.10 python3.10-dev python3.10-minimal ruby3.0 ruby3.0-dev ruby3.0-doc
      sphinx-common
    Use 'sudo apt autoremove' to remove them.
    0 upgraded, 0 newly installed, 0 to remove and 5 not upgraded.



```c++
! echo kali | sudo -S apt install sqlitebrowser
```

    [sudo] password for kali: 
    WARNING: apt does not have a stable CLI interface. Use with caution in scripts.
    
    Reading package lists...
    Building dependency tree...
    Reading state information...
    sqlitebrowser is already the newest version (3.12.2-1).
    The following packages were automatically installed and are no longer required:
      catfish dh-elpa-helper docutils-common gir1.2-xfconf-0 libcfitsio9 libgdal31
      libmpdec3 libnginx-mod-http-geoip libnginx-mod-http-image-filter
      libnginx-mod-http-xslt-filter libnginx-mod-mail libnginx-mod-stream
      libnginx-mod-stream-geoip libpoppler123 libprotobuf23 libpython3.10
      libpython3.10-dev libpython3.10-minimal libpython3.10-stdlib libtiff5
      libzxingcore1 nginx-common nginx-core php8.1-mysql python-pastedeploy-tpl
      python3-alabaster python3-commonmark python3-docutils python3-imagesize
      python3-roman python3-snowballstemmer python3-speaklater python3-sphinx
      python3.10 python3.10-dev python3.10-minimal ruby3.0 ruby3.0-dev ruby3.0-doc
      sphinx-common
    Use 'sudo apt autoremove' to remove them.
    0 upgraded, 0 newly installed, 0 to remove and 5 not upgraded.



```c++
! echo kali | sudo -S apt install libssl-dev -y
```

    [sudo] password for kali: 
    WARNING: apt does not have a stable CLI interface. Use with caution in scripts.
    
    Reading package lists...
    Building dependency tree...
    Reading state information...
    libssl-dev is already the newest version (3.0.8-1).
    The following packages were automatically installed and are no longer required:
      catfish dh-elpa-helper docutils-common gir1.2-xfconf-0 libcfitsio9 libgdal31
      libmpdec3 libnginx-mod-http-geoip libnginx-mod-http-image-filter
      libnginx-mod-http-xslt-filter libnginx-mod-mail libnginx-mod-stream
      libnginx-mod-stream-geoip libpoppler123 libprotobuf23 libpython3.10
      libpython3.10-dev libpython3.10-minimal libpython3.10-stdlib libtiff5
      libzxingcore1 nginx-common nginx-core php8.1-mysql python-pastedeploy-tpl
      python3-alabaster python3-commonmark python3-docutils python3-imagesize
      python3-roman python3-snowballstemmer python3-speaklater python3-sphinx
      python3.10 python3.10-dev python3.10-minimal ruby3.0 ruby3.0-dev ruby3.0-doc
      sphinx-common
    Use 'sudo apt autoremove' to remove them.
    0 upgraded, 0 newly installed, 0 to remove and 5 not upgraded.


### SQL Introduction
- SQL uses the following meta characters as speical symbols with special meaning
- `' '` - represents a string
- `;` - terminate a command
- `--` - single line comment
- `/* */` - multiline comment
- has many keywords such as INSERT INTO, SLECT, CREATE, DELETE, UPDATE, VALUES, etc.
- SQL is case insensitive
- learn SQL from this tutorial - [[https://www.w3schools.com/sql/default.asp]](https://www.w3schools.com/sql/default.asp)

### SQLite Datatypes
- SQLite uses static typing; each column/field in database table must have a type associated with it
- details - [https://sqlite.org/datatype3.html](https://sqlite.org/datatype3.html)
- NULL - null value
- INTEGER - signed integer
- REAL - floating point
- TEXT - string
- BLOB - blob of data - stored exactly as it was input; can be binary data

### Sqlitebrowser
- GUI-based SQLite aplication that allows us to work with sqlite databases
- use sqlitebrowswer to practice the following CRUD SQL statements

### SQLite CRUD statements
- CRUD - Create, Retrieve, Update, Delete

### Create
- create database, table, insert/create record
- with SQLite, database is simply a single special binary file

#### CREATE TABLE
- the CREATE TABLE statement is used to create a new table in a database
- syntax
```sql
CREATE TABLE table_name (
    column1 datatype,
    column2 datatype,
    column3 datatype,
   ....
);
```
- example

```sql
CREATE TABLE account (
    uid INTEGER,
    username TEXT,
    password TEXT
);
```

#### INSERT INTO
- the INSERT INTO statement is used to insert new records into a table
- two ways to insert records to a table
- syntax

```sql
INSERT INTO table_name (column1, column2, column3, ...)
VALUES (value1, value2, value3, ...);

INSERT INTO table_name
VALUES (value1, value2, value3, ...);
```

- examples

```sql
INSERT INTO account (uid, username, password)
VALUES (1000, 'kali', 'kali');
    
INSERT INTO account
VALUES (1001, 'user', 'user');
```

#### SELECT
- the SELECT statement is used to select data from a database
- the data returned is stored in a result table, called the result-set
- syntax

```sql
SELECT column1, column2, ...
FROM table_name;

SELECT column1, column2, ...
FROM table_name;
```

- examples

```sql
SELECT uid, username, password
FROM account;

SELECT * FROM account;
```

#### UPDATE
- the UPDATE statement is used to modify the existing records in a table
- syntax

```sql
UPDATE table_name
SET column1 = value1, column2 = value2, ...
WHERE condition;
```
- where clause specifies which record(s) to update
- if where clause is omitted or wrong condition is provided all records are updated or wrong records are updated
- once the SQL statment is executed and data is modified; SQL doen't provide UNDO option
- example

```sql
UPDATE account
SET username = 'kalikali', PASSWORD='kali123'
WHERE uid=1000;
```

#### DELETE
- the DELETE statement is used to delete existing records from a table
- syntax

```sql
DELETE FROM table_name WHERE condition;
```

- the WHERE clause specifies the matching records to be deleted
- all records will be deleted if WHERE clause is omitted or WRONG records can be deleted if condition is not set correctly
- example

```sql
DELETE FROM account WHERE username='kalikali';
```

## SQLite3 C++ API

- once the libsqlite3-dev libray is installed, sqlite3 database can be used programetically using C/C++
- C/C++ Interface APIs
- must include `sqlite3.h` header file
- must include `-l sqlite3` while building the C++ program that uses `sqlite3.h` API

### sqlite3_open(const char * filename, sqlite3 **db)

- opens a connection to an SQLite database file and returns a database connection object to be used by other SQLite routines
- ff the filename argument is NULL or ':memory:', sqlite3_open() will create an in-memory database in RAM that lasts only for the duration of the session
- if the filename is not NULL, sqlite3_open() attempts to open the database file by using its value
    - if no file by that name exists, sqlite3_open() will open a new database file by that name.

### sqlite3_exec(sqlite3 * db, const char *sql, sqlite_callback, void *data, char **errmsg)
- execute SQL commands provided by sql argument which can consist of more than one SQL command
- the first argument sqlite3 is an open database object, sqlite_callback is a call back for which data is the 1st argument and errmsg will be returned to capture any error raised by the routine.
- sqlite3_exec() routine parses and executes every command given in the sql argument until it reaches the end of the string or encounters an error

### callback
- callback function is used to handle the result of SQL statements such as SELECT *
- structure of callback

```cpp
typedef int (*sqlite3_callback)(
   void*,    /* Data provided in the 4th argument of sqlite3_exec() */
   int,      /* The number of columns in row */
   char**,   /* An array of strings representing fields in the row */
   char**    /* An array of strings representing column names */
);
```

### sqlite3_close(sqlite3 *db )
- closes a database connection previously opened by a call to sqlite3_open()
- all prepared statements associated with the connection should be finalized prior to closing the connection
- if any queries remain that have not been finalized, sqlite3_close() will return SQLITE_BUSY with the error message Unable to close due to unfinalized statements

### Examples
- see [demos/SQL/CPP-API](demos/SQL/CPP-API) folder for sqlite3 demos
- detail tutorials on the API can be found here: [https://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm](https://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm)


```c++

```
