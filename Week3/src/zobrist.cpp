#include "../include/Zobrist.h"//this imports the class declaration
#include <random>//provides random no generator as we are generating random nos
//header tells the compiler these what exist and .cpp creates memory and implements the function
uint64_t Zobrist::pieceKeys[12][64];//creates and allocates memory to the table
uint64_t Zobrist::sideKey;//this creates memory for this white to move key
void Zobrist::init()//means the function belongs to the class
//called as Zobrist::init()
{//123456 is called the seed means start the random generator machine from this no; it can be any no
    // Using the same seed generates the same sequence of random numbers every run.
    std::mt19937_64 rng(123456);//Creates a random-number generator machine.
// 12 piece types × 64 board squares = 768 random keys this is the table size
    for(int piece = 0; piece < 12; piece++)
    {
        for(int square = 0; square < 64; square++)
        {
            pieceKeys[piece][square] = rng(); //assign random values to these 
        }
    }

    // Random number used to distinguish positions where White is to move
    sideKey = rng();

}
// In Zobrist hashing every piece-square combination gets a unique random number.
// For a given board position we XOR together the numbers corresponding to all pieces on the board.
// The resulting value is the hash of that position.
// Random 64-bit numbers make hash collisions extremely unlikely.
// Using simple values would cause many different positions to produce the same hash.
//this is done just for the initialization purpose


//so initially we have assigned a random no to alll positions making move changes only few components of the xor 
//so no need to compute for the rest unchnaged square only move and whose chance to move
//for a white i have a random no for sidekey assigned for black no randon no the hash is just the piecekeys
//for white is xor of piecekeys and sidekey