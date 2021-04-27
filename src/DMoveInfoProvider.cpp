#include <iostream>
#include "DMoveInfoProvider.h"
#include "position.h"
#include "search.h"
#include "bitboard.h"
#include "movegen.h"
#include "types.h"
#include "thread.h"
#include "uci.h"
#include <vector>

using namespace std;

namespace DSpace {


//-----------------------------------
DMoveInfoProvider::DMoveInfoProvider(Stockfish::Position& position, Stockfish::Move move)
    : m_position(position), m_move(move) 
{
}
//-----------------------------------
Stockfish::Position& DMoveInfoProvider::Position() const
{
    return m_position;
}
//-----------------------------------
Stockfish::Move DMoveInfoProvider::Move() const
{
    return m_move;
}
//-----------------------------------
bool DMoveInfoProvider::IsCapture() const
{
    /* No need to check if the piece on the destination square is of opposite color
        since we already know the move is legal */
    return !Position().empty(Stockfish::to_sq(Move())); 
}
//-----------------------------------
bool DMoveInfoProvider::IsSEEPositiveCapture() const
{
    return IsCapture() && Position().see_ge(Move(), Stockfish::Value(1));
}


} // namespace DSpace
