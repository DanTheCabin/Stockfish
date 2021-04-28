#include <iostream>
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
namespace Util 
{
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
//----------------------------------
string piece_name_on(Stockfish::Position& pos, Stockfish::Square sq)
{
    const string pieceNames[7] = {"", "pawn", "knight", "bishop", "rook", "queen", "king"};
    return pieceNames[Stockfish::type_of(pos.piece_on(sq))];
}
//----------------------------------
vector<Stockfish::Move> all_see_positive_captures(Stockfish::Position& pos)
{
    vector<Stockfish::Move> moves;
    for (const auto& m : Stockfish::MoveList<Stockfish::CAPTURES>(pos))
    {
        if (pos.legal(m) && pos.see_ge(m, Stockfish::Value(1)))
        {
            moves.push_back(m);
        }
    }
    return moves;
}
} // namespace Util

/// 

}
 // namespace DSpace
