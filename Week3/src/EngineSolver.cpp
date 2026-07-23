#include "../include/EngineSolver.h"
#include "../include/zobrist.h"
#include "NNUE.h"
#include <iostream>
using namespace std;

// ===== CONSTRUCTOR DEFINITION=====
EngineSolver::EngineSolver(string fen,int depth) {//because we have 3 files with diff depth required
    // TODO: Initialize board, maxDepth, nodesEvaluated
    maxDepth=depth;
    nodesEvaluated=0;
    board=chess::Board(fen);//look for the board class in the chess namespace
    Zobrist::init();
    hashKey=generateHash();//constructor should generate initial hash value
//create a new board object from the chess library using the fen string

    if(!nnue.loadWeights("nnue_weights.bin")){
        cout<<"NNUE loading failed\n";
    }else{
    cout<<"NNUE loaded successfully\n";
    }
}
//when we initialise a var in the header file it means this is the initialisation every time a new object is created
//when we initialise in a function means every time the function is called the var is initialised
//we need both initialisation as for the first time the function is called it is ok but it needs to be reset for every time function is called
// ===== MAIN SOLVER =====
chess::Move EngineSolver::solvePuzzle() {
    
    nodesEvaluated=0;//this must be set to 0 as it will measure only the evaluted nodes for that puzzle not the total nodes till now if the function is runa no of times
    // TODO: Implement puzzle solving logic
    chess::Movelist moves;
    chess::movegen::legalmoves(moves, board);
    
    //move store all legal moves possible for the given board(LIBRARY AUTO GIVES THE LEGAL MOVES FOR THE CURRENT PLAYER NO NEED TO CHECK)
    int eval=0;
    int besteval=(board.sideToMove()==chess::Color::WHITE)?(INT_MIN):(INT_MAX);
    chess::Move bestMove;//variable type is chess::move and name of the var is bestMove
//chess::move is a object of class move which stores all the details of the move 
//it only declares a var doent initialise (it means to create a empty box with no value)
bool isWhiteToMove=(board.sideToMove()==chess::Color::WHITE);//this needs to be saved before the move is taken

    for(auto move : moves){
        
        
            //in the board class use the function make move which makes the given move 
//we cannot check whose turn it is after making a move as then it would be the next players move so we need to store whose move it is before making a move   
//minimax already increments the nodes evaluated no need to do it here
        
        board.makeMove(move);
        eval=minimax(maxDepth-1,INT_MIN,INT_MAX,(board.sideToMove() == chess::Color::WHITE));//since already made a move we need to search maxDepth-1 deeper
        board.unmakeMove(move);//we need to unmake the move before comparision after the evaluation is done
        if(isWhiteToMove){ 
            if(eval>besteval){
                besteval=eval;
                bestMove=move;
                
            }
            else{
                
                continue;
            }
        }else{
            //MAXDEPTH IS ALREADY DEFINED IN THE HEADER FILE
            if(eval<besteval){
                besteval=eval;
                bestMove=move;
                
            }
            else{
               
                continue;
            }
        }    
    
        
    }
//return the single best move for that position and current player
     return bestMove;
}

// ===== MINIMAX WITH ALPHA-BETA =====
int EngineSolver::minimax(int depth, int alpha, int beta, bool isMaximizing) {
    
    // TODO: Implement minimax + alpha-beta pruning
    nodesEvaluated++;//every time minimax algorithm with alpha beta pruning is executed that node becomes evaluated
    chess::Movelist moves;
    chess::movegen::legalmoves(moves, board);

    if(moves.empty()){//its a checkmate or stalemate(kind of draw when the player has no check mate but also no possible move to make else it may end up in a check if it does so)
        if(board.inCheck())// need to check for checkmate first before is quiet
            return isMaximizing ? -10000000 : 10000000;//WHEN A CHECKMATE 
        else // as it may be checkmate but quiet so 
            return 0;//draw no one wins
    }



    if(depth==0){
        if(isQuiet()){
            return evaluate();

        }
        else{
            return quiescence(alpha,beta,3,isMaximizing);//dont return apply quienscence search
        }
        
    }
    
    if(isMaximizing){
        int maxeval=INT_MIN;
        
        for(auto move:moves){//////
            board.makeMove(move);
            int eval=minimax(depth-1,alpha,beta,false);
            board.unmakeMove(move);
            maxeval=max(eval,maxeval);
            alpha=max(eval,alpha);
            if(alpha>=beta){
                break;//if alpha is 50 and beta is 40 then maximiser will never go down the branch with lower evaluation hence we break to not evaluate the rest branch of that node B
            }
            
        }
        
        return maxeval;
    }
    else{
        int mineval=INT_MAX;
        
        for(auto move:moves){
            board.makeMove(move);
            int eval=minimax(depth-1,alpha,beta,true);
            board.unmakeMove(move);
            mineval=min(eval,mineval);
            beta=min(eval,beta);/////
            if(alpha>=beta){
                break;
            }
            
        }

        
        return mineval;

    }
    
}


    
    

// ===== QUIESCENCE SEARCH =====
int EngineSolver::quiescence(int alpha, int beta,int depth_limit,bool is_maximizing) {
    

    if(depth_limit==0){
        return evaluate();
    }
    nodesEvaluated++;

    



//there is a problem in this if there are many captures moves and they are not ending however evaluated then the engine will get stuck calculating the best move
    // TODO: Implement quiescence search
    //we call the function at a node hence that node becomes evaluated so count the nodes at the start of the function
    int eval=evaluate();//why evaluate and not minimax as we have reached depth 0 and we calculate how good is the position/score
    //if evaluation is greater than beta prune it as the opponent wont let us reach here

    if(is_maximizing){
    if(eval>=beta) return beta;
    alpha=max(alpha,eval);
    } else {
    if(eval<=alpha) return alpha;
    beta=min(beta,eval);
}
    //update the guarenteed score 
    chess::Movelist moves;
    chess::movegen::legalmoves(moves, board);//get all the legal moves for the current position
    //we need to check those legal moves which lead to captures

//check if the board is in check mate and if there are no valid moves available
    if (moves.empty()) {
        if (board.inCheck())
            return is_maximizing ? -10000000 : 10000000;
        return 0;
    }


    for(auto move:moves){//the legal moves are the moves possible at that node at that depth
        if(board.isCapture(move)){//if it is a capture move
        board.makeMove(move);//make the move
        eval=quiescence(alpha,beta,depth_limit-1,!is_maximizing);//recursively search for more captures beyond this move
        board.unmakeMove(move);
        
            if(is_maximizing){
                if(eval>=beta) return beta;
                alpha=max(alpha,eval);
            } else {
                if(eval<=alpha) return alpha;
                beta=min(beta,eval);
            }


        }
        else{
            continue;//if not a capture move no need to evaluate check for the next move
        }
        
    }
    return is_maximizing?alpha:beta;
    //eval is the current position evaluation not the best score till now and we want to return the best score 
    




}


// ===== HELPER: Check if position is quiet =====
bool EngineSolver::isQuiet() {
    // TODO: Check if captures available
    //check captures,promotions,checks
    chess::Movelist moves;
    chess::movegen::legalmoves(moves, board);
    for(auto move:moves){//there is a variable move which iterates over moves
        if(board.isCapture(move)){
            return false;
        }else{
            continue;
        }

    }
    return true;
    

    
}

// ===== HELPER: Evaluate position =====
int EngineSolver::evaluate() {
    // TODO: Count material and return score
//     int whiteScore=0;
//     int blackScore=0;
    
//     for(int i=0;i<64;i++){
//         chess::Piece piece = board.at(i);
//         if(piece.type()==chess::PieceType::BISHOP){
//             if(piece.color()==chess::Color::WHITE){
//                 whiteScore+=3;
//             }
//             else{
//                 blackScore+=3;
//             }
//         }
//         else if(piece.type()==chess::PieceType::KING){
//             continue;
//         }
//         else if(piece.type()==chess::PieceType::QUEEN){
//             if(piece.color()==chess::Color::WHITE){
//                 whiteScore+=9;
//             }
//             else{
//                 blackScore+=9;
//             }
//         }
//         else if(piece.type()==chess::PieceType::PAWN){
//             if(piece.color()==chess::Color::WHITE){
//                 whiteScore+=1;
//             }
//             else{
//                 blackScore+=1;
//             }
//         }
//         else if(piece.type()==chess::PieceType::ROOK){
//             if(piece.color()==chess::Color::WHITE){
//                 whiteScore+=5;
//             }
//             else{
//                 blackScore+=5;
//             }
//         }
//         else if(piece.type()==chess::PieceType::KNIGHT){
//             if(piece.color()==chess::Color::WHITE){
//                 whiteScore+=3;
//             }
//             else{
//                 blackScore+=3;
//             }
//         }
//         else{
//             continue;
//         }
//     }
//         int finalScore=whiteScore-blackScore;
//         return finalScore;
    

    return nnue.evaluate(board);
}
 

void EngineSolver::makeMove(chess::Move move){
    board.makeMove(move);

}
uint64_t EngineSolver::generateHash()
{
    uint64_t hash = 0;

    for (int sq = 0; sq < 64; sq++)
    {
        chess::Piece piece = board.at(chess::Square(sq));

        if (piece != chess::Piece::NONE)
        {
            hash ^= Zobrist::pieceKeys[int(piece)][sq];
        }
    }

    if (board.sideToMove() == chess::Color::WHITE)
    {
        hash ^= Zobrist::sideKey;
    }

    return hash;
}




// ===== GETTER =====
long long EngineSolver::getNodesEvaluated() const {
    return nodesEvaluated;
}