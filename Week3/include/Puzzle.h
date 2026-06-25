#ifndef PUZZLE_H//in the project if puzzle_h is not defined yet include it it prevents the same file being included in the project twice
#define PUZZLE_H //so now define puzzle_h since it was not defined and next time ifndef will stop it from including again
#include<string>//bring the string class from the standard library without this strings type won't exist
using namespace std;//lets you write string instead of std::string everywhere in the file

struct Puzzle{//creates a new data type puzzle which grps the related data
    string fen;//stores the chess position as a fen string
    string expectedsolution;//stores the expected soln in the form of a string

};
#endif//cleses the ifndef block