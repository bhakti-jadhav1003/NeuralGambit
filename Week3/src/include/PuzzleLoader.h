#ifndef PUZZLE_LOADER_H
#define PUZZLE_LOADER_H
#include <vector>
#include "Puzzle.h"
#include "json.hpp"
#include<string>
using namespace std;
class PuzzleLoader{//define a class
    public://these are accessible from outside
    static vector<Puzzle> loadFromJson(string filepath);
};//it is a static function and can be called PuzzleLoader::loadFromJson() without creating a object
//static function means it can be called without creating a object
#endif
//PuzzleLoader stores nothing. It just takes a file, reads it, returns puzzles, done. No data to remember. So making an object of it is pointless — hence static.
//like enginesolver had depth,nodesevaluated are the data the class needs to store along with the function