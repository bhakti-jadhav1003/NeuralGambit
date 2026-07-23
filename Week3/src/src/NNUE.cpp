#include "../include/NNUE.h"

#include <fstream>
#include <algorithm>


NNUE::NNUE()
{
    b3 = 0;
}



bool NNUE::loadWeights(const std::string& filename)
{

    std::ifstream file(filename,std::ios::binary);


    if(!file)
        return false;



    w1.resize(256*768);
    b1.resize(256);


    w2.resize(128*256);
    b2.resize(128);


    w3.resize(128);



    file.read(
        reinterpret_cast<char*>(w1.data()),
        w1.size()*sizeof(float)
    );


    file.read(
        reinterpret_cast<char*>(b1.data()),
        b1.size()*sizeof(float)
    );


    file.read(
        reinterpret_cast<char*>(w2.data()),
        w2.size()*sizeof(float)
    );


    file.read(
        reinterpret_cast<char*>(b2.data()),
        b2.size()*sizeof(float)
    );


    file.read(
        reinterpret_cast<char*>(w3.data()),
        w3.size()*sizeof(float)
    );


    file.read(
        reinterpret_cast<char*>(&b3),
        sizeof(float)
    );


    return true;
}




std::vector<float> NNUE::boardToFeatures(
        const chess::Board& board)
{

    std::vector<float> features(768,0.0f);



    for(int sq=0;sq<64;sq++)
    {

        chess::Piece piece = board.at(sq);


        if(piece.type()==chess::PieceType::NONE)
            continue;



        int index;



        switch(static_cast<int>(piece.type()))
        {

            case static_cast<int>(chess::PieceType::PAWN):
                index=0;
                break;


            case static_cast<int>(chess::PieceType::KNIGHT):
                index=1;
                break;


            case static_cast<int>(chess::PieceType::BISHOP):
                index=2;
                break;


            case static_cast<int>(chess::PieceType::ROOK):
                index=3;
                break;


            case static_cast<int>(chess::PieceType::QUEEN):
                index=4;
                break;


            case static_cast<int>(chess::PieceType::KING):
                index=5;
                break;


            default:
                continue;
        }



        if(piece.color()==chess::Color::BLACK)
            index+=6;



        features[index*64+sq]=1.0f;

    }


    return features;

}






int NNUE::evaluate(const chess::Board& board)
{

    auto input = boardToFeatures(board);



    // ---------- Layer 1 ----------

    std::vector<float> layer1(256);


    for(int i=0;i<256;i++)
    {

        float sum=b1[i];


        for(int j=0;j<768;j++)
        {
            sum += w1[i*768+j]*input[j];
        }


        layer1[i]=std::max(0.0f,sum);

    }




    // ---------- Layer 2 ----------


    std::vector<float> layer2(128);



    for(int i=0;i<128;i++)
    {

        float sum=b2[i];


        for(int j=0;j<256;j++)
        {
            sum += w2[i*256+j]*layer1[j];
        }


        layer2[i]=std::max(0.0f,sum);

    }





    // ---------- Output ----------


    float output=b3;



    for(int i=0;i<128;i++)
    {
        output += w3[i]*layer2[i];
    }



    return (int)output;

}