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

## Chapter 4: Filters

There is a large family of UNIX programs that read some input, perform a simple transformation on it, and write some output.
Examples include `grep` and `tail` to select part of the input, `sort` to sort it, `wc` to count it, and so on.
Such programs are called *filters*.

This chapter discusses the most frequently used filters.
We begin with `grep`, concentrating on patterns more complicated than those illustrated in Chapter 1.
We will also present two other members of the `grep` family, `egrep` and `fgrep`.

The next section briefly describes a few other useful filters, including `tr` for character transliteration, `dd` for dealing with data from other systems, and `uniq` for detecting repeated text lines.
`sort` is also presented in more detail than in Chapter 1.

The remainder of the chapter is devoted to two general purpose "data transformers" or "programmable filters."
They are called programmable because the particular transformation is expressed as a program in a simple programming language.
Different programs can produce very different transformations.

The programs are `sed` which stands for *stream editor*, and `awk`, named after its authors.
Both are derived from a generalization of `grep`:
```
$ program pattern-action filenames ...
```
scans the files in sequence, looking for lines that match a pattern; when one is found a corresponding action is performed.
For `grep`, the pattern is a regular expression as in `ed`, and the default action is to print each line that matches the pattern.

`sed` and `awk` generalize both the patterns and the actions.
`sed` is a derivative of `ed` that takes a "program" of editor commands and streams data from the files past them, doing the commands of the program on every line.
`awk` is not as convenient for text substitution as `sed` is, but it includes arithmetic, variables, built-in functions, and a programming language that looks quite a bit like C.
This chapter doesn't have the complete story on either program; Volume 2B of the *UNIX Programmer's Manual* has tutorials on both.

### 4.1 The `grep` Family

We mentioned `grep` briefly in Chapter 1, and have used it in examples since then.
```
$ grep pattern filenames
```
searches the named files or the standard input and prints each line that contains an instance of the *pattern*.
`grep` is invaluable for finding occurrences of variables in programs or words in documents, or for selecting parts of the output of a program:
```
$ grep -n variable *.[ch]             Locate variable in C source code
$ grep From $MAIL                     Print message headers in mailbox
$ grep From $MAIL | grep -v mary      Headers that didn't come from mary
$ grep -y mary $HOME/lib/phone-book		Find mary's phone number
$ who | grep mary                     See if mary is logged in
$ ls | grep -v temp                   Filenames that don't contain temp
```
The option `-n` prints line numbers, `-v` inverts the since of the test, and `-y` makes lower case letters in the pattern match letters of either case in the file (upper case still matches only upper case).

In all the examples we've seen so far, `grep` has looked for ordinary strings of letters and numbers.
But `grep` can actually search for much more complicated patterns: `grep` was originally created (in an evening) by straightforward surgery on `ed`.

Regular expressions are specified by giving special meaning to certain characters, just like the `*`, etc., used by the shell.
There are a few more metacharacters, and, regrettably, differences in meanings.
Table 4.1 shows all the regular expression metacharacters but we will review them briefly here.

The metacharacters `^` and `$` "anchor" the pattern to the beginning (`^`) or end (`$`) of the line.
For example
```
$ grep 'From' $MAIL
```
locates lines containing `From` in your mailbox, but
```
$ grep '^From' $MAIL
```
prints lines that *begin* with `From`, which are more likely to be message header lines.
Regular expression metacharacters overlap with shell metacharacters, so it's always a good idea to enclose `grep` patterns in single quotes.

`grep` supports *character classes* much like those in the shell, so `[a-z]` matches any lower case letter.
But there are differences; if a `grep` character class begins with a ~circumflex~ carat `^`, the pattern matches any character *except* those in the class.
Therefore, `[^0-9]` matches any non-digit.
Also, in the shell a backslash protects `]` and `-` in a character class, but `grep` and `ed` require that these characters appear where their meaning is unambiguous.
For example, `[][-]` (sic) matches either an opening or closing square bracket or a minus sign.

A period `.` is equivalent to the shell's `?`: it matches any character.
(The period is probably the character with the most different meanings to different UNIX programs.)
Here are a couple of examples:
```
$ ls -l | grep '^d'             List subdirectory names
$ ls -l | grep '^.......rw'			List files others can read and write
```
The `^` and seven periods match any seven characters at the beginning of the line, which when applied to the output of `ls -l` means any permission string.

The *closure* operator `*` applies to the previous character or metacharacter (including a character class) in the expression, and collectively they match any number of successive matches of the character or metacharacter.
For example, `x*` matches a sequence of `x`'s as long as possible, `[a-zA-Z]*` matches an alphabetic string, `.*` matches anything up to a newline, and `.*x` matches anything up to and including the *last* `x` on the line.

There are a couple of important things to note about closures.
First, closure applies to only one character, so `xy*` matches an `x` followed by `y`'s, not a sequence like `xyxyxy`.
Second, "any number" includes zero, so if you want at least one character to be matched, you must duplicate it.
For example, to match a string of letters the correct expression is `[a-zA-Z][a-zA-Z]*` (a letter followed by zero or more letters).
The shell's `*` filename matching character is similar to the regular expression `.*`.

No `grep` regular expression matches a newline; the expressions are applied to each line individually.

With regular expressions, `grep` is a simple programming language.
For example, recall that the second field of the password file is the encrypted password.
This command searches for users without passwords:
```
$ grep '^[^:]*::' /etc/passwd
```
The pattern is: beginning of line, any number of non-colons, double colon.

`grep` is actually the oldest of a family of programs, the other members of which are called `fgrep` and `egrep`.
The basic behavior is the same, but `fgrep` searches for many literal strings simultaneously, while `egrep` interprets true regular expressions - the same as `grep`, but with an "or" operator and parentheses to group expressions, explained below.

Both `fgrep` and `egrep` accept a `-f` option to specify a file from which to read the pattern.
In the file, newlines separate patterns to be searched for in parallel.
If there are words you habitually misspell, for example, you could check your documents for their occurrence by keeping them in a file, one per line, and using `fgrep`:
```
$ fgrep -f common-errors document
```
The regular expressions interpreted by `egrep` (also listed in Table 4.1) are the same as `grep`, with a couple of additions.
Parentheses can be used to group, so `(xy)*` matches any of the empty string, `xy`, `xyxy`, `xyxyxy`, and so on.
The vertical bar `|` is an "or" operator; `today|tomorrow` matches either `today` or `tomorrow`, as does `to(day|morrow)`.
Finally, there are two other closure operators in `egrep`, `+` and `?`.
The pattern `x+` matches one or more `x`'s, and `x?` marches zero or one `x`, but no more.

`egrep` is excellent at word games that involve searching the dictionary for words with special properties.
Our dictionary is Webster's Second International, and is stored on-line as the list of words, one per line, without definitions.
Your system may have `/usr/dict/words`, a smaller dictionary intended for checking spelling; look at it to check the format.
Here's a pattern to find words that contain all five vowels in alphabetical order:
```
$ cat alphvowels
^[^aeiou]*a[^aeiou]*e[^aeiou]*i[^aeiou]*o[^aeiou]*u[^aeiou]*$
$ egrep -f alphvowels /usr/dict/web2 | 3
TODO:
```
The pattern is not enclosed in quotes in the file `alphvowels`.
When quotes are used to enclose `egrep` patterns, the shell protects the ~~commands~~ patterns from interpretation but strips off the quotes; `egrep` never sees them.
Since the file is not examined by the shell, however, quotes are *not* used around its contents.
We could have used `grep` for this example, but because of the way `egrep` works, it is much faster when searching for patterns that include closures, especially when scanning large files.

As another example, to find all the words of six or more letters that have the letters in alphabetical order:
```
$ cat monotonic
^a?b?c?d?e?f?g?h?i?j?k?l?m?n?o?p?q?r?s?t?u?v?w?x?y?z?$
$ egrep -f monotonic /usr/dict/web2 | grep '......' | 5
TODO:
```
(Egilops is a disease that attacks wheat.)
Notice the use of `grep` to filter the output of `egrep`.

Why are there three `grep` programs?
`fgrep` interprets no metacharacters, but can look efficiently for thousands of words in parallel (once initialized, its running time is independent of the number of words), and thus is used primarily for tasks like bibliographic searches.
The size of typical `fgrep` patterns is beyond the capacity of the algorithms used in `grep` and `egrep`.
The distinction between `grep` and `egrep` is harder to justify.
`grep` came much earlier, uses the regular expressions from `ed`, and has tagged regular expressions and a wider set of options.
`egrep` interprets more general regular expressions (except for tagging), and runs significantly faster (with speed independent of the pattern), but the standard version takes longer to start when the expression is complicated.
A newer version exists that starts immediately, so `egrep` and `grep` could now be combined into a single pattern matching program.

| character  | meaning                                                     |
|------------|-------------------------------------------------------------|
| `c`        | any non-special character `c` matches itself                |
| `]c`       | turn off any special meaning of character `\c`              |
| `^`        | beginning of line                                           |
| `$`        | end of line                                                 |
| `.`        | any single character                                        |
| `[...]`    | any one of characters in `...`; ranges like `a-z` are legal |
| `[^...]`   | any single of character not in `...`; ranges are legal      |
| `\n`       | what the `n`'th `\(...\)` matched (`grep` only)             |
| `r*`       | zero or more occurrences of `r`                             |
| `r+`       | one or more occurrences of `r` (`egrep` only)               |
| `r?`       | zero or more occurrences of `r` (`egrep` only)              |
| `r1r2`     | `r1` followed by `r2`                                       |
| `r1piper2` | `r1` or `r2` (`egrep` only)                                 |
| `\(r\)`    | tagged regular expression `r` (`grep` only); can be nested  |
| `(r)`      | regular expression `r` (`egrep` only); can be nested        |

### 4.2 Other filters



### 4.3 The stream editor `sed`
### 4.4 The `awk` pattern scanning and processing language
#### Fields
#### Printing
#### Patterns
#### The `BEGIN` and `END` patterns
#### Control flow
#### Arrays
#### Associative arrays
#### Strings
#### Interaction with the shell
#### A calendar service based on awk
#### Loose ends
### 4.5 Good files and good filters
