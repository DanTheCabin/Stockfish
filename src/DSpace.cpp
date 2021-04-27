#include <iostream>
#include "DSpace.h"
#include "DSquareInfoProvider.h"
#include "DSquareInfoFormatter.h"
#include "DPositionInfoProvider.h"
#include "DPositionInfoFormatter.h"
#include "DMoveInfoProvider.h"
#include "DMoveInfoFormatter.h"
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
    DPositionInfoProvider provider = DPositionInfoProvider(pos, states);
    DPositionInfoFormatter positionInfo = DPositionInfoFormatter(provider);
    cout << positionInfo.BestMovesString(numberOfMoves) << endl;
}
//-----------------------------------
void pieceinfo(Stockfish::Position& pos, istringstream& is)
{
    string token;
    is >> token;
    Stockfish::Square square = to_square(token);
    if (pos.piece_on(square) != Stockfish::NO_PIECE)
    {
        DSquareInfoProvider provider = DSquareInfoProvider(pos, square);
        DSquareInfoFormatter pieceInfo = DSquareInfoFormatter(provider);
        cout << pieceInfo.LegalMovesString() << endl;
        cout << pieceInfo.CaptureMovesString() << endl;
        cout << pieceInfo.IsPinnedString() << endl;
        cout << pieceInfo.IsHangingString() << endl;        
    }
    else 
    {
        cout << "There is no piece on that square." << endl;
    }
}
//-----------------------------------
void moveinfo(Stockfish::Position& pos, istringstream& is, Stockfish::StateListPtr& states)
{
    string token;
    is >> token;
    string off = "Off";
    Stockfish::Options["Analysis Contempt"] = off;
    Stockfish::Move move = Stockfish::UCI::to_move(pos, token);
    if (move != Stockfish::MOVE_NONE)
    {
        DSquareInfoProvider toSquareProvider = DSquareInfoProvider(pos, Stockfish::to_sq(move));
        DMoveInfoProvider provider = DMoveInfoProvider(pos, move, states, toSquareProvider);
        DMoveInfoFormatter moveInfo = DMoveInfoFormatter(provider);
        cout << moveInfo.CentipawnChangeString() << endl;
        cout << moveInfo.IsSEEPositiveCaptureString() << endl;
        cout << moveInfo.CapturesHangingPieceString() << endl;
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
