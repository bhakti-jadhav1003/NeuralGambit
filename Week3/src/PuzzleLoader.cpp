#include "../include/PuzzleLoader.h"//every .cpp file has to include its own header file
#include<iostream>
#include<fstream>


#include"json.hpp"//<> angle brackets are for system libraries and "" are for your own libraries
using json = nlohmann::json;//to define json ,when we write json we mean nlohmann::json
using namespace std;
vector<Puzzle> PuzzleLoader::loadFromJson(string filepath){//PuzzleLoader:: tells the compiler loadFromJson function belongs to this class
    vector<Puzzle>puzzles;//declare a empty vector puzzles
    ifstream file(filepath);
    if(!file.is_open()){
    cerr << "Failed to open: "
         << filepath << endl;
    return {};
    }
    json data = json::parse(file);
    for(auto& [key, value] : data.items()) {
    Puzzle P;
    P.fen=key;
    P.expectedsolution = value.get<string>();
    puzzles.push_back(P);
}
return puzzles;

}
//value is a JSON object, you need .get<string>() to convert it to an actual C++ string.