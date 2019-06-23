# The Unix Programming Environment

## Chapter 1: Unix for Beginners

### Changes
- `ps -ag` --> `ps -A`
- `at`(1)

### Terms

|                   Term                    |                                                                                    Definition                                                                                     |
| ----------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Kernel                                    | a program that controls the resources of a computer and allocates them among its users                                                                                            |
| ~Full Duplex~                             | the characters you type on the keyboard are sent to the system, which sends them back to the terminal to be printed on the screen                                                 |
| Control Character                         | an invisible character that controls some aspect of input and output on the terminal                                                                                              |
| Prompt                                    | usually a single character, indicating that the system is ready to accept commands from you                                                                                       |
| Command Interpreter (Shell, Command Line) | your main interface to the system                                                                                                                                                 |
| Command (Program)                         | requests that the system do something                                                                                                                                             |
| Escape Character                          | `\\`; used extensively to indicate that the following character is in some way special                                                                                            |
| Text Editor (Screen Editor)               | a program for storing and manipulating information in the computer                                                                                                                |
| newline                                   | `\n`;  how the system represents RETURN                                                                                                                                           |
| Options (Command Flags)                   | used to alter a program's default behavior                                                                                                                                        |
| Arguments                                 | options or names of files to be used by the command                                                                                                                               |
| Directories                               | Folders                                                                                                                                                                           |
| Home Directory (login directory)          | `~`, `$HOME`; contains only the files that belong to a specific user. <br/> When you log in, you are "in" your home directory.                                                    |
| Current Directory (working directory)     | `.`; the directory you are currently working in                                                                                                                                   |
| Root Directory                            | `/`; The top level of the file ststem; is its own parent directory                                                                                                                |
| Pathname (Absolute Path)                  | represents the full name of the path from the root through the tree of directories to a particular file   <br/> wherever you can use an ordinary filename, you can use a pathname |
| Pipe                                      | `&vert;`;   a way to connect the output of one program to the input of another program                                                                                            |
| Pipeline                                  | a connection of two or more programs through pipes                                                                                                                                |
| Standard Input                            |                                                                                                                                                                                   |
| Standard Output                           | by default sent to the terminal                                                                                                                                                   |
| Standard Error                            | normally directed to your terminal                                                                                                                                                |
| Process                                   | an instance of a running program                                                                                                                                                  |
| Process ID                                | can be used to refer to a specific running program                                                                                                                                |
| Shell Variable                            | `$...`;                                                                                                                                                                           |
| Search Path                               | `$PATH`; a list of folders that is searched for commands to run when you type them into the shell                                                                                 |

## Chapter 2: The File System

### Changes
- (2.2) [Hex file signatures](https://en.wikipedia.org/wiki/List_of_file_signatures)
- (2.3) [Can't use `od` command with directories](https://unix.stackexchange.com/questions/173849/cant-use-od-command-with-directories)
- (2.4) `crypt` --> (`gpg` maybe)
- (2.4) `ls -lg /etc/passwd` --> `ls -l /etc/passwd`

### Terms 

| Term              | Definition                                                                                 |
| ----------------- | ---------------------------------------------------------------------------------          |
| File              | A file is a sequence of bytes. Everything is a file.                                       |
| Special Character | `\n`, `\t`, `\r`; characters associated with some terminal control operation               |
| File Permissions  | They determine who can do what with the file                                               |
| Super-User        | `root`; A user that exists on every system and has all permissions on every file.          |
| Login-Id          | *username*                                                                                 |
| User-Id           | user-id, used by the system to identify a user.                                            |
| Group-Id          | Used to give permissions to a group of users.                                              |
| Inodes            | Administrative information about a file                                                    |
| i-number          | The system's internal name for a file                                                      |
| link              | A filename in a directory (because it links a name in the directory hierarchy to an inode) |
| peripherals       | Discs, drives, terminals; devices connected to the computer                                |

### Questions
- What 3 types of permissions does every file have?

## Chapter 3: Using the Shell

### Terms

|         Term          |                                                                                       Definition                                                                                       |
| --------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Command Terminator    | RETURN, `;`;                                                                                                                                                                           |
| Pipeline              | Connecting two or more programs with a pipe forms a single command called a *pipeline*                                                                                                 |
| Arguments             | Arguments are words separated by blanks and tabs, that typically name files to be processed by the command, but they are strings that may be interpreted any way the program sees fit. |
| Metacharacters        | Characters that have special properties when written to the shell                                                                                                                      |
| Secondary Prompt      | A character printed by the shell when it expects you to type more input to complete a command.                                                                                         |
| Shell File            | A file containing shell commands                                                                                                                                                       |
| Sub-Shell             | a shell process invoked by your current shell                                                                                                                                          |
| Parameters            | `$*`; When you define them in a function, they are called *parameters*, when you give them to a command, they are called *arguments*. Same thing.                                      |
| Positional Parameters | `$1`-`$9`; variables that hold the arguments to a shell file                                                                                                                           |
| File Descriptors      | `0`, `1`, and `2`; Used in the shell to refer to standard input (`0`), standard output (`1`), and standard error (`2`).                                                                |
| Here Document         | `<<`; When input provided is right here instead of in a file somewhere                                                                                                                 |

## Chapter 4: Filters

### Terms

| Term                | Definition                                                                                  |
|---------------------|---------------------------------------------------------------------------------------------|
| Filters             | programs that read some input, perform a simple transformation on it, and write some output |
| Regular Expressions |                                                                                             |
| Character Classes   |                                                                                             |
| fields              |                                                                                             |
| seeking             |                                                                                             |
| associative array   |


### Filters
- `grep`
  - `fgrep`
  - `egrep`
- `sort`
- `uniq`
- `comm`
- `tr`
- `dd`
- `sed`
- `awk`


## Chapter 5: Shell Programming

### Terms

| Term | Definition |
|------|------------|
|      |            |

### `set`

