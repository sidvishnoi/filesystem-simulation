> A Hierarchical Filesystem simulation in C++.

This is a project I made during my Masters at Computer Applications course at Dept. of Computer Science, university of Delhi.

The program lets you create a filesystem as a file and allows you to do file/directory operations in a Linux like terminal interface.

See `help.txt` for a list of available commands.

How to use:

```
$ make
$ ./filesystem FILE_SYSTEM_TITLE FILE_SYSTEM_SIZE_IN_MiB
```

It creates a binary file (using `fstream`) inside which files, directories and file/dir entries are created in binary form.

It allows you to create files, create hierarchical directories, delete files and (attempt to) recover deleted files, along with operations like list directory, print directory tree, print working directory, change directory. See `help.txt` for a complete list of commands, or use the inbuilt `help` command in program.

All operations are done at sector levels.

Copyright 2017 Sid Vishnoi under MIT license.