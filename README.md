# B+ Tree file

This project is an implementation of a generic 2-field disk file, with B+ indexing, based on a BlockFile library.
It has AM(Access method) functions, to properly handle a file with primary indexing, which is made possible with a perfectly balanced tree index.
The file can store integer, float or string values.

The most significant functions to handle the B+ tree file are:

- **AM_CreateIndex(..)** Used to create a new B+ tree file.
- **AM_DestroyIndex(..)** Used to destroy a B+ tree file.
- **AM_OpenIndex(..)** Used to open a previously created B+ tree file.
- **AM_CloseIndex(..)** Used to close a previously opened B+ tree file.
- **AM_Insert_Entry(..)** Used to insert an entry into the file. All the entries must be of the same type, else, an error will be returned
- **AM_FindNextEntry(..)** Used to find the next entry that satisfies the condition of a previously opened search
- **AM_PrintError(..)** Used for error handling.


## Getting Started
Make sure you have a gcc compiler
### Downloading
Download source code by typing:

``` git clone https://github.com/nikosgalanis/B-Plus-Tree.git ```


## Examples

In the directory examples, there is a simple main aimed to insert entries in the file, and then search for one of them. 

## Compiling and running
There are 2 main functions implemented to test the functionability of the B+ Tree.
You can run either main by:
``` ./run.sh```
or 
``` ./main.sh```


## Contributors 

[Nikos Galanis](https://github.com/nikosgalanis).

[Pantelis Papageorgiou](https://github.com/panpapag).

[Sofoklis Strompolas](https://github.com/SofoSt).

This project was implemented for the needs of the course "Implementation of Database Management Systems"
