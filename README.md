# Huffman archiver

## Preface
This was my first serious project, written in the spring of 2017 while studying in NSU.
All code has in initial state. The only thing I added cmake and removed broken comments. 
Archiver based on huffman algorithm.
You are free to use this archiver.

## Install
Install cmake and run:
``` 
    cmake .
    make
    ./archiver
```

## Usage
```
./archiver <arg> <arch_name.eue> <file1> <file2>...
```

### Args 
1. a - add files to archive
1. x - extract files
1. d - delete files from archive
1. l - info about files in archive