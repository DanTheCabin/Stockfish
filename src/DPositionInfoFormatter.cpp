#include <iostream>
#include "DPositionInfoProvider.h"
#include "DPositionInfoFormatter.h"
#include "DUtil.h"
#include "position.h"
#include "search.h"
#include "bitboard.h"
#include "movegen.h"
#include "types.h"
#include "thread.h"
#include <vector>

using namespace std;

namespace DSpace {


//-----------------------------------
DPositionInfoFormatter::DPositionInfoFormatter(DPositionInfoProvider& provider)
    : m_provider(provider)
{
}
//-----------------------------------
DPositionInfoProvider& DPositionInfoFormatter::Provider() const
{
    return m_provider;
}
//-----------------------------------
string DPositionInfoFormatter::BestMovesString(int numberOfMoves)
{
    string str = "The best " + to_string(numberOfMoves) + " moves are:";
    vector<Stockfish::Move> bestmoves = Provider().BestMoves(numberOfMoves);
    for (size_t i=0; i < bestmoves.size(); ++i)
    {
        Stockfish::Move m = bestmoves[i];
        str += " " + to_string(i+1) + ". " + Util::from_move(m);
    }
    return str;
}

} // namespace DSpace
