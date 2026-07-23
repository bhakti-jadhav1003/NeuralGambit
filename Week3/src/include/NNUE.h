#ifndef NNUE_H
#define NNUE_H

#include "../lib/chess/include/chess.hpp"
#include <vector>
#include <string>

class NNUE
{
private:

    // Layer 1
    std::vector<float> w1;
    std::vector<float> b1;

    // Layer 2
    std::vector<float> w2;
    std::vector<float> b2;

    // Layer 3
    std::vector<float> w3;
    float b3;


public:

    NNUE();

    bool loadWeights(const std::string& filename);

    int evaluate(const chess::Board& board);


private:

    std::vector<float> boardToFeatures(const chess::Board& board);

};

#endif