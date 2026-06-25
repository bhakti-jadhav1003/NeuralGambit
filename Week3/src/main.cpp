//why main.cpp is needed
//it needs to take the puzzles from the puzzleloader function and use the enginesolver to giev teh soln and verify the soln with the expected soln
#include<iostream>
#include<vector>
#include<string>
#include "../include/EngineSolver.h"
#include "../include/PuzzleLoader.h"
using namespace std;

int main(){
    int correct=0;
    int total=0;
    int puzzleNumber=1;
    vector<string> files = {
    "mate_in_2.json",
    "mate_in_3.json",
    "mate_in_4.json"
};
for(const auto& file : files){

    cout << "\nTesting " << file << endl;

    vector<Puzzle> Puzzles = PuzzleLoader::loadFromJson(file);

    for(auto puzzle : Puzzles){
    //loadfromjson is a static function
    //create your specific engine for each puzzle
    EngineSolver solver(puzzle.fen,10);//enginesolver is the class and solver is the object of that class
    
    chess::Board b(puzzle.fen);

    chess::Move move1 = solver.solvePuzzle();
    string predictedSan = " 1. "+chess::uci::moveToSan(b, move1);
    solver.makeMove(move1);
    b.makeMove(move1); // keep b in sync

    chess::Move move2 = solver.solvePuzzle();
    predictedSan = predictedSan + " " + chess::uci::moveToSan(b, move2);
    solver.makeMove(move2);
    b.makeMove(move2); // keep b in sync

    chess::Move move3 = solver.solvePuzzle();
    predictedSan = predictedSan + " 2. " + chess::uci::moveToSan(b, move3);
    b.makeMove(move3);

    
    //we use the method/function solvePuzzle which belongs to the class EngineSolver
    //move is a class inside the chess namespace/library and to access it we use chess::move
    //the bestmove returned by the function is stored in the chess:move object and not a simple variable as a simple var cannot store diff pieces of info in a single var hence store it in a object 
    //convert the bestmove object to simple string uci is the class inside the chess library having a function moveToUci for this conversion
    
    cout<<"predicted soln is"<<predictedSan<<endl;
    cout<<"\nPuzzle "<<puzzleNumber<<endl;
    puzzleNumber++;
    total++;




    }

}
return 0;
}