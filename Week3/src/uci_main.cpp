#include<iostream>
#include<string>
#include <chrono>
#include "../include/EngineSolver.h"
#include "../lib/chess/include/chess.hpp"
#include "../include/zobrist.h"
using namespace std;
int main(){
    Zobrist::init();//when the program starts every object run the same zobrist function so they have the same hash value
    string command;
    chess::Board board;//this board object stores the current board position
    while(getline(cin,command)){//this loop continues to take input after every command till the program exits
        if(command=="uci"){
            cout<<"id name MyEngine"<<endl;
            cout<<"id author Bhakti"<<endl;
            cout<<"uciok"<<endl;
        }
        else if(command=="isready"){
            cout<<"readyok"<<endl;
        }
        else if(command=="quit"){
            cout<<"Exiting"<<endl;
            break;
        }
        else if(command=="position startpos"){
            board = chess::Board();//set the board to the normal starting position
        }
        else if(command.rfind("go",0)==0){//if "go" is starting from the starting index 0 

        auto start =std::chrono::steady_clock::now();
        chess::Move bestMove;
        for(int depth=1; depth<=10; depth++){
            EngineSolver solver(board.getFen(),depth);
            bestMove =solver.solvePuzzle();
            auto now =std::chrono::steady_clock::now();
            auto elapsed =std::chrono::duration_cast<std::chrono::milliseconds>(now-start).count();
            if(elapsed > 1000){
                break;
            }
        }
            
            cout<<"bestmove is"<<chess::uci::moveToUci(bestMove)<<endl;
            
        }
        else if(command == "ucinewgame")
        {
            board = chess::Board();
        }



}
return 0;
}