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

The purpose of this section is to alert you to the existence and possibilities of the rich set of small filters provided by the system, and to give a few examples of their use.
This live is by no means all-inclusive - there are many more that were part of the 7th Edition, and each installation creates some of its own.
All of the standard ones are described in Section 1 of the manual.

We begin with `sort`, which is probably the most useful of all.
The basics of `sort` were covered in Chapter 1: it sorts its input by line in ASCII order.
Although this is the obvious thing to do by default, there are lots of other ways that one might want data sorted, and `sort` tries to cater to them by providing lots of different options.
For example, the `-f` option causes upper and lower case to be "folded", so case distinctions are eliminated.
The `-d` option (dictionary order) ignores all characters except letters, digits, and blanks in comparisons.

Although alphabetic comparisons are most common, sometimes a numeric comparison is needed.
The `-n` option sorts by numeric value, and the `-r` option reverses the sense of any comparison.
So,
```
$ ls | sort -f            Sort filenames in alphabetic order
$ ls -s | sort -n         Sort with smallest files first
$ ls -s | sort -nr        Sort with largest files first 
```

`sort` normally sorts on the entire line, but it can be told to direct its attention only to specific fields.
The notation *+m* means that the comparison skips the first *m* fields; `+0` is the beginning of the line.
So, for example,
```
$ ls -l | sort +3nr       Sort by byte count, largest first
$ who | sort +4n          Sort by time of login, oldest first
```

Other useful `sort` options include `-o`, which specifies a filename for the output (it can be one of the input files), and `-u`, which suppresses all but one of each group of lines that are identical in the sort fields.

Multiple sort keys can be used, as illustrated by this cryptic example from the manual page of `sort`(1):
```
$ sort +0f +0 -u filenames
```
`+0f` sorts the line, folding upper and lower case together, but lines that are identical may not be adjacent.
So `+0` is a secondary key that sorts the equal lines from the first sort into normal ASCII order.
Finally, `-u` discards any adjacent duplicates.
Therefore, given a list of words, one per line, the command prints the unique words.
The index for this book was prepared with a similar `sort` command, using even more of `sort`'s capabilities.
See `sort`(1).

The command `uniq` is the inspiration for the `-u` flag of `sort`: it discards all but one of each group of adjacent duplicate lines.
Having a separate program for this function allows it to do tasks unrelated to sorting.
For example, `uniq` will remove multiple blank lines whether its input is sorted or not.
Options invoke special ways to process the duplicates: `uniq -d` prints only those lines that are duplicated; `uniq -u` prints only those that are unique (i.e., not duplicated); and `uniq -c` counts the number of occurrences of each line.
We'll see an example shortly.

The `comm` command is a file comparison program.
Given two *sorted* input files `f1` and `f2`, `comm` prints three columns of output: lines that occur only in `f1`, lines that occur only in `f2`, and lines that occur in both files.
Any of these columns can be suppressed by an option:
```
$ comm -12 f1 f2          
```
prints only those lines that are in both files, and
```
$ comm -23 f1 f2
```
prints the lines that are in the first file but not in the second.
This is useful for comparing directories and for comparing a word list with a dictionary.

The `tr` command transliterates the characters in its input.
By far the most common use of `tr` is case conversion:
```
$ tr a-z A-Z              Map lower case to upper
$ tr A-Z a-z              Map upper case to lower
```

The `dd` command is rather different from all of the other commands we have looked at.
It is intended primarily for processing tape data from other systems - its very name is a reminder of OS/360 job control language.
`dd` will do case conversion (with a syntax very different from `tr`); it will convert from ASCII to EBCDIC and vice versa; and it will read or write data in the fixed size records with blank padding that characterize non-UNIX systems.
In practice, `dd` is often used to deal with raw, unformatted data, whatever the source; it encapsulates a set of facilities for dealing with binary data.

To illustrate what can be accomplished by combining filters, consider the following pipeline, which prints the 10 most frequent words in its input:
```
cat $* |
tr -sc A-Za-z '\012' |    Compress runs of non-letters into newline
sort |
uniq -c |
sort -n |
tail |
5
```
`cat` collects the files, since `tr` only reads its standard input.
The `tr` command is from the manual: it compresses adjacent non-letters into newlines, and thus converting the input into one word per line.
The words are then sorted and `uniq -c` compresses each group of identical words into one line prefixed by a count, which becomes the sort field for `sort -n`.
(This combination of two `sorts` around a `uniq` occurs often enough to be called an idiom.)
The result is the unique words in the document, sorted in increasing frequency.
`tail` selects the 10 most common words (the end of the sorted list) and `5` prints them in five columns.

By the way, notice that ending a line with `|` is a valid way to continue it.

### 4.3 The stream editor `sed`

Let us now turn to `sed`.
Since it is derived directly from `ed`, it should be easy to learn, and it will consolidate your knowledge of `ed`.

The basic idea of `sed` is simple:
```
$ sed 'list of ed commands' filenames...
```
reads one line at a time from the input files; it applies the commands from the list, in order, to each line and writes its edited form to the standard output.
So, for instance, you can change `UNIX` to `UNIX(TM)` everywhere it occurs in a set of files with
```
$ sed 's/UNIX/UNIX(TM)/g' filenames ... >output
```

Don't misinterpret what happens here.
`sed` does *not* alter the contents of its input files.
It writes on the standard output, so the original files are not changes.
By now you should have enough shell experience to realize that
```
$ sed '...' file >file
```
is a bad idea: to replace the contents of files, you ~~must~~ should use a temporary file, or another program.
(We will talk later about a program to encapsulate the idea of overwriting an existing file; look at `overwrite` in Chapter 5).

`sed` outputs each line automatically, so no `p` was needed after the substitution command above; indeed, if there had been one, each modified line would have been printed twice.
Quotes are almost always necessary, however, since so many `sed` metacharacters mean something to the shell as well.
For example, consider using `du -a` to generate a list of filenames.
Normally `du` prints the size and the filename:
```
$ du -a ch4.*
18			ch4.1
13			ch4.2
14			ch4.3
17			ch4.4
2				ch4.9
$
```
You can use `sed` to discard the size part, but the editing command needs quotes to protect a `*` and a tab from being interpreted by the shell:
```
$ du -a ch4.* | sed 's/.*→//'
ch4.1
ch4.2
ch4.3
ch4.4
ch4.9
$
```
The substitution deletes all characters (`.*`) up to and including the rightmost tab (shown in the pattern as `→`).

In a similar way, you could select the user names and login times from the output of `who`:
```
$ who
lr			tty1		Sep 29 07:14
ron			tty3		Sep 29 10:30
you			tty4		Sep 29 08:37
td			tty5		Sep 29 08:47
$ who | sed 's/ .* / /'
lr	07:14
ron	10:30
you	08:37
td	08:47
$
```
The `s` command replaces a blank and everything that follows it (as much as possible, including more blanks) up to another blank by a single blank.
Again, quotes are needed.

Almost the same `sed` command can be used to make a program `getname` that will return your user name:
```
$ cat getname
who am i | sed 's/ .*//'
$ getname
you
$
```

Another `sed` sequence is used so frequently that we have made it into a shell file called `ind`.
The `ind` command indents it's input one tab stop; it is handy for moving something over to fit better onto line-printer paper.

The implementation of `ind` is easy - stick a tab at the front of each line:
```
sed 's/^/→/' $*                    Version 1 of ind
```
This version even puts tabs on an empty line, which seems unnecessary.
A better version uses `sed`'s ability to select the lines to be modified.
If you prefix a pattern to the command, only the lines that match the pattern will be affected:
```
sed '/./s/^/→/' $*                 Version 2 of ind
```
The pattern `/./` matches any line that has at least one character on it other than a newline; the `s` command is done for those lines but not for empty lines.
Remember that `sed` outputs all lines regardless of whether they were changed, so the empty lines are still produced as they should be.

There's yet another way that `ind` could be written.
It is possible to do the commands only on lines that *don't* march the selection pattern, by preceding the command with an exclamation mark `!`.
In
```
sed '/^$/!s/^/→/' $*               Version 3 of ind
```
the pattern `/^$/` matches empty lines (the end immediately follows the beginning), so `/^$/!` says, "don't do the command on empty lines"

As we said above, `sed` prints each line automatically, regardless of what was done to it (unless it was deleted).
Furthermore, most `ed` commands can be used.
So it's easy to write a `sed` program that will print the first three (say) lines of its input, then quit:
```
sed 3q
```
Although `3q` is not a legal `ed` command, it makes sense to `sed`: copy lines, then quit after the third one.

You might want to do other processing to the data, such as indent it.
One way is to run the output from `sed` through `ind`, but since `sed` accepts multiple commands, it can be done with a single (somewhat unlikely) invocation of `sed`:
```
sed 's/^/→/
     3q'
```
Notice where the quotes and the newline are: the commands have to be on separate lines, but `sed` ignores leading blanks and tabs.

With these ideas, it might seem sensible to write a program, called `head`, to print the first few lines of each filename argument.
But `sed 3q` (or `10q`) is so easy to type that we've never felt the need.
We do, however, have an `ind`, since its equivalent `sed` command is harder to type.
(In the process of writing this book we replaced the existing 30-line C program by version 2 of the one-line implementation shown earlier).
There is no clear criterion for when it's worth making a separate command from a complicated command line; the best rule we've found is to put it in your `bin` directory and see if you actually use it.

It's also possible to put `sed` commands in a file and execute them from there, with
```
$ sed -f cmdfile ...
```

You can use line selectors other than numbers like `3`:
```
$ sed '/pattern/q'
```
prints its input up to and including the first line matching *pattern*, and
```
$ sed '/pattern/d'
```
deletes every line that contains *pattern*; the deletion happens before the line is automatically printed, so deleted lines are discarded.
 
Although automatic printing is usually convenient, sometimes it gets in the way. It can be turned off by the `-n` option; in that case, only lines explicitly printed with a `p` command appear on the output.
For example,
```
$ sed -n '/pattern/p'
```
does what `grep` does.
Since the matching condition can be inverted by following it with `!`,
```
$ sed -n '/pattern/!p'
```
is `grep -v`.
(So is `sed '/pattern/d'`)

Why do we have both `sed` and `grep`?
After all, `grep` is just a simple special case of `sed`.
Part of the reason is history - `grep` came well before `sed`.
But `grep` survives, and indeed thrives, because for the particular job that they both do, it is significantly easier to use than `sed` is: it does the common case about as succinctly as possible.
(It also does a few things that `sed` won't; look at the `-b` option, for instance.)
Programs do die, however.
There was once a program called `gres` that did simple substitution, but it expired almost immediately when `sed` was born.

Newlines can be inserted with `sed`, using the same syntax as in `ed`:
```
$ sed 's/$/\
/g'
```
adds a newline to the end of each line, thus double-spacing its input, and
```
$ sed 's/[ →][ →]*/\
/g'
```
replaces each string of blanks or tabs with a newline and thus splits its input into one word per line.
(the regular expression `[ →]` matches a blank or tab; `[ →]*` matches zero or more of these, so the whole pattern matches one or more blanks and/or tabs.)

You can also use pairs of regular expressions or line numbers to select a *range* of lines over which any of the commands will operate.
```
$ sed -n '20,30p'        Print only lines 20 through 30
$ sed '1,10d'            Delete lines 1 through 10 (= tail +11)
$ sed '1,/^$/d'          Delete up to and including first blank line
$ sed -n '/^$/,/^end/p'  Print each group of lines from an empty line to the line starting with 'end'
$ sed '$d'               Delete last line
```
Line numbers go from the beginning of the input; they do not reset at the beginning of a new file.

There is a fundamental limitation of `sed` that is not shared by `ed`, however: relative line numbers are not supported.
In particular, `+` and `-` are not understood in line number expressions, so it is impossible to reach backwards in the input:
```
$ sed '$-1d'                       Illegal: can't refer backward
Unrecognized command: $-1d
```
Once a line is read, the previous line is gone forever: there is no way to identify the next-to-last line, which is what this command requires.
(In fairness, there is a way to handle this with `sed`, but it is pretty advanced. Look up the "hold" command in the manual.)
There is also no way to do relative addressing forward:
```
$ sed '/thing/+1d'                 Illegal: can't refer forward
```

`sed` provides the ability to write on multiple output files.
For example,
```
$ sed -n '/pat/w file1
>         /pat/!w file2' filenames...
$
```
writes lines matching *pat* on `file1` and lines not matching *pat* on `file2`.
Or, to revisit our first example,
```
$ sed 's/UNIX/UNIX(TM)/gw u.out' filenames ... >output
```
writes the entire output to file `output` as before, but also writes just the changed lines to file `u.out`.

Sometimes it's necessary to cooperate with the shell to get shell file arguments into the middle of a `sed` command.
One example is the program `newer`, which lists all files in a directory that are newer than a specified one.
```
$ cat newer
# newer f: list files newer than f
ls -t | sed '/$'$1'$/q'
$
```
The quotes protect the various special characters aimed at `sed`, while leaving the `$1` exposed so the shell will replace it by the filename.
An alternate way to write the argument is
```
"/^$1\$/q"
```
since the `$1` will be replaced by the argument while the `\$` becomes just `$`.

In the same way, we can write `older`, which lists all the files older than the named one:
```
$ cat older
# older f: list files older than f
ls -tr | sed '/^'$1'$/q'
$
```
The only difference is the `-r` opting on `ls`, to reverse the order.

Although `sed` will do much more than we have illustrated, including testing conditions, looping, and branching, remembering the previous lines, and of course many of the `ed` commands described in Appendix 1, most of the use of `sed` is similar to what we have shown here - one or two simple editing commands - rather than long or complicated sequences.
Table 4.2 summarizes some of `sed`'s capabilities, although it omits the multi-line functions.

`sed` is convenient because it will handle arbitrarily long inputs, because it is fast, and because it is similar to `ed` with its regular expressions and line-at-a-time processing.
On the other side of the coin, however, `sed` provides a relatively limited form of memory (it's hard to remember text from one line to another), it only makes one pass over the data, it's not possible to go backwards, there's no way to do forward references like `/.../+1`, and it provides no facilities for manipulating numbers - it is purely a text editor.

| command       | description                                                                                                   |
| ------------- | -----------                                                                                                   |
| `a\\`         | append lines to output until one not ending in `\`                                                            |
| `b label`     | branch to command `: label`                                                                                   |
| `c\`          | change lines to following text as in `a`                                                                      |
| `d`           | delete line; read next input line                                                                             |
| `i\\`         | insert following text before next input                                                                       |
| `l`           | list line, making all non-printing characters visible                                                         |
| `p`           | print line                                                                                                    |
| `q`           | quit                                                                                                          |
| `r file`      | read `file`, copy contents to output                                                                          |
| `s/old/new/f` | substitute `old` for `new`. <br/> If `f=g`, replaces all occurrences; `f=p`, print; `f=w file`, write to file |
| `t label`     | test: branch to `label` if substitution made to current line                                                  |
| `w file`      | write line to `file`                                                                                          |
| `y/str1/str2` | replace each character from `str1` with corresponding character from `str2` (no ranges allowed)               |
| `=`           | print current input line number                                                                               |
| `!cmd`        | do `cmd` only if line is not selected                                                                         |
| `: label`     | set label for `b` and `t` commands                                                                            |
| `{`           | treat commands up to matching `}` as a group                                                                  |

### 4.4 The `awk` pattern scanning and processing language

Some of the limitations of `sed` are remedied by `awk`.
The idea in `awk` is much the same as in `sed`, but the details are based more on the C programming language than on a text editor.
The usage is just like `sed`:
```
$ awk 'program' filenames ...
```
but the *program* is different:
```
pattern { action }
pattern { action }
...
```
`awk` reads the input in the *filenames* one line at a time.
Each line is compared with each *pattern* in order; for each *pattern* that matches the line, the corresponding *action* is performed.
Like `sed`, `awk` does not alter its input files.

The patterns can be regular expressions exactly as in `egrep`, or they can be more complicated conditions reminiscent of C.
As a simple example, though,
```
$ awk '/regular expression/' filenames...
```
does what `egrep` does: it prints every line that matches the *regular expression*.

Either the pattern or the action is optional. If the action is omitted, the default action is to print matched lines, so
```
$ awk '/regular expression/' filenames...
```
does the same job as the previous example.
Conversely, if the pattern is omitted, then the action part is done for *every* input line.
So
```
$ awk '{ print }' filenames...
```
does what `cat` does, albeit more slowly.

One final note before we get on to interesting examples.
As with `sed`, it is possible to present the *program* to `awk` from a file:
```
$ awk -f cmdfile filenames...
```

#### Fields

`awk` splits each input line into *fields*, that is, strings of non-blank characters separated by blanks or tabs.
By this definition, the output of `who` has five fields:
```
$ who
you			tty2		Sep 29 11:53
jim			tty4		Sep 29 11:27
$
```
`awk` calls the fields `$1`, `$2`, ..., `$NF`, where `NF` is a variable whose value is set to the number of fields.
In this case, `NF` is 5 for both lines.
(Note the difference between `NF`, the number of fields, and `$NF`, the last field on the line. In `awk`, unlike the shell, only fields begin with a `$`; variables are unadorned.)
For example, to discard the file sizes produced by `du -a`
```
$ du -a | awk '{ print $2 }'
```
and print the names of people logged in and the time of login, one per line:
```
$ who | awk '{ print $1, $5 }'
you	11:53
jim	11:27
$
```
To print the name and time of login sorted by time:
```
$ who | awk '{ print $5, $1 }' | sort
11:27 jim
11:53 you
$
```
These are alternatives to the `sed` versions shown earlier in this chapter.
Although `awk` is easier to use than `sed` for operations like these, it is usually slower, both getting started and in execution when there's a lot of input.

`awk` normally assumes that white space (any number of blanks and tabs) separates fields, but the separator can be changed to any single character.
One way is with the `-F` (upper case) command-line option.
For example, the fields in the password file `/etc/passwd` are separated by colons:
```
$ sed 3q /etc/passwd
root:3D.fHR5KoB.3s:0:1:S.User:/:
ken:y.68wd1.i.jayz:6:1:K.Thompson:/usr/ken:
dmr:z4u3dJWbg7wCK:7:1:D.M.Ritchie:/usr/dmr:
$
```
To print the user names, which come from the first field,
```
$ sed 3q /etc/passwd | awk -F: '{ print $1 }'
root
ken
dmr
$
```

The handling of blanks and tabs is intentionally special.
By default, both blanks and tabs are separators, and leading separators are discarded.
If the separator is set to anything other than blank, however, then leading separators are counted in determining fields.
In particular, if the separator is a tab, then blanks are not separator characters, leading blanks are part of the field, and each tab defines a field.

#### Printing

`awk` keeps track of other interesting quantities besides the number of input fields.
The built-in variable `NR` is the number of the current input "record" or line.
So to add line numbers to an input stream, use this:
```
$ awk '{ print NR, $0 }'
```
The field `$0` is the entire input line, unchanged.
In a `print` statement items separated by commas are printed separated by the output field separator, which is by default a blank.

The formatting that `print` does is often acceptable, but if it isn't, you can use a statement called `printf` for complete control of your output.
For example, to print line numbers in a field four digits wide, you might use the following:
```
$ awk '{ printf "%4d %s\n", NR, $0 }'
```
`%4d` specifies a decimal integer (`NR`) in a field four digits wide, `%s` a string of characters (`$0`), and `\n` a newline character, since `printf` doesn't print any spaces or newlines automatically.
The `printf` statement in `awk` is like the C function; see `printf`(3).

We could have written the first version of `ind` (from early in this chapter) as
```
awk '{ printf "\t%s\n", $0 }' $*
```
which prints a tab (`\t`) and the input record.

#### Patterns

Suppose you want to look in `/etc/passwd` for people who have no passwords.
The encrypted password is the second field, so the program is just a pattern:
```
$ awk -F: '$2 == ""' /etc/passwd
```
The pattern asks if the second field is an empty string (`==` is the equality test operator).
You can write this pattern in a variety of ways:
```
$2 == ""                 2nd field is empty
$2 ~ /^$/                2nd field matches empty string (regular expression)
$2 !~ /./                2nd field doesn't match any character (regular expression)
length($2) == 0          Length of 2nd field is zero
```
The `~` symbol indicates a regular expression match, and `!~` means "does not match."
The regular expression itself is enclosed in slashes.

`length` is an `awk` built-in function that produces the length of a string of characters.
A pattern can be preceded by `!` to negate it, as in
```
!($2 == "")
```
The `!` operator is like that in C, but opposite to `sed`, where the `!` follows the pattern.

One common use of patterns in `awk` is for simple data validation tasks.
Many of these amount to little more than looking for lines that fail to meet some criterion; if there is no output, the data is acceptable ("no news is good news").
For example, the following pattern makes sure that every input record has an even number of fields, using the operator `%` to compute the remainder:
```
NF % 2 != 0     # print if odd number of fields
```
Another prints excessively long lines, using the built-in function `length`:
```
length($0) > 72 # print if too long
```
`awk` uses the same comment convention as the shell does: a `#` marks the beginning of a comment.

You can make the output somewhat more informative by printing a warning and part of the too-long line, using another built-in function, `substr`:
```
length($0) > 72 { print "Line", NR, "too long:", substr($0, 1, 60) }
```
`substr(s,m,n)` produces the substring of *s* that begins at position *m* and is *n* characters long.
(The string begins at position 1.)
If *n* is omitted, the substring from *m* to the end is used.
`substr` can be used for extracting fixed-position fields, for instance, selecting the hour and minute from the output of `date`:
```
$ date
Thu Sep 29 12:17:01 EDT 1983
$ date | awk '{ print substr($4, 1, 5) }'
12:17
$
```

#### The `BEGIN` and `END` patterns

`awk` provides two special patterns, `BEGIN` and `END`.
`BEGIN` actions are performed the first input line has been read; you can use the `BEGIN` pattern to initialize variables, to print headings, or to set the field separator by assigning to the variable `FS`:
```
$ awk 'BEGIN { FS = ":" }
>      $2 == ""' /etc/passwd
$                              No output: we all use passwords
```
`END` actions are done after the last line of input has been processed:
```
$ awk 'END { print NR }' ...
```
prints the number of lines of input.

#### Arithmetic and Variables

The examples so far have involved only simple text manipulation.
`awk`'s real strength lies in its ability to do calculations on the input data as well; it is easy to count things, compute sums and averages, and the like.
A common use of `awk` is to sum columns of numbers.
For example, to add up all the numbers in the first column:
```
       { s = s + $1 }
END    { print s }
```
Since the number of values is available in the variable `NR`, changing the last line to
```
END    { print s, s/NR }
```
prints both sum and average.

This example also illustrates the use of variables in `awk`.
`s` is not a built-in variable, but one defined by being used.
Variables are initialized to zero by default so you usually don't have to worry about initialization.

`awk` also provides the same shorthand arithmetic operators that C does, so the example would normally be written
```
       { s += $1 }
END    { print s }
```
`s += 1` is the same as `s = s + 1`, but notationally more compact.

You can generalize the example that counts input lines like this:
```
       { nc += length($0) + 1  # number of chars, 1 for \n
         nw += NF              # number of words
       }
END    { print NR, nw, nc }
```
This counts the lines, words, and characters in its input, so it does the same job as `wc` (although it doesn't break the totals down by file).

As another example of arithmetic, this program computes the number of 66-line pages that will be produced by running a set of files through `pr`.
This can be wrapped up in a command called `prpages`:
```
$ cat prpages
# prpages:	compute number of pages that pr will print
wc $* |
awk '!/ total$/ { n += int(($1+55) / 56) }
     END        { print n }'
     $
```
`pr` puts 56 lines of text on each page (a fact determined empirically).
The number of pages is rounded up, then truncated to an integer with the built-in function `int`, for each line of `wc` output that does not match `total` at the end of a line.
```
$ wc ch4.*
  753
  612
  637
  802
   50
 2854
$ prpages ch4.*
53
$
```
To verify this result, run `pr` into `awk` directly:
```
$ pr ch4.* | awk 'END { print NR/66 }'
53
$
```

Variables in `awk` also store strings of characters.
Whether a variable is to be treated as a number or as a string of characters depends on the context.
Roughly speaking, in an arithmetic expression like `s+=$1`, the numeric value is used; in a string context like `x="abc"`, the string value is used; and in an ambiguous case like `x>y`, the string value is used unless the operands are clearly numeric.
(The rules are stated precisely in the `awk` manual.)
String variables are initialized to the empty string.
Coming sections will put strings to good use.

`awk` itself maintains a number of built-in variables of both types, such as `NR` and `FS`.
Table 4.3 gives the complete list.
Table 4.4 lists the operators.

| variable   | description                                               |
|------------|-----------------------------------------------------------|
| `FILENAME` | name of current input file                                |
| `FS`       | field separator character (default blank & tab)           |
| `NF`       | number of fields in input record                          |
| `NR`       | number of input record                                    |
| `OFMT`     | output format for numbers (default `%g`; see `printf`(3)) |
| `OFS`      | output field separator string (default blank)             |
| `ORS`      | output record separator string (default newline)          |
| `RS`       | input record separator character (default newline)        |

| operator               | description                                                                                  |
|------------------------|----------------------------------------------------------------------------------------------|
| `= += -= *= /= %=`     | assignment; `v op= expr` is `v = v op expr`
| &vert;&vert;           | OR: *expr1* &vert;&vert; *expr2* true if either is; *expr2* not evaluates if *expr1* is true |
| `&&`                   | AND: `expr1 && expr2` true if both are;  *expr2* not evaluates if *expr1* is false           |
| `!`                    | negative value of expression                                                                 |
| `> >= < <= == != ~ !~` | relational operators; `~` and `!~` are match and not-match                                   |
| *nothing*              | string concatenation                                                                         |
| `+ -`                  | plus, minus                                                                                  |
| `* / %`                | multiply, divide, remainder                                                                  |
| `++ --`                | increment, decrement (prefix or postfix)                                                     |

#### Control flow

It is remarkably easy (speaking from experience) to create adjacent duplicate words accidentally when editing a big document, and it is obvious that that almost never happens intentionally.
To prevent such problems, one of the components of the Writer's Workbench family of programs, called `double`, looks for pairs of identical adjacent words.
Here is an implementation of `double` in `awk`:
```
$ cat double
awk '
FILENAME != prevfile {    # new file
  NR = 1                  # reset line number
  prevfile = FILENAME
}
NF > 0 {
  if ($1 == lastword)
    printf "double %s, file %s, line %d\n", $1, FILENAME, NR
  for (i = 2; i <= NF; i++)
    if ($i == $(i-1))
      printf "double %s, file %s, line %d\n", $i, FILENAME, NR
  if (NF > 0)
    lastword = $NF
}' $*
$
```
The operator `++` increments its operand, and the operator `--` decrements.

The built-in variable `FILENAME` contains the name of the current input file.
Since `NR` counts lines from the beginning of the input, we reset it every time the filename changes so an offending line is properly identified.

The `if` statement is just like that in C:
```
if (condition)
    statement1
else
    statement 2
```
If *condition* is true, then *statement1* is executed; if false, and if there is an `else` part, then *statement2* is executed.
The `else` part is optional.

The `for` statement is a loop like the one in C, but different from the shell's:
```
for (expression1; condition; expression2)
    statement
```
The `for` is identical to the following `while` statement, which is also valid in `awk`:
```
expression1
while (condition) {
    statement
    expression2
}
```
For example:
```
for (i = 2; i <= NF; i++)
```
runs the loop with `i` set to turn to `2`, `3`, ..., up to the number of fields, `NF`.

The `break` statement causes an immediate exit from the enclosing `while` or `for`; the `continue` statement causes the next iteration to begin (at *condition* in the `while` and *expression2* in the `for`).
The `next` statement causes the next input line to be read and pattern matching to resume at the beginning of the `awk` program.
The `exit` statement causes an immediate transfer to the `END` pattern.

#### Arrays

`awk` provides arrays, as do most programming languages.
As a trivial example, this `awk` program collects each line of input in a separate array element, indexed by line number, then prints them out in reverse order:
```
$ cat backwards
# backwards: print input in backward line order
awk ' { line[NR] =$0 }
END   { for (i = NR; i > 0; i--) }' $*
$
```
Notice that, like variables, arrays don't have to be declared; the size of an array is limited only by the memory available on your machine.
Of course if a very large file is being read into an array, it may eventually run out of memory.
To print the end of a large file in reverse order requires cooperation with `tail`:
```
$ tail -5 /usr/dict/web2 | backwards
TODO:
$
```
`tail` takes advantage of a file system operation called *seeking*, to advance to the end of a file without reading the intervening data.
Look at the discussion of `lseek` in Chapter 7.
(Our version of `tail` has an option `-r` that prints the lines in reverse order, which supersedes `backwards`.)

Normal input processing splits each input line into fields.
It is possible to perform the same field-splitting operation on any string with the built-in function `split`:
```
n = split(s, arr, sep)
```
splits the string *s* into fields that are stored in elements 1 through *n* of the array *arr*.
If a separatir character *sep* is provided, it is used; otherwise the current value of `FS` is used.
For example, `split($0, a, ":")` splits the input line on colons, which is suitable for processing `/etc/passwd`, and `split("9/28/83", date, "/")` splits a date on slashes.
```
$ sed 1q /etc/passwd | awk '{split($0,a,":"); print a[1]}'
root
$ echo 9/29/83 | awk '{split($0,date,"/"); print date[3]}'
83
$
```

Table 4.5 lists the `awk` built-in functions.

| function            | description                                                   |
|---------------------|---------------------------------------------------------------|
| `cos(expr)`         | cosine of `expr`                                              |
| `exp(expr)`         | exponential of `expr`: `e^expr`                               |
| `getline()`         | reads next input line; returns `0` if end of file, `1` if not |
| `index(s1, s2)`     | position of string `s2` in `s1`; returns `0` if not present   |
| `int(expr)`         | integer part of `expr`; truncates toward `0`                  |
| `length(s)`         | length of string `s`                                          |
| `log(expr)`         | natural logarithm of `expr`                                   |
| `sin(expr)`         | sine of `expr`                                                |
| `split(s, a, c)`    | split `s` into `a[1]...a[n]` on character `c`; return `n`     |
| `sprintf(fmt, ...)` | format `...` according to specification of `fmt`              |
| `substr(s, m, n)`   | `n`-character substring of `s` beginning at position `m`      |

#### Associative arrays

A standard problem in data processing is to accumulate values for a set of value-name pairs.
That is, from input like
```
Susie   400
John    100
Mary    200
Mary    300
John    100
Susie   100
Mary    100
```
we want to compute the total for each name:
```
John    200
Mary    600
Susie   500
```
`awk` provides a neat way to do this, the *associative array*.
Although one normally thinks of array subscripts as integers, in `awk` any value can be used as a subscript.
So
```
        { sum[$1] += $2 }
END     { for (name in sum) print name, sum [base] }
```
is the complete program for adding up and printing the sums for the name-value pairs like those above, whether or not they are sorted.
Each name (`$1`) is used as a subscript in `sum`; at the end, a special form of the `for` statement is used to cycle through all of the elements of `sum`, printing them out.
Syntactically, this variant of the `for` statement is
```
for (var in array)
    statement
```
Although it might look superficially like the `for` loop in the shell, it's unrelated.
It loops over the subscripts of `array`, not the elements, setting `var` to each subscript in turn.
The subscripts are produced in an unpredictable order, however, so it may be necessary to sort them.
In the example above, the output can be piped into `sort` to list the people with the largest values at the top.
```
$ awk '...' | sort +1nr
```

The implementation of associative memory uses a hashing scheme to ensure that access to any element takes about the same time as to any other, and that (at least for moderate array sizes) the time doesn't depend on how many elements are in the array.

The associative memory is effective for tasks like counting all the words in the input:
```
$ cat wordfreq
awk '   { for (i = 1; i <= NF; i++) num[$i]++ }
END     { for (word in num) print word, num[word] }
' $*
$ wordfreq ch4.* | sort +1 -nr | sed 20q | 4
TODO:
$
```
The first `for` loop looks at each word in the input line, incrementing the element of array `num` subscripted by the word.
(Don't confuse `awk`'s `$i`, the `i`'th field of the input line, with any shell variables.)
After the files have been read, the second `for` loop prints, in arbitrary order, the words and their counts.

#### Strings

Although both `sed` and `awk` are used for tiny jobs like selecting a single field, only `awk` is used to any extent for tasks that really require programming.
One example is a program that folds long lines to 80 columns.
Any line that exceeds 80 characters is broken after the 80th; a `\` is applied as a warning, and the residue is processed.
The final section of a folded line is right-justified, not left-justified, since this produces more convenient output for program listings, which is what we most often use `fold` for.
As an example, using 20-character lines instead of 80,
```
$ cat test
A short line.
A somewhat longer line.
This line is quite a bit longer than the last one.
$ fold test
A short line.
A somewhat longer li\
                 ne.
This line is quite a\
 bit longer than the\ 
           last one.
$
```

Strangely enough, the 7th Edition provides no program for adding or removing tabs, although `pr` in System V will do both.
Out implementation of `fold` uses `sed` to convert tabs into spaces so that `awk`'s character count is right.
This works properly for leading tabs (again typical of program source) but does not preserve columns for tabs in the middle of a line.
```
# fold:	fold long lines
sed 's/	/        /g' $* |  # convert tabs to 8 spaces
awk '
BEGIN {
  N = 80                   # folds at column 80
  for (i = 1; i<= N; i++)  # making a string of blanks
    blanks = blanks " "
}
{ if ((n = length($0)) <= N)
    print
  else {
    for (i = 1; n > N; n -= N) {
      printf "%s\\\n", substr($0, i, N)
      i += N
    }
    printf "%s%s\n", substr(blanks, 1, N-n), substr($0, i)
  }
} '
```
In `awk` there is no explicit string concatenation operator; strings are concatenated when they are adjacent.
Initially, `blanks` is a null string.
The loop in the `BEGIN` part creates a long string of blanks by concatenation: each trip around the loop adds one more blank to the end of `blanks`.
The second loop processes the input line in chunks until the remaining part is short enough.
As in C, an assignment statement can be used as an expression, so the construction
```
if ((n = length($0)) <= N) ...
```
assigns the length of the input line to `n` before testing the value.
Notice the parentheses.

#### Interaction with the shell

Suppose you want to write a program `field n` that will print the `n`-th field from each line of input, so that you could say, for example,
```
$ who | field 1
```
to print only the login names.
`awk` clearly provides the field selection capability; the main problem is passing the field number to `n` in an `awk` program.
Here is one implementation:
```
awk '{ print $'$1' }'
```
The `$1` is exposed (it's not inside any quotes) and thus becomes the field number seen by `awk`.
Another approach uses double quotes:
```
awk "{ print \$$1 }"
```
In this case, the argument is interpreted by the shell, so the `\$` becomes a `$` and the `$1` is replaced by the value of `n`.
We prefer the single-quote style because so many extra `\`'s are needed with the double-quote style in a typical `awk` program.

A second example is `addup n`, which adds up the numbers in the `n`-th field:
```
awk '{ s+= $'$1' } 
 END { print s }'
```

A third example forms separate sums of each of `n` columns, plus a grand total:
```
awk '
BEGIN { n = '$1' }
{       for (i = 1; i<= n; i++)
          sum[i] += $i
}
END {   for (i = 1; i <= n; i++) {
          printf "%6g ", sum[i]
          total += sum[i]
        }
        printf "; total = %6g\n", total
}'
```
We use `BEGIN` to insert the value of `n` into a variable, rather than cluttering up the rest of the program with quotes.

The main problem with all of these examples is not keeping track of whether one is inside or outside the quotes (although that is a bother), but that as currently written, such programs can read only their standard input; there is no way to pass them both the parameter `n` and an arbitrarily long list of filenames.
This requires some shell programming that we'll address in the next chapter.

#### A calendar service based on `awk`

Our final example uses associative arrays; it is also an illustration of how to interact with the shell, and demonstrates a bit about program evolution.

The task is to have the system send you mail every morning that contains a reminder of upcoming events.
(There may already be such a calendar service, see `calendar`(1). This section shows an alternate approach.)
The basic service should tell you of events happening today; the second step is to give a day of warning - events of tomorrow as well as today.
The proper handling of weekends and holidays is left as an exercise.

The first requirement is a place to keep your calendar.
For that, a file called `calendar` in `/usr/you` seems easiest.
```
$ cat calendar
Sep 30 mother's birthday
Oct 1  lunch with joe, noon
Oct 1  meeting 4pm
$
```

Second, you need a way to scan the calendar for the date.
There are many choices here; we will use `awk` because it is best at doing the arithmetic necessary to get from "today" to "tomorrow", but other programs like `sed` or `egrep` can also serve.
The lines selected from the calendar are shipped off by `mail`, of course.

Third, you need a way to have `calendar` scanned reliably and automatically every day, probably in the morning.
This can be done with `at`, which we mentioned briefly in Chapter 1.

If we restrict the format of `calendar` so each line begins with a month name and a day as produced by `date`, the first draft of the calendar program is easy:
```
$ date
Thu Sep 29 15:23:12 EDT 1983
$ cat bin/calendar
# calendar:  version 1 -- today only
awk <$HOME/calendar '
  BEGIN { split("'"`date`"'", date) }
  $1 == date[2] && $2 == date[3]

' | mail $NAME
$
```
The `BEGIN` block splits the date produced by `date` into an array; the second and third elements of the array are the month and the day.
We are assuming that the shell variable `NAME` contains your login name.

The remarkable sequence of quote characters is required to capture the date in a string in the middle of the `awk` program.
An alternative that is easier to understand is to pass the date in as the first line of input:
```
$ cat bin/calendar
# calendar:  version 2 -- today only, no quotes
(date; cat $HOME/calendar)
  NR == 1    { mon = $2; day = $3 } # set the date
  NR > 1 && $1 == mon && $2 == day  # print calendar lines
' | mail $NAME
$
```

The next step is to arrange for `calendar` to look for tomorrow as well as today.
Most of the time all that is needed is to take today's date and add 1 to the day.
But at the end of the month, we have to get the next month and set the day back to 1.
And of course each month has a different number of days.

This is where the associative array comes in handy.
Two arrays, `days` and `nextmon`, whose subscripts are month names, hold the number of days in the month and the name of the next month.
Then `days["Jan"]` is `31`, and `nextmon["Jan"]` is `Feb`.
Rather than create a whole sequence of statements like
```
days["Jan"] = 31; nextmon["Jan"] = "Feb"
days["Feb"] = 28; nextmon["Feb"] = "Mar"
...
```
we will use `split` to convert a convenient data structure into the one really needed:
```
$ cat calendar
# calendar:  version 3 -- today and tomorrow
awk <$HOME/calendar '
BEGIN {
    x = "Jan 31 Feb 28 Mar 31 Apr 30 May 31 Jun 30 " \
        "Jul 31 Aug 31 Sup 30 Oct 31 Nov 30 Dec 31 Jun 31"
    split(x, data)
    for(i = 1; i < 24; i += 2) {
        days[data[i]] = data[i+1]
        nextmon[data[i]] = data[i+2]
    }
    split("'"`date`"'", date) 
    mon1 = date[2]; day1 = date[3]
    mon2 = mon1; day2 = day1 + 1
    if (day1 >= days[mon1]) {
        day2 = 1
        mon2 = nextmon[mon1]
    }
}
$1 == mon1 && $2 == day1 || $1 == mon2 && $2 = day2
' | mail $NAME
$
```
Notice that `Jan` appears twice in the data; a "sentinel" data value like this simplifies processing for December.

The final stage is to arrange for the calendar program to be run every day.
What you want is for someone to wake up every morning around 5 AM and run `calendar`.
You can do this yourself by remembering to say (every day!)
```
$ at 5am
calendar
ctl-d
$
```
but that's not exactly automatic or reliable.
The trick is to tell `at` not only to run the calendar, but also to schedule the next run as well.
```
$ cat early.morning
calendar
echo early.morning | at 5am
$
```
The second line schedules another command for the next day, so once started, this sequence is self-perpetuating.
The `at` command sets your `PATH` current directory and other parameters for the commands it processes, so you don't need to do anything special.

#### Loose ends

`awk` is an ungainly language, and it's impossible to show all its capabilities in a chapter of reasonable size.
Here are some other things to look at in the manual:
- Redirecting the output of `print` into files and pipes:
  - Any print or `printf` statement can be followed by `>` and a filename (as a quoted string or in a variable); the output will be sent to the file.
  - As with the shell, `>>` appends instead of overwriting.
  - Printing into pipes uses `|` instead of `>`.
- Multi-line records:
  - If the record separator `RS` is set to newline, then input records will be separated by an empty line.
  - In this way, several input lines can be treated as a single record.
- "Pattern, pattern" as a selector:
  - As in `ed` and `sed`, a range of lines can be specified by a pair of patterns.
  - This matches lines from an occurrence of the first pattern until the next occurrence of the second.
  - A simple example is 
    ```
    NR == 10, NR == 20
    ```
    which matches lines 10 through 20 inclusive.


### 4.5 Good files and good filters

Although the last few `awk` examples are self-contained commands, many uses of `awk` are simple one- or two-line programs to do some filtering as part of a larger pipeline.
This is true of most filters - sometimes the problem at hand can be solved by the application of a single filter, but more commonly it breaks down into subproblems solvable by filters joined together into a pipeline.
This use of tools is often cited as the heart of the UNIX programming environment.
That view is overly restrictive; nevertheless, the use of filters pervades the system, and it is worth observing why it works.

The output produced by UNIX programs is in a format understood as input by other programs.
Filterable files contain lines of text, free of decorative headers, trailers, or blank lines.
Each line is an object of interest - a filename, a word, a description of a running process - so programs like `wc` and `grep` can count interesting items or search for them by name.
When more information is present for each object, the file is still line-by-line, but columnated into fields separated by blanks or tabs, as in the output of `ls -l`.
Given data divided into such fields, programs like `awk` can easily select, process, or rearrange the information.

Filters share a common design.
Each writes on its standard output the result of processing the argument files, or the standard input if no arguments are given.
The arguments specify *input*, never output, so the output of a command can always be fed to a pipeline.
Optional arguments (or non-filename arguments such as the `grep` pattern) precede any filenames.
Finally, error messages are written on the standard error, so they will not vanish down a pipe.

These conventions have little effect on individual commands, but when uniformly applied to all programs result in a simplicity of interconnection, illustrated by many examples throughout this book, but perhaps most spectacularly by the word-counting example at the end of Section 4.2.
If any of the programs demanded a named input or output file, required interaction to specify parameters, or generated headers or trailers, the pipeline wouldn't work.
And of course, if the UNIX system didn't provide pipes, someone would have to write a conventional program to do the job.
But there are pipes, and the pipeline works, and is even easy write if you are familiar with the tools.

