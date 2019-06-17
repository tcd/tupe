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
        - Control flow
        - Arrays
        - Associative arrays
        - Strings
        - Interaction with the shell
        - A calendar service based on awk
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

## Chapter 3: Using the Shell

The shell - the program that interprets your request to run programs - is the most important program for UNIX users; with the possible exception of your favorite text editor, you will spend more time working with the shell than any other program.
In this chapter and in Chapter 5, we will spend a fair amount of time on the shell's capabilities.
The main point we want to make is that you can accomplish a lot without much hard work, and certainly without resorting to programming in a conventional language, like C, if you know how to use the shell.

We have divided our coverage of the shell into two chapters.
This chapter goes one step beyond the necessities covered in Chapter 1 to some fancier but commonly used shell features, such as metacharacters, quoting, creating new commands, passing arguments to them, the use of shell variables, and some elementary
flow control.
These are topics you should know for your own use of the shell.
The material in Chapter 5 is heavier going - and is intended for writing serious shell programs, ones that are bullet-proofed for use by others.
The division between the two chapters is somewhat arbitrary, of course, so both should be read eventually.

### 3.1 Command line structure

To proceed, we need a slightly better understanding of just what a command is, and how it is interpreted by the shell.
This section is a more formal coverage, with some new information, of the shell basics introduced in the first chapter.

The simplest command is a single *word*, usually naming a file for execution (later we will see some other types of commands):
```
$ who
you        tty2    Sep 28 07:51
jpl        tty4    Sep 28 08:32
$
```
A command usually ends with a newline, but a semicolon `;` is also a *command terminator*:
```
$ date;
Wed Sep 28 09:07:15 EDT 1983
$ date; who
Wed Sep 28 09:07:23 EDT 1983
you        tty2    Sep 28 07:51
jpl        tty4    Sep 28 08:32
$
```
Although semicolons can be used to terminate commands, as usual nothing happens until you type RETURN.
Notice that the shell only prints one prompt after multiple commands, but except for the prompt,
```
$ date; who
```
is identical to typing the two commands on different lines.
In particular, `who` doesn't run until `date` is finished.

Try sending the output of `date; who` through a pipe:
```
$ date; who | wc
Wed Sep 28 09:08:48 EDT 1983
      2     10     60
$
```
This might not be what you expected, because only the output of `who` goes to `wc`.
Connecting `who` and `wc` with a pipe forms a single command called a *pipeline*, that runs after `date`.
The precedence of `|` is higher than that of `;` as the shell parses your command line.

Parentheses can be used to group commands:
```
$ (date; who)
Wed Sep 28 09:11:09 EDT 1983
you        tty2    Sep 28 07:51
jpl        tty4    Sep 28 08:32
$ (date; who) | wc
      3      16     89
$
```
The outputs of `date` and `who` are concatenated into a single stream that can be sent down a pipe.

Data glowing through a pipe can be tapped and placed in a file (but not another pipe) with the `tee` command, which is not part of the shell, but is nonetheless handy for manipulating pipes.
One use is to save intermediate output in a file:
```
$ (date; who) | tee save | wc
      3      16     89
$ cat save
Wed Sep 28 09:13:22 EDT 1983
you        tty2    Sep 28 07:51
jpl        tty4    Sep 28 08:32
$ wc < save
      3      16     89
$
```
`tee` copies its input to the named file or files, as well as to its output, so `wc` receives the same data as if `tee` weren't in the pipeline.

Another command terminator is the ampersand `&`.
It's exactly like the semicolon or newline, except that it tells the shell not to wait for the command to complete.
Typically, `&` is used to run a long-running command "in the background" while you continue to type interactive commands:
```
$ long-running-command &
5273                             Process-id of long-running-command
$                                Prompt appears immediately
```
Given the ability to group commands, there are some more interesting uses of background processes.
The command `sleep` waits the specified number of seconds before exiting:
```
$ sleep 5
$                                Five seconds pass before prompt
$ (sleep 5; date) & date
5278
Wed Sep 28 09:18:20 EDT 1983     Output from second date
$ Wed Sep 28 09:18:25 EDT 1983   Prompt appears, then date 5 seconds later
```
The background process starts but immediately sleeps; meanwhile, the second `date` command prints the current time and the shell prompts for a new command.
Five seconds later, the `sleep` exits and the first `date` prints the new time.
It's hard to represent the passage of time on paper, so you should try this example.
(Depending on how busy your machine is and other such details, the difference between the two times might not be exactly five seconds.)
This is an easy way to run a command in the future; consider
```
$ (sleep 300; echo "Tea is ready") &
5291
$
```
as a handy reminder mechanism.
(A `ctl-g` in the string to be `echo`ed will ring the terminal's bell when it's printed.)
The parentheses are needed in these examples, since the precedence of `&` is higher than that of `;`.

The `&` terminator applies to commands, and since pipelines are commands you don't need parentheses to run pipelines in the background:
```
$ pr file | lpr &
```
arranges to print the file on the line printer without making you wait for the command to finish.
Parenthesizing the pipeline has the same effect, but requires more typing:
```
$ (pr file | lpr) &              Same as last example
```

Most programs accept *arguments* on the command line, such as `file` (an argument to `pr`) in the above example.
Arguments are words separated by blanks and tabs, that typically name files to be processed by the command, but they are strings that may be interpreted any way the program sees fit.
For example, `pr` accepts names of files to print, `echo` echoes it's arguments without interpretation, and `grep`'s first argument specifies a text pattern to search for.
And, of course, most programs also have options, indicated by arguments beginning with a minus sign.

The various special characters interpreted by the shell, such as `<`, `>`, `|`, `;`, and `&`, are *not* arguments for the programs the shell runs.
They instead control how the shell runs them.
For example,
```
$ echo Hello > junk
```
tells the shell to run `echo` with the single argument `Hello`, and place the output in the file called `junk`.
The string `> junk` is not an argument to `echo`; it is interpreted by the shell and never seen by `echo`.
In fact, it need not be the last string in the command:
```
> junk echo Hello
```
is identical, but less obvious.

### 3.2 Metacharacters

The shell recognizes a number of other characters as special; the most commonly used is the asterisk `*` which tells the shell to search the directory for filenames in which any string of characters occurs in the position of the `*`.
For example,
```
$ echo *
```
is a poor facsimile of `ls`.
Something we didn't mention in Chapter 1 is that the filename-matching characters do not look at filenames beginning with a dot, to avoid problems with the names `.` and `..` that are in every directory.
The rule is: the filename-matching characters only match filenames beginning with a period if the period is explicitly supplied in the pattern.
As usual, a judicious `echo` or two will clarify what happens:
```
$ ls
.profile
junk
temp
$ echo *
junk temp
$ echo .*
. .. .profile
$
```

Characters like `*` that have special properties are known as *metacharacters*.
There are a lot of them: Table 3.1 is the complete list, although a few of them won't be discussed until Chapter 5.

| Character          | Description                                                                          |
|--------------------|--------------------------------------------------------------------------------------|
| `>`                | `prog > file` direct standard output to `file`                                       |
| `>>`               | `prog >> file` append standard output to `file`                                      |
| `<`                | `prog < file` take standard input from `file`                                        |
| `pipe`             | `p1 pipe p2` connect standard output of `p1` to standard input of `p2`               |
| `<<str`            | *here document*: standard input follows, up to next `str` on a line by itself        |
| `*`                | match any string of zero or more characters in filenames                             |
| `?`                | match any single character in filenames                                              |
| `[ccc]`            | match any single character from `ccc` in filenames; <br/> ranges like `0-9` or `a-z` |
| `;`                | command terminator: `p1;p2` does `p1`, then `p2`                                     |
| `&`                | like `;` but doesn't wait for `p1` to finish                                         |
| \`...\`            | run command(s) in `...`; output replaces \`...\`                                     |
| `(...)`            | run command(s) in `...` in a sub-shell                                               |
| `{...}`            | run command(s) in `...` in current shell (rarely used)                               |
| `$1`, `$2`, *etc.* | `$0`...`$9` replaced by arguments to shell file                                      |
| `$var`             | value of shell variable `var`                                                        |
| `${var}`           | value of `var`; avoids confusion when concatenated with text                         |
| `\\`               | `\c` take character `c` literally, `\newline` discarded                              |
| `'...'`            | take `...` literally                                                                 |
| `"..."`            | take `...` literally after `$`, `'...'`, and `\\` interpreted                        |
| `#`                | if `#` starts word, rest of line is a comment                                        |
| `var=value`        | assign to variable `var`                                                             |
| `p1 && p2`         | run `p1`; if successful, run `p2`                                                    |
| `p1 pipe pipe p2`  | run `p1`; if unsuccessful run `p2 `                                                  |

Given the number of shell metacharacters, there has to be some way to tell the shell, "Leave it alone."
The easiest and best way to protect special characters from being interpreted is to enclose them in single quote characters"
```
$ echo '***'
***
$
```
It's also possible to use the double quotes `"..."`, but the shell actually peeks inside these quotes to look for `$`, \`...\`, and `\\`, so don't use `"..."` unless you intend some processing of the quoted string.

A third possibility is to put a backslash `\\` in front of *each* character that you want to protect from the shell, as in 
```
$ echo \*\*\*
***
$
```
Although `\*\*\*` isn't much like English, the shell terminology for it is still a *word*, which is any single string the shell accepts as a unit, including blanks if they are quoted.

Quotes of one kind protect quotes of the other kind:
```
$ echo "Don't do that!"
Don't do that!
$
```
and they don't have to surround the whole argument:
```
$ echo x'*'y
x*y
$ echo '*'A'?'
*A?
$
```
In this last example, because the quotes are discarded after they've done their job, `echo` sees a single argument containing no quotes.

Quoted strings can contain newlines:
```
$ echo 'hello
> world'
hello
world
$
```
The string `'> '` is a *secondary prompt* printed by the shell when it expects you to type more input to complete a command.
In this example the quote on the first line has to be balanced with another.
The secondary prompt string is stored in the shell variable `PS2`, and can be modified to taste.

In all of these examples, the quoting of a metacharacter prevents the shell from trying to interpret it.
The command
```
$ echo x*y
```
echoes all the filenames beginning in `x` and ending in `y`.
As always, `echo` knows nothing about files or shell metacharacters; the interpretation of `*`, if any, is supplied by the shell.

What happens if no files match the pattern?
The shell, rather than complaining (as it did in early versions), passes the string on as though it had been quoted.
It's usually a bad idea to depend on this behavior, but it can be exploited to learn of the existence of files matching a pattern:
```
$ ls x*y
x*y not found          Message from ls: no such files exist
$ > xyzzy              Create xyzzy
$ ls x*y
xyzzy                  File xyzzy matches x*y
$ ls 'x*y'
x*y not found          ls doesn't interpret the *
$
```

A backslash at the end of a line causes the line to be continued; this is the way to present a very long line to the shell.
```
echo abc\
> def\
> ghi
abcdefghi
$
```
Notice that the newline is discarded when preceded by a backslash, but is retained when it appears in quotes.

The metacharacter `#` is almost always universally used for shell comments; if a shell word begins with `#`, the rest of the line is ignored:
```
$ echo hello # there
hello
$ echo hello#there
hello#there
$
```
The `#` was not part of the original 7th Edition, but it has been adopted very widely, and we will use it in the rest of the book.

#### A digression on `echo`

### 3.3 Creating new commands

It's now time to move on to something that we promised in Chapter 1 - how to create new commands out of old ones.

Given a sequence of commands that is to be repeated more than a few times, it would be convenient to make it into a "new" command with its own name, so you can use it like a regular command.
To be specific, suppose you intend to count users frequently with the pipeline
```
$ who | wc -l
```
that was mentioned in Chapter 1, and you want to make a new program `nu` (number users) to do that.

The first step is to create an ordinary file that contains `who | wc -l`.
You can use a favorite editor, or you can get creative:
```
$ echo 'who | wc -l' > nu
```
(Without the quotes what would appear in `nu`?)

As we said in Chapter 1, the shell is a program just like an editor or `who` or `wc`; its name is `sh`.
And since it's a program, you can run it and redirect *its* input.
So run the shell with its input coming from the `nu` file instead of the terminal:
```
$ who
you     tty2   Sep 28 07:51
rhh     tty4   Sep 28 10:02
moh     tty5   Sep 28 09:38
ava     tty6   Sep 28 10:17
$ cat nu
who | wc -l
$ sh < nu
      4
$
```
The output is the same as it would have been if you had typed `who | wc -l` at the terminal.

Again like most other programs, the shell takes its input from a file if one is named as an argument; you could have written
```
$ sh nu
```
for the same result.
But it's a nuisance to have to type `"sh"` in either case: its longer, and it creates a distinction between programs written in, say, C and ones written by connecting programs with the shell.
Therefore, if a file is executable and if it contains text, then the shell assumes it to be a file of shell commands.
Such a file is called a *shell file*.
All you have to do is to make `nu` executable, once:
```
$ chmod +x nu
```
and thereafter you can invoke it with
```
$ nu
```
From now on, users of `nu` cannot tell, just by running it, that you implemented it in this easy way.

The way a shell actually runs `nu` is to create a new shell process exactly as if you had typed
```
$ sh nu
```
This child shell is called a *sub-shell* - a shell process invoked by your current shell.
`sh nu` is not the same as `sh < nu`, because its standard input is still connected to the terminal.

As it stands, `nu` only works if it's in your current directory (provided, of course, that the current directory is in your `PATH`, which we will assume from now on).
To make `nu` part of your repertoire regardless of what directory you're in, move it to your private `bin` directory, and add `/usr/you/bin` to your search path:
```
$ pwd
/usr/you
$ mkdir bin
$ echo $PATH
:/usr/you/bin:/bin:/usr/bin
$ mv nu bin
$ ls nu
nu not found
$ nu
      4
$
```
Of course, your `PATH` should be set properly by your `.profile`, so you don't have to respect it every time you log in.

There are other simple commands that you might create this way to tailor your environment to your own taste.
Some that we have found convenient include
- `cs`
  - which echoes the proper sequence of mysterious characters to clear the screen on your terminal (24 newlines is a fairly general implementation)
- `what`
  - Which runs `who` and `ps -a` to tell who's logged on and what they are doing
- `where`
  - which prints out the identifying name of the UNIX system you're using - it's handy if you use several regularly. (Setting `PS1` serves a similar purpose.)

### 3.4 Command arguments and parameters

Although `nu` is adequate as it stands, most shell programs interpret arguments, so that, for example, filenames and options can be specified when the program is run.

Suppose we want to make a program called `cx` to change the mode of a file to executable, so
```
$ cx nu
```
is shorthand for
```
$ chmod +x nu
```
We already know almost enough to do this.
We need a file called `cx` whose contents are
```
chmod +x filename
```
The only thing new we need to know is how to tell `cx` what the name of the file is, since it will be different each time `cx` is run.

When the shell file executes a file of commands, each occurrence of `$1` is replaced by the first argument, each `$2` is replaced by the second argument, and so on through `$9`.
So if the file `cx` contains
```
chmod +x $1
```
when the command
```
$ cx nu
```
is run, the sub-shell replaces `$1` by its first argument, `nu`.

Let's look at the whole sequence of operations:
```
$ echo 'chmod +x $1' > cx           Create cx originally
$ sh cx cx                          Make cx itself executable
$ echo 'echo Hi, there!' > hello    Make a test program
$ hello                             Try it
hello: cannot execute
$ cx hello                          Make it executable
$ hello                             Try again
Hi, there!                          It works
$ mv cx /usr/you/bin                Install cx
$ rm hello                          Clean up
$
```
Notice that we said
```
$ sh cx cx
```
exactly as the shell would have automatically done if `cx` were already executable and we typed
```
$ cx cx
```

What if you want to handle more than one argument, for example to make a program like `xc` handle several files at once?
A crude first cut is to put nine arguments into the shell program, as in
```
chmod +x $1 $2 $3 $4 $5 $6 $7 $8 $9
```
(it only works up to `$9`, because the string `$10` is parsed as "first argument `$1` followed by a `0`"!)
If the user of this shell file provides fewer than nine arguments, the missing ones are null strings; the effect is that only the arguments that were actually provided are passed to `chmod` by the sub-shell.
So this implementation works, but it's obviously unclean, and it fails if more than nine arguments are provided.

Anticipating this problem, the shell provides a shorthand `$*` that means "all the arguments."
The proper way to define `cx`, then, is
```
chmod +x $*
```
which works regardless of how many arguments are provided.

With `$*` added to your repertoire, you can make some convenient shell files, such as `lc` or `m`:
```
$ cd /usr/you/bin
$ cat lc
# lc: count number of lines in files
wc -l $*
$ cat m
# m: a concise way to type mail
mail $*
$
```
Both can sensibly be used without arguments.
If there are no arguments, `$*` will be null, and no arguments at all will be passed to `wc` or `mail`.
With or without arguments, the command is invoked properly:
```
$ lc /usr/you/bin/*
      1 /usr/you/bin/cx
      2 /usr/you/bin/lc
      2 /usr/you/bin/m
      1 /usr/you/bin/nu
      2 /usr/you/bin/what
      1 /usr/you/bin/where
      9 total
$ ls /usr/you/bin | lc
      6
$
```

These commands and the others in this chapter are examples of *personal* programs, the sort of things you write for yourself and put in your `bin`, but are unlikely to make publicly available because they are too dependent on personal taste.
In Chapter 5 we will address the issues of writing shell programs suitable for public use.

The arguments to a shell file need not be filenames.
For example, consider searching a personal telephone directory.
If you have a file names `/usr/you/lib/phone-book` that contains lines like
```
dial-a-joke	212-976-3838
dial-a-prayer	212-246-4200
dial santa	212-976-3636
dow jones report	212-976-4141
```
then the `grep` command can be used to search it.
(Your own `lib` directory is a good place to store such personal data bases.)
Since `grep` doesn't care about the format of information, you can search for names, addresses, zip-codes, or anything else that you like.
Let's make a directory assistance program, which we'll call `411` in honor of the telephone directory assistance number where we live:
```
$ echo 'grep $* /usr/you/lib/phone-book' > 411
$ cx 411
$ 411 joke
dial-a-joke	212-976-3838
$ 411 dial
dial-a-joke	212-976-3838
dial-a-prayer	212-246-4200
dial santa	212-976-3636
$ 411 'dow jones'
grep: can't open jones
$                                 Something is wrong
```
The final example is included to show a potential problem: even though `dow jones` is presented to `411` as a single argument, it contains a space and is no longer in quotes, so the sub-shell interpreting the `411` command converts it into two arguments to `grep`: it's as if you had typed
```
$ grep dow jones /usr/you/lib/phone-book
```
and that's obviously wrong.

One remedy relies on the way the shell treats double quotes.
Although anything quoted with `'...'` us inviolate, the shell looks inside `"..."` for `$`'s `\\`'s, and `'...'`'s.
So if you revise `411` to look like
```
grep "$*" /usr/you/lib/phone-book
```
the `$*` will be replaced by the arguments, but it will be passed to `grep` as a single argument even if it contains spaces.
```
$ 411 dow jones
dow jones report	212-976-4141
$
```

By the way, you can make `grep` (and thus `411`) case-independent with the `-y` option:
```
$ grep -y pattern ...
```
With `-y`, lower case letters in `pattern` will also match upper case letters in the input.
(This option is in the 7th Edition `grep`, but is absent from some other systems.)

There are fine points about command arguments that we are skipping over until Chapter 5, but one is worth noting here.
The argument `$0` is the name of the program being executed - in `cx`, `$0` is `"cx"`.
A novel use of `$0` is in the implementation of the programs `2`, `3`, `4`, ..., which print their output in that many columns:
```
$ who | 2
drh			tty0		Sep 28 21:23			cvw			tty5		Sep 28 21:09
dmu			tty6		Sep 28 22:10			scj			tty7		Sep 28 22:11
you			tty9  	Sep 28 23:00			jib			ttyb		Sep 28 19:58
```

The implementations of `2`, `3`, ... are identical; in fact they are links to the same file:
```
$ ln 2 3; ln 2 4; ln 2 5; ln 2 6; 
$ ls -li [1-9]
16722 -rwxrwxrwx 5 you			51 Sep 28 23:21 2
16722 -rwxrwxrwx 5 you			51 Sep 28 23:21 3
16722 -rwxrwxrwx 5 you			51 Sep 28 23:21 4
16722 -rwxrwxrwx 5 you			51 Sep 28 23:21 5
16722 -rwxrwxrwx 5 you			51 Sep 28 23:e1 6
$ ls /usr/you/bin | 5
...
...
$ cat 5
# 2, 3, ...: print in n columns
pr -$0 -t -l1 $*
```
The `-t` option turns off the heading at the top of the page and the `-ln` option sets the page length to `n` lines.
The name of the program becomes the number-of-columns argument to `pr`, so the output is printed a row at a time in the number of columns specified by `$0`.

### 3.5 Program output as arguments

Let us turn now from command arguments within a shell file to the generation of arguments.
Certainly filename expansion from metacharacters like `*` is the most common way to generate arguments (other than by providing them explicitly), but another good way is by running a program.
The output of any program can be placed in a command line by enclosing the invocation in back-quotes (\`...\`)


### 3.6 Shell variables
### 3.7 More on I/O redirection
### 3.8 Looping in shell programs
### 3.9 `bundle`: Putting it All Together
### 3.10 Why a Programmable Shell?

