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

## Chapter 7: UNIX System Calls

This chapter concentrates on the lowest level of interaction with the UNIX operating system - the system calls.
These are entries to the kernel.
They *are* the facilities that the operating system provides; everything else is built on top of them.

We will cover several major areas.
First is the I/O system, the foundation beneath library routines like `fopen` and `putc`.
We'll talk more about the file system as well, particularly directories and inodes.
Next comes a discussion of processes - how to run programs from within a program.
After that we will talk about signals and interrupts: what happens when you push the DELETE key, and how to handle that sensibly in a program.

As in Chapter 6, many of our examples are useful programs that were not part of the 7th Edition.
Even if they are not directly helpful to you, you should learn something from reading them, and they might suggest similar tools that you could build for your system.

Full details on the system calls are in Section 2 of the *UNIX Programmer's Manual*; this chapter describes the most important parts, but makes no pretense of completeness.

### 7.1 Low-level I/O

The lowest level of I/O is a direct entry into the operating system.
Your program reads or writes files in chunks of any convenient size.
The kernel buffers your data into chunks that match the peripheral devices, and schedules operations on the devices to optimize their performance over all users.

#### File descriptors

All input and output is done by reading or writing files, because all peripheral devices, even your terminal, are files in the file system.
This means that a single interface handles all communication between a program and peripheral devices.

In the most general case, before reading or writing a file, it is necessary to inform the system of your intent to do so, a process called *opening* the file.
If you are going to write on a file, it may also be necessary to *create* it.
The system checks your right to do so (Does the file exist? Do you have permissions to access it?), and if all is well, returns a non-negative integer called a *file descriptor*.
Whatever I/O is to be done on the file, the file descriptor is used instead of the name to identify the file.
All information about an open file is maintained by the system; your program refers to the file only by the file descriptor.
A `FILE` pointer as discussed in Chapter 6 points to a structure that contains, among other things, the file descriptor; the macro `fileno(fp)` defined in `<stdio.h>` returns the file descriptor.

There are special arrangements to make terminal input and output convenient.
When it is started by the shell, a program inherits three open files, with file descriptors `0`, `1`, and `2`, called the standard input, the standard output, and the standard error.
All of these are by default connected to the terminal, so if a program only reads file descriptor `0` and writes file descriptors `1` and `2`, it can do I/O without having to open files.
If the program opens any other files, they will have file descriptors `3`, `4`, etc.

If I/O is redirected to or from tiles or pipes, the shell changes the default assignments for file descriptors `0` and `1` from the terminal to the named files.
Normally file descriptor `2` remains attached to the terminal, so error messages can go there.
Shell incantations such as `2>filename` and `2>&1` will cause rearrangements of the defaults, but the file assignments are changed by the shell, not by the program.
(The program itself can rearrange these further if it wishes, but this is rare.)

#### File I/O - `read` and `write`

All input and output is done by two system calls, `read` and `write`, which are accessed from C by functions of the same name.
For both, the first argument is a file descriptor.
The second argument is an array of bytes that serves as the data source or destination.
The third argument is the number of bytes to be transferred.
```c
int fd, n, nread, nwritten;
char buf[SIZE];

nread = read(fd, buf, n);
nwritten = write(fd, buf, n);
```
Each call returns a count of the number of bytes transferred.
On reading, the number of bytes returned may be less than the number requested, because fewer than `n` bytes remained to be read.
(When the file is a terminal, `read` normally reads only up to the next newline, which is usually less than what was requested.)
A return value of zero implies end of file, and `-1` indicates an error of some sort.
For writing, the value returned is the number of bytes actually written; an error has occurred if this isn't equal to the number supposed to be written.

While the number of bytes to be read or written is not restricted, the two most common values are 1, which means one character at a time ("unbuffered"), and the size of a block on a disc, most often 512 or 1024 bytes. 
(The parameter `BUFSIZ` in `<stdio.h>` has this value.)

To illustrate, here is a program to copy its input to its output.
Since the input and output can be redirected to any file or device, it will actually copy anything to anything: it's a bare-bones implementation of `cat`.
```c
/* cat:  minimal version */

#define SIZE 512  /* arbitrary */

int main() 
{
    char buf[SIZE];
    int n;

    while ((n = read(0, buf, sizeof buf)) > 0)
        write(1, buf, n);
    exit(0);
}
```
If the file size is not a multiple of `SIZE`, some `read` will return a smaller number of bytes to be written by `write`; the next call to `read` after that will return zero.

Reading and writing in chunks that match the disc will be most efficient, but even character-at-a-time I/O is feasible for modest amounts of data, because the kernel buffers your data; the main cost is the system calls.
`ed`, for example, uses one-byte reads to retrieve its standard input.
We timed this version of `cat` on a file of 54,000 bytes, for six values of `SIZE`:
```
TODO:
```
The disc block size is 512 bytes on the PDP-11 system and 1024 on the VAX.

It is quite legal for several processes to be accessing the same file at the same time; indeed, one process can be writing while another is reading.
If this isn't what you wanted, it can be disconcerting, but it's sometimes useful.
Even though one call to `read` returns `0` and thus signals the end of file, if more data is written on that file, a subsequent `read` will find more bytes available.
This observation is the basis of a program called `readslow`, which continues to read its input, regardless of whether it got an end of file or not.
`readslow` is handy for watching the progress of a program:
```
$ slowprog >temp &
5213
$ readslow <temp | grep something
```
In other words, a slow program produces output in a file; `readslow`, perhaps in collaboration with some other program, watches the data accumulate.

Structurally, `readslow` is identical to `cat` except that it loops instead of quitting when it encounters the current end of the input.
It has to use low-level I/O because the standard library routines continue to report `EOF` after the first end of file.
```c
/* readslow:  keep reading, waiting for more */

#define SIZE 512  /* arbitrary */

main() 
{
    char buf[SIZE];
    int n;

    for (;;) {
        while ((n = read(0, buf, sizeof buf)) > 0)
            write(1, buf, n);
        sleep(10);
    }
}
```

The function `sleep` causes the program to be suspended for the specified number of seconds; it is described in `sleep`(3).
We don't want `readslow` to bang away at the file continuously looking for more data; that would be too costly in CPU time.
Thus this version of `readslow` copies its input up to the end of file, sleeps a while, then tries again.
If more data arrives while it is asleep, it will be read by the next `read`.

#### File creation - `open`, `creat`, `close`, `unlink`

Other than the default standard input, output, and error files, you must explicitly open files in order to read or write them.
There are two system calls for this, `open` and `creat`.

`open` is rather line `fopen` in the previous chapter, except that instead of returning a file pointer, it returns a file descriptor, which is an `int`.
```c
char *name;
int fd, rwmode;

fd = open(name, rwmode);
```
As with `fopen`, the `name` argument is a character string containing the filename.
The access mode argument is different, however: `rwmode` is `0` for read, `1` for write, and `2` to open a file for both reading *and* writing. 
`open` returns `-1` if any error occurs; otherwise it returns a valid file descriptor.

It is an error to try to `open` a file that does not exist.
The system call `creat` is provided to create new files, or to rewrite o̷l̶d̸ ̵o̸n̷e̵s̵.
```c
int perms;

fd = creat(name, perms);
```
`creat` returns a file descriptor if it was able to create the file called `name`, and `-1` if not.
If the file does not exist, `creat` creates it with the *permissions* specified by the `perms` argument.
If the file already exists, `creat` will truncate it to zero length; it is not an error to `creat` a file that already exists.
(The permissions will not be changed.)
Regardless of `perms`, a `creat`ed file is open for writing.

As described in Chapter 2, there are nine bits of protection information associated with a file, controlling read, write, and execute permission, so a three-digit octal number is convenient for specifying them.
For example, `0755` specifies read, write, and execute permission for the owner, and read and execute permission for the group and everyone else.
Don't forget about the leading `0`, which is how octal numbers are specified in C.

To illustrate, here is a simplified version of `cp`.
The main simplification is that our version copies only one file, and does not permit the second argument to be a directory.
Another blemish is that our version does not preserve the permissions of the source file; we will show how to remedy this later.
```c
/* cp:  minimal version */

#include <stdio.h>
#define PERMS 0644  /* RW for owner, R for group, others */

char *progname;


int main(int argc, char *argv[]) 
{
    int f1, f2, n;
    char buf[BUFSIZ];
    progname = argv[0];

    if (argc != 3)
        error("Usage: %s from to", progname);
    if ((f1 = open(argv[1], 0)) == -1)
        error("can't open %s", argv[1]);
    if ((f2 = creat(argv[2], PERMS)) == -1)
        error("can't create %s", argv[2]);

    while ((n = read(f1, buf, BUFSIZ)) > 0)
        if (write(f2, buf, n) != n)
            error("write error", (char *) 0);

    return 0;
}
```
We will discuss `error` in the next subsection.

There is a limit (typically about 20; look for `NOFILE` in `<sys/param.h>`) on the number of files that a program may have open simultaneously.
Accordingly, any program that intends to process many files must be prepared to reuse file descriptors.
The system call `close` breaks the connection between a filename and a file descriptor, freeing the file descriptor for use with some other file.
Termination of a program via `exit` or return from the main program closes all open files.

The system call `unlink` removes a file from the file system.

#### Error processing - `errno`

The system calls discussed in this section, and in fact all system calls, can incur errors.
Usually they indicate an error by returning a value of `-1`.
Sometimes it is nice to know what specific error occurred; for this purpose all system calls, when appropriate, leave an error number in an external integer called `errno`.
(The meanings of the various error numbers are listed in the introduction to Section 2 of the *UNIX Programmer's Manual*).
By using `errno`, your program can, for example, determine whether an attempt to open a file failed because it did not exist or because you lacked permission to read it.
There is also an array of character strings `sys_errlist` indexed by `errno` that translates the numbers into a meaningful string.
Our version of `error` uses these data structures:
```c
/* print error message and die */
error(char *s1, *s2) 
{
    extern int errno, sys_nerr;
    extern char *sys_errlist[], *progname;

    if (progname)
        fprintf(stderr, "%s: ", progname);
    fprintf(stderr, s1, s2);
    if (errno > 0 && errno < sys_nerr)
        fprintf(stderr, " (%s)", sys_errlist[errno]);
    fprintf(stderr, "\n");
    exit(1);
}
```
`errno` is initially zero, and should always be less than `sys_nerr`.
It is not reset to zero when things go well, however, so you must reset it after each error if your program intends to continue.

Here is how error messages appear with this version of `cp`:
```
$ cp foo bar
cp: can't open foo (No such file or directory)
$ date >foo; chmod 0 foo
$ cp foo bar
cp: can't open foo (Permission denied)
$
```

#### Random access - `lseek`

File I/O is normally sequential: each `read` or `write` takes place in the file right after the previous one.
When necessary, however, a file can be read or written in an arbitrary order.
The system call `lseek` provides a way to move around in a file without actually reading or writing:
```c
int fd, origin;
long offset, pos, lseek();

pos = lseek(fd, offset, origin);
```
forces the current position in the file whose descriptor is `fd` to move to position `offset`, which is taken relative to the location specified by `origin`.
Subsequent reading or writing will begin at that position.
`origin` can be `0`, `1`, or `2` to specify that `offset` is to be measured from the beginning, from the current position, or from the end of the file.
The value returned is the new absolute position, or `-1` for an error.
For example, to append to a file, seek to the end before writing:
```c
lseek(fd, 0L, 1)
```
To get back to the beginning ("rewind"),
```c
lseek(fd, 0L, 0)
```
To determine the current position,
```c
pos = lseek(fd 0L, 1)
```
Notice the `0L` argument: the offset is a `long` integer.
(The `'l'` in `lseek` stands for 'long,' to distinguish it from the 6th Edition `seek` system call that used short integers.)

With `lseek`, it is possible to treat files more or less like large arrays, at the price of slower access.
For example, the following function reads any number of bytes from any place in a file.
```c
// read n bytes from position pos
get(int fd, long pos, char *buf, int n)
{
    if (lseek(fd, pos, 0) == -1)  // get to pos
      return -1;
    else
      return read(fd, buf, n);
}
```

### 7.2 File System: directories
### 7.3 File System: inodes
#### `sv`: An illustration of error handling
### 7.4 Processes
#### Low-level process creation - `execlp` and `execvp`
#### Control of processes - `fork` and `wait`
### 7.5 Signals and Interrupts
#### Alarms
