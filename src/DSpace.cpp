#include <iostream>
#include "DSpace.h"
#include "DPieceInfoProvider.h"
#include "DPieceInfoFormatter.h"
#include "DPositionInfoProvider.h"
#include "DMoveInfoProvider.h"
#include "position.h"
#include "movegen.h"
#include "types.h"
#include <string>
#include "uci.h"
#include "search.h"
#include "thread.h"

using namespace std;

namespace DSpace {
//-----------------------------------
void bestmoves(Stockfish::Position& pos, istringstream& is, Stockfish::StateListPtr& states)
{
    string numberOfMovesStr;
    int numberOfMoves;
    is >> numberOfMovesStr;
    
    try
    {
        numberOfMoves = std::stoi(numberOfMovesStr);
    }
    catch (std::exception& e)
    {
        numberOfMoves = 3;
    }

    Stockfish::Options["MultiPV"] = to_string(numberOfMoves);
    DPositionInfoProvider positionInfo = DPositionInfoProvider(pos, states);
    cout << positionInfo.BestMovesString(numberOfMoves) << endl;
}
//-----------------------------------
void pieceinfo(Stockfish::Position& pos, istringstream& is)
{
    string token;
    is >> token;
    Stockfish::Square pieceSquare = to_square(token);
    try
    {
        DPieceInfoProvider provider = DPieceInfoProvider(pos, pieceSquare);
        DPieceInfoFormatter pieceInfo = DPieceInfoFormatter(provider);
        cout << pieceInfo.LegalMovesString() << endl;
        cout << pieceInfo.CaptureMovesString() << endl;
        cout << pieceInfo.IsPinnedString() << endl;
        cout << pieceInfo.IsHangingString() << endl;        
    }
    catch (const std::runtime_error& e)
    {
        cout << e.what() << endl;
    }
}
//-----------------------------------
void moveinfo(Stockfish::Position& pos, istringstream& is)
{
    string token;
    is >> token;
    Stockfish::Move move = Stockfish::UCI::to_move(pos, token);
    if (move != Stockfish::MOVE_NONE)
    {
        DMoveInfoProvider moveInfo = DMoveInfoProvider(pos, move);
        cout << moveInfo.IsSEEPositiveCapture() << endl;
    }
    else
    {
        cout << token + " is not a legal move" << endl;
    }
}
//-----------------------------------
// Square to_square(string): Helper function that would actually belong in uci.cpp where "to_move" is a related function, but I put it here for separation
//-----------------------------------
Stockfish::Square to_square(string sqStr)
{
    Stockfish::Square sq = Stockfish::SQ_NONE;
    for (Stockfish::Square sqi = Stockfish::SQ_A1; sqi <= Stockfish::SQ_H8; ++sqi)
    {
        if (sqStr == Stockfish::UCI::square(sqi))
        {
            sq = sqi;
            break;
        }
    }
    return sq;
}
//-----------------------------------
/// 

}
 // namespace DSpace
