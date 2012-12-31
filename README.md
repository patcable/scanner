Scanner
=======

A virus scanner for 20122-4055.760, created by Patrick Cable.

Using Scanner
-------------
This virus scanner will scan all files in a directory structure. 
To use Scanner, you must build it by running `make scan` in the 
directory you extract to. Then, create test structure by running 
`./make_test` which will create a grouping of files but will copy
the virus specimen into one of them. Finally, execute the virus scanner
using `./scan [folder] [signatures]` where folder is a folder (perhaps 
use your test_struct folder created with make_test) and signatures is the
signatures file that was included.

