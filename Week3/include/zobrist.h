#ifndef ZOBRIST_H
#define ZOBRIST_H//header guards to ensure no duplicate definitions
#include <cstdint>//provides uint64_t which is a unsigned 64 bit integer
//zobrist hashes are stored in uint64_t
//creating a class to store the hashing data
class Zobrist//a box containing functions and variables
{
    public://everything inside this can be accessed from outside
    // [piece type][square]
    //static so that only one table is shared by the entire engine and not diff copies are created
    static uint64_t pieceKeys[12][64];//there are 12 piecetypes and 64 squares on the board
    //this is one extra no stores so that even if the piece placement is same but the side to move is diff both get diff hash values
    // white-to-move key
    static uint64_t sideKey;

    // generate random numbers
    static void init();// example Zobrist::init();
    //this is a function declaration
    //purpose is to fill all the entries of piecekeys with random nos




};
#endif//end of header guard
//no memeory is allocated yet just a class and its functions and variables are defined so far like it is just a blueprint