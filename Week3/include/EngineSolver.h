#ifndef ENGINE_SOLVER_H
#define ENGINE_SOLVER_H //these two are the guard lines which prevent file from being included twice

#include "../lib/chess/include/chess.hpp"
#include <climits>  //for INT_MAX and INT_MIN
#include <iostream> //for std::cout(debugging)
#include <NNUE.h>

class EngineSolver {
private:
    // ===== MEMBER VARIABLES (Data the engine stores) =====
    //these are private because only enginesolver class uses these no external code should access these directly
    chess::Board board;           // stores the current chess position
    int maxDepth;                 // How deep to search (usually 4-6)/search depth
    long long nodesEvaluated;     // Counter for statistics/count the no of nodes evaluated/how many positions are evaluated
    uint64_t hashKey;//as we need the hash for the current board position
    NNUE nnue;
    //the board must know hash of its current position 
    // ===== PRIVATE METHODS (Helper functions for internal use) =====
    //these are not directly called by the user/cannot be accessed by the user they are only used by the engine internally 
    //user doent call minimax,quiescence function they call the solver function which includes the use of these functions
    // Core minimax with alpha-beta pruning
    int minimax(int depth, int alpha, int beta, bool isMaximizing);
    uint64_t generateHash();//the engine must calculate the initial hash value with the fen fed to it
    
    // Quiescence search to handle tactical positions
    int quiescence(int alpha, int beta,int depth_limit,bool ismaximizing);
    
    // Check if a position is quiet (no captures available) to tell the quiescence search where to stop
    bool isQuiet();
    int evaluate();
    

public:
    // ===== CONSTRUCTOR DECLARATION=====
    
    // Create solver/engine from FEN position/string
    EngineSolver(std::string fen,int depth);
    void makeMove(chess::Move move);//it doesnt return anything but makes the move on the board
    
    
    // ===== PUBLIC METHODS (What external code can call) =====
    
    // Find and return the best move for the puzzle
    chess::Move solvePuzzle();
    
    // Get statistics return the no of nodes evaluated
    long long getNodesEvaluated() const;
};

#endif
//this is a header file which contains the declaration of variables (what var exist) and methods (what methods and pparameters exist) available not the actual code logic