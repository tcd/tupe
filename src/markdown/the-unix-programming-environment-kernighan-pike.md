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

## Chapter 6: Programming with Standard I/O

So far we have used existing tools to build new ones, but we are at the limit of what can be reasonably done with the shell, `sed`, and `awk`.
In this chapter we are going to write some simple programs in the C programming language.
The basic philosophy of making things that work together will continue to dominate the discussion and the design of the programs -  we want to create tools that others can use and build on.
In each case, we will also try to show a sensible implementation strategy: start with the bare minimum that does something useful, then add features and options (only) if the need arises.

There are good reasons for writing new programs from scratch.
It may be that the program at hand just can't be solved with existing programs.
This is often true when the program must deal with non-text files, for example - the majority of the programs we have shown so far really work well only on textual information.
Or it may be too difficult to achieve adequate robustness or efficiency with just the shell and other general purpose tools.
In such cases, a shell version may be good for honing the definition and user interface of a program.
(And if it works well enough, there's no point re-doing it.)
The `zap` program from the last chapter is a good example: it took only a few minutes to write the first version in the shell, and the final version has an adequate user interface, but it's too slow.

We will be writing in C because it is the standard language of UNIX systems - the kernel and all user programs are written in C - and, realistically, no other language is nearly as well supported.
We will assume that you know C, at least well enough to read along.
If not, read *The C Programming Language*, by B. W. Kernighan and D. M. Ritchie (Prentice-Hall, 1978).

We will also be using the "standard I/O library," a collection of routines that provide efficient and portable I/O and system services for C programs.
The standard I/O library is available on many non-UNIX systems that support C, so programs that confine their system interactions to its facilities can easily be transported.

The examples we have chosen for this chapter have a common property: they are small tools that we use regularly, but that were not part of the 7th Edition.
If your system has similar programs, you may find it enlightening to compare designs.
And if they are new to you, you may find them useful as we have.
In any case, they should help to make the point that no system is perfect, and that often it is quite easy to improve things and to cover up defects with modest effort.

### 6.1 Standard input and output: `vis`

Many programs read only one input and write output; for such programs, I/O that uses only standard input and standard output may be entirely adequate, and it is almost always enough to get started.

Let us illustrate with a program called `vis` that copies its standard input to its standard output, except that it makes all non-printing characters visible by printing them as *\nnn*, where *nnn* is the octal value of the character.
`vis` is invaluable for detecting strange or unwanted characters that may have crept into files.
For instance, `vis` will print each backspace as `\010`, which is the octal valye of the backspace character:
```
$ cat x
abc
$ vis <x
anc\010\010\010___
$
```
To scan multiple files with this rudimentary version of `vis`, you can just use `cat` to collect the files:
```
$ cat file1 file2 ... | vis
...
$ cat file1 file2 ... | vis | grep '\\'
```
and thus avoid learning how to access files from a program.

By the way, it might seem that you could do this job with `sed`, since the `1` command displays non-printable characters in an understandable form:
```
$ sed -n 1 x
abc←←←___
$
```
The `sed` output is probably clearer than that from `vis`.
But `sed` was never meant for non-text files:
```
$ sed -n 1 /usr/you/bin
$                        Nothing at all!
```
(This was on a PDP-11; on one VAX system, `sed` aborted, probably because the input looks like a very long line of text.)
So `sed` is inadequate, and we are forced to write a new program.

The simplest input and output routines are called `getchar` and `putchar`.
Each call to `getchar` gets the next character from the standard input, which may be a file or a pipe or the terminal (the default) - the program doesn't know which.
Similarly, `putchar(c)` puts the character `c` on the standard output, which is also by default the terminal.

The function `printf`(3) does output format conversion.
Calls to `printf` and `putchar` may be interleaved in any order; the output will appear in the order of the calls.
There is a corresponding function `scanf`(3) for input format conversion; it will read the standard input and break it up into strings, numbers, etc., as desired.
Calls to `scanf` and `getchar` may also be intermixed.

Here is the first version of `vis`:
```c
/* vis:  make funny characters visible (version 1) */

#include <stdio.h>
#include <ctype.h>

main()
{
    int c;

    while ((c = getchar()) != EOF)
        if (isascii(c) && 
            (isprint(c) || c=='\n' || c=='\t' || c==' '))
            putchar(c);
        else
            printf("\\%03o", c);
    exit(0);
}
```
`getchar` returns the next byte from the input, or the value `EOF` when it encounters the end of file (or an error).
By the way, `EOF` is *not* a byte from the file; recall the discussion of end of file in Chapter 2.
The value of `EOF` is guaranteed to be different from any value that occurs in a single byte so it can be distinguished from real data; `c` is declared `int` not `char`, so that it is big enough to hold the `EOF` value. The line
```c
#include <stdio.h>
```
should appear at the beginning of each source file.
It causes the C compiler to read a *header file* (`/usr/include/stdio.h`) of standard routines and symbols that includes the definition of `EOF`.
We will use `<stdio.h>` as a shorthand for the full filename in the text.

The file `<ctype.h>` is another header file in `/usr/include` that defines machine-independent tests for determining the properties of characters.
We used `isascii` and `isprint` here, to determine whether the input character is ASCII (i.e., value less than `0200`) and printable; other tests are listed in Table 6.1.
Notice that newline, tab, and blank are not "printable" by the definitions in `<ctype.h>`.

The call to `exit` at the end of `vis` is not necessary to make the program work properly, but it ensures that any caller of the program will see a normal exit status (conventionally zero) from the program when it completes.
An alternate way to return status is to leave `main` with `return 0`; the return value from `main` is the program's exit status.
If there is no explicit `return` or `exit`, the exit status is unpredictable.

To compile a C program, put the source in a file whose name ends in `.c`, such as `vids.c`, compile it with `cc`, then run the result, which the compiler leaves in a file called `a.out` (`a` is for assembler):
```
$ cc vis.c
$ a.out
hello worldctl-g
hello world\007
ctl-d
$
```

Normally you would rename `a.out` once it's working, or use the `cc` option `-o` to do it directly:
```
$ cc -o vis vis.c                  Output in vis, not a.out
```

| Function      | Description                                         |
|---------------|-----------------------------------------------------|
| `isalpha(c)`  | alphabetic: `a-z` `A-Z`                             |
| `isupper(c)`  | upper case: `A-Z`                                   |
| `islower(c)`  | lower case: `a-z`                                   |
| `isdigit(c)`  | digit: `0-9`                                        |
| `isxdigit(c)` | hexadecimal digit: `0-9` `a-f` `A-F`                |
| `isalnum(c)`  | alphabetic or digit                                 |
| `isspace(c)`  | blank, tab, newline, vertical tab, formfeed, return |
| `ispunct(c)`  | not alphanumeric or control or space                |
| `isprint(c)`  | printable: any graphic                              |
| `iscntrl(c)`  | control character `0 <= c < 040 || c == 0177`       |
| `isascii(c)`  | ASCII character: `0 <= c <= 0177`                   |

### 6.2 Program arguments: `vis` version 2

When a C program is executed, the command-line arguments are made available to the function `main` as count `argc` and an array of `argv` pointers to character strings that contain the arguments.
By convention, `argv[0]` is the command name itself, so `argc` is always greater than 0; the "useful arguments" are `argv[1] ... argv[argc-1]`.
Recall that redirection with `<` and `>` is done by the shell, not by individual programs, so redirection has no effect on the number of arguments seen by the program.

To illustrate argument handling, let's modify `vis` by adding an optional argument: `vis -s` strips out any non-printing characters rather than displaying them prominently.
This option is handy for cleaning up files from other systems, for example those that use `CRLF` (carriage return and line feed) instead of newline to terminate lines.
```c

/* vis:  make funny characters visible (version 2) */

#include <stdio.h>
#include <ctype.h>

main(int argc, char *argv[])
{
    int c, strip = 0;

    if ((argc > 1) && (strcmp(argv[1], "-s") == 0))
        strip = 1;

    while ((c = getchar()) != EOF)
        if (isascii(c) && (isprint(c) || c=='\n' || c=='\t' || c==' '))
            putchar(c);
        else if (!strip)
            printf("\\%3o", c);

    exit(1);
}
```
`argv` is a pointer to an array whose individual elements are pointers to arrays of characters; each array is terminated by the ASCII character `NUL` (`'\0'`), so it can be treated as a string.
This version of `vis` starts by checking to see if there is an argument and if it is `-s`.
(Invalid arguments are ignored.)
The function `strcmp`(3) compares two strings, returning zero if they are the same.

Table 6.2 lists a set of string handling and general utility functions, of which `strcmp` is one.
It's usually best to use these functions instead of writing your own, since they are standard, they are debugged, and they are often faster than what you can write yourself because they have been optimizes for particular machines (sometimes by being written in assembly language).

| Function         | Description |
|------------------|-------------|
| `strcat(s,t)`    | append string `t` to string `s`; return `s` |
| `strncat(s,t,n)` | append at most `n` characters of `t` to `s` |
| `strcpy(s,t)`    | copy `t` to `s`; return `s` |
| `strncpy(s,t,n)` | copy exactly `n` characters; null pad if necessary |
| `strcmp(s,t)`    | compare `s` and `t`, return `<0`, `0`, `>0` for `<`, `==`, `>` |
| `strncmp(s,t,n)` | compare at most `n` characters |
| `strlen(s)`      | return length of `s` |
| `strchr(s,c)`    | return pointer to first `c` in `s`, `NULL` if none |
| `strrchr(s,c)`   |  return pointer to last `c` in `s`, `NULL` if none |
| `atoi(s)`        | return integer value of `s` |
| `atof(s)`        | return floating point value of `s`; <br/> needs declaration `double atof()` |
| `malloc(n)`      | return pointer to `n` bytes of memory, `NULL` if can't |
| `calloc(n,m)`    | return pointer to `n`\*`m` bytes, set to `0`, `NULL` if can't; <br/> `malloc` and `calloc` return `char *` |
| `free(p)`        | free memory allocated by `malloc` or `calloc` |

### 6.3 File access: `vis` version 3

The first two versions of `vis` read the standard input and write the standard output, which are both inherited from the shell.
The next step is to modify `vis` to access files by their names, so that
```
$ vis file1 file2 ...
```
will scan the named files instead of the standard input.
If there are no filename arguments, though, we still want `vis` to read its standard input.

The question is how to arrange for the files to be read - that is, how to collect the filenames from the I/O statements that actually read the data.

The rules are simple.
Before it can be read or written a file must be *opened* by the standard library function `fopen`.
`fopen` takes a filename (like `temp` or `/etc/passwd`), does some housekeeping and negotiation with the kernel, and returns an internal name to be used in subsequent operations on the file.

This internal name is actually a pointer, called a *file pointer*, to a structure that contains information about the file, such as the location of a buffer, the current character position in the buffer, whether the file is being read or written, and the like.
One of the definitions obtained by including `<stdio.h>` is for a structure called `FILE`.
The declaration for a file pointer is
```c
FILE *fp;
```
This says that `fp` is a pointer to a `FILE`.
`fopen` returns a pointer to a `FILE`; there is a type declaration for `fopen` in `<stdio.h>`.

The actual call to `fopen` in a program is
```c
char *name, *mode;

fp = fopen(name, mode);
```
The first argument of `fopen` is the name of the file, as a character string.
The second argument, also a character string, indicates how you intend to use the file; the legal modes are read (`"r"`), write (`"w"`), or append (`"a"`).

If a file that you open for writing or appending does not exist, it is created if possible.
Opening an existing file for writing causes the old contents to be discarded.
Trying to read a file that does not exist is an error, as is trying to read or write a file when you don't have permission.
If there is any error, `fopen` will return the invalid pointer value `NULL` (which is defined, usually as `(char *)0`, in `<stdio.h>`)

The next thing needed is a way to read or write the file once it is open.
There are several possibilities, of which `getc` and `putc` are the simplest.
`getc` gets the next character from a file.
```c
c = getc(fp)
```
places in `c` the next character from the file referred to by `fp`; it returns `EOF` when it reaches end of file.
`putc` is analogous to `getc`:
```c
putc(c, fp)
```
puts the character `c` on the file `fp` and returns `c`.
`getc` and `putc` return `EOF` if an error occurs.

When a program is started, three files are open already, and file pointers are provided for them.
These files are the standard input, the standard output, and the standard error output; the corresponding file pointers are called `stdin`, `stdout`, and `stderr`.
These file pointers are declared in `<stdio.h>`; they may be used anywhere an object of type `FILE *` can be.
They are constants, however, *not* variables, so you can't assign to them.

`getchar()` is the same as `getc(stdin)` and `putchar(c)` is the same as `putc(c,stdout)`.
In fact, all four of these "functions" are defined as macros in `<stdio.h>`, since they run faster by avoiding the overhead of a function call for each character.
See Table 6.3 for some other definitions in `<stdio.h>`.

With some preliminaries out of the way, we can now write the third version of `vis`.
If there are command-line arguments, they are processed in order.
If there are no arguments, the standard input is processed.
```c
/* vis:  make funny characters visible (version 3) */

#include <stdio.h>
#include <ctype.h>

int strip = 0;  /* 1 => discard special characters */

int main(int argc, char *argv[]) {
    int i;
    FILE *fp;
    int vis(FILE *fp);

    while (argc > 1 && argv[1][0] == '-') {
        switch (argv[1][1]) {
            case 's':  /* -s: strip funny characters */
                strip = 1;
                break;
            default:
                fprintf(stderr, "%s: unknown arg %s\n", argv[0], argv[1]);
                return 1;
        }
        argc--;
        argv++;
    }

    if (argc == 1) {
        vis(stdin);
    } else {
        for (i = 1; 1 < argc; i++) {
            if ((fp=fopen(argv[i], "r")) == NULL) {
                fprintf(stderr, "%s: can't open %s\n", argv[0], argv[i]);
                return 1;
            } else {
                vis(fp);
                fclose(fp);
            }
        }
    }
    return 0;
}
```
This code relies on the convention that optional arguments come first.
After each optional argument is processed, `argc` and `argv` are adjusted so the rest of the program is independent of the presence of that argument.
Even though `vis` only recognizes a single option, we wrote the code as a loop to show one way to organize argument processing.
In Chapter 1 we remarked on the disorderly way that UNIX programs handle optional arguments.
One reason, aside from a taste for anarchy, is that it's obviously easy to write code to handle argument parsing for any variation.
The function `getopt`(3) found on some systems is an attempt to rationalize the situation; you might investigate it before writing your own.

The routine `vis` prints a single file:
```c
/* make chars visible in FILE *fp */
vis(FILE *fp)
{
    int c;

    while ((c = getc(fp)) != EOF)
        if (isascii(c) && (isprint(c) || c=='\n' || c=='\t' || c==' '))
            putchar(c);
        else if (!strip)
            printf("\\%03o", c);
} 
```

The function `fprintf` is identical to `printf`, except for a file pointer argument that specifies the file to be written.

The function `fclose` breaks the connection between the file pointer and the external name that was established by `fopen`, freeing the file pointer for another file.
Since there is a limit (about 20) on the number of files that a program may have open simultaneously, it's best to free files when they are no longer needed.
Normally, output produced with any of the standard library functions like `printf`, `putc`, etc., is buffered so it can be written in large chunks for efficiency.
(The exception is output to a terminal, which is usually written as it is produced, or at least when a newline is printed.)
Calling `fclose` on an output file also forces out any buffered output.
`fclose` is also called automatically, for each open file when a program calls `exit` or returns from `main`.

`stderr` is assigned to a program in the same way that `stdin` and `stdout` are.
Output written on `stderr` appears on the user's terminal even if the standard output is redirected.
`vis` writes its diagnostics on `stderr` instead of `stdout` so that if one of the files can't be accessed for some reason, the message finds its way to the user's terminal instead of disappearing down a pipeline or into an output file.
(The standard error was invented somewhat after pipes, after error messages *did* start disappearing into pipelines.)

Somewhat arbitrarily, we decided that `vis` will quit if it can't open an input file; this is reasonable for a program most often used interactively, and with a single input file.
You can argue for the other design as well, however.

| Name         | Definition                                     |
|--------------|------------------------------------------------|
| `stdin`      | standard input                                 |
| `stdout`     | standard output                                |
| `stderr`     | standard error                                 |
| `EOF`        | end of file; normally `-1`                     |
| `NULL`       | invalid pointer, normally `0`                  |
| `FILE`       | used for declaring file pointers               |
| `BUFSIZ`     | normal I/O buffer size (often `512` or `1024`) |
|              |
| `getc(fp)`   | return one character from stream `fp`          |
| `getchar()`  | `getc(stdin)`                                  |
| `putc(c,fp)` | put character `c` on stream `fp`               |
| `putchar(c)` | `putc(c,stdout)`                               |
| `feof(fp)`   | non-zero when end of file on stream `fp`       |
| `ferror(fp)` | non-zero when any error on stream `fp`         |
| `fileno(fp)` | file descriptor for stream `fp`; see Chapter 7 |

### 6.4 A screen-at-a-time printer: `p`

So far we have used `cat` to examine files.
But if a file is long, and if you are connected to your system by a high-speed connection, `cat` produces the output too fast to be read, even if you are quick with *ctl-s* and *ctl-q*.

There clearly should be a program to print a file in small, controllable chunks, but there isn't a standard one, probably because the original UNIX system was written in the days of hard-copy (paper) terminals and slow communications lines.
So our next example is a program called `p` that will print a file a screenful at a time, waiting for a response from the user after each screen before continuing to the next.
(`"p"` is a nice short name for a program that we use a lot.)
As with other programs, `p` reads either from files named as arguments or from its standard input:
```
$ p vis.c
...
$ grep '#define' *.[ch] | p
...
$
```

This program is best written in C because it's easy in C, and hard otherwise; the standard tools are not good at mixing the input from a file or pipe with terminal input.

The basic no-frills design is to print the input in small chunks.
A suitable chunk size is 22 lines: it's slightly less than the 24-line screen of most video terminals, and one third of a standard 66-line page.
A simple way for `p` to prompt the user is to not print the last newline of each 22-line chunk.
The cursor will thus pause at the right end of the line rather than at the left margin.
When the user presses RETURN, that will supply the missing newline and thus cause the next line to appear in the proper place.
If the user types *ctl-d* or `q` at the end of a screen, `p` will exit.

We will take no special action for long lines.
We will also not worry about multiple files: we'll merely skip from one to the next without comment.
That way the behavior of
```
$ p filenames
```
will be the same as
```
$ cat filenames... | p
```
If filenames are needed, they can be added with a `for` loop like
```
$ for i in filenames...
> do
>     echo $i:
>     cat $i
> done | p
```
Indeed, there are too many features that we can add to this program.
It's better to make a stripped-down version, then let it evolve as experience dictates.
That way, the features are the ones that people really want, not the ones we thought they would want.

The basic structure of `p` is the same as `vis`: the main routine cycles through the files, calling a routine `print` that does the work on each.
```c
/* p:  print input in chunks (version 1) */

#include <stdio.h>
#define PAGESIZE 22
char *progname;  /* program name for error message */

main(int argc, char *argv[]) 
{

    int i;
    FILE *fp, *efopen();

    progname = argv[0];

    if (argc == 1)
        print(stdin, PAGESIZE);
    else
        for (i = 1; i < argc; i++) {
            fp = efopen(argv[i], "r");
            print(fp, PAGESIZE);
            fclose(fp);
        }
    exit(0);
}
```

The routine `efopen` encapsulates a very common operation: try to open a file; if it's not possible, print an error message and exit.
To encourage error messages that identify the offending (or offended) program, `efopen` refers to an external string `progname` containing the name of the program, which is set to `main`.

```c
/* fopen file, die if can't */
FILE *efopen(char *file, char *mode) 
{
    FILE *fp, *fopen();
    extern char *progname;

    if ((fp = fopen(file, mode)) != NULL)
        return fp;
    fprintf(stderr, "%s: can't open file %s mode %s\n", progname, file, mode);
    exit(1);
}
```
We tried a couple of other designs for `efopen` before settling on this.
One was to have it return after printing the message, with a null pointer indicating failure.
This gives the caller the option of continuing or exiting.
Another design provided `efopen` with a third argument specifying whether it should return after failing to open the file.
In almost all of our examples, however, there's no point in continuing if a file can't be accessed, so the current version of `efopen` is best for our use.

The real work of the `p` command is done in `print`:
```c
/* print fp in pagesize chunks */
print(FILE *fp, int pagesize) {
    static int lines = 0; /* nu,ber of lines so far */
    char buf[BUFSIZ];

    while (fgets(buf, sizeof buf, fp) != NULL) {
        if (++lines < pagesize) {
            fputs(buf, stdout);
        } else {
            buf[strlen(buf)-1] = '\0';
            fputs(buf, stdout);
            fflush(stdout);
            ttyin();
            lines = 0;
        }
    }
}
```

We used `BUFSIZ`, which is defined in `<stdio.h>`, as the size of the input buffer.
`fgets(buf,zise,fp)` fetches the next line of input from `fp`, up to and including a newline, into `buf`, and adds a terminating `\0`; at most `size-1` characters are copied.
It returns `NULL` at end of file.
(`fgets` could be better designed: it returns `buf` instead of a character count; furthermore it provides no warning if the input line was too long. No characters are lost, but you have to look at `buf` to see what really happened.)

The function `strlen` returns the length of a string; we use that to knock the trailing newline off the last input line.
`fpus(buf,fp)` writes the string `buf` on file `fp`.
The call to `fflush` at the end of the page forces out any buffered output.

The task of reading the response from the user after each page has been printed is delegated to a routine called `ttyin`.
`ttyin` can't read the standard input, since `p` must work even when its input comes from a file or pipe.
To handle this, the program opens the file `/dev/tty`, which is the user's terminal regardless of any redirection of standard input.
We wrote `ttyin` to return the first character of the response, but don't use that feature here.
```c
/* process response from /dev/tty (version 1) */
ttyin() 
{
    char buf[BUFSIZ];
    FILE *efopen();
    static FILE *tty = NULL;

    if (tty == NULL)
        tty = efopen("/dev/tty", "r");
    if (fgets(buf, BUFSIZ, tty) == NULL || buf[0] == 'q')
        exit(0);
    else
        return buf[0];
}
```

### 6.5 An example: `pick`
### 6.6 On bugs and debugging
### 6.7 An example: `zap`
### 6.8 An interactive file comparison program: `idiff`
### 6.9 Accessing the environment
