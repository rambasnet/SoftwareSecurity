# Linux Security Fundamentals

## Users and Groups
- in Linux, users are assigned unique user IDs
- user ID and other information are stored in `/etc/passwd` file
- you can find the user ID using `id` command or reading the `/etc/passwd` file
- each line belongs to a user with the following ':' delimited format:
- `username:password:userid:groupid:User Info:home folder:default shell`


```python
! cat /etc/passwd
```

    root:x:0:0:root:/root:/bin/bash
    daemon:x:1:1:daemon:/usr/sbin:/usr/sbin/nologin
    bin:x:2:2:bin:/bin:/usr/sbin/nologin
    sys:x:3:3:sys:/dev:/usr/sbin/nologin
    sync:x:4:65534:sync:/bin:/bin/sync
    games:x:5:60:games:/usr/games:/usr/sbin/nologin
    man:x:6:12:man:/var/cache/man:/usr/sbin/nologin
    lp:x:7:7:lp:/var/spool/lpd:/usr/sbin/nologin
    mail:x:8:8:mail:/var/mail:/usr/sbin/nologin
    news:x:9:9:news:/var/spool/news:/usr/sbin/nologin
    uucp:x:10:10:uucp:/var/spool/uucp:/usr/sbin/nologin
    proxy:x:13:13:proxy:/bin:/usr/sbin/nologin
    www-data:x:33:33:www-data:/var/www:/usr/sbin/nologin
    backup:x:34:34:backup:/var/backups:/usr/sbin/nologin
    list:x:38:38:Mailing List Manager:/var/list:/usr/sbin/nologin
    irc:x:39:39:ircd:/var/run/ircd:/usr/sbin/nologin
    gnats:x:41:41:Gnats Bug-Reporting System (admin):/var/lib/gnats:/usr/sbin/nologin
    nobody:x:65534:65534:nobody:/nonexistent:/usr/sbin/nologin
    _apt:x:100:65534::/nonexistent:/usr/sbin/nologin
    systemd-timesync:x:101:101:systemd Time Synchronization,,,:/run/systemd:/usr/sbin/nologin
    systemd-network:x:102:103:systemd Network Management,,,:/run/systemd:/usr/sbin/nologin
    systemd-resolve:x:103:104:systemd Resolver,,,:/run/systemd:/usr/sbin/nologin
    messagebus:x:104:105::/nonexistent:/usr/sbin/nologin
    codespace:x:1000:1000::/home/codespace:/bin/bash
    sshd:x:105:65534::/run/sshd:/usr/sbin/nologin



```python
! cat /etc/passwd | grep kali
```

    kali:x:1000:1000:kali,,,:/home/kali:/usr/bin/zsh



```python
! id
```

    uid=1000(kali) gid=1000(kali) groups=1000(kali),4(adm),20(dialout),24(cdrom),25(floppy),27(sudo),29(audio),30(dip),44(video),46(plugdev),100(users),105(netdev),117(wireshark),120(bluetooth),133(scanner),140(kaboxer)


## Add Users and Switch between Users

- you can add users by directly updating the `/etc/passwd` file
    - not recommended as one could easily mess up the format of the password file and the system may render unbootable
    - always backup `/etc/passwd` file if you want to update it directly
- use `adduser` command instead
    - friendlier front ends to the low level tools like `useradd`
- requires root/admin priviledge
- open a Terminal and add a new user called `bob`

```bash
sudo adduser
```

- provide some password and other option information
- check to make sure `bob` is added


```python
! man adduser
```

    [4mADDUSER[24m(8)                  System Manager's Manual                 [4mADDUSER[24m(8)
    
    [1mNAME[0m
           adduser, addgroup - add or manipulate users or groups
    
    [1mSYNOPSIS[0m
           [1madduser [22m[[1m--add-extra-groups[22m] [[1m--allow-all-names[22m] [[1m--allow-bad-names[22m]
                   [[1m--comment [4m[22mcomment[24m] [[1m--conf [4m[22mfile[24m] [[1m--debug[22m] [[1m--disabled-login[22m]
                   [[1m--disabled-password[22m] [[1m--firstgid [4m[22mid[24m] [[1m--firstuid [4m[22mid[24m]
                   [[1m--gid [4m[22mid[24m] [[1m--home [4m[22mdir[24m] [[1m--ingroup [4m[22mgroup[24m] [[1m--lastgid [4m[22mid[24m]
                   [[1m--lastuid [4m[22mid[24m] [[1m--no-create-home[22m] [[1m--shell [4m[22mshell[24m] [[1m--quiet[22m]
                   [[1m--uid [4m[22mid[24m] [[1m--verbose[22m] [[1m--stdoutmsglevel [4m[22mprio[24m]
                   [[1m--stderrmsglevel [4m[22mprio[24m] [[1m--logmsglevel [4m[22mprio[24m] [1muser[0m
    
           [1madduser --system [22m[[1m--comment [4m[22mcomment[24m] [[1m--conf [4m[22mfile[24m] [[1m--debug[22m] [[1m--gid [4m[22mid[24m]
                   [[1m--group[22m] [[1m--home [4m[22mdir[24m] [[1m--ingroup [4m[22mgroup[24m] [[1m--no-create-home[22m]
                   [[1m--shell [4m[22mshell[24m] [[1m--uid [4m[22mid[24m] [[1m--quiet[22m] [[1m--verbose[22m]
                   [[1m--stdoutmsglevel [4m[22mprio[24m] [[1m--stderrmsglevel [4m[22mprio[24m]
                   [[1m--logmsglevel [4m[22mprio[24m] [1muser[0m
    
           [1madduser --group [22m[[1m--conf [4m[22mfile[24m] [[1m--debug[22m] [[1m--firstgid [4m[22mid[24m] [[1m--gid [4m[22mID[24m]
                   [[1m--lastgid [4m[22mid[24m] [[1m--quiet[22m] [[1m--verbose[22m] [[1m--stdoutmsglevel [4m[22mprio[24m]
                   [[1m--stderrmsglevel [4m[22mprio[24m] [[1m--logmsglevel [4m[22mprio[24m] [1mgroup[0m
    
           [1maddgroup [22m[[1m--conf [4m[22mfile[24m] [[1m--debug[22m] [[1m--firstgid [4m[22mid[24m] [[1m--gid [4m[22mID[24m]
                    [[1m--lastgid [4m[22mid[24m] [[1m--quiet[22m] [[1m--verbose[22m] [[1m--stdoutmsglevel [4m[22mprio[24m]
                    [[1m--stderrmsglevel [4m[22mprio[24m] [[1m--logmsglevel [4m[22mprio[24m] [1mgroup[0m
    
           [1maddgroup --system [22m[[1m--gid [4m[22mid[24m] [[1m--conf [4m[22mfile[24m] [[1m--quiet[22m] [[1m--verbose[22m]
                    [[1m--stdoutmsglevel [4m[22mprio[24m] [[1m--stderrmsglevel [4m[22mprio[24m]
                    [[1m--logmsglevel [4m[22mprio[24m] [1mgroup[0m
    
           [1madduser [22m[[1m--conf [4m[22mfile[24m] [[1m--debug[22m] [[1m--quiet[22m] [[1m--verbose[22m]
                   [[1m--stdoutmsglevel [4m[22mprio[24m] [[1m--stderrmsglevel [4m[22mprio[24m]
                   [[1m--logmsglevel [4m[22mprio[24m] [1muser group[0m
    
           [1madduser --help[0m
    
           [1madduser --version[0m
    
    [1mDESCRIPTION[0m
           [1madduser  [22mand  [1maddgroup  [22madd users and groups to the system according to
           command  line  options  and  configuration  information   in   [4m/etc/ad‐[0m
           [4mduser.conf[24m.  They are friendlier front ends to the low level tools like
           [1museradd[22m, [1mgroupadd [22mand [1musermod [22mprograms, by default choosing Debian pol‐
           icy  conformant  UID  and  GID  values,  creating a home directory with
           skeletal configuration, running a custom script, and other features.
    
           [1madduser [22mand [1maddgroup [22mare intended as a policy layer, making  it  easier
           for package maintainers and local administrators to create local system
           accounts  in the way Debian expects them to be created, taking the bur‐
           den to adapt to the probably changing specifications of Debian  policy.
           [1madduser  --system [22mtakes special attention on just needing a single call
           in the package maintainer scripts without any conditional wrappers, er‐
           ror suppression or other scaffolding.
    
           [1madduser [22mhonors the distinction  between  [4mdynamically[24m  [4mallocated[24m  [4msystem[0m
           [4musers[24m  [4mand[24m [4mgroups[24m and [4mdynamically[24m [4mallocated[24m [4muser[24m [4maccounts[24m that is docu‐
           mented in Debian Policy, Chapter 9.2.2.
    
           For a full list and explanations of all options, see the  OPTIONS  sec‐
           tion.
    
           [1madduser [22mand [1maddgroup [22mcan be run in one of five modes:
    
       [1mAdd a normal user[0m
           If  called  with  one  non-option  argument and without the [1m--system [22mor
           [1m--group  [22moptions, [1madduser [22mwill add a normal user, that means a  [4mdynami‐[0m
           [4mcally[24m  [4mallocated[24m  [4muser[24m  [4maccount[24m in the sense of Debian Policy.  This is
           commonly referred to in [1madduser [22mas a [4mnon-system[24m [4muser.[0m
    
           [1madduser [22mwill choose the first available UID from the range specified by
           [1mFIRST_UID [22mand [1mLAST_UID [22min the configuration file.   The  range  may  be
           overridden with the [1m--firstuid [22mand [1m--lastuid [22moptions.  Finally, the UID
           can be set fully manually with the [1m--uid [22moption.
    
           By  default,  each  user  is  given a corresponding group with the same
           name.  This is commonly called [4mUsergroups[24m and allows group writable di‐
           rectories to be easily maintained by placing the appropriate  users  in
           the  new  group, setting the set-group-ID bit in the directory, and en‐
           suring that all users use a umask of 002.
    
           For a usergroup, [1madduser [22mwill choose the first available GID  from  the
           range  specified  by  [1mFIRST_GID [22mand [1mLAST_GID [22min the configuration file.
           The range may be overridden with the [1m--firstgid [22mand [1m--lastgid  [22moptions.
           Finally, the GID can be set fully manually with the [1m--gid [22moption.
    
           The  interaction  between [1mUSERS_GID[22m, [1mUSERS_GROUP[22m, and [1mUSERGROUPS [22mis ex‐
           plained in detail in [1madduser.conf[22m(5).
    
           Users' primary groups can also be overridden from the command line with
           the [1m--gid  [22mor [1m--ingroup [22moptions to set the group by id or name, respec‐
           tively.  Also, users can be added to one or  more  supplemental  groups
           defined  as  [1mEXTRA_GROUPS  [22min  the configuration file either by setting
           [1mADD_EXTRA_GROUPS  [22mto  1  in  the  configuration  file,  or  by  passing
           [1m--add-extra-groups [22mon the command line.
    
           [1madduser  [22mwill  copy  files  from  [4m/etc/skel[24m into the home directory and
           prompt for the comment field and a password if those functions have not
           been turned off / overridden from the command line.
    
           UID, comment, home directory and shell might be pre-determined with the
           [1mUID_POOL [22mand [1mGID_POOL [22moption, documented in [1madduser.conf[22m(5).
    
       [1mAdd a system user[0m
           If called with one non-option argument and the [1m--system [22moption, [1madduser[0m
           will add a [4mdynamically[24m [4mallocated[24m [4msystem[24m [4muser,[24m often abbreviated as [4msys‐[0m
           [4mtem[24m [4muser[24m in the context of the [1madduser [22mpackage.
    
           [1madduser [22mwill choose the first available UID from the range specified by
           [1mFIRST_SYSTEM_UID [22mand [1mLAST_SYSTEM_UID [22min the configuration  file.   This
           can be overridden with the [1m--uid [22moption.
    
           By default, system users are placed in the [1mnogroup [22mgroup.  To place the
           new  system  user  in an already existing group, use the [1m--gid [22mor [1m--in‐[0m
           [1mgroup [22moptions.  If the [1m--group [22mis given and the identically named group
           does not already exist, it is created with the same ID.
    
           If no home directory is specified, the default home directory for a new
           system user is [4m/nonexistent[24m.  This directory should never exist on  any
           Debian system, and [1madduser [22mwill never create it automatically.
    
           Unless  a shell is explicitly set with the [1m--shell [22moption, the new sys‐
           tem user will have the shell set to [4m/usr/sbin/nologin[24m.  [1madduser  --sys‐[0m
           [1mtem  [22mdoes  not set a password for the new account.  Skeletal configura‐
           tion files are not copied.
    
           Other options will behave as for the creation of a  normal  user.   The
           files referenced by [1mUID_POOL [22mand [1mGID_POOL [22mdo also work.
    
       [1mAdd a group[0m
           If  [1madduser  [22mis called with the [1m--group [22moption and without the [1m--system[0m
           option, or [1maddgroup [22mis called respectively, a user group will be added.
    
           A [4mdynamically[24m [4mallocated[24m [4msystem[24m [4mgroup,[24m often abbreviated as [4msystem[24m [4mgroup[0m
           in the context of the [1madduser [22mpackage, will be created  if  [1madduser  [22mis
           called with the [1m--system [22moption.
    
           A  GID  will  be chosen from the respective range specified for GIDs in
           the  configuration   file   ([1mFIRST_GID[22m,   [1mLAST_GID[22m,   [1mFIRST_SYSTEM_GID[22m,
           [1mLAST_SYSTEM_GID[22m).  To override that mechanism, you can give the GID us‐
           ing the [1m--gid [22moption.
    
           For  non-system  groups,  the range specified in the configuration file
           may be overridden with the [1m--firstgid [22mand [1m--lastgid [22moptions.
    
           The group is created with no members.
    
       [1mAdd an existing user to an existing group[0m
           If called with two non-option arguments, [1madduser [22mwill add  an  existing
           user to an existing group.
    
    [1mOPTIONS[0m
           Different  modes of [1madduser [22mallow different options.  If no valid modes
           are listed for a option, it is accepted in all modes.
    
           Short versions for certain options may exist  for  historical  reasons.
           They  are  going to stay supported, but are removed from the documenta‐
           tion.  Users are advised to migrate to the long version of options.
    
           [1m--add-extra-groups[0m
                  Add new user to extra groups defined in the configuration files'
                  [1mEXTRA_GROUPS [22msetting.  The old  spelling  [1m--add_extra_groups  [22mis
                  deprecated and will be supported in Debian bookworm only.  Valid
                  Modes: [1madduser[22m, [1madduser --system[22m.
    
           [1m--allow-all-names[0m
                  Allow any user- and groupname which is supported by the underly‐
                  ing [1museradd[22m(8), including names containing non-ASCII characters.
                  See  VALID  NAMES in [1madduser.conf[22m(5).  Valid Modes: [1madduser[22m, [1mad‐[0m
                  [1mduser --system[22m, [1maddgroup[22m, [1maddgroup --system[22m.
    
           [1m--allow-bad-names[0m
                  Disable [1mNAME_REGEX [22mand [1mSYS_NAME_REGEX [22mcheck of  names.   Only  a
                  weaker  check  for  validity  of the name is applied.  See VALID
                  NAMES in [1madduser.conf[22m(5).  Valid Modes: [1madduser[22m, [1madduser  --sys‐[0m
                  [1mtem[22m, [1maddgroup[22m, [1maddgroup --system[22m.
    
           [1m--comment [4m[22mcomment[0m
                  Set the comment field for the new entry generated.  [1madduser [22mwill
                  not ask for the information if this option is given.  This field
                  is  also  known under the name GECOS field and contains informa‐
                  tion that is used by the [1mfinger[22m(1) command.  This used to be the
                  [1m--gecos [22moption, which is deprecated and will  be  removed  after
                  Debian bookworm.  Valid Modes: [1madduser[22m, [1madduser --system[22m.
    
           [1m--conf [4m[22mfile[0m
                  Use  [4mfile[24m instead of [4m/etc/adduser.conf[24m.  Multiple [1m--conf [22moptions
                  can be given.
    
           [1m--debug[0m
                  Synonymous to --stdoutmsglevel=debug. Deprecated.
    
           [1m--disabled-login[0m
           [1m--disabled-password[0m
                  Do not run [1mpasswd[22m(1) to set a password.  In most situations, lo‐
                  gins are still possible though (for example using  SSH  keys  or
                  through  PAM)  for  reasons  that  are  beyond  [1madduser[22m's scope.
                  [1m--disabled-login   [22mwill   additionally   set   the   shell    to
                  [4m/usr/sbin/nologin[24m.  Valid Mode: [1madduser[22m.
    
           [1m--firstuid [4m[22mID[0m
           [1m--lastuid [4m[22mID[0m
           [1m--firstgid [4m[22mID[0m
           [1m--lastgid [4m[22mID[0m
                  Override  the first UID / last UID / first GID / last GID in the
                  range  that  the  uid  is  chosen  from  ([1mFIRST_UID[22m,   [1mLAST_UID[22m,
                  [1mFIRST_GID   [22mand   [1mLAST_GID[22m,  [1mFIRST_SYSTEM_UID[22m,  [1mLAST_SYSTEM_UID[22m,
                  [1mFIRST_SYSTEM_GID [22mand [1mLAST_SYSTEM_GID [22min the configuration file).
                  If a group is created as a usergroup, [1m--firstgid  [22mand  [1m--lastgid[0m
                  are  ignored.   The  group  gets the same ID as the user.  Valid
                  Modes: [1madduser[22m, [1madduser --system[22m, for [1m--firstgid  [22mand  [1m--lastgid[0m
                  also [1maddgroup[22m.
    
           [1m--force-badname[0m
           [1m--allow-badname[0m
                  These are the deprecated forms of [1m--allow-bad-names[22m.  It will be
                  removed  during  the  release  cycle of the Debian release after
                  [4mbookworm[24m.
    
           [1m--gid [4m[22mID[0m
                  When creating a group, this option sets the group ID  number  of
                  the  new  group  to [4mGID[24m.  When creating a user, this option sets
                  the primary group ID number of  the  new  user  to  [4mGID[24m.   Valid
                  Modes: [1madduser[22m, [1madduser --system[22m, [1maddgroup[22m, [1maddgroup --system[22m.
    
           [1m--group[0m
                  Using  this  option  in  [1madduser --system [22mindicates that the new
                  user should get an identically named group as its primary group.
                  If that identically named group is not already  present,  it  is
                  created.   If not combined with [1m--system[22m, a group with the given
                  name is created.  The latter is the default action if  the  pro‐
                  gram is invoked as [1maddgroup[22m.  Valid Modes: [1madduser --system[22m, [1mad‐[0m
                  [1mdgroup[22m, [1maddgroup --system[22m.
    
           [1m--help [22mDisplay brief instructions.
    
           [1m--home [4m[22mdir[0m
                  Use  [4mdir[24m  as  the user's home directory, rather than the default
                  specified by the configuration file (or [4m/nonexistent[24m if  [1madduser[0m
                  [1m--system  [22mis used).  If the directory does not exist, it is cre‐
                  ated.  Valid Modes: [1madduser[22m, [1madduser --system[22m.
    
           [1m--ingroup [4m[22mGROUP[0m
                  When creating a user, this option sets the primary group ID num‐
                  ber of the new user to the GID of the named group.  Unlike  with
                  the  [1m--gid  [22moption,  the  group is specified here by name rather
                  than by numeric ID number.  The group must already exist.  Valid
                  Modes: [1madduser[22m, [1madduser --system[22m.
    
           [1m--lastuid [4m[22mID[0m
           [1m--lastgid [4m[22mID[0m
                  Override the last UID / last GID.  See [1m--firstuid[22m.
    
           [1m--no-create-home[0m
                  Do not create a home directory for the new user.  Note that  the
                  pathname for the new user's home directory will still be entered
                  in  the  appropriate  field in the [4m/etc/passwd[24m file.  The use of
                  this option does not imply that  this  field  should  be  empty.
                  Rather,  it  indicates to [1madduser [22mthat some other mechanism will
                  be responsible for initializing the new user's  home  directory.
                  Valid Modes: [1madduser[22m, [1madduser --system[22m.
    
           [1m--quiet[0m
                  Synonymous to --stdoutmsglevel=warn. Deprecated.
    
           [1m--shell [4m[22mshell[0m
                  Use  [4mshell[24m  as  the  user's login shell, rather than the default
                  specified by the configuration file (or [4m/usr/sbin/nologin[24m if [1mad‐[0m
                  [1mduser --system [22mis used).  Valid Modes: [1madduser[22m,  [1madduser  --sys‐[0m
                  [1mtem[22m.
    
           [1m--system[0m
                  Nomally, [1madduser [22mcreates [4mdynamically[24m [4mallocated[24m [4muser[24m [4maccounts[24m [4mand[0m
                  [4mgroups[24m  as  defined  in Debian Policy, Chapter 9.2.2.  With this
                  option, [1madduser [22mcreates a [4mdynamically[24m [4mallocated[24m [4msystem[24m [4muser[24m  [4mand[0m
                  [4mgroup[24m  and changes its mode respectively.  Valid Modes: [1madduser[22m,
                  [1maddgroup[22m.
    
           [1m--uid [4m[22mID[0m
                  Force the new userid to be the given number.  [1madduser [22mwill  fail
                  if  the  userid is already taken.  Valid Modes: [1madduser[22m, [1madduser[0m
                  [1m--system[22m.
    
           [1m--verbose[0m
                  Synonymous to --stdoutmsglevel=info. Deprecated.
    
           [1m--stdoutmsglevel [4m[22mprio[0m
           [1m--stderrmsglevel [4m[22mprio[0m
           [1m--logmsglevel [4m[22mprio[0m
                  Minimum priority for messages logged to syslog/journal  and  the
                  console,  respectively.   Values  are  [4mtrace[24m, [4mdebug[24m, [4minfo[24m, [4mwarn[24m,
                  [4merr[24m, and [4mfatal[24m.  Messages with the priority set here  or  higher
                  get  printed  to  the  respective  medium.   Messages printed to
                  stderr are not repeated on stdout.  That allows the local  admin
                  to  control  [1madduser[22m's  chattiness on the console and in the log
                  independently, keeping probably confusing information to  itself
                  while still leaving helpful information in the log.
    
           [1m-v [22m, [1m--version[0m
                  Display version and copyright information.
    
    [1mEXIT VALUES[0m
           [1m0      [22mSuccess: The user or group exists as specified.  This can have 2
                  causes: The user or group was created by this call to [1madduser [22mor
                  the user or group was already present on the system as specified
                  before  [1madduser [22mwas invoked.  If [1madduser --system [22mis invoked for
                  a user already existing with the requested or compatible attrib‐
                  utes, it will also return 0.
    
           [1m11     [22mThe object that [1madduser [22mwas asked to create does already exist.
    
           [1m12     [22mThe object that [1madduser [22mor [1mdeluser [22mwas asked to operate on  does
                  not exist.
    
           [1m13     [22mThe  object that [1madduser [22mor [1mdeluser [22mwas asked to operate on does
                  ont have the properties that are required to complete the opera‐
                  tion: A user (a group) that was requested to  be  created  as  a
                  system  user (group) does already exist and is not a system user
                  (group), or A user (group) that was requested to be created with
                  a certain UID (GID) does already exist and has a  different  UID
                  (GID), or A system user (group) that was requested to be deleted
                  does exist, but is not a system user (group).
    
           [1m21     [22mThe  UID  (GID)  that  was  explicitly  requested for a new user
                  (group) is already in use.
    
           [1m22     [22mThere is no available UID (GID) in the requested range.
    
           [1m23     [22mThere is no group with the requested GID for the  primary  group
                  for a new user.
    
           [1m31     [22mThe  chosen  name for a new user or a new group does not conform
                  to the selected naming rules.
    
           [1m32     [22mThe home directory of a new user must be an absolute path.
    
           [1m41     [22mThe group that was requested to be deleted is not empty.
    
           [1m42     [22mThe user that was requested to be removed from a group is not  a
                  member in the first place.
    
           [1m43     [22mIt  is  not possible to remove a user from its primary group, or
                  no primary group selected for a new user by any method.
    
           [1m51     [22mIncorrect number or order of command line parameters detected.
    
           [1m52     [22mIncompatible options set in configuration file.
    
           [1m53     [22mMutually incompatible command line options detected.
    
           [1m54     adduser [22mand [1mdeluser [22minvoked as non-root and thus cannot work.
    
           [1m55     deluser [22mwill refuse to delete the [4mroot[24m account.
    
           [1m56     [22mA function was requested that needs  more  packages  to  be  in‐
                  stalled.  See Recommends: and Suggests: of the adduser package.
    
           [1m61     [22mAdduser  was  aborted for some reason and tried to roll back the
                  changes that were done during execution.
    
           [1m62     [22mInternal adduser error.  This should not happen.  Please try  to
                  reproduce the issue and file a bug report.
    
           [1m71     [22mError creating and handling the lock.
    
           [1m72     [22mError accessing the configuration file(s).
    
           [1m73     [22mError accessing a pool file.
    
           [1m74     [22mError reading a pool file, syntax error in file.
    
           [1m75     [22mError accessing auxiliary files.
    
           [1m81     [22mAn  executable  that  is  needed by [1madduser [22mor [1mdeluser [22mcannot be
                  found. Check your installation and dependencies.
    
           [1m82     [22mExecuting an external command returned some unexpected error.
    
           [1m83     [22mAn external command was terminated with a signal.
    
           [1m84     [22mA syscall terminated with unexpected error.
    
           Or for many other yet undocumented reasons which are printed to console
           then.  You may then consider to remove [1m--quiet  [22mto  make  [1madduser  [22mmore
           verbose.
    
    [1mSECURITY[0m
           [1madduser  [22mneeds  root privileges and offers, via the [1m--conf [22mcommand line
           option to use different configuration files.  Do  not  use  [1msudo[22m(8)  or
           similar  tools  to  give  partial privileges to [1madduser [22mwith restricted
           command line parameters.  This is easy to circumvent  and  might  allow
           users to create arbitrary accounts.  If you want this, consider writing
           your own wrapper script and giving privileges to execute that script.
    
    [1mFILES[0m
           [4m/etc/adduser.conf[0m
                  Default configuration file for [1madduser[22m(8) and [1maddgroup[22m(8)
    
           [4m/usr/local/sbin/adduser.local[0m
                  Optional custom add-ons, see [1madduser.local[22m(8)
    
    [1mNOTES[0m
           Unfortunately,  the  term [4msystem[24m [4maccount[24m suffers from double use in De‐
           bian.  It both means an account for the actual Debian  system,  distin‐
           guishing  itself  from  an [4mapplication[24m [4maccount[24m which might exist in the
           user database of some application running on Debian.  A [4msystem[24m  [4maccount[0m
           in  this  definition  has the potential to log in to the actual system,
           has a UID, can be member in system groups, can own files and processes.
           Debian Policy, au contraire, in its Chapter 9.2.2, makes a distinguish‐
           ment of [4mdynamically[24m [4mallocated[24m [4msystem[24m [4musers[24m [4mand[24m [4mgroups[24m  and  [4mdynamically[0m
           [4mallocated[24m  [4muser[24m  [4maccounts[24m,  meaning  in both cases special instances of
           [4msystem[24m [4maccounts[24m.  Care must be taken to not confuse  this  terminology.
           Since  [1madduser  [22mand  [1mdeluser[22m(8)  never address [4mapplication[24m [4maccounts[24m and
           everything in this package concerns [4msystem[24m [4maccounts[24m here, the usage  of
           the  terms [4muser[24m [4maccount[24m and [4msystem[24m [4maccount[24m is actually not ambiguous in
           the context of this package.  For clarity, this document uses the defi‐
           nition [4mlocal[24m [4msystem[24m [4maccount[24m [4mor[24m [4mgroup[24m if the distinction to  [4mapplication[0m
           [4maccounts[24m or accounts managed in a directory service is needed.
    
           [1madduser  [22mused  to  have the vision to be the universal front end to the
           various directory services for creation and  deletion  of  regular  and
           system  accounts  in  Debian  since  the 1990ies.  This vision has been
           abandoned as of 2022.  The rationale  behind  this  includes:  that  in
           practice, a small server system is not going to have write access to an
           enterprise-wide  directory service anyway, that locally installed pack‐
           ages are hard to manage with centrally controlled system accounts, that
           enterprise directory services have their own management processes  any‐
           way and that the personpower of the [1madduser [22mteam is unlikely to be ever
           strong  enough to write and maintain support for the plethora of direc‐
           tory services that need support.
    
           [1madduser [22mwill constrict itself to being a policy layer for  the  manage‐
           ment  of local system accounts, using the tools from the [1mpassword [22mpack‐
           age for the actual work.
    
    [1mBUGS[0m
           Inconsistent use of terminology around the term [4msystem[24m [4maccount[24m in  docs
           and  code  is  a  bug.   Please report this and allow us to improve our
           docs.
    
           [1madduser [22mtakes special attention to be directly usable in  Debian  main‐
           tainer  scripts  without  conditional  wrappers,  error suppression and
           other scaffolding.  The only thing that the package  maintainer  should
           need  to  code  is  a  check  for the presence of the executable in the
           postrm script.  The [1madduser [22mmaintainers consider  the  need  for  addi‐
           tional  scaffolding  a  bug  and  encourage their fellow Debian package
           maintainers to file bugs against the [1madduser [22mpackage in this case.
    
    [1mSEE ALSO[0m
           [1madduser.conf[22m(5), [1mdeluser[22m(8), [1mgroupadd[22m(8), [1museradd[22m(8),  [1musermod[22m(8),  De‐
           bian Policy 9.2.2.
    
    Debian GNU/Linux                                                    [4mADDUSER[24m(8)


## Exercise

- Add two 3 users: `bob`, `alice`, and `kali`
- if using Kali Linux, `kali` user is already created


```python
! ls /home
```

    bob  kali



```python
! finger bob
```

    Login: bob            			Name: 
    Directory: /home/bob                	Shell: /bin/bash
    Never logged in.
    No mail.
    No Plan.


## Delete Users

- run `deluser` with sudo
- take caution when deleting anything on Linux as it can't be undone


```python
! man deluser
```

    [4mDELUSER[24m(8)                  System Manager's Manual                 [4mDELUSER[24m(8)
    
    [1mNAME[0m
           deluser, delgroup - remove a user or group from the system
    
    [1mSYNOPSIS[0m
           [1mdeluser [22m[[1m--backup[22m] [[1m--backup-suffix [4m[22mstr[24m] [[1m--backup-to [4m[22mdir[24m]
                   [[1m--conf [4m[22mfile[24m] [[1m--debug[22m] [[1m--remove-all-files[22m] [[1m--remove-home[22m]
                   [[1m--quiet[22m] [[1m--verbose[22m] [[1m--stdoutmsglevel [4m[22mprio[24m]
                   [[1m--stderrmsglevel [4m[22mprio[24m] [[1m--logmsglevel [4m[22mprio[24m] [1muser[0m
    
           [1mdeluser [22m[[1m--system[22m] [[1m--backup[22m] [[1m--backup-suffix [4m[22mstr[24m] [[1m--backup-to [4m[22mdir[24m]
                   [[1m--conf [4m[22mfile[24m] [[1m--debug[22m] [[1m--remove-all-files[22m] [[1m--remove-home[22m]
                   [[1m--quiet[22m] [[1m--verbose[22m] [[1m--stdoutmsglevel [4m[22mprio[24m]
                   [[1m--stderrmsglevel [4m[22mprio[24m] [[1m--logmsglevel [4m[22mprio[24m] [1muser[0m
    
           [1mdeluser --group [22m[[1m--conf [4m[22mfile[24m] [[1m--debug[22m] [[1m--only-if-empty[22m] [[1m--quiet[22m]
                   [[1m--verbose[22m] [[1m--stdoutmsglevel [4m[22mprio[24m] [[1m--stderrmsglevel [4m[22mprio[24m]
                   [[1m--logmsglevel [4m[22mprio[24m] [1mgroup[0m
           [1mdelgroup [22m[[1m--system[22m] [[1m--conf [4m[22mfile[24m] [[1m--debug[22m] [[1m--only-if-empty[22m] [[1m--quiet[22m]
                    [[1m--verbose[22m] [[1m--stdoutmsglevel [4m[22mprio[24m] [[1m--stderrmsglevel [4m[22mprio[24m]
                    [[1m--logmsglevel [4m[22mprio[24m] [1mgroup[0m
    
           [1mdeluser [22m[[1m--conf [4m[22mfile[24m] [[1m--debug[22m] [[1m--quiet[22m] [[1m--verbose[22m]
                   [[1m--stdoutmsglevel [4m[22mprio[24m] [[1m--stderrmsglevel [4m[22mprio[24m]
                   [[1m--logmsglevel [4m[22mprio[24m] [1muser group[0m
    
           [1mdeluser --help[0m
    
           [1mdeluser --version[0m
    
    [1mDESCRIPTION[0m
           [1mdeluser  [22mand [1mdelgroup [22mremove users and groups from the system according
           to   command   line   options   and   configuration   information    in
           [4m/etc/deluser.conf[24m and [4m/etc/adduser.conf[24m.
    
           They  are  friendlier  front ends to the [1muserdel [22mand [1mgroupdel [22mprograms,
           removing the home directory as option or even all files on  the  system
           owned  by  the  user  to be removed, running a custom script, and other
           features.
    
           For a full list and explanations of all options, see the  OPTIONS  sec‐
           tion.
    
           [1mdeluser [22mand [1mdelgroup [22mcan be run in one of three modes:
    
       [1mRemove a user[0m
           If  called with one non-option argument and without the [1m--group [22moption,
           [1mdeluser [22mwill remove a non-system user.
    
           By default, [1mdeluser [22mwill remove the user without removing the home  di‐
           rectory,  the  mail spool or any other files on the system owned by the
           user.  Removing the home directory and mail spool can be achieved using
           the [1m--remove-home [22moption.
    
           The  [1m--remove-all-files [22moption removes all files on the system owned by
           the user.  Note that if you activate both  options  [1m--remove-home  [22mwill
           have  no  additional effect because all files including the home direc‐
           tory and mail spool are already covered by the  [1m--remove-all-files  [22mop‐
           tion.
    
           If  you  want to backup all files before deleting them you can activate
           the [1m--backup [22moption which will create a file [4musername.tar(.gz|.bz2)[24m  in
           the directory specified by the [1m--backup-to [22moption.
    
           By  default,  the backup archive is compressed with [1mgzip[22m(1).  To change
           this, the [1m--backup-suffix [22moption can be set to any suffix supported  by
           [1mtar --auto-compress [22m(e.g. .gz, .bz2, .xz).
    
           [1mdeluser [22mwill refuse to remove the root account.
    
           If  the [1m--system [22moption is given on the command line, the delete opera‐
           tion is actually executed only if the user  is  a  system  user.   This
           avoids  accidentally  deleting  non-system users.  Additionally, if the
           user does not exist, no error value is returned.  Debian package  main‐
           tainer scripts may use this flag to remove system users or groups while
           ignoring the case where the removal already occurred.
    
       [1mRemove a group[0m
           If [1mdeluser [22mis called with the [1m--group  [22moption, or [1mdelgroup [22mis called, a
           group will be removed.  The primary group of an existing user cannot be
           removed.   If  the  option [1m--only-if-empty [22mis given, the group won't be
           removed if it has any members left.
    
           The [1m--system [22moption adds the same functionality as for  users,  respec‐
           tively.
    
       [1mRemove a user from a specific group[0m
           If  called  with  two  non-option arguments, [1mdeluser [22mwill remove a user
           from a specific group.
    
    [1mOPTIONS[0m
           Different modes of [1mdeluser [22mallow different options.  If no valid  modes
           are listed for a option, it is accepted in all modes.
    
           Short  versions  for  certain options may exist for historical reasons.
           They are going to stay supported, but are removed from  the  documenta‐
           tion.  Users are advised to migrate to the long version of options.
    
           [1m--backup[0m
                  Backup  all  files  contained  in the userhome and the mailspool
                  file to a file named [4musername.tar.bz2[24m or [4musername.tar.gz[24m.  Valid
                  Modes: [1mdeluser[22m, [1mdeluser --system[22m,
    
           [1m--backup-suffix [22mstr
                  Select compression algorithm for a home directory  backup.   Can
                  be  set  to  any  suffix recognized by [1mtar --auto-compress[22m.  De‐
                  faults to [4m.gz[24m.  Valid Modes: [1mdeluser[22m, [1mdeluser --system[22m,
    
           [1m--backup-to [4m[22mdir[0m
                  Place the backup files not in the current directory but in  [4mdir[24m.
                  This  implicitly sets [1m--backup [22malso.  (defaulting to the current
                  working directory).  Valid Modes: [1mdeluser[22m, [1mdeluser --system[22m,
    
           [1m--conf [4m[22mfile[0m
                  Use [4mfile[24m instead of  the  default  files  [4m/etc/deluser.conf[24m  and
                  [4m/etc/adduser.conf[24m.  Multiple --conf options may be given.
    
           [1m--debug[0m
                  Synonymous to --stdoutmsglevel=debug. Deprecated.
    
           [1m--group[0m
                  Remove  a  group.   This is the default action if the program is
                  invoked as [4mdelgroup[24m.  Valid Mode: [1mdeluser[22m.
    
           [1m--help [22mDisplay brief instructions.
    
           [1m--only-if-empty[0m
                  Only remove if  no  members  are  left.   Valid  Modes:  [1mdeluser[0m
                  [1m--group[22m, [1mdelgroup[22m,
    
           [1m--quiet[0m
                  Synonymous to --stdoutmsglevel=warn. Deprecated.
    
           [1m--remove-all-files[0m
                  Remove  all  files  from  the  system owned by this user.  Note:
                  --remove-home does not have an effect any more.  If [1m--backup  [22mis
                  specified,  the  files  are  deleted  after having performed the
                  backup.  Valid Modes: [1mdeluser[22m, [1mdeluser --system[22m,
    
           [1m--remove-home[0m
                  Remove the home directory of the user  and  its  mailspool.   If
                  [1m--backup  [22mis  specified, the files are deleted after having per‐
                  formed the backup.  Valid Modes: [1mdeluser[22m, [1mdeluser --system[22m,
    
           [1m--system[0m
                  Only delete if user/group is a system user/group.  If  the  user
                  does  not  exist,  no  error  value  is  returned.  Valid Modes:
                  [1mdeluser[22m, [1mdeluser --system[22m,
    
           [1m--verbose[0m
                  Synonymous to --stdoutmsglevel=info. Deprecated.
    
           [1m--stdoutmsglevel [4m[22mprio[0m
           [1m--stderrmsglevel [4m[22mprio[0m
           [1m--logmsglevel [4m[22mprio[0m
                  Minimum priority for messages logged to syslog/journal  and  the
                  console,  respectively.   Values  are  [4mtrace[24m, [4mdebug[24m, [4minfo[24m, [4mwarn[24m,
                  [4merr[24m, and [4mfatal[24m.  Messages with the priority set here  or  higher
                  get  printed  to  the  respective  medium.   Messages printed to
                  stderr are not repeated on stdout.  That allows the local  admin
                  to  control  [1madduser[22m's  chattiness on the console and in the log
                  independently, keeping probably confusing information to  itself
                  while still leaving helpful information in the log.
    
           [1m--version[0m
                  Display version and copyright information.
    
    [1mEXIT VALUES[0m
           The exit values documented in adduser(8) also apply for [1mdeluser[22m.
    
    [1mSECURITY[0m
           [1mdeluser  [22mneeds  root privileges and offers, via the [1m--conf [22mcommand line
           option to use different configuration files.  Do  not  use  [1msudo[22m(8)  or
           similar  tools  to  give  partial privileges to [1mdeluser [22mwith restricted
           command line parameters.  This is easy to circumvent  and  might  allow
           users to create arbitrary accounts.  If you want this, consider writing
           your own wrapper script and giving privileges to execute that script.
    
    [1mFILES[0m
           [4m/etc/deluser.conf[24m  Default  configuration  file for [1mdeluser[22m(8) and [1mdel‐[0m
           [1mgroup[22m(8)
    
           [4m/usr/local/sbin/deluser.local[0m
                  Optional custom add-ons, see [1mdeluser.local[22m(8)
    
    [1mSEE ALSO[0m
           [1madduser[22m(8),   [1mdeluser.conf[22m(5),   [1mdeluser.local.conf[22m(8),    [1mgroupdel[22m(8),
           [1muserdel[22m(8)
    
    Debian GNU/Linux                                                    [4mDELUSER[24m(8)


## Exercise

- remove user `bob` and all their files
- check if the user is removed and their home folder deleted!
- add user `bob` again


```python
# on Jupyter Notebook you must read sudo password from a pipe; it doesn't support interactive input
# if kali is the password for current user with sudo privileges, use the following command
! echo kali | sudo -S deluser --remove-home bob
```

    /usr/sbin/deluser: The user `bob' does not exist.



```python
! man sudo
```

    [4mSUDO[24m(8)                     System Manager's Manual                    [4mSUDO[24m(8)
    
    [1mNAME[0m
           sudo, sudoedit — execute a command as another user
    
    [1mSYNOPSIS[0m
           [1msudo -h [22m| [1m-K [22m| [1m-k [22m| [1m-V[0m
           [1msudo -v [22m[[1m-ABkNnS[22m] [[1m-g [4m[22mgroup[24m] [[1m-h [4m[22mhost[24m] [[1m-p [4m[22mprompt[24m] [[1m-u [4m[22muser[24m]
           [1msudo  -l [22m[[1m-ABkNnS[22m] [[1m-g [4m[22mgroup[24m] [[1m-h [4m[22mhost[24m] [[1m-p [4m[22mprompt[24m] [[1m-U [4m[22muser[24m] [[1m-u [4m[22muser[24m]
                [[4mcommand[24m [[4marg[24m [4m...[24m]]
           [1msudo  [22m[[1m-ABbEHnPS[22m]  [[1m-C  [4m[22mnum[24m]  [[1m-D  [4m[22mdirectory[24m]  [[1m-g  [4m[22mgroup[24m]  [[1m-h   [4m[22mhost[24m]
                [[1m-p  [4m[22mprompt[24m]  [[1m-R  [4m[22mdirectory[24m]  [[1m-r  [4m[22mrole[24m]  [[1m-t  [4m[22mtype[24m] [[1m-T [4m[22mtimeout[24m]
                [[1m-u [4m[22muser[24m] [[4mVAR[24m=[4mvalue[24m] [[1m-i [22m| [1m-s[22m] [[4mcommand[24m [[4marg[24m [4m...[24m]]
           [1msudoedit [22m[[1m-ABkNnS[22m]  [[1m-C  [4m[22mnum[24m]  [[1m-D  [4m[22mdirectory[24m]  [[1m-g  [4m[22mgroup[24m]  [[1m-h  [4m[22mhost[24m]
                [[1m-p  [4m[22mprompt[24m]  [[1m-R  [4m[22mdirectory[24m]  [[1m-r  [4m[22mrole[24m]  [[1m-t  [4m[22mtype[24m] [[1m-T [4m[22mtimeout[24m]
                [[1m-u [4m[22muser[24m] [4mfile[24m [4m...[0m
    
    [1mDESCRIPTION[0m
           [1msudo [22mallows a permitted user to execute a [4mcommand[24m as the  superuser  or
           another user, as specified by the security policy.  The invoking user's
           real  ([4mnot[24m  effective)  user-ID is used to determine the user name with
           which to query the security policy.
    
           [1msudo [22msupports a plugin architecture for  security  policies,  auditing,
           and  input/output  logging.   Third  parties can develop and distribute
           their own plugins to work seamlessly with the [1msudo [22mfront-end.  The  de‐
           fault  security  policy  is  [4msudoers[24m,  which is configured via the file
           [4m/etc/sudoers[24m, or via LDAP.  See the “Plugins” section for more informa‐
           tion.
    
           The security policy determines what privileges, if any, a user  has  to
           run  [1msudo[22m.   The  policy may require that users authenticate themselves
           with a password or another authentication mechanism.  If authentication
           is required, [1msudo [22mwill exit if  the  user's  password  is  not  entered
           within  a  configurable time limit.  This limit is policy-specific; the
           default password prompt timeout for the [4msudoers[24m security  policy  is  0
           minutes.
    
           Security  policies  may support credential caching to allow the user to
           run [1msudo [22magain for a period of time without  requiring  authentication.
           By default, the [4msudoers[24m policy caches credentials on a per-terminal ba‐
           sis  for  15 minutes.  See the [4mtimestamp_type[24m and [4mtimestamp_timeout[24m op‐
           tions in [4msudoers[24m(5) for more information.  By running [1msudo [22mwith the  [1m-v[0m
           option,  a  user  can  update  the cached credentials without running a
           [4mcommand[24m.
    
           On systems where [1msudo [22mis the primary method of gaining superuser privi‐
           leges, it is imperative to avoid syntax errors in the  security  policy
           configuration  files.   For  the  default  security policy, [4msudoers[24m(5),
           changes to the configuration files should be made using  the  [4mvisudo[24m(8)
           utility which will ensure that no syntax errors are introduced.
    
           When invoked as [1msudoedit[22m, the [1m-e [22moption (described below), is implied.
    
           Security  policies  and audit plugins may log successful and failed at‐
           tempts to run [1msudo[22m.  If  an  I/O  plugin  is  configured,  the  running
           [4mcommand[24m's input and output may be logged as well.
    
           The options are as follows:
    
           [1m-A[22m, [1m--askpass[0m
                   Normally, if [1msudo [22mrequires a password, it will read it from the
                   user's  terminal.   If  the [1m-A [22m([4maskpass[24m) option is specified, a
                   (possibly graphical) helper program is  executed  to  read  the
                   user's password and output the password to the standard output.
                   If  the  SUDO_ASKPASS environment variable is set, it specifies
                   the path to the helper  program.   Otherwise,  if  [4msudo.conf[24m(5)
                   contains a line specifying the askpass program, that value will
                   be used.  For example:
    
                       # Path to askpass helper program
                       Path askpass /usr/X11R6/bin/ssh-askpass
    
                   If  no askpass program is available, [1msudo [22mwill exit with an er‐
                   ror.
    
           [1m-B[22m, [1m--bell[0m
                   Ring the bell as part of the password prompt when a terminal is
                   present.  This option has no effect if an  askpass  program  is
                   used.
    
           [1m-b[22m, [1m--background[0m
                   Run the given [4mcommand[24m in the background.  It is not possible to
                   use  shell  job  control  to  manipulate  background  processes
                   started by [1msudo[22m.  Most interactive [4mcommand[24ms will fail  to  work
                   properly in background mode.
    
           [1m-C [4m[22mnum[24m, [1m--close-from[22m=[4mnum[0m
                   Close  all file descriptors greater than or equal to [4mnum[24m before
                   executing a [4mcommand[24m.  Values less than three are not permitted.
                   By default, [1msudo [22mwill close all  open  file  descriptors  other
                   than  standard  input, standard output, and standard error when
                   executing a [4mcommand[24m.  The  security  policy  may  restrict  the
                   user's  ability  to  use  this option.  The [4msudoers[24m policy only
                   permits use of the [1m-C [22moption when the administrator has enabled
                   the [4mclosefrom_override[24m option.
    
           [1m-D [4m[22mdirectory[24m, [1m--chdir[22m=[4mdirectory[0m
                   Run the [4mcommand[24m in the specified [4mdirectory[24m instead of the  cur‐
                   rent  working directory.  The security policy may return an er‐
                   ror if the user does not have permission to specify the working
                   directory.
    
           [1m-E[22m, [1m--preserve-env[0m
                   Indicates to the security policy that the user wishes  to  pre‐
                   serve  their existing environment variables.  The security pol‐
                   icy may return an error if the user does not have permission to
                   preserve the environment.
    
           [1m--preserve-env=list[0m
                   Indicates to the security policy that the user  wishes  to  add
                   the comma-separated list of environment variables to those pre‐
                   served  from  the  user's environment.  The security policy may
                   return an error if the user does not have  permission  to  pre‐
                   serve  the  environment.  This option may be specified multiple
                   times.
    
           [1m-e[22m, [1m--edit[0m
                   Edit one or more [4mfile[24ms instead of running a [4mcommand[24m.   In  lieu
                   of  a  path name, the string "sudoedit" is used when consulting
                   the security policy.  If the user is authorized by the  policy,
                   the following steps are taken:
    
                   1.   Temporary  copies  are made of the files to be edited with
                        the owner set to the invoking user.
    
                   2.   The editor specified by the policy is run to edit the tem‐
                        porary files.  The [4msudoers[24m policy  uses  the  SUDO_EDITOR,
                        VISUAL  and  EDITOR environment variables (in that order).
                        If none of SUDO_EDITOR, VISUAL  or  EDITOR  are  set,  the
                        first  program  listed  in the [4meditor[24m [4msudoers[24m(5) option is
                        used.
    
                   3.   If they have been modified, the temporary files are copied
                        back to their original location and the temporary versions
                        are removed.
    
                   To help prevent the editing of unauthorized files, the  follow‐
                   ing  restrictions are enforced unless explicitly allowed by the
                   security policy:
    
                    [1m•  [22mSymbolic links  may  not  be  edited  (version  1.8.15  and
                       higher).
    
                    [1m•  [22mSymbolic links along the path to be edited are not followed
                       when  the parent directory is writable by the invoking user
                       unless that user is root (version 1.8.16 and higher).
    
                    [1m•  [22mFiles located in a directory that is writable by the invok‐
                       ing user may not be edited unless that user is  root  (ver‐
                       sion 1.8.16 and higher).
    
                   Users are never allowed to edit device special files.
    
                   If  the specified file does not exist, it will be created.  Un‐
                   like most [4mcommand[24ms run by [4msudo[24m, the editor is run with the  in‐
                   voking  user's  environment  unmodified.  If the temporary file
                   becomes empty after editing, the user will be  prompted  before
                   it is installed.  If, for some reason, [1msudo [22mis unable to update
                   a file with its edited version, the user will receive a warning
                   and the edited copy will remain in a temporary file.
    
           [1m-g [4m[22mgroup[24m, [1m--group[22m=[4mgroup[0m
                   Run  the [4mcommand[24m with the primary group set to [4mgroup[24m instead of
                   the primary group specified by the target user's password data‐
                   base entry.  The [4mgroup[24m may be either a group name or a  numeric
                   group-ID  (GID) prefixed with the ‘#’ character (e.g., ‘#0’ for
                   GID 0).  When running a [4mcommand[24m as a GID, many  shells  require
                   that  the  ‘#’ be escaped with a backslash (‘\’).  If no [1m-u [22mop‐
                   tion is specified, the [4mcommand[24m will  be  run  as  the  invoking
                   user.   In either case, the primary group will be set to [4mgroup[24m.
                   The [4msudoers[24m policy permits any of the target user's  groups  to
                   be  specified via the [1m-g [22moption as long as the [1m-P [22moption is not
                   in use.
    
           [1m-H[22m, [1m--set-home[0m
                   Request that the security policy set the HOME environment vari‐
                   able to the home directory specified by the target user's pass‐
                   word database entry.  Depending on the policy, this may be  the
                   default behavior.
    
           [1m-h[22m, [1m--help[0m
                   Display a short help message to the standard output and exit.
    
           [1m-h [4m[22mhost[24m, [1m--host[22m=[4mhost[0m
                   Run  the  [4mcommand[24m  on the specified [4mhost[24m if the security policy
                   plugin supports remote [4mcommand[24ms. The [4msudoers[24m  plugin  does  not
                   currently  support  running  remote  [4mcommand[24ms. This may also be
                   used in conjunction with the [1m-l [22moption to list a user's  privi‐
                   leges for the remote host.
    
           [1m-i[22m, [1m--login[0m
                   Run  the shell specified by the target user's password database
                   entry as a login shell.  This  means  that  login-specific  re‐
                   source files such as [4m.profile[24m, [4m.bash_profile[24m, or [4m.login[24m will be
                   read  by the shell.  If a [4mcommand[24m is specified, it is passed to
                   the shell as a simple [4mcommand[24m using the [1m-c [22moption.  The [4mcommand[0m
                   and any [4marg[24ms are concatenated, separated by spaces,  after  es‐
                   caping  each character (including white space) with a backslash
                   (‘\’) except for alphanumerics, underscores, hyphens, and  dol‐
                   lar signs.  If no [4mcommand[24m is specified, an interactive shell is
                   executed.   [1msudo  [22mattempts to change to that user's home direc‐
                   tory before running the shell.  The [4mcommand[24m is run with an  en‐
                   vironment  similar  to  the one a user would receive at log in.
                   Most shells behave differently when a [4mcommand[24m is  specified  as
                   compared  to an interactive session; consult the shell's manual
                   for details.  The [4mCommand[24m [4menvironment[24m section in the [4msudoers[24m(5)
                   manual documents how the [1m-i [22moption affects the  environment  in
                   which a [4mcommand[24m is run when the [4msudoers[24m policy is in use.
    
           [1m-K[22m, [1m--remove-timestamp[0m
                   Similar  to  the [1m-k [22moption, except that it removes every cached
                   credential for the user, regardless of the terminal  or  parent
                   process  ID.  The next time [1msudo [22mis run, a password must be en‐
                   tered if the security policy requires  authentication.   It  is
                   not possible to use the [1m-K [22moption in conjunction with a [4mcommand[0m
                   or other option.  This option does not require a password.  Not
                   all security policies support credential caching.
    
           [1m-k[22m, [1m--reset-timestamp[0m
                   When used without a [4mcommand[24m, invalidates the user's cached cre‐
                   dentials for the current session.  The next time [1msudo [22mis run in
                   the  session, a password must be entered if the security policy
                   requires authentication.  By default, the [1msudoers [22mpolicy uses a
                   separate record in the credential cache for each  terminal  (or
                   parent  process  ID  if no terminal is present).  This prevents
                   the [1m-k [22moption from interfering with [1msudo [22mcommands run in a dif‐
                   ferent terminal session.   See  the  [4mtimestamp_type[24m  option  in
                   [4msudoers[24m(5)  for more information.  This option does not require
                   a password, and was added to allow a user to revoke  [1msudo  [22mper‐
                   missions from a [4m.logout[24m file.
    
                   When  used  in conjunction with a [4mcommand[24m or an option that may
                   require a password, this option will cause [1msudo [22mto  ignore  the
                   user's cached credentials.  As a result, [1msudo [22mwill prompt for a
                   password  (if  one is required by the security policy) and will
                   not update the user's cached credentials.
    
                   Not all security policies support credential caching.
    
           [1m-l[22m, [1m--list[0m
                   If no [4mcommand[24m is specified, list the privileges for the  invok‐
                   ing  user  (or the [4muser[24m specified by the [1m-U [22moption) on the cur‐
                   rent host.  A longer list format is  used  if  this  option  is
                   specified  multiple  times  and  the security policy supports a
                   verbose output format.
    
                   If a [4mcommand[24m is specified and is permitted by the security pol‐
                   icy for the invoking user (or the, [4muser[24m specified by the [1m-U [22mop‐
                   tion) on the current host,  the  fully-qualified  path  to  the
                   [4mcommand[24m  is  displayed  along with any [4marg[24ms. If [1m-l [22mis specified
                   more than once (and  the  security  policy  supports  it),  the
                   matching  rule  is displayed in a verbose format along with the
                   [4mcommand[24m.  If a [4mcommand[24m is specified but not allowed by the pol‐
                   icy, [1msudo [22mwill exit with a status value of 1.
    
           [1m-N[22m, [1m--no-update[0m
                   Do not update the user's cached credentials, even if  the  user
                   successfully  authenticates.   Unlike  the  [1m-k  [22mflag,  existing
                   cached credentials are used if they are valid.  To detect  when
                   the user's cached credentials are valid (or when no authentica‐
                   tion is required), the following can be used:
    
                       sudo -Nnv
    
                   Not all security policies support credential caching.
    
           [1m-n[22m, [1m--non-interactive[0m
                   Avoid  prompting the user for input of any kind.  If a password
                   is required for the [4mcommand[24m to run, [1msudo [22mwill display an  error
                   message and exit.
    
           [1m-P[22m, [1m--preserve-groups[0m
                   Preserve  the  invoking  user's group vector unaltered.  By de‐
                   fault, the [4msudoers[24m policy will initialize the group  vector  to
                   the  list  of  groups the target user is a member of.  The real
                   and effective group-IDs, however, are still set  to  match  the
                   target user.
    
           [1m-p [4m[22mprompt[24m, [1m--prompt[22m=[4mprompt[0m
                   Use  a  custom  password prompt with optional escape sequences.
                   The following percent (‘%’) escape sequences are  supported  by
                   the [4msudoers[24m policy:
    
                   %H  expanded  to  the host name including the domain name (only
                       if the machine's host name is fully qualified or  the  [4mfqdn[0m
                       option is set in [4msudoers[24m(5))
    
                   %h  expanded to the local host name without the domain name
    
                   %p  expanded  to  the  name of the user whose password is being
                       requested (respects the [4mrootpw[24m, [4mtargetpw[24m, and [4mrunaspw[24m flags
                       in [4msudoers[24m(5))
    
                   %U  expanded to the login name of the user the [4mcommand[24m will  be
                       run as (defaults to root unless the [1m-u [22moption is also spec‐
                       ified)
    
                   %u  expanded to the invoking user's login name
    
                   %%  two  consecutive ‘%’ characters are collapsed into a single
                       ‘%’ character
    
                   The custom prompt will override the default prompt specified by
                   either the security policy or the SUDO_PROMPT environment vari‐
                   able.  On systems that use PAM, the  custom  prompt  will  also
                   override  the  prompt  specified  by  a  PAM  module unless the
                   [4mpassprompt_override[24m flag is disabled in [4msudoers[24m.
    
           [1m-R [4m[22mdirectory[24m, [1m--chroot[22m=[4mdirectory[0m
                   Change to the specified root [4mdirectory[24m (see  [4mchroot[24m(8))  before
                   running  the  [4mcommand[24m.  The security policy may return an error
                   if the user does not have permission to specify the root direc‐
                   tory.
    
           [1m-r [4m[22mrole[24m, [1m--role[22m=[4mrole[0m
                   Run the [4mcommand[24m with an SELinux security context that  includes
                   the specified [4mrole[24m.
    
           [1m-S[22m, [1m--stdin[0m
                   Write  the  prompt  to the standard error and read the password
                   from the standard input instead of using the terminal device.
    
           [1m-s[22m, [1m--shell[0m
                   Run the shell specified by the SHELL environment variable if it
                   is set or the shell specified by the invoking  user's  password
                   database entry.  If a [4mcommand[24m is specified, it is passed to the
                   shell as a simple command using the [1m-c [22moption.  The [4mcommand[24m and
                   any  [4marg[24ms are concatenated, separated by spaces, after escaping
                   each character (including white space) with a  backslash  (‘\’)
                   except  for  alphanumerics,  underscores,  hyphens,  and dollar
                   signs.  If no [4mcommand[24m is specified, an interactive shell is ex‐
                   ecuted.  Most shells behave differently when a [4mcommand[24m is spec‐
                   ified as  compared  to  an  interactive  session;  consult  the
                   shell's manual for details.
    
           [1m-t [4m[22mtype[24m, [1m--type[22m=[4mtype[0m
                   Run  the [4mcommand[24m with an SELinux security context that includes
                   the specified [4mtype[24m.  If no [4mtype[24m is specified, the default  type
                   is derived from the role.
    
           [1m-U [4m[22muser[24m, [1m--other-user[22m=[4muser[0m
                   Used  in  conjunction with the [1m-l [22moption to list the privileges
                   for [4muser[24m instead of for the invoking user.  The security policy
                   may restrict listing other users' privileges.  When  using  the
                   [4msudoers[24m  policy,  the  [1m-U [22moption is restricted to the root user
                   and users with either the “list”  priviege  for  the  specified
                   [4muser[24m  or  the ability to run any [4mcommand[24m as root or [4muser[24m on the
                   current host.
    
           [1m-T [4m[22mtimeout[24m, [1m--command-timeout[22m=[4mtimeout[0m
                   Used to set a timeout for the [4mcommand[24m.  If the timeout  expires
                   before  the [4mcommand[24m has exited, the [4mcommand[24m will be terminated.
                   The security policy may restrict  the  user's  ability  to  set
                   timeouts.   The  [4msudoers[24m  policy  requires  that user-specified
                   timeouts be explicitly enabled.
    
           [1m-u [4m[22muser[24m, [1m--user[22m=[4muser[0m
                   Run the [4mcommand[24m as a user other than the  default  target  user
                   (usually  [1mroot[22m).   The  [4muser[24m may be either a user name or a nu‐
                   meric user-ID (UID) prefixed with the ‘#’ character (e.g., ‘#0’
                   for UID 0).  When running [4mcommand[24ms as a UID,  many  shells  re‐
                   quire that the ‘#’ be escaped with a backslash (‘\’).  Some se‐
                   curity  policies may restrict UIDs to those listed in the pass‐
                   word database.  The [4msudoers[24m policy allows UIDs that are not  in
                   the  password  database  as  long as the [4mtargetpw[24m option is not
                   set.  Other security policies may not support this.
    
           [1m-V[22m, [1m--version[0m
                   Print the [1msudo [22mversion string as well as the version string  of
                   any  configured plugins.  If the invoking user is already root,
                   the [1m-V [22moption will display the options passed to configure when
                   [1msudo [22mwas built; plugins may display additional information such
                   as default options.
    
           [1m-v[22m, [1m--validate[0m
                   Update the user's cached credentials, authenticating  the  user
                   if  necessary.   For  the [4msudoers[24m plugin, this extends the [1msudo[0m
                   timeout for another 15 minutes by default, but does not  run  a
                   [4mcommand[24m.  Not all security policies support cached credentials.
    
           [1m--      [22mThe  [1m-- [22mis used to delimit the end of the [1msudo [22moptions.  Subse‐
                   quent options are passed to the [4mcommand[24m.
    
           Options that take a value may only be specified once  unless  otherwise
           indicated  in  the description.  This is to help guard against problems
           caused by poorly written scripts that invoke [1msudo [22mwith  user-controlled
           input.
    
           Environment  variables  to be set for the [4mcommand[24m may also be passed as
           options   to   [1msudo   [22min    the    form    [4mVAR[24m=[4mvalue[24m,    for    example
           LD_LIBRARY_PATH=[4m/usr/local/pkg/lib[24m.   Environment variables may be sub‐
           ject to restrictions  imposed  by  the  security  policy  plugin.   The
           [4msudoers[24m  policy subjects environment variables passed as options to the
           same restrictions as existing environment variables with one  important
           difference.   If the [4msetenv[24m option is set in [4msudoers[24m, the [4mcommand[24m to be
           run has the SETENV tag set or the [4mcommand[24m matched is [1mALL[22m, the user  may
           set  variables  that  would otherwise be forbidden.  See [4msudoers[24m(5) for
           more information.
    
    [1mCOMMAND EXECUTION[0m
           When [1msudo [22mexecutes a [4mcommand[24m, the security policy specifies the  execu‐
           tion  environment  for  the [4mcommand[24m.  Typically, the real and effective
           user and group and IDs are set to match those of the  target  user,  as
           specified in the password database, and the group vector is initialized
           based on the group database (unless the [1m-P [22moption was specified).
    
           The following parameters may be specified by security policy:
    
           [1m•  [22mreal and effective user-ID
    
           [1m•  [22mreal and effective group-ID
    
           [1m•  [22msupplementary group-IDs
    
           [1m•  [22mthe environment list
    
           [1m•  [22mcurrent working directory
    
           [1m•  [22mfile creation mode mask (umask)
    
           [1m•  [22mSELinux role and type
    
           [1m•  [22mscheduling priority (aka nice value)
    
       [1mProcess model[0m
           There are two distinct ways [1msudo [22mcan run a [4mcommand[24m.
    
           If  an  I/O logging plugin is configured to log terminal I/O, or if the
           security policy explicitly requests it, a new  pseudo-terminal  (“pty”)
           is  allocated  and [4mfork[24m(2) is used to create a second [1msudo [22mprocess, re‐
           ferred to as the [4mmonitor[24m.  The [4mmonitor[24m creates a new  terminal  session
           with  itself  as  the  leader  and the pty as its controlling terminal,
           calls [4mfork[24m(2) again, sets up the  execution  environment  as  described
           above,  and  then  uses the [4mexecve[24m(2) system call to run the [4mcommand[24m in
           the child process.  The [4mmonitor[24m exists to relay job control signals be‐
           tween the user's terminal and the pty the  [4mcommand[24m  is  being  run  in.
           This  makes  it  possible  to  suspend and resume the [4mcommand[24m normally.
           Without the [4mmonitor[24m, the [4mcommand[24m  would  be  in  what  POSIX  terms  an
           “orphaned  process group” and it would not receive any job control sig‐
           nals from the kernel.  When the [4mcommand[24m exits or  is  terminated  by  a
           signal,  the  [4mmonitor[24m passes the [4mcommand[24m's exit status to the main [1msudo[0m
           process and exits.  After receiving the [4mcommand[24m's exit status, the main
           [1msudo [22mprocess passes the [4mcommand[24m's exit status to the security  policy's
           close  function,  as well as the close function of any configured audit
           plugin, and exits.  This mode is the default for sudo  versions  1.9.14
           and above when using the sudoers policy.
    
           If  no  pty is used, [1msudo [22mcalls [4mfork[24m(2), sets up the execution environ‐
           ment as described above, and uses the [4mexecve[24m(2) system call to run  the
           [4mcommand[24m  in  the  child process.  The main [1msudo [22mprocess waits until the
           [4mcommand[24m has completed, then passes the [4mcommand[24m's exit status to the se‐
           curity policy's close function, as well as the close  function  of  any
           configured  audit plugins, and exits.  As a special case, if the policy
           plugin does not define a close function, [1msudo [22mwill execute the  [4mcommand[0m
           directly  instead  of calling [4mfork[24m(2) first.  The [4msudoers[24m policy plugin
           will only define a close function when I/O logging is enabled, a pty is
           required, an SELinux role is specified, the [4mcommand[24m has  an  associated
           timeout,  or  the [4mpam_session[24m or [4mpam_setcred[24m options are enabled.  Both
           [4mpam_session[24m and [4mpam_setcred[24m are enabled by  default  on  systems  using
           PAM.   This  mode is the default for sudo versions prior to 1.9.14 when
           using the sudoers policy.
    
           On systems that use PAM, the security policy's close  function  is  re‐
           sponsible  for  closing the PAM session.  It may also log the [4mcommand[24m's
           exit status.
    
       [1mSignal handling[0m
           When the [4mcommand[24m is run as a child of the [1msudo [22mprocess, [1msudo [22mwill relay
           signals it receives to the [4mcommand[24m.  The SIGINT and SIGQUIT signals are
           only relayed when the [4mcommand[24m is being run in a new  pty  or  when  the
           signal  was  sent by a user process, not the kernel.  This prevents the
           [4mcommand[24m from receiving SIGINT twice each time the user enters  control-
           C.   Some  signals,  such  as SIGSTOP and SIGKILL, cannot be caught and
           thus will not be relayed to the [4mcommand[24m.  As a  general  rule,  SIGTSTP
           should  be  used  instead of SIGSTOP when you wish to suspend a [4mcommand[0m
           being run by [1msudo[22m.
    
           As a special case, [1msudo [22mwill not relay signals that were  sent  by  the
           [4mcommand[24m  it  is  running.   This prevents the [4mcommand[24m from accidentally
           killing itself.  On some systems, the [4mreboot[24m(8) utility  sends  SIGTERM
           to all non-system processes other than itself before rebooting the sys‐
           tem.   This  prevents [1msudo [22mfrom relaying the SIGTERM signal it received
           back to [4mreboot[24m(8), which might then exit before the system was actually
           rebooted, leaving it in a half-dead state similar to single user  mode.
           Note,  however, that this check only applies to the [4mcommand[24m run by [1msudo[0m
           and not any other processes that the [4mcommand[24m may create.  As a  result,
           running a script that calls [4mreboot[24m(8) or [4mshutdown[24m(8) via [1msudo [22mmay cause
           the  system  to  end up in this undefined state unless the [4mreboot[24m(8) or
           [4mshutdown[24m(8) are run using the [1mexec[22m() family  of  functions  instead  of
           [1msystem[22m()  (which interposes a shell between the [4mcommand[24m and the calling
           process).
    
       [1mPlugins[0m
           Plugins may be specified via  [4mPlugin[24m  directives  in  the  [4msudo.conf[24m(5)
           file.   They  may  be loaded as dynamic shared objects (on systems that
           support them), or compiled  directly  into  the  [1msudo  [22mbinary.   If  no
           [4msudo.conf[24m(5)  file  is  present,  or  if  it doesn't contain any [4mPlugin[0m
           lines, [1msudo [22mwill use [4msudoers[24m(5) for the policy, auditing, and I/O  log‐
           ging   plugins.   See  the  [4msudo.conf[24m(5)  manual  for  details  of  the
           [4m/etc/sudo.conf[24m file and the [4msudo_plugin[24m(5) manual for more  information
           about the [1msudo [22mplugin architecture.
    
    [1mEXIT VALUE[0m
           Upon  successful execution of a [4mcommand[24m, the exit status from [1msudo [22mwill
           be the exit status of the program that was executed.   If  the  [4mcommand[0m
           terminated  due  to receipt of a signal, [1msudo [22mwill send itself the same
           signal that terminated the [4mcommand[24m.
    
           If the [1m-l [22moption was specified without a [4mcommand[24m, [1msudo [22mwill exit with a
           value of 0 if the user is allowed to run [1msudo  [22mand  they  authenticated
           successfully  (as  required  by  the security policy).  If a [4mcommand[24m is
           specified with the [1m-l [22moption, the exit value will  only  be  0  if  the
           [4mcommand[24m is permitted by the security policy, otherwise it will be 1.
    
           If there is an authentication failure, a configuration/permission prob‐
           lem,  or  if  the  given  [4mcommand[24m cannot be executed, [1msudo [22mexits with a
           value of 1.  In the latter case, the error string  is  printed  to  the
           standard  error.   If  [1msudo  [22mcannot  [4mstat[24m(2) one or more entries in the
           user's PATH, an error is printed to the standard error.  (If the direc‐
           tory does not exist or if it is not really a directory,  the  entry  is
           ignored  and no error is printed.)  This should not happen under normal
           circumstances.   The  most  common  reason  for   [4mstat[24m(2)   to   return
           “permission denied” is if you are running an automounter and one of the
           directories in your PATH is on a machine that is currently unreachable.
    
    [1mSECURITY NOTES[0m
           [1msudo [22mtries to be safe when executing external [4mcommand[24ms.
    
           To prevent command spoofing, [1msudo [22mchecks "." and "" (both denoting cur‐
           rent  directory)  last  when searching for a [4mcommand[24m in the user's PATH
           (if one or both are in the PATH).  Depending on  the  security  policy,
           the  user's  PATH  environment  variable  may be modified, replaced, or
           passed unchanged to the program that [1msudo [22mexecutes.
    
           Users should [4mnever[24m be granted [1msudo [22mprivileges to execute files that are
           writable by the user or that reside in a directory that is writable  by
           the  user.   If  the user can modify or replace the [4mcommand[24m there is no
           way to limit what additional [4mcommand[24ms they can run.
    
           By default, [1msudo [22mwill only log the [4mcommand[24m it explicitly  runs.   If  a
           user runs a [4mcommand[24m such as ‘sudo su’ or ‘sudo sh’, subsequent [4mcommand[24ms
           run  from  that  shell  are not subject to [1msudo[22m's security policy.  The
           same is true for [4mcommand[24ms that offer shell escapes (including most edi‐
           tors).  If I/O logging is enabled, subsequent [4mcommand[24ms will have  their
           input  and/or output logged, but there will not be traditional logs for
           those [4mcommand[24ms. Because of this, care must be taken when  giving  users
           access  to  [4mcommand[24ms via [1msudo [22mto verify that the [4mcommand[24m does not inad‐
           vertently give the user an effective root shell.   For  information  on
           ways  to  address  this,  see  the  [4mPreventing[24m [4mshell[24m [4mescapes[24m section in
           [4msudoers[24m(5).
    
           To prevent the disclosure of potentially  sensitive  information,  [1msudo[0m
           disables  core  dumps by default while it is executing (they are re-en‐
           abled for the [4mcommand[24m that is run).   This  historical  practice  dates
           from  a  time when most operating systems allowed set-user-ID processes
           to dump core by default.  To aid in debugging  [1msudo  [22mcrashes,  you  may
           wish  to re-enable core dumps by setting “disable_coredump” to false in
           the [4msudo.conf[24m(5) file as follows:
    
               Set disable_coredump false
    
           See the [4msudo.conf[24m(5) manual for more information.
    
    [1mENVIRONMENT[0m
           [1msudo [22mutilizes the following environment variables.  The security policy
           has control over the actual content of the [4mcommand[24m's environment.
    
           EDITOR           Default editor to use in [1m-e [22m(sudoedit) mode if neither
                            SUDO_EDITOR nor VISUAL is set.
    
           MAIL             Set to the mail spool of the target user when  the  [1m-i[0m
                            option  is  specified, or when [4menv_reset[24m is enabled in
                            [4msudoers[24m (unless MAIL is present in the [4menv_keep[24m list).
    
           HOME             Set to the home directory of the target user when  the
                            [1m-i  [22mor [1m-H [22moptions are specified, when the [1m-s [22moption is
                            specified  and  [4mset_home[24m  is  set  in  [4msudoers[24m,   when
                            [4malways_set_home[24m   is   enabled  in  [4msudoers[24m,  or  when
                            [4menv_reset[24m is  enabled  in  [4msudoers[24m  and  HOME  is  not
                            present in the [4menv_keep[24m list.
    
           LOGNAME          Set  to  the login name of the target user when the [1m-i[0m
                            option is specified, when the  [4mset_logname[24m  option  is
                            enabled  in  [4msudoers[24m,  or when the [4menv_reset[24m option is
                            enabled in [4msudoers[24m (unless LOGNAME is present  in  the
                            [4menv_keep[24m list).
    
           PATH             May be overridden by the security policy.
    
           SHELL            Used to determine shell to run with [1m-s [22moption.
    
           SUDO_ASKPASS     Specifies  the  path  to a helper program used to read
                            the password if no terminal is available or if the  [1m-A[0m
                            option is specified.
    
           SUDO_COMMAND     Set  to  the  [4mcommand[24m run by sudo, including any [4marg[24ms.
                            The [4marg[24ms are truncated at 4096 characters to prevent a
                            potential execution error.
    
           SUDO_EDITOR      Default editor to use in [1m-e [22m(sudoedit) mode.
    
           SUDO_GID         Set to the group-ID of the user who invoked sudo.
    
           SUDO_PROMPT      Used as the default password prompt unless the [1m-p  [22mop‐
                            tion was specified.
    
           SUDO_PS1         If  set,  PS1 will be set to its value for the program
                            being run.
    
           SUDO_UID         Set to the user-ID of the user who invoked sudo.
    
           SUDO_USER        Set to the login name of the user who invoked sudo.
    
           USER             Set to the same value as LOGNAME, described above.
    
           VISUAL           Default  editor  to  use  in  [1m-e  [22m(sudoedit)  mode  if
                            SUDO_EDITOR is not set.
    
    [1mFILES[0m
           [4m/etc/sudo.conf[24m            [1msudo [22mfront-end configuration
    
    [1mEXAMPLES[0m
           The following examples assume a properly configured security policy.
    
           To get a file listing of an unreadable directory:
    
               $ sudo ls /usr/local/protected
    
           To list the home directory of user yaz on a machine where the file sys‐
           tem holding ~yaz is not exported as root:
    
               $ sudo -u yaz ls ~yaz
    
           To edit the [4mindex.html[24m file as user www:
    
               $ sudoedit -u www ~www/htdocs/index.html
    
           To view system logs only accessible to root and users in the adm group:
    
               $ sudo -g adm more /var/log/syslog
    
           To run an editor as jim with a different primary group:
    
               $ sudoedit -u jim -g audio ~jim/sound.txt
    
           To shut down a machine:
    
               $ sudo shutdown -r +15 "quick reboot"
    
           To make a usage listing of the directories in the /home partition.  The
           [4mcommands[24m  are  run  in  a  sub-shell to allow the ‘cd’ command and file
           redirection to work.
    
               $ sudo sh -c "cd /home ; du -s * | sort -rn > USAGE"
    
    [1mDIAGNOSTICS[0m
           Error messages produced by [1msudo [22minclude:
    
           [1mediting files in a writable directory is not permitted[0m
                 By default, [1msudoedit [22mdoes not permit editing a file when  any  of
                 the  parent  directories are writable by the invoking user.  This
                 avoids a race condition that could allow the user to overwrite an
                 arbitrary file.  See the [4msudoedit_checkdir[24m option  in  [4msudoers[24m(5)
                 for more information.
    
           [1mediting symbolic links is not permitted[0m
                 By  default, [1msudoedit [22mdoes not follow symbolic links when opening
                 files.  See the [4msudoedit_follow[24m option in [4msudoers[24m(5) for more in‐
                 formation.
    
           [1meffective uid is not 0, is sudo installed setuid root?[0m
                 [1msudo [22mwas not run with root privileges.  The [1msudo [22mbinary  must  be
                 owned  by  the root user and have the set-user-ID bit set.  Also,
                 it must not be located on a file system mounted with the ‘nosuid’
                 option or on an NFS file system that maps uid 0  to  an  unprivi‐
                 leged uid.
    
           [1meffective  uid  is  not  0,  is sudo on a file system with the 'nosuid'[0m
                 [1moption set or an NFS file system without root privileges?[0m
                 [1msudo [22mwas not run with root privileges.  The [1msudo [22mbinary  has  the
                 proper  owner  and permissions but it still did not run with root
                 privileges.  The most common reason for this  is  that  the  file
                 system the [1msudo [22mbinary is located on is mounted with the ‘nosuid’
                 option or it is an NFS file system that maps uid 0 to an unprivi‐
                 leged uid.
    
           [1mfatal error, unable to load plugins[0m
                 An error occurred while loading or initializing the plugins spec‐
                 ified in [4msudo.conf[24m(5).
    
           [1minvalid environment variable name[0m
                 One  or  more environment variable names specified via the [1m-E [22mop‐
                 tion contained an equal sign (‘=’).  The arguments to the [1m-E  [22mop‐
                 tion  should  be environment variable names without an associated
                 value.
    
           [1mno password was provided[0m
                 When [1msudo [22mtried to read the password,  it  did  not  receive  any
                 characters.   This may happen if no terminal is available (or the
                 [1m-S [22moption is specified) and the standard  input  has  been  redi‐
                 rected from [4m/dev/null[24m.
    
           [1ma terminal is required to read the password[0m
                 [1msudo  [22mneeds to read the password but there is no mechanism avail‐
                 able for it to do so.  A terminal is  not  present  to  read  the
                 password  from,  [1msudo  [22mhas  not  been configured to read from the
                 standard input, the [1m-S [22moption was not used, and no askpass helper
                 has been specified  either  via  the  [4msudo.conf[24m(5)  file  or  the
                 SUDO_ASKPASS environment variable.
    
           [1mno writable temporary directory found[0m
                 [1msudoedit [22mwas unable to find a usable temporary directory in which
                 to store its intermediate files.
    
           [1mThe  [22m“no  new privileges” flag is set, which prevents sudo from running
                 as root.
                 [1msudo [22mwas run by a process that has the Linux “no new  privileges”
                 flag  is set.  This causes the set-user-ID bit to be ignored when
                 running an executable, which will prevent [1msudo [22mfrom  functioning.
                 The most likely cause for this is running [1msudo [22mwithin a container
                 that  sets  this  flag.   Check the documentation to see if it is
                 possible to configure the container such that  the  flag  is  not
                 set.
    
           [1msudo must be owned by uid 0 and have the setuid bit set[0m
                 [1msudo  [22mwas not run with root privileges.  The [1msudo [22mbinary does not
                 have the correct owner or permissions.  It must be owned  by  the
                 root user and have the set-user-ID bit set.
    
           [1msudoedit is not supported on this platform[0m
                 It  is only possible to run [1msudoedit [22mon systems that support set‐
                 ting the effective user-ID.
    
           [1mtimed out reading password[0m
                 The user did not enter a password before the password timeout  (5
                 minutes by default) expired.
    
           [1myou do not exist in the passwd database[0m
                 Your user-ID does not appear in the system passwd database.
    
           [1myou may not specify environment variables in edit mode[0m
                 It is only possible to specify environment variables when running
                 a  [4mcommand[24m.   When  editing  a  file,  the editor is run with the
                 user's environment unmodified.
    
    [1mSEE ALSO[0m
           [4msu[24m(1), [4mstat[24m(2), [4mlogin_cap[24m(3), [4mpasswd[24m(5), [4msudo.conf[24m(5),  [4msudo_plugin[24m(5),
           [4msudoers[24m(5), [4msudoers_timestamp[24m(5), [4msudoreplay[24m(8), [4mvisudo[24m(8)
    
    [1mHISTORY[0m
           See     the     HISTORY.md    file    in    the    [1msudo    [22mdistribution
           (https://www.sudo.ws/about/history/) for a brief history of sudo.
    
    [1mAUTHORS[0m
           Many people have worked on [1msudo [22mover the years; this  version  consists
           of code written primarily by:
    
                 Todd C. Miller
    
           See    the    CONTRIBUTORS.md    file    in   the   [1msudo   [22mdistribution
           (https://www.sudo.ws/about/contributors/) for  an  exhaustive  list  of
           people who have contributed to [1msudo[22m.
    
    [1mCAVEATS[0m
           There  is  no  easy  way to prevent a user from gaining a root shell if
           that user is allowed to run arbitrary [4mcommands[24m via  [1msudo[22m.   Also,  many
           programs (such as editors) allow the user to run [4mcommand[24ms via shell es‐
           capes,  thus  avoiding  [1msudo[22m's  checks.  However, on most systems it is
           possible to prevent shell escapes with the [4msudoers[24m(5)  plugin's  [4mnoexec[0m
           functionality.
    
           It is not meaningful to run the ‘cd’ [4mcommand[24m directly via sudo, e.g.,
    
               $ sudo cd /usr/local/protected
    
           since when the [4mcommand[24m exits the parent process (your shell) will still
           be  the same.  The [1m-D [22moption can be used to run a [4mcommand[24m in a specific
           [4mdirectory[24m.
    
           Running shell scripts via [1msudo [22mcan expose the  same  kernel  bugs  that
           make  set-user-ID  shell  scripts  unsafe on some operating systems (if
           your OS has a /dev/fd/ directory, set-user-ID shell scripts are  gener‐
           ally safe).
    
    [1mBUGS[0m
           If  you  believe you have found a bug in [1msudo[22m, you can submit a bug re‐
           port at https://bugzilla.sudo.ws/
    
    [1mSUPPORT[0m
           Limited free support is available via the sudo-users mailing list,  see
           https://www.sudo.ws/mailman/listinfo/sudo-users  to subscribe or search
           the archives.
    
    [1mDISCLAIMER[0m
           [1msudo [22mis provided “AS IS” and any express or implied warranties, includ‐
           ing, but not limited to, the implied warranties of merchantability  and
           fitness  for  a  particular purpose are disclaimed.  See the LICENSE.md
           file distributed with [1msudo  [22mor  https://www.sudo.ws/about/license/  for
           complete details.
    
    Sudo 1.9.15p5                   August 9, 2023                         [4mSUDO[24m(8)



```python
! ls /home
```

    bob  kali


## Group Management

- a group represent a group of users
- a user can belong to multiple groups
- allwo to provide permissions based on group to all users in the group
- a user's primary group is listed in `/etc/passwd`
- detailed group infromation is stored in `/etc/group` file
    - each line in the file represents a group
    - each line has 4 sections separated by ':'
    - `group_name:password:group_id:group_list`
- you can see the contents of `/etc/group` file or use `groups <username>`  command


```python
! cat /etc/passwd | grep bob
```

    bob:x:1001:1001:,,,:/home/bob:/bin/bash



```python
! cat /etc/passwd | grep kali
```

    kali:x:1000:1000:kali,,,:/home/kali:/usr/bin/zsh



```python
! grep kali  /etc/group
```

    adm:x:4:kali
    dialout:x:20:kali
    cdrom:x:24:kali
    floppy:x:25:kali
    sudo:x:27:kali
    audio:x:29:pulse,kali
    dip:x:30:kali
    video:x:44:kali
    plugdev:x:46:kali
    users:x:100:kali,bob
    netdev:x:105:kali
    wireshark:x:117:kali
    bluetooth:x:120:kali
    scanner:x:133:saned,kali
    kali-trusted:x:139:
    kali:x:1000:
    kaboxer:x:140:kali



```python
! groups
```

    kali adm dialout cdrom floppy sudo audio dip video plugdev users netdev wireshark bluetooth scanner kaboxer



```python
! groups bob
```

    bob : bob users



```python
# print user's group id
! id -g bob
```

    1001



```python
# print user's group names
! id -Gn bob
```

    bob users


## Add New Group

- use `groupadd` or `addgroup` commands
- let's add a group called `hackers` and add `bob` into the group
- use `usermod` command to modify user's info or add it to group

```bash
usermod -a -G <groupname> <username>
```


```python
! echo kali | sudo -S groupadd hackers
```

    [sudo] password for kali: 


```python
! grep hackers /etc/group
```

    hackers:x:1002:



```python
! echo kali | sudo -S usermod -aG hackers bob
```

    [sudo] password for kali: 


```python
! id -Gn bob
```

    bob users hackers


## Exercise

- create a group called `users` and add `bob`, `alice`, and `kali` to the group
- check the group membership of `bob`, `alice`, and `kali`

## Traditional Permission and Access Control on Linux
- default permission system in Linux is user-group-other:read-write-execute
- you can change the permission using `chmod` command
- `ls -l` command displays the permission of each file and folder
- 3 types of Access Controls on Files and Folders
    - Read
    - Write
    - Execute

### Types of Access on Files

#### read (r)
- user can view/read the contents of the file

#### write (w)
- user can change the contents of the file

#### execute (x)
- user can execute/run the file if it is a program/script

### Types of access on directories

#### read (r)
- user can list the contents of the directory (e.g., using `ls`)

#### write (w)
- user can create files and sub-directories inside the directory

#### execute (x)

- user can enter into the directory (e.g., using `cd`)

- `ls -al <folder>` command displays detail of each file and folder in the provided folder
- provides space separated list of information for each file and folders
- [LS details and contents](https://www.ibm.com/docs/en/i/7.1?topic=directories-ls)

`
 file_type rwx(owner) rwx(group) rwx(other) owner_name group_owner file_size Last_modified_timestamp path_name
`


```python
! ls -al
```

    total 2524
    drwxr-xr-x 11 kali kali   4096 Jan 17 14:08 .
    drwxr-xr-x  3 kali kali   4096 Jan 17 12:07 ..
    drwxr-xr-x  8 kali kali   4096 Jan 17 12:07 .git
    -rw-r--r--  1 kali kali    381 Jan 17 12:07 .gitignore
    drwxr-xr-x  2 kali kali   4096 Jan 17 13:47 .ipynb_checkpoints
    -rw-r--r--  1 kali kali   4535 Jan 17 13:00 00-TableOfContents.ipynb
    -rw-r--r--  1 kali kali  35107 Jan 17 12:07 BufferOverflowBasics.ipynb
    -rw-r--r--  1 kali kali  29856 Jan 17 12:07 BufferOverflowProtections.ipynb
    -rw-r--r--  1 kali kali  25444 Jan 17 12:07 C-Arrays.ipynb
    -rw-r--r--  1 kali kali   7763 Jan 17 12:07 C-Strings.ipynb
    -rw-r--r--  1 kali kali  48332 Jan 17 12:07 CS1-Review.ipynb
    -rw-r--r--  1 kali kali  25731 Jan 17 12:07 CTF.ipynb
    -rw-r--r--  1 kali kali  49945 Jan 17 12:07 DetectingMemoryCorruptionErrors.ipynb
    -rw-r--r--  1 kali kali 229579 Jan 17 12:07 ELF-ReverseEngineeing.ipynb
    -rw-r--r--  1 kali kali    678 Jan 17 12:59 EnvVarSetUID.ipynb
    -rw-r--r--  1 kali kali  25745 Jan 17 12:07 ExploitCode-UsingBashScript.ipynb
    -rw-r--r--  1 kali kali  19210 Jan 17 12:07 ExploitCode-UsingCPP.ipynb
    -rw-r--r--  1 kali kali  27285 Jan 17 12:07 ExploitCode-UsingPythonScript.ipynb
    -rw-r--r--  1 kali kali 411965 Jan 17 12:07 FormatStrings.ipynb
    -rw-r--r--  1 kali kali   6889 Jan 17 12:07 Function-Pointers.ipynb
    -rw-r--r--  1 kali kali  23327 Jan 17 12:07 FunctionsAndUnittesting.ipynb
    -rw-r--r--  1 kali kali   9247 Jan 17 12:07 GDB-Peda.ipynb
    -rw-r--r--  1 kali kali  64126 Jan 17 12:07 GlobalSegmentOverflow.ipynb
    -rw-r--r--  1 kali kali 470021 Jan 17 12:07 HackingTools.ipynb
    -rw-r--r--  1 kali kali  13573 Jan 17 13:49 HeapOverflowExploitation.ipynb
    -rw-r--r--  1 kali kali   1067 Jan 17 12:07 LICENSE
    -rw-r--r--  1 kali kali 130911 Jan 17 14:08 LinuxSecurity.ipynb
    -rw-r--r--  1 kali kali   7272 Jan 17 12:07 MemorySegments.ipynb
    -rw-r--r--  1 kali kali  53905 Jan 17 12:07 OverflowingFunctionPointers.ipynb
    -rw-r--r--  1 kali kali   8484 Jan 17 12:07 PenTesting.ipynb
    -rw-r--r--  1 kali kali  14457 Jan 17 12:07 Pointers.ipynb
    -rw-r--r--  1 kali kali  65722 Jan 17 12:07 PwntoolsTutorials.ipynb
    -rw-r--r--  1 kali kali  42195 Jan 17 12:07 PythonBasics.ipynb
    -rw-r--r--  1 kali kali   2015 Jan 17 12:07 README.md
    -rw-r--r--  1 kali kali  31600 Jan 17 12:07 ROP-new.ipynb
    -rw-r--r--  1 kali kali  27536 Jan 17 12:07 ROP.ipynb
    -rw-r--r--  1 kali kali  14599 Jan 17 12:07 SQLInjection.ipynb
    -rw-r--r--  1 kali kali  14748 Jan 17 12:07 SQLite3-Database.ipynb
    -rw-r--r--  1 kali kali  12099 Jan 17 12:07 SetupVM.ipynb
    -rw-r--r--  1 kali kali  10524 Jan 17 12:07 StackOverflow-EnvVariable.ipynb
    -rw-r--r--  1 kali kali  40501 Jan 17 12:07 StackOverflow-ExecutionFlow.ipynb
    -rw-r--r--  1 kali kali 249078 Jan 17 12:07 StackOverflow-MemoryCorruption.ipynb
    -rw-r--r--  1 kali kali  69909 Jan 17 12:07 StackOverflow-RemoteCodeExecution.ipynb
    -rw-r--r--  1 kali kali    371 Jan 17 12:07 aslr0-1.txt
    -rw-r--r--  1 kali kali    371 Jan 17 12:07 aslr0-2.txt
    -rw-r--r--  1 kali kali    371 Jan 17 12:07 aslr1-1.txt
    -rw-r--r--  1 kali kali    371 Jan 17 12:07 aslr1-2.txt
    -rw-r--r--  1 kali kali    371 Jan 17 12:07 aslr2-1.txt
    -rw-r--r--  1 kali kali    371 Jan 17 12:07 aslr2-2.txt
    -rwxr-xr-x  1 kali kali    699 Jan 17 12:07 compile.sh
    drwxr-xr-x  6 kali kali   4096 Jan 17 12:07 ctf-demos
    drwxr-xr-x 13 kali kali   4096 Jan 17 12:07 demos
    -rw-r--r--  1 kali kali    100 Jan 17 12:07 exploit_fmt.bin
    -rw-r--r--  1 kali kali   1244 Jan 17 12:07 exploit_vuln_cpp.py
    -rw-r--r--  1 kali kali     51 Jan 17 12:07 gdb-so-arg.txt
    -rw-r--r--  1 kali kali     55 Jan 17 12:07 gdb-so-stdio.txt
    -rw-r--r--  1 kali kali   1222 Jan 17 12:07 hacking.h
    drwxr-xr-x  5 kali kali   4096 Jan 17 12:07 labs
    drwxr-xr-x  2 kali kali   4096 Jan 17 12:07 media
    -rw-r--r--  1 kali kali    200 Jan 17 12:07 pattern.txt
    -rw-r--r--  1 kali kali     64 Jan 17 12:07 payload.bin
    -rw-r--r--  1 kali kali     80 Jan 17 12:07 payload_env.bin
    -rw-r--r--  1 kali kali     12 Jan 17 12:07 peda-session-so_arg.exe.txt
    -rw-r--r--  1 kali kali      4 Jan 17 12:07 peda-session-so_stdio.exe.txt
    -rw-r--r--  1 kali kali      1 Jan 17 12:07 peda-session-vuln_cpp.exe.txt
    -rwxr-xr-x  1 kali kali    655 Jan 17 12:07 pwn_io_demo.py
    drwxr-xr-x  5 kali kali   4096 Jan 17 12:07 pwntools-demos
    drwxr-xr-x  2 kali kali   4096 Jan 17 12:07 python
    drwxr-xr-x  2 kali kali   4096 Jan 17 12:07 shellcode
    -rw-r--r--  1 kali kali     24 Jan 17 12:07 shellcode.bin
    -rw-r--r--  1 kali kali     24 Jan 17 12:07 shellcode1_py3.bin
    -rw-r--r--  1 kali kali     24 Jan 17 12:07 shellcode_bash.bin
    -rw-r--r--  1 kali kali     24 Jan 17 12:07 shellcode_py3.bin
    -rw-r--r--  1 kali kali     35 Jan 17 12:07 shellcode_root.bin
    -rwxr-xr-x  1 kali kali    293 Jan 17 12:07 so_arg_bash_exploit_aslr_on.sh
    -rw-r--r--  1 kali kali    101 Jan 17 12:07 so_arg_cpp_exploit.sh
    -rw-r--r--  1 kali kali    110 Jan 17 12:07 so_arg_cpp_exploit_aslr_on.sh
    -rwxr-xr-x  1 kali kali   1997 Jan 17 12:07 so_arg_exploit.sh
    -rw-r--r--  1 kali kali    144 Jan 17 12:07 stdio_payload.bin
    -rw-r--r--  1 kali kali    126 Jan 17 12:07 testfile
    -rw-r--r--  1 kali kali   5893 Jan 17 12:07 x86-AssemblyAndStack.ipynb




## File Access Control List (ACL)

- fine grained ACL
- helps assign permissions to individual users/groups
- coexist with the traditional permission model
- use `setfacl` and `getfacl` commands to set and get file access control list

```bash
setfacl {-m -x} {u, g}:<name>:{r, w, x} <file, directory>
```
- may have to install `acl` and its commands if not available by default

```bash
sudo apt install acl
```


```python
! sudo apt install acl
```

    Reading package lists... Done
    Building dependency tree       
    Reading state information... Done
    The following NEW packages will be installed:
      acl
    0 upgraded, 1 newly installed, 0 to remove and 30 not upgraded.
    Need to get 37.8 kB of archives.
    After this operation, 197 kB of additional disk space will be used.
    Get:1 http://archive.ubuntu.com/ubuntu focal/main amd64 acl amd64 2.2.53-6 [37.8 kB]
    Fetched 37.8 kB in 0s (107 kB/s)[0m[33m
    
    7[0;23r8[1ASelecting previously unselected package acl.
    (Reading database ... 70786 files and directories currently installed.)
    Preparing to unpack .../acl_2.2.53-6_amd64.deb ...
    7[24;0f[42m[30mProgress: [  0%][49m[39m [..........................................................] 87[24;0f[42m[30mProgress: [ 20%][49m[39m [###########...............................................] 8Unpacking acl (2.2.53-6) ...
    7[24;0f[42m[30mProgress: [ 40%][49m[39m [#######################...................................] 8Setting up acl (2.2.53-6) ...
    7[24;0f[42m[30mProgress: [ 60%][49m[39m [##################################........................] 87[24;0f[42m[30mProgress: [ 80%][49m[39m [##############################################............] 8Processing triggers for man-db (2.9.1-1) ...
    
    7[0;24r8[1A[J


```python
! man setfacl
```

    SETFACL(1)                   Access Control Lists                   SETFACL(1)
    
    NAME
           setfacl - set file access control lists
    
    SYNOPSIS
           setfacl [-bkndRLPvh] [{-m|-x} acl_spec] [{-M|-X} acl_file] file ...
    
           setfacl --restore={file|-}
    
    DESCRIPTION
           This utility sets Access Control Lists (ACLs) of files and directories.
           On the command line, a sequence of commands is followed by  a  sequence
           of  files  (which  in  turn can be followed by another sequence of com‐
           mands, ...).
    
           The -m and -x options expect an ACL on the command line.  Multiple  ACL
           entries  are separated by comma characters (`,'). The -M and -X options
           read an ACL from a file or from standard input. The ACL entry format is
           described in Section ACL ENTRIES.
    
           The  --set and --set-file options set the ACL of a file or a directory.
           The previous ACL is replaced.  ACL entries for this operation must  in‐
           clude permissions.
    
           The  -m  (--modify)  and -M (--modify-file) options modify the ACL of a
           file or directory.  ACL entries for this operation must include permis‐
           sions.
    
           The -x (--remove) and -X (--remove-file) options remove ACL entries. It
           is not an error to remove an entry which does not exist.  Only ACL  en‐
           tries  without  the  perms  field  are  accepted  as parameters, unless
           POSIXLY_CORRECT is defined.
    
           When reading from files using the -M and -X  options,  setfacl  accepts
           the  output getfacl produces.  There is at most one ACL entry per line.
           After a Pound sign (`#'), everything up to  the  end  of  the  line  is
           treated as a comment.
    
           If  setfacl  is used on a file system which does not support ACLs, set‐
           facl operates on the file mode permission bits. If the ACL does not fit
           completely  in the permission bits, setfacl modifies the file mode per‐
           mission bits to reflect the ACL as closely as possible, writes an error
           message to standard error, and returns with an exit status greater than
           0.
    
       PERMISSIONS
           The file owner and processes capable  of  CAP_FOWNER  are  granted  the
           right  to  modify  ACLs of a file. This is analogous to the permissions
           required for accessing the file mode. (On current Linux  systems,  root
           is the only user with the CAP_FOWNER capability.)
    
    OPTIONS
           -b, --remove-all
               Remove all extended ACL entries. The base ACL entries of the owner,
               group and others are retained.
    
           -k, --remove-default
               Remove the Default ACL. If no Default ACL exists, no  warnings  are
               issued.
    
           -n, --no-mask
               Do  not recalculate the effective rights mask. The default behavior
               of setfacl is to recalculate the ACL mask entry, unless a mask  en‐
               try  was  explicitly  given.  The mask entry is set to the union of
               all permissions of the owning group, and all named user  and  group
               entries.  (These  are  exactly the entries affected by the mask en‐
               try).
    
           --mask
               Do recalculate the effective rights mask, even if an ACL mask entry
               was explicitly given. (See the -n option.)
    
           -d, --default
               All operations apply to the Default ACL. Regular ACL entries in the
               input set are promoted to Default ACL entries. Default ACL  entries
               in  the  input set are discarded. (A warning is issued if that hap‐
               pens).
    
           --restore={file|-}
               Restore a permission backup created by `getfacl -R' or similar. All
               permissions of a complete directory subtree are restored using this
               mechanism. If the input contains owner comments or group  comments,
               setfacl  attempts to restore the owner and owning group. If the in‐
               put contains flags comments (which define the setuid,  setgid,  and
               sticky bits), setfacl sets those three bits accordingly; otherwise,
               it clears them. This option cannot be mixed with other options  ex‐
               cept  `--test'.  If the file specified is '-', then it will be read
               from standard input.
    
           --test
               Test mode. Instead of changing the ACLs of any files, the resulting
               ACLs are listed.
    
           -R, --recursive
               Apply operations to all files and directories recursively. This op‐
               tion cannot be mixed with `--restore'.
    
           -L, --logical
               Logical walk, follow symbolic links to directories. The default be‐
               havior  is  to  follow  symbolic  link arguments, and skip symbolic
               links encountered in subdirectories.  Only effective in combination
               with -R.  This option cannot be mixed with `--restore'.
    
           -P, --physical
               Physical  walk,  do not follow symbolic links to directories.  This
               also skips symbolic link arguments.  Only effective in  combination
               with -R.  This option cannot be mixed with `--restore'.
    
           -v, --version
               Print the version of setfacl and exit.
    
           -h, --help
               Print help explaining the command line options.
    
           --  End  of  command  line options. All remaining parameters are inter‐
               preted as file names, even if they start with a dash.
    
           -   If the file name parameter is a single dash, setfacl reads  a  list
               of files from standard input.
    
       ACL ENTRIES
           The  setfacl utility recognizes the following ACL entry formats (blanks
           inserted for clarity):
    
           [d[efault]:] [u[ser]:]uid [:perms]
                  Permissions of a named user. Permissions of the  file  owner  if
                  uid is empty.
    
           [d[efault]:] g[roup]:gid [:perms]
                  Permissions of a named group. Permissions of the owning group if
                  gid is empty.
    
           [d[efault]:] m[ask][:] [:perms]
                  Effective rights mask
    
           [d[efault]:] o[ther][:] [:perms]
                  Permissions of others.
    
           Whitespace between delimiter characters and non-delimiter characters is
           ignored.
    
           Proper ACL entries including permissions are used in modify and set op‐
           erations. (options -m, -M, --set and --set-file).  Entries without  the
           perms field are used for deletion of entries (options -x and -X).
    
           For  uid  and gid you can specify either a name or a number.  Character
           literals may be specified with a backslash followed by the 3-digit  oc‐
           tal  digits  corresponding  to  the ASCII code for the character (e.g.,
           \101 for 'A').  If the name contains a literal backslash followed by  3
           digits, the backslash must be escaped (i.e., \\).
    
           The  perms  field is a combination of characters that indicate the read
           (r), write (w), execute (x) permissions.  Dash characters in the  perms
           field  (-) are ignored.  The character X stands for the execute permis‐
           sion if the file is a directory or already has execute  permission  for
           some  user.   Alternatively, the perms field can define the permissions
           numerically, as a bit-wise combination of read (4), write (2), and exe‐
           cute  (1).   Zero  perms  fields  or  perms fields that only consist of
           dashes indicate no permissions.
    
       AUTOMATICALLY CREATED ENTRIES
           Initially, files and directories contain only the three  base  ACL  en‐
           tries  for  the owner, the group, and others. There are some rules that
           need to be satisfied in order for an ACL to be valid:
    
           *   The three base entries cannot be removed. There must be exactly one
               entry of each of these base entry types.
    
           *   Whenever an ACL contains named user entries or named group objects,
               it must also contain an effective rights mask.
    
           *   Whenever an ACL contains any Default ACL entries, the three Default
               ACL base entries (default owner, default group, and default others)
               must also exist.
    
           *   Whenever a Default ACL contains named user entries or  named  group
               objects, it must also contain a default effective rights mask.
    
           To  help  the user ensure these rules, setfacl creates entries from ex‐
           isting entries under the following conditions:
    
           *   If an ACL contains named user or named group entries, and  no  mask
               entry  exists,  a mask entry containing the same permissions as the
               group entry is created. Unless the -n option is given, the  permis‐
               sions  of  the mask entry are further adjusted to include the union
               of all permissions affected by the mask entry. (See the  -n  option
               description).
    
           *   If  a Default ACL entry is created, and the Default ACL contains no
               owner, owning group, or others entry, a copy of the ACL owner, own‐
               ing group, or others entry is added to the Default ACL.
    
           *   If  a  Default  ACL  contains named user entries or named group en‐
               tries, and no mask entry exists, a mask entry containing  the  same
               permissions  as the default Default ACL's group entry is added. Un‐
               less the -n option is given, the permissions of the mask entry  are
               further  adjusted  to include the union of all permissions affected
               by the mask entry. (See the -n option description).
    
    EXAMPLES
           Granting an additional user read access
                  setfacl -m u:lisa:r file
    
           Revoking write access from all groups and all named  users  (using  the
           effective rights mask)
                  setfacl -m m::rx file
    
           Removing a named group entry from a file's ACL
                  setfacl -x g:staff file
    
           Copying the ACL of one file to another
                  getfacl file1 | setfacl --set-file=- file2
    
           Copying the access ACL into the Default ACL
                  getfacl --access dir | setfacl -d -M- dir
    
    CONFORMANCE TO POSIX 1003.1e DRAFT STANDARD 17
           If the environment variable POSIXLY_CORRECT is defined, the default be‐
           havior of setfacl changes as follows: All non-standard options are dis‐
           abled.   The  ``default:''  prefix  is disabled.  The -x and -X options
           also accept permission fields (and ignore them).
    
    AUTHOR
           Andreas Gruenbacher, <andreas.gruenbacher@gmail.com>.
    
           Please send your bug reports, suggested features and  comments  to  the
           above address.
    
    SEE ALSO
           getfacl(1), chmod(1), umask(1), acl(5)
    
    May 2000                      ACL File Utilities                    SETFACL(1)



```python
! echo "Hello" > example.txt
```


```python
! cat example.txt
```

    Hello



```python
! getfacl example.txt
```

    # file: example.txt
    # owner: kali
    # group: kali
    user::rw-
    group::r--
    other::r--
    



```python
! setfacl -m u:bob:rw- example.txt
```


```python
! getfacl example.txt
```

    # file: example.txt
    # owner: kali
    # group: kali
    user::rw-
    user:bob:rw-
    group::r--
    mask::rw-
    other::r--
    



```python
# note the + after permission
# indicates that ACLs are defined
! ls -al example.txt
```

    -rw-rw-r--+ 1 kali kali 6 Jan 17 16:08 example.txt


## Exercise

- create a file called `secret.txt` and assign read and write permission to `bob`
- assign only write permission to `alice`
- assign read permission to group `users`
- note ACL permissions on the file

## Running Command With Privilege

### sudo
- super-user do
- run command as a superuser
- a user must be authorized or be in `sudoers` group which is in the `/etc/sudoers`


```python
! echo kali | sudo -S cat /etc/sudoers
```

    [sudo] password for kali: #
    # This file MUST be edited with the 'visudo' command as root.
    #
    # Please consider adding local content in /etc/sudoers.d/ instead of
    # directly modifying this file.
    #
    # See the man page for details on how to write a sudoers file.
    #
    Defaults	env_reset
    Defaults	mail_badpass
    Defaults	secure_path="/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"
    
    # This fixes CVE-2005-4890 and possibly breaks some versions of kdesu
    # (#1011624, https://bugs.kde.org/show_bug.cgi?id=452532)
    Defaults	use_pty
    
    # This preserves proxy settings from user environments of root
    # equivalent users (group sudo)
    #Defaults:%sudo env_keep += "http_proxy https_proxy ftp_proxy all_proxy no_proxy"
    
    # This allows running arbitrary commands, but so does ALL, and it means
    # different sudoers have their choice of editor respected.
    #Defaults:%sudo env_keep += "EDITOR"
    
    # Completely harmless preservation of a user preference.
    #Defaults:%sudo env_keep += "GREP_COLOR"
    
    # While you shouldn't normally run git as root, you need to with etckeeper
    #Defaults:%sudo env_keep += "GIT_AUTHOR_* GIT_COMMITTER_*"
    
    # Per-user preferences; root won't have sensible values for them.
    #Defaults:%sudo env_keep += "EMAIL DEBEMAIL DEBFULLNAME"
    
    # "sudo scp" or "sudo rsync" should be able to use your SSH agent.
    #Defaults:%sudo env_keep += "SSH_AGENT_PID SSH_AUTH_SOCK"
    
    # Ditto for GPG agent
    #Defaults:%sudo env_keep += "GPG_AGENT_INFO"
    
    # Host alias specification
    
    # User alias specification
    
    # Cmnd alias specification
    
    # User privilege specification
    root	ALL=(ALL:ALL) ALL
    
    # Allow members of group sudo to execute any command
    %sudo	ALL=(ALL:ALL) ALL
    
    # See sudoers(5) for more information on "@include" directives:
    
    @includedir /etc/sudoers.d



```python
! groups kali
```

    kali : kali adm dialout cdrom floppy sudo audio dip video plugdev users netdev wireshark bluetooth scanner kaboxer



```python
! grep kali /etc/group 
```

    adm:x:4:kali
    dialout:x:20:kali
    cdrom:x:24:kali
    floppy:x:25:kali
    sudo:x:27:kali
    audio:x:29:pulse,kali
    dip:x:30:kali
    video:x:44:kali
    plugdev:x:46:kali
    users:x:100:kali,bob
    netdev:x:105:kali
    wireshark:x:117:kali
    bluetooth:x:120:kali
    scanner:x:133:saned,kali
    kali-trusted:x:139:
    kali:x:1000:
    kaboxer:x:140:kali


## Adding User to sudo group
- use `usermod` command


```python
! echo kali | sudo -S usermod -aG sudo bob
```

    [sudo] password for kali: 


```python
! groups bob
```

    bob : bob sudo users hackers


## Exercise

- add `kali` and `alice` to `sudo` group
- switch to `kali` user and run `sudo` command
- check if `kali` can run command with `sudo`

## Removing User from group

```bash
gpasswd --deletee user group
deluser user group
```


```python
! gpasswd
```

    Usage: gpasswd [option] GROUP
    
    Options:
      -a, --add USER                add USER to GROUP
      -d, --delete USER             remove USER from GROUP
      -h, --help                    display this help message and exit
      -Q, --root CHROOT_DIR         directory to chroot into
      -r, --remove-password         remove the GROUP's password
      -R, --restrict                restrict access to GROUP to its members
      -M, --members USER,...        set the list of members of GROUP
      -A, --administrators ADMIN,...
                                    set the list of administrators for GROUP
    Except for the -A and -M options, the options cannot be combined.



```python
! echo kali | sudo -S gpasswd -d bob sudo
```

    [sudo] password for kali: Removing user bob from group sudo



```python
! groups bob
```

    bob : bob users hackers


## Root Login

- newer Kali and Ubuntu 20.04 and newer don't allow root login
    - why is it a good idea?
- not recommended to run commands using a root shell
    - use sudo to run individual commands
    
### Switch to root shell

```bash
sudo -s
sudo bash
sudo su
```

### Running commands as another user

- default is `root`; but can run command as another user

```bash
sudo -u <another_user> <command>
```


```python
! echo kali | sudo -S -u bob id
```

    [sudo] password for kali: uid=1001(bob) gid=1001(bob) groups=1001(bob),100(users),1002(hackers)


## Linux/POSIX Capabilities

- divide the root privilege into smaller privilege units called **capabalities**
- capabilities are assigned to processes to bypass or set what the spawned process is capable of doing

- example capabiltiies:

### CAP_CHOWN
- Make arbitrary changes to file UIDs and GIDs

### CAP_DAC_OVERRIDE
- Bypass file read/write/execute permission checks

### CAP_DAC_READ_SEARCH
- Bypass the file read permission checks

### CAP_NET_RAW
- use RAW and PACKET sockets

- https://manpages.ubuntu.com/manpages/noble/en/man7/capabilities.7.html

```bash
sudo setcap CAP_NAME={effective|permitted} <file_name>
```

! man capabilities


```python
# copy bash program into the current directory
! cp /bin/bash mybash
```


```python
! getfacl mybash
```

    # file: mybash
    # owner: kali
    # group: kali
    user::rwx
    group::r-x
    other::r-x
    



```python
! getcap mybash
```


```python
! ls mybash -al
```

    -rwxr-xr-x 1 kali kali 1277936 Jan 17 17:13 mybash



```python
# permission is denied to read /etc/shadow file
! cat < /etc/shadow
```

    zsh:1: permission denied: /etc/shadow



```python
! getfacl /etc/shadow
```

    getfacl: Removing leading '/' from absolute path names
    # file: etc/shadow
    # owner: root
    # group: shadow
    user::rw-
    group::r--
    other::---
    



```python
# let's set the capability to by pass file read permission check
! echo kali | sudo -S setcap CAP_DAC_READ_SEARCH=ep mybash
```

    [sudo] password for kali: 


```python
! getcap mybash
```

    mybash cap_dac_read_search=ep


- run the mybash from Terminal after setting CAP_DAC_READ_SEARCH

```bash
(base) ┌──(kali㉿kali)-[~/projects/SoftwareSecurity]
└─$ ./mybash         
┌──(kali㉿kali)-[~/projects/SoftwareSecurity]
└─$ cat < /etc/shadow
root:!:19662:0:99999:7:::
daemon:*:19662:0:99999:7:::
bin:*:19662:0:99999:7:::
sys:*:19662:0:99999:7:::
sync:*:19662:0:99999:7:::
games:*:19662:0:99999:7:::
man:*:19662:0:99999:7:::
lp:*:19662:0:99999:7:::
mail:*:19662:0:99999:7:::
news:*:19662:0:99999:7:::
uucp:*:19662:0:99999:7:::
proxy:*:19662:0:99999:7:::
www-data:*:19662:0:99999:7:::
backup:*:19662:0:99999:7:::
list:*:19662:0:99999:7:::
irc:*:19662:0:99999:7:::
_apt:*:19662:0:99999:7:::
nobody:*:19662:0:99999:7:::
systemd-network:!*:19662::::::
_galera:!:19662::::::
mysql:!:19662::::::
tss:!:19662::::::
strongswan:!:19662::::::
systemd-timesync:!*:19662::::::
redsocks:!:19662::::::
rwhod:!:19662::::::
_gophish:!:19662::::::
iodine:!:19662::::::
messagebus:!:19662::::::
miredo:!:19662::::::
redis:!:19662::::::
usbmux:!:19662::::::
mosquitto:!:19662::::::
tcpdump:!:19662::::::
sshd:!:19662::::::
_rpc:!:19662::::::
dnsmasq:!:19662::::::
statd:!:19662::::::
avahi:!:19662::::::
stunnel4:!*:19662::::::
Debian-snmp:!:19662::::::
_gvm:!:19662::::::
speech-dispatcher:!:19662::::::
sslh:!:19662::::::
postgres:!:19662::::::
pulse:!:19662::::::
inetsim:!:19662::::::
lightdm:!:19662::::::
geoclue:!:19662::::::
saned:!:19662::::::
polkitd:!*:19662::::::
rtkit:!:19662::::::
colord:!:19662::::::
nm-openvpn:!:19662::::::
nm-openconnect:!:19662::::::
kali:$y$j9T$K/agt9PK0eYweoRVbd9yK0$6BUblkWhNb00ZJWw7nYm0fZL1bpF.1BAs5h0CRF4xj2:19662:0:99999:7:::
bob:$y$j9T$KoIINXes24L0Z2qTeAdEX0$aNnlXFBUXuSZrsiCKPFPCxMIz/KNoz0gIzFnOppN2F1:19739:0:99999:7:::
```
    
### Case Study 1 - Wireshark

- Wireshark is a sniffing tool that needs root privilege
- the GUI is not privileged
- the backend sniffing part uses priviledge `dumpcap`


```python
! getcap /usr/bin/dumpcap
```

    /usr/bin/dumpcap cap_net_admin,cap_net_raw=eip


### Case Study 2 - ping
- the **ping** program uses raw socket
- has the CAP_NET_RAW capability


```python
! getcap /usr/bin/ping
```

    /usr/bin/ping cap_net_raw=ep


## Authentication Methods

- **authentication** is a way to verify user's identity
- multifactor authentication
- typical authentication methods:
1. based on something the **you know**: password
2. based on something the **you have**: ID card
3. based on something the **you are**: fingerprint


## Login Shell/Terminal

- after the user logins, the shell program to execute as the login shell
- the last field in `/etc/passwd` file has the shell program path to be executed
- `x` in the second column means the passowrd is stored somewhere else
    - password can still be stored in this file, but not normally
- passwords are hashed/salted and stored in `/etc/shadow` file
- https://www.cyberciti.biz/faq/understanding-etcshadow-file/
- similar to passwd file, shadow file is ':' delimited with the following structure:

```bash
kali:$y$j9T$K/agt9PK0eYweoRVbd9yK0$6BUblkWhNb00ZJWw7nYm0fZL1bpF.1BAs5h0CRF4xj2:19662:0:99999:7:::
usrname:$Algorithm_ID$Salt$Hashed_Password$:last_changed:minimum_days_before_next_pw_change:maximum_days:Warn:Inactive:Expire
```



```python
! cat /etc/passwd
```

    root:x:0:0:root:/root:/usr/bin/zsh
    daemon:x:1:1:daemon:/usr/sbin:/usr/sbin/nologin
    bin:x:2:2:bin:/bin:/usr/sbin/nologin
    sys:x:3:3:sys:/dev:/usr/sbin/nologin
    sync:x:4:65534:sync:/bin:/bin/sync
    games:x:5:60:games:/usr/games:/usr/sbin/nologin
    man:x:6:12:man:/var/cache/man:/usr/sbin/nologin
    lp:x:7:7:lp:/var/spool/lpd:/usr/sbin/nologin
    mail:x:8:8:mail:/var/mail:/usr/sbin/nologin
    news:x:9:9:news:/var/spool/news:/usr/sbin/nologin
    uucp:x:10:10:uucp:/var/spool/uucp:/usr/sbin/nologin
    proxy:x:13:13:proxy:/bin:/usr/sbin/nologin
    www-data:x:33:33:www-data:/var/www:/usr/sbin/nologin
    backup:x:34:34:backup:/var/backups:/usr/sbin/nologin
    list:x:38:38:Mailing List Manager:/var/list:/usr/sbin/nologin
    irc:x:39:39:ircd:/run/ircd:/usr/sbin/nologin
    _apt:x:42:65534::/nonexistent:/usr/sbin/nologin
    nobody:x:65534:65534:nobody:/nonexistent:/usr/sbin/nologin
    systemd-network:x:998:998:systemd Network Management:/:/usr/sbin/nologin
    _galera:x:100:65534::/nonexistent:/usr/sbin/nologin
    mysql:x:101:106:MySQL Server,,,:/nonexistent:/bin/false
    tss:x:102:107:TPM software stack,,,:/var/lib/tpm:/bin/false
    strongswan:x:103:65534::/var/lib/strongswan:/usr/sbin/nologin
    systemd-timesync:x:996:996:systemd Time Synchronization:/:/usr/sbin/nologin
    redsocks:x:104:108::/var/run/redsocks:/usr/sbin/nologin
    rwhod:x:105:65534::/var/spool/rwho:/usr/sbin/nologin
    _gophish:x:106:110::/var/lib/gophish:/usr/sbin/nologin
    iodine:x:107:65534::/run/iodine:/usr/sbin/nologin
    messagebus:x:108:111::/nonexistent:/usr/sbin/nologin
    miredo:x:109:65534::/var/run/miredo:/usr/sbin/nologin
    redis:x:110:114::/var/lib/redis:/usr/sbin/nologin
    usbmux:x:111:46:usbmux daemon,,,:/var/lib/usbmux:/usr/sbin/nologin
    mosquitto:x:112:116::/var/lib/mosquitto:/usr/sbin/nologin
    tcpdump:x:113:118::/nonexistent:/usr/sbin/nologin
    sshd:x:114:65534::/run/sshd:/usr/sbin/nologin
    _rpc:x:115:65534::/run/rpcbind:/usr/sbin/nologin
    dnsmasq:x:116:65534:dnsmasq,,,:/var/lib/misc:/usr/sbin/nologin
    statd:x:117:65534::/var/lib/nfs:/usr/sbin/nologin
    avahi:x:118:122:Avahi mDNS daemon,,,:/run/avahi-daemon:/usr/sbin/nologin
    stunnel4:x:995:995:stunnel service system account:/var/run/stunnel4:/usr/sbin/nologin
    Debian-snmp:x:119:123::/var/lib/snmp:/bin/false
    _gvm:x:120:124::/var/lib/openvas:/usr/sbin/nologin
    speech-dispatcher:x:121:29:Speech Dispatcher,,,:/run/speech-dispatcher:/bin/false
    sslh:x:122:125::/nonexistent:/usr/sbin/nologin
    postgres:x:123:126:PostgreSQL administrator,,,:/var/lib/postgresql:/bin/bash
    pulse:x:124:128:PulseAudio daemon,,,:/run/pulse:/usr/sbin/nologin
    inetsim:x:125:130::/var/lib/inetsim:/usr/sbin/nologin
    lightdm:x:126:131:Light Display Manager:/var/lib/lightdm:/bin/false
    geoclue:x:127:132::/var/lib/geoclue:/usr/sbin/nologin
    saned:x:128:134::/var/lib/saned:/usr/sbin/nologin
    polkitd:x:993:993:polkit:/nonexistent:/usr/sbin/nologin
    rtkit:x:129:135:RealtimeKit,,,:/proc:/usr/sbin/nologin
    colord:x:130:136:colord colour management daemon,,,:/var/lib/colord:/usr/sbin/nologin
    nm-openvpn:x:131:137:NetworkManager OpenVPN,,,:/var/lib/openvpn/chroot:/usr/sbin/nologin
    nm-openconnect:x:132:138:NetworkManager OpenConnect plugin,,,:/var/lib/NetworkManager:/usr/sbin/nologin
    kali:x:1000:1000:kali,,,:/home/kali:/usr/bin/zsh
    bob:x:1001:1001:,,,:/home/bob:/bin/bash



```python
! echo kali | sudo -S su bin
```

    [sudo] password for kali: This account is currently not available.



```python
! echo kali | sudo -S cat /etc/shadow
```

    [sudo] password for kali: root:!:19662:0:99999:7:::
    daemon:*:19662:0:99999:7:::
    bin:*:19662:0:99999:7:::
    sys:*:19662:0:99999:7:::
    sync:*:19662:0:99999:7:::
    games:*:19662:0:99999:7:::
    man:*:19662:0:99999:7:::
    lp:*:19662:0:99999:7:::
    mail:*:19662:0:99999:7:::
    news:*:19662:0:99999:7:::
    uucp:*:19662:0:99999:7:::
    proxy:*:19662:0:99999:7:::
    www-data:*:19662:0:99999:7:::
    backup:*:19662:0:99999:7:::
    list:*:19662:0:99999:7:::
    irc:*:19662:0:99999:7:::
    _apt:*:19662:0:99999:7:::
    nobody:*:19662:0:99999:7:::
    systemd-network:!*:19662::::::
    _galera:!:19662::::::
    mysql:!:19662::::::
    tss:!:19662::::::
    strongswan:!:19662::::::
    systemd-timesync:!*:19662::::::
    redsocks:!:19662::::::
    rwhod:!:19662::::::
    _gophish:!:19662::::::
    iodine:!:19662::::::
    messagebus:!:19662::::::
    miredo:!:19662::::::
    redis:!:19662::::::
    usbmux:!:19662::::::
    mosquitto:!:19662::::::
    tcpdump:!:19662::::::
    sshd:!:19662::::::
    _rpc:!:19662::::::
    dnsmasq:!:19662::::::
    statd:!:19662::::::
    avahi:!:19662::::::
    stunnel4:!*:19662::::::
    Debian-snmp:!:19662::::::
    _gvm:!:19662::::::
    speech-dispatcher:!:19662::::::
    sslh:!:19662::::::
    postgres:!:19662::::::
    pulse:!:19662::::::
    inetsim:!:19662::::::
    lightdm:!:19662::::::
    geoclue:!:19662::::::
    saned:!:19662::::::
    polkitd:!*:19662::::::
    rtkit:!:19662::::::
    colord:!:19662::::::
    nm-openvpn:!:19662::::::
    nm-openconnect:!:19662::::::
    kali:$y$j9T$K/agt9PK0eYweoRVbd9yK0$6BUblkWhNb00ZJWw7nYm0fZL1bpF.1BAs5h0CRF4xj2:19662:0:99999:7:::
    bob:$y$j9T$KoIINXes24L0Z2qTeAdEX0$aNnlXFBUXuSZrsiCKPFPCxMIz/KNoz0gIzFnOppN2F1:19739:0:99999:7:::


## Purpose of Salt

- defeat the brute-force attacks: dictionary attack, rainbow-table attack
- makes the same passwords different when hashed; can't guess password from hash alone

## Locking Account

- put an invalid value in the password field

```bash
root:!:0:099999:7:::
```
- the root account is locked!


```python

```
