# The UNIX™ Programming Environment
## by Brian W. Kernighan & Rob Pike

## Contents

1. UNIX for Beginners
    - 1.1 Getting started
        - Some prerequisites about terminals
        - A Session with UNIX
        - Logging in
        - Typing Commands
        - Strange terminal behavior
        - Mistakes in typing
        - Type-ahead
        - Stopping a program
        - Logging out
        - Mail
        - Writing to other users
        - News
        - The manual
        - Computer-aided instruction
        - Games
    - 1.2 Day-to-day use: files and common commands
        - Creating files - the editor
        - What files are out there?
        - Printing files - `cat` and `pr`
        - Moving, copying, removing files - `mv`, `cp`, `rm`
        - What's in a filename?
        - A handful of useful commands
        - A summary of file system commands
    - 1.3 More about files: directories
        - Changing directory - `cd`
    - 1.4 The shell
        - Filename shorthand       
        - Input-output redirection 
        - Pipes                    
        - Processes                
        - Tailoring the environment
    - 1.5 The rest of the UNIX system
2. The File System
    - 2.1 The basics of files
    - 2.2 What's in a file
    - 2.3 Directories and filenames
    - 2.4 Permissions
    - 2.5 Inodes
    - 2.6 The directory hierarchy
    - 2.7 Devices
3. Using the Shell
    - 3.1 Command line structure
    - 3.2 Metacharacters
        - A digression on `echo`
    - 3.3 Creating new commands
    - 3.4 Command arguments and parameters
    - 3.5 Program output as arguments
    - 3.6 Shell variables
    - 3.7 More on I/O redirection
    - 3.8 Looping in shell programs
    - 3.9 `bundle`: Putting it All Together
    - 3.10 Why a Programmable Shell?
4. Filters
    - 4.1 The `grep` Family
    - 4.2 Other filters
    - 4.3 The stream editor `sed`
    - 4.4 The `awk` pattern scanning and processing language
        - Fields
        - Printing
        - Patterns
        - The `BEGIN` and `END` patterns
        - Arithmetic and Variables
        - Control flow
        - Arrays
        - Associative arrays
        - Strings
        - Interaction with the shell
        - A calendar service based on `awk`
        - Loose ends
    - 4.5 Good files and good filters
5. Shell Programming
    - 5.1 Customizing the `cal` command
    - 5.2 Which command is `which`?
    - 5.3 `while` and `until` loops: watching for things
    - 5.4 Traps: catching interrupts
    - 5.5 Replacing a file: `overwrite`
    - 5.6 `zap`: killing processes by name
    - 5.7 The `pick` command: blanks vs. arguments
    - 5.8 The `news` command: community service messages
    - 5.9 `get` and `put`: tracking file changes
    - 5.10 A look back
6. Programming with Standard I/O
    - 6.1 Standard input and output: `vis`
    - 6.2 Program arguments: `vis` version 2
    - 6.3 File access: `vis` version 3
    - 6.4 A screen-at-a-time printer: `p`
    - 6.5 An example: `pick`
    - 6.6 On bugs and debugging
    - 6.7 An example: `zap`
    - 6.8 An interactive file comparison program: `idiff`
    - 6.9 Accessing the environment
7. UNIX System Calls
    - 7.1 Low-level I/O
        - File descriptors
        - File I/O - `read` and `write`
        - File creation - `open`, `creat`, `close`, `unlink`
        - Error processing - `errno`
        - Random access - `lseek`
    - 7.2 File System: directories
    - 7.3 File System: inodes
        - `sv`: An illustration of error handling
    - 7.4 Processes
        - Low-level process creation - `execlp` and `execvp`
        - Control of processes - `fork` and `wait`
    - 7.5 Signals and Interrupts
        - Alarms
8. Program Development
    - 8.1 Stage 1: A four-function calculator
        - Grammars
        - Overview of `yacc`
        - Stage 1 program
        - Making changes - unary minus
        - A digression on `make`
    - 8.2 Stage 2: Variables and error recovery
    - 8.3 Stage 3: Arbitrary variable names; built-in functions
        - Another digression on `make`
        - A digression on `lex`
    - 8.4 Stage 4: Compilation into a machine
        - A third digression on `make`
    - 8.5 Stage 5: Control flow and relational operators
    - 8.6 Stage 6: Functions and procedures; input/output
    - 8.7 Performance evaluation
    - 8.8 A look back
9. Document Preparation
    - 9.1 The `ms` macro package
        - Displays
        - Font changes
        - Miscellaneous commands
        - The `mm` macro package
    - 9.2 The `troff` level
        - Character names
        - Font and size changes
        - Basic `troff` commands
        - Defining macros
    - 9.3 The `tbl` and `eqn` preprocessors
        - Tables
        - Mathematical expressions
        - Getting output
    - 9.4 The manual page
    - 9.5 Other document preparation tools
10. Epilog
    - Appendix 1: Editor Summary
    - Appendix 2: `hoc` Manual
    - Appendix 3: `hoc`

---

## Chapter 5: Shell Programming

Although most users think of the shell as an interactive command interpreter, it is really a programming language in which each statement returns a command.
Because it must satisfy both the interactive and programming aspects of command execution, it is a strange language, shaped as much by history as by design.
The range of its application leads to an unsettling quantity of detail in the language, but you don't need to understand every nuance to use it effectively. 
This chapter explains the basics of shell programming by showing the evolution of some useful shell programs.
It is *not* a manual for the shell.
That is in the manual page `sh`(1) of the *Unix Programmer's Manual*, which you should have handy while you are reading.

With the shell, as with most commands, the details of behavior can often be most quickly discovered by experimentation.
The manual can be cryptic, and there is nothing better than a good example to clear things up.
For that reason, this chapter is organized around examples rather than shell features; it is a guide to *using* the shell for programming, rather than an encyclopedia of its capabilities.
We will talk not only about what the shell can do, but also about developing and writing shell programs, with an emphasis on testing ideas interactively.

When you've written a program, in the shell or any other language, it may be helpful enough that other people on your system would like to use it.
But the standards other people expect of a program are usually more rigorous than those you apply for yourself.
A major theme in shell programming is therefore making programs so robust they can handle improper input and give helpful information when things go wrong.

### 5.1 Customizing the `cal` command

One common use of a shell program is to enhance or to modify the user interface to a program.
As an example of a program that could stand enhancement, consider the `cal`(1) command:
```
$ cal
usage: cal [month] year            Good so far
$ cal october 1983
Bad argument                       Not so good
$ cal 10 1983
    October 1983
Su Mo Tu We Th Fr Sa
                   1
 2  3  4  5  6  7  8
 9 10 11 12 13 14 15
16 17 18 19 20 21 22
23 24 25 26 27 28 29
30 31
$
```
It's a nuisance that the month has to be provided numerically.
And, as it turns out, `cal 10` prints out the calendar for the entire year `10`, rather than for the current October, so you must always specify the year to get a calendar for a single month.

The important point here is that no matter what interface the `cal` command provides, you can change it without changing `cal` itself.
You can place a command in your private `bin` directory that converts a more convenient argument syntax into whatever the real `cal` requires.
You can even call your version `cal`, which means one less thing for you to remember.

The first issue is design: what should `cal` do?
Basically, we want `cal` to be reasonable.
It should recognize a month by name.
With two arguments, it should behave just as the old `cal` does, except for converting month names into numbers.
Given one argument, it should print the month or year's calendar as appropriate, and given zero arguments, it should print the *current* month's calendar, since that is certainly the most common use of a `cal` command.
So the problem is to decide how many arguments there are, then map them to what the standard `cal` wants.

The shell provides a `case` statement that is well suited for making such decisions:
```
case word in
pattern)	commands ;;
pattern)	commands ;;
...
esac
```
The *case* statement compares *word* to the *pattern*s from top to bottom, and performs the *commands* associated with the first, *pattern* that matches.
The patterns are written using the shell's pattern matching rules, slightly generalized from what is available for filename matching.
Each action is terminated by the double semicolon `;;`.
(The `;;` may be left off the last case, but we often leave it in for easy editing.)

Our version of `cal` decides how many arguments are present, processes alphabetic month names, then calls the real `cal`.
The shell variable `$#` holds the number of arguments that a shell file was called with; other special variables are listed in Table 5.1.

| variable | description                                               |
|----------|-----------------------------------------------------------|
| `$#`     | the number of arguments                                   |
| `$*`     | all arguments to the shell                                |
| `$@`     | similar to `$*`; see Section 5.7                          |
| `$-`     | options supplied to the shell                             |
| `$?`     | return value of the last command executed                 |
| `$$`     | process id of the shell                                   |
| `$!`     | process id of the last command started with `&`           |
| `$HOME`  | default argument for `cd` command                         |
| `$IFS`   | list of characters that separate words in arguments       |
| `$MAIL`  | file that, when changed, triggers "you have mail" message |
| `$PATH`  | list of directories to search for commands                |
| `$PS1`   | prompt string, default `'$ '`                             |
| `$PS2`   | prompt string for continued command line, default `'> '`  |

```
$ cat cal
# cal:	nicer interface to /usr/bin/cal

case $# in 
0)	set `date`; m=$2; y=$6 ;;  # no args: use today
1)  m=$1; set `date`; y=$6 ;;  # 1 arg: use this year
*)  m=$1; y=$2 ;;              # 2+ args: month and year
esac

case $m in
[jJ]an*)    m=1  ;;
[fF]eb*)    m=2  ;;
[mM]ar*)    m=3  ;;
[aA]pr*)    m=4  ;;
[mM]ay*)    m=5  ;;
[jJ]un*)    m=6  ;;
[jJ]ul*)    m=7  ;;
[aA]ug*)    m=8  ;;
[sS]ep*)    m=9  ;;
[oO]ct*)    m=10 ;;
[nN]ov*)    m=11 ;;
[dD]ec*)    m=12 ;;
[1-9]|10|11|12)  ;;  # numeric month
*)    y=$m; m="" ;;  # plain year
esac

/usr/bin/cal $m $y   # run the real one
$
```
The first `case` checks the number of arguments, `$#`, and chooses the appropriate action.
The final `*` pattern in the first `case` is a catch-all: if the number of arguments is neither `0` nor `1`, the last case will be executed.
(Since patterns are scanned in order, the catch-all must be last.)
This sets `m` and `y` to the month and year - given two arguments, our `cal` is going to act the same as the original.

The first `case` statement has a couple of tricky lines containing
```
set `date`
```
Although not obvious from appearance, it is easy to see what this statement does by trying it:
```
$ date
Sat Oct 1 06:05:18 EDT 1983
$ set `date`
$ echo $1
Sat
$ echo $4
06:05:20
$
```
`set` is a shell built-in command that does too many things.
With no arguments, it shows the values of the environment, as we saw in Chapter 3.
Ordinary arguments reset the values of `$1`, `$2`, and so on.
So `set $(date)` sets `$1` to the day of the week, `$2` to the name of the month, and so on.
The first `case` in `cal`, therefore, sets the month and year from the current date if there are no arguments; if there's one argument, it's used as the month and the year is taken from the date.

`set` also recognizes several options, of which the most often used are `-v` and `-x`; they turn on echoing of commands as they are being processed by the shell.
These are indispensable for debugging complicated shell programs.

The remaining problem is to convert the month, if it is in textual form, into a number.
This is done by the second `case` statement, which should be largely self-explanatory.
The only twist is that the `|` character in `case` statement patterns, as in `egrep`, indicates an alternative: `big:small` matches either `big` or `small`.
Of course, these cases could also be written as `[jJ]an*` and so on.
The program accepts month names either in all lower case, because most commands accept lower case input, or with the first letter capitalized, because that is the format printed by `date`.
The rules for shell pattern matching are given in Table 5.2.

| pattern    | description |
|------------|-------------|
| `*`        |
| `?`        |
| `[ccc]`    |
| `"..."`    |
| `\c`       |
| `a&vert;b` |
| `/`        |
| `.`        |

The last two cases in the second `case` statement deal with a single argument that could be a year; recall that the first `case` statement assumed it was a month.
If it is a number that could be a month, it is left alone.
Otherwise, it is assumed to be a year.

Finally, the last line calls `/usr/bin/cal` (the real `cal`) with the converted arguments.
Our version of `cal` works as a newcomer might expect:
```
$ date
Sat Oct 1 06:09:55 EDT 1983
$ cal
    October 1983      
Su Mo Tu We Th Fr Sa  
                   1  
 2  3  4  5  6  7  8  
 9 10 11 12 13 14 15  
16 17 18 19 20 21 22  
23 24 25 26 27 28 29  
30 31                 
$ cal dec
   December 1983      
Su Mo Tu We Th Fr Sa  
             1  2  3  
 4  5  6  7  8  9 10  
11 12 13 14 15 16 17  
18 19 20 21 22 23 24  
25 26 27 28 29 30 31  
$
```
And `cal 1984` prints out the calendar for all of 1984.

Our enhanced `cal` program does the same job as the original, but in a simpler, easier-to-remember way.
We therefore chose to call it `cal`, rather than `calendar` (which is already a command) or something less mnemonic like `ncal` (new `cal`).
Leaving the name alone also has the advantage that users don't have to develop a new set of reflexes for printing a calendar.

Before we leave the `case` statement, it's worth a brief comment on why the shell's pattern matching rules are different from those in `ed` and its derivatives.
After all, two kinds of patterns means two sets of rules to learn and two pieces of code to process them.
Some of the differences are simply bad choices that were never fixed - for example, there is no reason except compatibility with a past now lost that `ed` uses `.` and the shell uses `?` for "match any character."
But sometimes the patterns do different jobs.
Regular expressions in the editor search for a string that can occur anywhere in a line; the special characters `^` and `$` are needed to *anchor* the search to the beginning and end of the line.
For filenames, however, we want the search anchored by default, since that is the most common case; having to write something like
```
$ ls ^?*.c$              Doesn't work this way
```
instead of
```
$ ls *.c
```
would be a nuisance.

### 5.2 Which command is `which`?

There are problems with making private versions of commands such as `cal`.
The most obvious is that if you are working with Mary and type `cal` while logged in as `mary`, you will get the standard `cal` instead of the new one, unless of course Mary has linked the new `cal` into her `bin` directory.
This can be confusing - recall that the error messages from the original `cal` are not very helpful - but is just an example of a general problem.
Since the shell searches for commands in a set of directories specified by `PATH`, it is always possible to get a version of a command other than the one you expect.
For instance, if you type a command, say `echo`, the pathname of the file that is actually run could be `./echo` or `/bin/echo` or `/usr/bin/echo` or something else, depending on the components of your `PATH` and where the files are.
It can be very confusing if there happens to be an executable file with the right name but the wrong behavior earlier in your search path than you expect.
Perhaps the most common is the `test` command, which we will discuss later: its name is such an obvious one for a temporary version of a program that the wrong `test` program gets called annoyingly often.
A command that reports which version of a program will be executed would provide a useful service.

One implementation is to loop over the directories in `PATH`, searching each for an executable file of the given name.
In Chapter 3, we used the `for` to loop over filenames and arguments.
Here, we want a loop that says
```
for i in each component of PATH
do
    if given name is in directory i
        print its full pathname
done
```
Because we can run any command inside backquotes (\`...\`), the obvious solution is to run `sed` over `$PATH`, converting colons into spaces.
We can test it out with our old friend `echo`:
```
$ echo $PATH
:/usr/you/bin:/bin:/usr/bin
$ echo $PATH sed 's/:/ /g'
 /usr/you/bin /bin /usr/bin
$ echo `echo $PATH 's/:/ /g'`
/usr/you/bin /bin /usr/bin
$
```
There is clearly a problem.
A null string in `PATH` is a synonym for `.`.
Converting the colons in `PATH` to blanks is therefore not good enough - the information about null components will be lost.
To generate the correct list of directories, we must convert a null component of `PATH` into a dot.
The null component could be in the middle or at either end of the string, so it takes a little work to catch all the cases:
```
$ echo $PATH | sed 's/^:/.:/
>                   s/::/:.:/g
>                   s/:$/:./
>                   s/:/ /g'
. /usr/you/bin /bin /usr/bin
$
```
We could have written this as four separate `sed` commands, but since `sed` does the substitution in order, one invocation can do it all.

Once we have the directory components of `PATH`, the `test`(1) command we've mentioned can tell us whether a file exists in each directory.
The `test` command is actually one of the clumsier UNIX programs.
For example, `test -r file` tests if `file` exists and can be read, and `test -w file` tests if `file` exists and can be written, but the 7th Edition provides no `test -x` (although System V and other versions do) which would otherwise be the one for us.
We'll have to settle for `test -f`, which tests that the file exists and is not a directory, in other words, is a regular file.
You should look over the manual page for `test` on your own system, however, since there are several versions in circulation.

Every command returns an *exit status* - a value returned to the shell to indicate what happened.
The exit status is a small integer; by convention `0` means "true" (the command ran successfully) and non-zero means "false" (the command ran unsuccessfully).
Note that this is opposite to the values of true and false in C.

Since many different values can all represent "false," the reason for failure is often encoded in the "false" exit status.
For example, `grep` returns `0` if there was a match, `1` if there was no match, and `2` if there was an error in the pattern or filenames.
Every program returns a status, although we usually aren't interested in its value.
`test` is unusual because its *sole* purpose is to return an exit status.
It produces no output and changes no files.

The shell stores the exit status of the last program in the variable `$?`:
```
$ cmp /usr/you/.profile /usr/you/.profile
                                           No output, they're the same
$ echo $?
0                                          Zero implies ran O.K.: files identical
$ cmp /usr/you/.profile /usr/mary/.profile
/usr/you/.profile /usr/mary/.profile differ: char 6, line 3
echo $?
1                                          Non-zero means files were different
$
```
A few commands, such as `cmp` and `grep`, have an option `-s` that causes them to exit with an appropriate status but suppress all output.

The shell's `if` statement runs commands based on the exit status of a command, as in
```
if command
then
    commands if condition true
else
    commands if condition false
fi
```
The location of the newlines is important: `fi`, `then`, and `else` are recognized only after a newline or a semicolon.
The `else` part is optional.

The `if` statement always runs a command - the condition - whereas the `case` statement does pattern matching directly in the shell.
In some UNIX versions, including System V, `test` is a shell built-in function so an `if` and a `test` will run as fast as a `case`.
If `test` isn't built in, `case` statements are more efficient than `if` statements, and should be used for any pattern matching:
```
case "$1" in
hello)    command
esac
```
will be faster than
```
if test "$1" = hello     Slower unless test is a shell built-in
then
          command
fi
```
That is one reason why we sometimes use `case` statements in the shell for testing things that would be done with an `if` statement in most programming languages.
A `case` statement, on the other hand, can't easily determine whether a file has read permissions; that is better done with a `test` and an `if`.

So now the pieces are in place for the first version of the command `which`, to report which file corresponds to a command:
```
$ cat which
# which cmd:	which cmd in PATH is executed, version 1

case $# in
0)    echo 'Usage: which command' 1>&2; exit 2
esac
for i in `echo $PATH | sed 's/^:/.:/
                            s/::/:.:/g
                            s/:$/:./
                            s/:/ /g'`
do
    if test -f $i/$1        # use test -x if you can
    then
        echo $i/$1
        exit 0              # found it
    fi
done
exit 1                      # not found
$
```
Let's test it:
```
$ cx which               Make it executable
$ which which
./which
$ which ed
/bin/ed
$ mv which /usr/you/bin
$ which which
/usr/you/bin/which
$
```
The initial `case` statement is just error-checking.
Notice the redirection `1>&2` on the `echo` so the error message doesn't vanish down a pipe.
The shell built-in command `exit` can be used to return an exit status.
We wrote `exit 2 ` to return an error status if the command didn't work, `exit 1` if it couldn't find the file, and `exit 0` if it found one.
If there is no explicit `exit` statement, the exit status from a shell file is the status of the last command executed.

What happens if you have a program called `test` in the current directory?
(We're assuming that `test` is not a shell built-in.)
```
$ wcho `echo hello` >test          Make a fake test
$ cx test                          Make it executable
$ which which                      Try which now
hello                              Fails!
./which
$
```
More error-checking is called for.
You could run `which` (if there weren't a `test` in the current directory!) to find out the full pathname for `test`, and specify it explicitly.
But that is unsatisfactory: `test` may be in different directories on different systems, and `which` also depends on `sed` and `echo`, so we should specify their pathnames too.
There is a simpler solution: fix `PATH` in the shell file, so it only looks in `/bin` and `/usr/bin` for commands.
Of course, for the `which` command only, you have to save the old `PATH` for determining the sequence of directories to be searched.

```
$ cat which
# which cmd:	which cmd in PATH is executed, final version

opath=$PATH
PATH=/bin:/usr/bin

case $# in
0)    echo 'Usage: which command' 1>&2; exit 2
esac

for i in `echo $opath | sed 's/^:/.:/
                             s/::/:.:/g
                             s/:$/:./
                             s/:/ /g'`
do
    if test -f $i/$1        # this is /bin/test
    then                    # or /usr/bin/test only
        echo $i/$1
        exit 0              # found it
    fi
done
exit 1                      # not found
$
```
`which` now works even if there is a spurious `test` (or `sed` or `echo`) along the search path.
```
$ ls -l test
-rwxrwxrwx 1 you			11 Oct	1 06:55	test
$ which which
/usr/you/bin/which
$ which test
./test
$ rm test
$ which test
/bin/test
$
```

The shell provides two other operators for combining commands, `||` and `&&`, that are often more compact and convenient than the `if` statement.
For example, `||` can replace some `if` statements:
```
test -f filename || echo file filename doesnot exist
```
is equivalent to
```
if test ! -f filename
then
    echo file filename does not exist
fi
```
The operator `||`, despite appearances, has nothing to do with pipes - it is a conditional operator meaning `OR`.
The command to the left of `||` is executed.
If its exit status is zero (success), the command to the right of `||` is ignored.
If the left side returns non-zero (failure), the right side is executed and the value of the entire expression is the exit status of the right side.
In other words, `||` is a left conditional `OR` operator that does not execute its right-hand command if the left one succeeds.
The corresponding `&&` conditional is `AND`; it executes its right-hand command only if the left one succeeds.

### 5.3 `while` and `until` loops: watching for things

In Chapter 3, the `for` loop was used for a number of simple iterative programs.
Usually, a `for` loops over a set of filenames, as in `for i in *.c`, or all the arguments to a shell program, as in `for i in $*`.
But shell loops are more general than these idioms would suggest; consider the `for` loop in `which`.

There are three loops: `for`, `while`, and `until`.
The `for` is by far the most commonly used.
It executes a set of commands - the loop body - once for each element in a set of words.
Most often these are just filenames.
The `while` and `until` use the exit status from a command to control the execution of the commands in the body of the loop.
The loop body is executed until the condition command returns a non-zero status (for the `while`) or zero (for the `until`).
`while` and `until` are identical except for the interpretation of the exit status of the command.

Here are the basic forms of each loop:
```
for i in list of words
do
    loop body, $i is set to successive elements of list
done

for i          (List is implicitly all arguments to shell file, i.e., $*)
do
    loop body, $i is set to successive arguments
done


while command
do
    loop body executed as long as command returns true
done


until command
do
    loop body executed as long as command returns false
done
```
The second form of the `for`, in which an empty list implies `$*`, is a convenient shorthand for the most common usage.

The conditional command that controls a `while` or `until` can be any command.
As a trivial example, here is a `while` loop to watch for someone (say Mary) to log in:
```
while sleep 60
do
    who | grep mary
done
```
The `sleep`, which pauses for 60 seconds, will always execute normally (unless interrupted) and therefore return "success", so the loop will check once a minute to see if Mary has logged in.

This version has the disadvantage that if Mary is already logged in, you must wait 60 seconds to find out.
Also, if Mary stays logged in, you will be told about her once a minute.
The loop can be turned inside out and written with an `until`, to provide the information once, without delay, if Mary is on now:
```
until who | grep mary
do
    sleep 60
done
```
This is a more interesting condition.
If Mary is logged in, `who | grep mary` prints out her entry in the `who` listing and returns "true", because `grep` returns a status to indicate whether it found something, and the exit status of a pipeline is the exit status of the last element.

Finally, we can wrap up this command, give it a name and install it:
```
$ cat watchfor
# watchfor:	watch for someone to log in

PATH=/bin:/usr/bin

case $# in
0)		echo 'Usage: watchfor person' 1>&2; exit 1
esac

until who | egrep "$1"
do
    sleep 60
done
$ cx watchfor
$ watchfor you
you			tty0		Oct	1 08:01        It works
$ mv watchfor /usr/you/bin         Install it
$
```
We changed `grep` to `egrep` so you can type
```
$ watchfor 'joe|mary'
```
to watch for more than one person.

As a more complicated example, we could watch *all* people logging in and out, and report as people come and go - a sort of incremental `who`.
The basic structure is simple: once a minute, run `who`, compare its output to that from a minute ago, and report any differences.
The `who` output will be kept in a file, so we will store it in the directory `/tmp`.
To distinguish our files from those belonging to other processes, the shell variable `$$` (the process id of the shell command), is incorporated into the filenames; this is a common convention.
Encoding the command name in the temporary files is done mostly for the system administrator.
Commands (including this version of `watchwho`) often leave files lying around in `/tmp`, and its nice to know which command is doing it.

```
$ cat watchwho
# watchwho:	watch who logs in and out

PATH=/bin:/usr/bin
new=/tmp/wwho1.$$
old=/tmp/wwho2.$$
>$old           # create an empty file

while :         # loop forever
do
    who >$new
    diff $old $new
    mv $new $old
    sleep 60
done | awk '/>/ { $1 = "in:     "; print }
            /</ { $1 = "out:    "; print }'
$
```
`:` is a shell built-in command that does nothing but evaluate its arguments and return "true."
Instead, we could have used the command `true`, which merely returns a true exit status.
(There is also a `false` command.)
But `:` is more efficient than `true` because it does not execute a command from the file system.

`diff` output uses `<` and `>` to distinguish data from the two files; the `awk` program processes this to report the changes in an easier-to-understand format.
Notice that the entire `while` loop is piped into `awk`, rather than running a fresh `awk` once a minute.
`sed` is unsuitable for this processing, because its output is always behind its input by one line: there is always a line of input that has been processed but not printed, and this would introduce an unwanted delay.

Because `old` is created empty, the first output from `watchwho` is a list of all users currently logged in.
Changing the command that initially creates `old` to `who>$old` will  cause `watchwho` to print only the changes; it's a matter of taste.

Another looping program is one that watches your mailbox periodically; whenever the mailbox changes, the program prints `"You have mail."`
This is a useful alternative to the shell's built-in mechanism using the variable `MAIL`.
We have implemented it with shell variables instead of files, to illustrate a different way of doing things.

```
$ cat checkmail
# checkmail:	watch mailbox for growth

PATH=/bin:/usr/bin
MAIL=/usr/spool/mail/`getname`  # system dependent

t=${1-60}

x="`ls -l $MAIL`"
while :
do
    y="`ls -l $MAIL`"
    echo $x $y
    x="$y"
    sleep $t
done | awk '$4 < $12 { print "You have mail" }'
$
```
We have used `awk` again, this time to ensure that the message is printed only when the mailbox grows, not merely when it changes.
Otherwise, you'll get a message right after you delete mail.
(The shell's built-in version suffers from this drawback.)

The time interval is normally set to 60 seconds, but if there is a parameter on the command line, as in
```
$ checkmail 30
```
that is used instead.
The shell variable `t` is set to the time if one is supplied, and to `60` if no value was given, by the line
```bash
t=${1-60}
```
This introduces another feature of the shell.

`${var}` is equivalent to `$var`, and can be used to avoid problems with variables inside strings containing letters or numbers:
```
$ var=hello
$ varx=goodbye
$ echo $var
hello
$ echo $varx
goodbye
$ echo ${var}x
hellox
$
```
Certain characters inside braces specify special processing of the variable.
If the variable is undefined, and the name is followed by a question mark, then the string after the `?` is printed and the shell exits (unless it's interactive).
If the message is not provided, a standard one is printed:
```
$ echo ${var?}
hello                        O.K.; var is set
$ echo ${junk?}
junk: parameter not set      Default message
$ echo ${junk?error!}
junk: error!                 Message provided
$
```
Note that the message generated by the shell always contains the name of the undefined variable.

Another form is `${var-thing}` which evaluates to `$var` if it is defined, and `thing` if it is not.
`${var=thing}` is similar, but also sets `$var` to `thing`:
```
$ echo ${junk-'Hi there'}
Hi there
$ echo ${junk?}
junk: parameter not set
$ echo ${junk='Hi there'}
Hi there
$ echo ${junk?}
Hi there
$
```
The rules for evaluating variables are given in Table 5.3.

Returning to our original example,
```bash
t=${1-60}
```
sets `t` to `$1`, or if no argument is provided, to `60`.

| example          | description                                                                                                                |
|------------------|----------------------------------------------------------------------------------------------------------------------------|
| `$var`           | value of `var`; nothing if `var` undefined                                                                                 |
| `${var}`         | same; useful if alphanumerics follow variable name                                                                         |
| `${var-thing}`   | value of `var` if defined; otherwise `thing`. <br/> `$var` unchanged                                                       |
| `${var=thing}`   | value of `var` if defined; otherwise `thing`. <br/> If undefined, `$var` set to `thing`.                                   |
| `${var?message}` | if defined, `$var`. Otherwise print `message` and exit shell. <br/> If `message` is empty, print: `var: parameter not set` |
| `${var+thing}`   | `thing` if `$var` defined, otherwise nothing                                                                               |

### 5.4 Traps: catching interrupts

If you type DEL or hang up the phone while `watchwho` is running, one or two temporary files are left in `/tmp`.
`watchwho` should remove the temporary files before it exits.
We need a way to detect when such events happen, and a way to recover.

When you type DEL, an *interrupt signal* is sent to all the processes that you are running on the terminal.
Similarly, when you hang up, a *hangup signal* is sent.
There are other signals as well.
Unless a program has taken explicit action to deal with signals, the signal will terminate it.
The shell protects programs run with `&` from interrupts but not from hangups.

Chapter 7 discusses signals in detail, but you needn't know much to be able to handle them in a shell.
The shell built-in command `trap` sets up a sequence of commands to be executed when a signal occurs:
```
trap sequence-of-commands list of signal numbers
```
The `sequence-of-commands` is a single argument, so it must almost always be the quoted.
The *signal numbers* are small integers that identify the signal.
For example, `2` is the signal generated by the DEL key, and `1` is generated by hanging up the phone.
The signal numbers most often useful to shell programmers are listed in Table 5.4.

| number | description                                          |
|--------|------------------------------------------------------|
| `0`    | shell exit (for any reason, including end of file)   |
| `1`    | hangup                                               |
| `2`    | interrupt (DEL key)                                  |
| `3`    | quit (`ctl-\` causes program to produce a core dump) |
| `9`    | kill (cannot be caught or ignored)                   |
| `15`   | terminate, default signal generated by `kill`(1)     |

So to clean up the temporary files in `watchwho`, a `trap` call should go just before the loop, to catch hangup, interrupt, and terminate:
```bash
...
trap 'rm -f $new $old; exit 1' 1 2 15

while :
...
```
The command sequence that forms the first argument to `trap` is like a subroutine call that occurs immediately when the signal happens.
When it finishes, the program that was running will resume where it was unless the signal killed it.
Therefore, the `trap` command sequence must explicitly invoke `exit`, or the shell program will continue to execute after the interrupt.
Also, the command sequence will be read twice: once when the `trap` is set and once when it is invoked.
Therefore, the command sequence is best protected with single quotes, so variables are evaluated only when the `trap` routines are executed.
It makes no difference in this case, but we will see one later in which it matters.
By the way, the `-f` option tells `rm` not to ask questions.

`trap` is sometimes useful interactively, most often to prevent a program from being killed by the hangup signal generated by a broken phone connection:
```
$ (trap '' 1; long-running-command) &
2134
$
```
the null command sequence means "ignore interrupts" in this process and its children.
The parentheses cause the `trap` command to be run together in a background sub-shell; without them, the `trap` would apply to the login shell as well as to `long-running-command`.

The `nohup`(1) command is a short shell program to provide this service.
Here is the 7th Edition version, in its entirety:
```
$ cat `which nohup`
trap "" 1 15
if test -t 2>&1
then
    echo "Sending output to 'nohup.out'"
    exec nice -5 $* >>nohup.out 2>&1
else
    exec nice -5 $* 2>&1
fi
```
`test -t` tests whether the standard output is a terminal, to see if the output should be saved.
The background program is run with `nice` to give it a lower priority than interactive programs.
(Notice that `nohup` doesn't set `PATH`. Should it?)

The `exec` is just for efficiency; the command would run just as well without it.
`exec` is a shell built-in that replaces the process running this shell by the named program, thereby saving one process - the shell would normally wait for the program to complete.
We could have used `exec` in several other places, such as at the end of the enhanced `cal` program when it invokes `/usr/bin/cal`.

By the way, the signal `9` is one that can't be caught or ignored: it always kills.
From the shell, it is send as
```
$ kill -9 process-id
```
`kill -9` is not the default because a process killed that way is given no change to put its affairs in order before dying.

### 5.5 Replacing a file: `overwrite`

The `sort` command has an option `-o` to overwrite a file:
```
$ sort file1 -o file2
```
is equivalent to
```
$ sort file1 >file2
```
If `file1` and `file2` are the same file, redirection with `>` will truncate the input file before it is sorted.
The `-o` option, however, works correctly, because the input is sorted and saved in a temporary file before the output file is created.

Many other commands could also use a `-o` option.
For example, `sed` could edit a file in place:
```
$ sed 's/UNIX/UNIX(TM)/g' ch2 -o ch2         Doesn't work this way!
```
It would be impractical to modify all such commands to add the option.
Furthermore, it would be bad design: it is better to centralize functions, as the shell does with the `>` operator.
We will provide a program `overwrite` to do the job.
The first design is like this:
```
$ sed 's/UNIX/UNIX(TM)/g' ch2 | overwrite ch2
```

The basic implementation is straightforward - just save away the input until end of file, then copy the data to the argument file:
```bash
# overwrite:	copy standard input to output after EOF
# version 1.  BUG here

PATH=/bin:/usr/bin

case $# in
1)    ;;
*)    echo 'Usage: overwrite file' 1>&2; exit 2
esac

new=/tmp/overwr.$$
trap 'rm -f $new; exit 1' 1 2 15

cat >$new                # collect the input
cp $new $1               # overwrite the input file
rm -f $new
```
`cp` is used instead of `mv` so the permissions and owner of the output file aren't changed if it already exists.

Appealingly simple as this version is, it has a fatal flaw: if the user types DEL during the `cp`, the original input file will be ruined.
We must prevent an interrupt from stopping the overwriting of the input file:
```bash
# overwrite:	copy standard input to output after EOF
# version 2.  BUG here too

PATH=/bin:/usr/bin

case $# in
1)    ;;
*)    echo 'Usage: overwrite file' 1>&2; exit 2
esac

new=/tmp/overwr1.$$
new=/tmp/overwr2.$$
trap 'rm -f $new $old; exit 1' 1 2 15

cat >$new                # collect the input
cp $1 $old               # save original file

trap '' 1 2 15           # we are committed; ignore signals
cp $new $1               # overwrite the input file

rm -f $new $old
```
If a DEL happens before the original file is touched, then the temporary files are removed and the file is left alone.
After the backup is made, signals are ignored so the last `cp` won't be interrupted - once the `cp` starts, `overwrite` is committed to changing the original file.

There is still a subtle problem. Consider:
```
$ sed 's/UNIX/UNIX(TM)g' precious | overwrite precious
command garbled: s/UNIX/UNIX(TM)g
$ ls -l precious
-rw-rw-rw-	1	you			0 Oct	1	09:02	precious		#$%@*!
$
```
If the program providing input to `overwrite` gets an error, its output will be empty and `overwrite` will dutifully and reliably destroy the argument file.

A number of solutions are possible.
`overwrite` could ask for confirmation before replacing the file, but making `overwrite` interactive would negate much of its merit.
`overwrite` could check that its input is non-empty (by `test -z`), but that is ugly and not right either: some output might be generated before an error is detected.

The best solution is to run the data-generating program under `overwrite`'s control so its exit status can be checked.
This is against tradition and intuition -  in a pipeline, `overwrite` produces nothing on its standard output, however, so no generality is lost.
And its syntax isn't unheard of: `time`, `nice`, and `nohup` are all commands that take another command as arguments.

Here is the safe version:
```bash
# overwrite:	copy standard input to output after EOF
# final version

opath=$PATH
PATH=/bin:/usr/bin

case $# in
0|1)    echo 'Usage: overwrite file cmd [args]' 1>&2; exit 2
esac

file=$1; shift
new=/tmp/overwr1.$$
old=/tmp/overwr2.$$
trap 'rm -f $new $old; exit 1' 1 2 15  # clean up files

if PATH=$opath "$@" >$new              # collect input
then
    cp $file $old   # save original file
    trap '' 1 2 15  # we are committed; ignore signals
    cp $new $file
else
    echo "overwrite: $1 failed, $file unchanged" 1>&2
fi
rm -f $new $old
```

The shell built-in command `shift` moves the entire argument list one position to the left: `$2` becomes `$1`, `$3` becomes `$2`, etc.
`"$@"` provides all the arguments (after the `shift`), like `$*`, but uninterpreted; we'll come back to it in Section 5.7.

Notice that `PATH` is restored to run the user's command; if it weren't, commands that were not in `/bin` or `/usr/bin` would be inaccessable to `overwrite`.

`overwrite` now works (if somewhat clumsily):
```
$ cat notice
UNIX is a Trademark of Bell Laboratories
$ overwrite notice sed 's/UNIXUNIX(TM)/g' notice
command garbled: s/UNIXUNIX(TM)/g
overwrote: sed failed, notice unchanged
$ cat notice
UNIX is a Trademark of Bell Laboratories
$ overwrite notice sed 's/UNIX/UNIX(TM)/g' notice
$ cat notice
UNIX(TM) is a Trademark of Bell Laboratories
$
```

Using `sed` to replace all occurrences of one word with another is a common thing to do.
With `overwrite` in hand, a shell file to automate the task is easy:
```
$ cat replace
# replace: replace str1 in files with str2, in place

PATH=/bin:/usr/bin

case $# in
0|1|2) echo 'Usage: replace str1 str2 files' 1>&2; exit 1
esac

left="$1"; right="$2"; shift; shift

for i
do
    overwrite $i sed "s@$left@$right@g" $i
done
$ cat footnote
UNIX is not an acronym
$ replace UNIX Unix footnote
$ cat footnote
Unix is not an acronym
$
```
(Recall that if the list on a `for` statement is empty, it defaults to `$*`.)
We used `@` instead of `/` to delimit the substitute command, since `@` is somewhat less likely to conflict with an input string.

`replace` sets `PATH` to `/bin:/usr/bin`, excluding `$HOME/bin`.
This means that `overwrite` must be in `/usr/bin` for `replace` to work.
We made this assumption for simplicity; if you can't install `overwrite` in `/usr/bin`, you will have to put `$HOME/bin` in `PATH` inside `replace`, or five `overwrite`'s pathname explicitly.
From now on, we will assume that the commands we are writing reside in `/usr/bin`; they are meant to.

### 5.6 `zap`: killing processes by name

The `kill` command only terminates processes specified by process-id.
When a specific background process needs to be killed, you must usually run `ps` to find the process-id and then laboriously re-type it as an argument to `kill`.
But it's silly to have one program print a number that you immediately transcribe manually to another.
Why not write a program, say `zap`, to automate the job?

One reason is that killing a process is dangerous, and care must be taken to kill the right process.
A safeguard is always to run `zap` interactively, and use `pick` to select the victims.

A quick reminder about `pick`: it prints each of its arguments in turn and asks the user for a response; if the response is `y`, the argument is printed.
(`pick` is the subject of the next section.)
`zap` uses `pick` to verify that the processes chosen by name are the ones the user wants to kill:
```
$ cat zap
# zap pattern:	kill all processes matching pattern
# BUG in this version

PATH=/bin:/usr/bin

case $# in
0)    echo 'Usage: zap pattern' 1>&2; exit 1
esac

kill `pick \`ps -ag | grep "$*"\` | awk '{print $1}'`
$
```
Note the nested backquotes, protected by backslashes.
The `awk` program selects the process-id from the `ps` output selected by the `pick`:
```
$ sleep 1000 &
22126
$ ps -ag
PID   TTY TIME CMD
...
22126 0   0:00 sleep 1000
...
$ zap sleep
22126
0? q                       What's going on?
$
```

The problem is that the output of `ps` is being broken into words, which are seen by `pick` as individual arguments rather than being processed a line at a time.
The shell's normal behavior is to break strings into arguments at blank/non-blank boundries, as in
```bash
for i in 1 2 3 4 5
```
In this program we must control the shell's division of strings into arguments, so that only newlines separate adjacent "words."

The shell variable `IFS` (internal field separator) is a string of characters that separate words in argument lists such as backquotes and `for` statements.
Normally, `IFS` contains a blank, a tab, and a newline, but we can change it to anything useful such as just a newline:
```
$ echo 'echo $#' >nargs
$ cs nargs
$ who
you		tty0		Oct	1 05:59
pjw		tty2		Oct	1 11:26
$ nargs `who`
10                           Ten blank and newline-separated fields
$ IFS='
'                            Just a newline
$ nargs `who`
2                            Two lines, two fields
$
```
With `IFS` set to newline, `zap` works fine:
```
$ cat zap
# zap pat:	kill all processes matching pat
# final version

PATH=/bin:/usr/bin
IFS='
'

case $1 in
"")    echo 'Usage: zap [-2] pattern' 1>&2; exit 1 ;;
-*)    SIG=$1; shift
esac

echo '  PID TTY           TIME CMD'
kill $SIG `pick \`ps -ag | egrep "$*"\` | awk '{print $1}'`
$ ps -ag
  PID TTY TIME CMD
...
22126 0   0:00 sleep 1000
...
$ zap sleep
  PID TTY TIME CMD
22126 0   0:00 sleep 1000? y
23104 0   0:02 egrep sleep? n
$
```

We added a couple of wrinkles: an optional argument to specify the signal (note that `SIG` will be undefined, and therefore treated as a null string if the argument is not supplied) and the use of `egrep` instead of `grep` to permit more complicated patterns such as 'sleep|date'.
An initial `echo` prints out the column headers for the `ps` output.

You might wonder why this command is called `zap` instead of just `kill`.
The main reason is that, unlike our `cal` example, we aren't really providing a new `kill` command: `zap` is necessarily interactive, for one thing - and we want to retain `kill` for the real one.
`zap` is also annoyingly slow - the overhead of all the extra programs is appreciable, although `ps` (which must be run anyway) is the most expensive.
In the next chapter we will provide a more efficient implementation.

### 5.7 The `pick` command: blanks vs. arguments

We've encountered most of what we need to write a `pick` command in the shell.
The only new thing needed is a mechanism to read the user's input.
The shell built-in `read` reads one line of text from the standard input and assigns the text (without the newline) as the value of the named variable:
```
$ read greeting
hello, world             Type new value for greeting
$ echo $greeting
hello, world
$
```
The most common use of `read` is in `.profile` to set up the environment when logging in, primarily to set shell variables like `TERM`.

`read` can only read from the standard input; it can't even be redirected.
None of the shell built-in commands (as opposed to the control flow primitives like `for`) can be redirected with `>` or `<`:
```
$ read greeting </etc/passwd
goodbye                       Must type a value anyway
illegal io                    Now shell reports error
$ echo $ greeting
goodbye                       greeting has typed value, not one from file
$
```
This might be described as a bug in the shell, but it is a fact of life.
Fortunately, it can usually be circumvented by redirecting the loop surrounding the `read`.
This is the key to our implementation of the `pick` command:
```bash
# pick:	select arguments

PATH=/bin:/usr/bin

for i
do
    echo -n "$i? " >/dev/tty
    read response
    case $response in
    [yY]*)    echo $i ;;
    q*)       break
    esac
done </dev/tty
```
`echo -n` suppresses the final newline, so the response can be typed on the same line as the prompt.
And, of course, the prompts are printed on `/dev/tty` because the standard output is almost certainly not the terminal.

The `break` statement is borrowed from C: ti terminates the innermost enclosing loop.
In this case it breaks out of the `for` loop when a `q` is typed.
We let `q` terminate selection because it's easy to do, potentially convenient, and consistent with other programs.

It's interesting to play with blanks in the arguments to `pick`:
```
$ pick '1 2' 3
1 2?
3?
$
```
If you want to see how `pick` is reading its arguments, run it and just press RETURN after each prompt.
It's working fine as it stands: `for i` handles the arguments properly.
We could have written the loop in other ways:
```
$ grep for pick          See what this version does
for i in $*
$ pick '1 2' 3
1?
2?
3?
$
```
This form doesn't work, because the operands of the loop are rescanned, and the blank in the first argument cause it to become two arguments.
Try quoting the `$*`:
```
$ grep for pick          Try a different version
for i in "$*"
$ pick '1 2' 3
1 2 3?
$
```
This doesn't work either, because `"$*"` is a single word formed from all the arguments joined together, separated by blanks.

Of course there is a solution, but it is almost black magic: the string `"$@"` is treated specially by the shell, and converted into exactly the arguments to the shell file:
```
$ grep for pick          Try a third version
for i in "$@"
$ pick '1 2' 3
1 2?
3?
$
```
If `$@` is not quoted, it is identical to `$*`; the behavior is special only when it is enclosed in double quotes.
We used `overwrite` to preserve the arguments to the user's command.

In summary, here are the rules:
- `$*` and `$@` expand into the arguments, and are rescanned; blanks in arguments will result in multiple arguments.
- `"$*"` is a single word composed of all the arguments to the shell file joined together with spaces.
- `"$@"` is identical to the arguments received by the shell file: blanks in arguments are ignored, and the result is a list of words identical to the original arguments.

If `pick` has no arguments, it should probably read its standard input, so we could say
```
$ pick <mailinglist
```
instead of
```
$ pick `cat mailinglist`
```
But we won't investigate this version of `pick`: it involves some ugly complications and is significantly harder than the same program written in C, which we will present in the next chapter.

The first two of the following exercises are difficult, but educational to the advanced shell programmer.

### 5.8 The `news` command: community service messages

In Chapter 1 we mentioned that your system might have a `news` command to report messages of general interest to the user community.
Although the name and details of the command differ, most systems provide a news service.
One reason for presenting a `news` command is not to replace your local command, but to show how easily such a program can be written in the shell.
It might be interesting to compare the implementation of our `news` command to your local version.

The basic idea of such programs is usually that individual news items are stored, one per file, in a special directory like `/usr/news`.
`news` (that is, our `news` program) operates by comparing the modification times of the files in `/usr/news` with that of a file in your home directory (`.news_time`) that serves as a time stamp.
For debugging, we can use `.` as the directory for both the news files and `.news_time`; it can be changed to `/usr/news` when the program is ready for general use.

```
$ cat news
# news:  print news files, version 1

HOME=.                   # debugging only
cd .                     # place holder for /usr/news
for i in `ls -t * $HOME/.news_time`
do
    case $i in
    */.news_time) break ;;
    *)            echo news: $i
    esac
done
touch $HOME/.news_time
$ touch .news_time
$ touch x
$ touch y
news: x
news: y
$
```
`touch` changes the last-modified time of its argument file to the present time, without actually modifying the file.
For debugging, we can just echo the names of the news files, rather than printing them.
The loop terminates when it discovers `.news_time`, thereby listing only those files that are newer.
Note that the `$*` in case statements can match a `/`, which it cannot in filename patterns.

What happens if `.news_time` doesn't exist?
```
$ rm .news_time
$ news
$
```
This silence is unexpected, and wrong.
It happens because if `ls` can't find a file, it reports the problem on its standard output, before printing any information about existing files.
This is undeniably a bug - the diagnostic should be printed on the standard error -  but we can get around it by recognizing the problem in the loop and redirecting standard error to standard output so all versions work the same.
(This problem has been fixed in newer versions of the system, but we've left it as is to illustrate how you can often cope with minor botches.)
```
$ cat news
# news: print news files, version 2

HOME=.                   # debugging only
cd .                     # place holder for /usr/news
for i in `ls -t * $HOME/.news_time 2>&1`
do
    case $i in
    *' not found' ;;
    */.news_time) break ;;
    *)            echo news: $i
    esac
done
touch $HOME/.news_time
$ rm .news_time
$ news
news: news
news: y
news: x
$
```
We must set `IFS` to newline so the message
```
./.news_time not found
```
is not parsed as three words.

`news` must next print the news files, rather than echoing their names.
It's useful to know who posted a message and when, so we use the `set` command and `ls -l` to print a header before the message itself:
```
$ ls -l news
-rwxrwxrwx 1 you			208 Oct	1 12:05 news
$ set X`ls -l news` 
-rwxrwxrwx: bad option(s)                   Something is wrong!
$
```
Here is one example where the interchangeability of program and data in the shell gets in the way.
`set` complains because its argument (`"-rwxrwxrwx"`) begins with a minus sign and thus looks like an option.
An easy (if inelegant) fix is to prefix the argument by an ordinary character:
```
$ set X`ls -l news`
$ echo "news: ($3) $5 $6 $7"
news: (you) Oct 1 12:05
$
```
That is a reasonable format, showing the author and date of the message along with the filename.

Here is the final version of the `news` command:
```bash
# news:  print news files, final version

PATH=/bin:/usr/bin
IFS='
'                   # just a newline
cd /usr/news

for i in `ls -t * $HOME/.news_time 2>&1`
do 
    IFS=' '
    case $i in
    *' not found')  ;;
    */.news_time)   break ;;
    *)              set X`ls -l $i`
                    echo "
$i: ($3) $5 $6 $7
"
                    cat $i
    esac
done
touch $HOME/.news_time
```
The extra newlines in the header separate the news items as they are printed.
The first value of `IFS` is just a newline, so the `not found` message (if any) from `ls` is treated as a single argument.
The second assignment to `IFS` resets it to a blank, so the output of the second `ls` is split into multiple arguments.

### 5.9 `get` and `put`: tracking file changes
### 5.10 A look back

When you're faced with writing a new program, there's a natural tendency to start thinking immediately about how to write it in your favorite programming language.
In our case, that language is most often the shell.

Although it has some unusual syntax, the shell is an excellent programming language.
It is certainly high-level; its operators are whole programs.
Since it is interactive, programs can be developed interactively, and refined in small steps until they "work."
After that, if they are intended for more than personal use, they can be polished and hardened for a wider user population.
In those infrequent cases where a shell programs turns out to be too inefficient, some or all of it can be rewritten in C, but with the design already proven and a working implementation in hand.
(We'll follow this path a couple of times in the next chapter.)

This general approach is characteristic of the UNIX programming environment - build on what others have done instead of starting over from nothing; start with something small and let it evolve; use the tools to experiment with new ideas.

In this chapter, we've presented many examples that are easy to do with existing programs and the shell.
Sometimes it's enough merely to rearrange arguments; that was the case with `cal`.
Sometimes the shell provides a loop over a set of filenames or though a sequence of command executions, as in `watchfor` and `checkmail`.
More complicated examples are still less work than they would be in C; for instance, our 20-line shell version of `news` replaces a 350-line version written in C.

But it's not enough to have a programmable command language.
Nor is it enough to have a lot of programs.
What matters is that all of the components *work together*.
They share conventions about how information is represented and communicated.
Each is designed to focus on one job and do it well.
The shell then serves to bind them together, easily and efficiently, whenever you have a new idea.
This cooperation is why the UNIX programming environment is so productive.
