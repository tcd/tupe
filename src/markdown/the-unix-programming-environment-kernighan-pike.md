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
### 5.3 `while` and `until` loops: watching for things
### 5.4 Traps: catching interrupts
### 5.5 Replacing a file: `overwrite`
### 5.6 `zap`: killing processes by name
### 5.7 The `pick` command: blanks vs. arguments
### 5.8 The `news` command: community service messages
### 5.9 `get` and `put`: tracking file changes
### 5.10 A look back
