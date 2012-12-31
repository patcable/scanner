Scanner
=======

A virus scanner for 20122-4055.760, created by Patrick Cable based on
sample code given by Prof. Bo Yuan.

Using Scanner
-------------
This virus scanner will scan all files in a directory structure. 
To use Scanner, you must build it by running `make scan` in the 
directory you extract to. Then, create a test structure by running 
`./make_test` which will create a grouping of files but will copy
the virus specimen into one of them. Finally, execute the virus scanner
using `./scan [folder] [signatures]` where folder is a folder (perhaps 
use your test_struct folder created with make_test) and signatures is the
signatures file that was included.

To summarize:
* `make scan`
* `./make_test`
* `./scan test_struct/ signatures`

Design
------
Scanner implements the nftw() function defined in POSIX - it recursively
goes through a directory path specified in the function call, and can 
perform an action with those files if passed a function name. In Scanner,
a function called `scan_tree` performs the work of scanning each individual
file based on the global signature definitions stored in the `sig` character
variable.

Looking at the program step-by-step:

1. The program reads in the virus signatures and stores them in `char *sig`

2. The `nftw()` function is called, passing the directory path grabbed in 
   `argv[1]` and the `scan_tree()` function is passed. We pass a general 
   variable to capture flags, and open 20 file descriptors to facilitate 
   scanning

3. Within `scan_tree()` we scan the file if the file type matches `FTW_F`
   which is the flag for a file (as opposed to a directory, etc.) and 
   facilitate opening the file, reading it into the buffer, then comparing
   the strings contined inside with the contents of `char *sig`. If there 
   are matches, it will display that next to the filename. If not, it 
   will continue on.

4. On completion, memory is freed accordingly and the program terminates

Sample Output
-------------
```[1612][cable@dauntless:~/class]$ ./scan test_struct/ signatures
Scanning test_struct/folder4/file1...
Scanning test_struct/folder4/file3... Contains virus 1 Contains virus 2 Contains virus 4
Scanning test_struct/folder4/file2...
Scanning test_struct/folder1/file1...
Scanning test_struct/folder1/file3...
Scanning test_struct/folder1/file2...
Scanning test_struct/folder3/file1...
Scanning test_struct/folder3/file3...
Scanning test_struct/folder3/file2...
Scanning test_struct/folder5/file1...
Scanning test_struct/folder5/file3...
Scanning test_struct/folder5/file2...
Scanning test_struct/folder2/file1...
Scanning test_struct/folder2/file3...
Scanning test_struct/folder2/file2...
Scanning test_struct/folder6/file1...
Scanning test_struct/folder6/file3...
Scanning test_struct/folder6/file2...```

Notes
-----
Spending a lot of my time in Linux/UNIX, I tend to come across a lot of C.
However I tend to do more of my work in interpreted languages like Ruby and
PERL. So, this is the first time I have actually made something useful in
C besides various compiler tests and `printf("hello world\n")` - so I chose
to take this as a challenge. Hopefully this helps explain why I commented 
even the most basic of C functions.
