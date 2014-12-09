I have tried to take a highly modular approach
to the design of this program.  As per attached PDF,
Each module has its own set of structures, that are
inaccessible to other modules, except through accessor
and getter methods.  This abtracts the content of these
data structures and allows for safe, flexible design.
I feel it also allowed me to insert my extensions very
easily and neatly.

The testing module provides another interface for the 
user to interact with the program.  You can test that
moves give rise to expected board states via editing
the testMoves.txt file with the following format:

targetRow targetCol move (u,l,r,d)*

*up left right down

I utilised two hashing methods, attempting to use this
assignment as a investigation into hashing.  I felt that
Chained hashing was the best option for storing my keys,
due to the number of items needing stored.  

Sources used for Zobrist:
http://en.wikipedia.org/wiki/Zobrist_hashing
https://chessprogramming.wikispaces.com/Zobrist+Hashing

Sources used for the bit board approach, which involved
encoding the boards into a bit pattern:
https://chessprogramming.wikispaces.com/Bitboards
http://www.3dkingdoms.com/checkers/bitboards.htm

General hashing information:
Standish, A. Thomas.1994. Data Structures, Algorithms & Software principles in C.

In comparison, the bitBoards is the faster method.  It 
seems to generate a better hash key and drastically 
reduces the size of memory required to store the boards.  
As this particular board game only has 2*56 values required 
for Zobrist, it was not necessary to take advantage of the 
Xor functionality.  Searching for unique boards was the 
bottleneck: not generating a key for each board.


