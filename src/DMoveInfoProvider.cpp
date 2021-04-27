#include <iostream>
#include "DMoveInfoProvider.h"
#include "DSquareInfoProvider.h"
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
DMoveInfoProvider::DMoveInfoProvider(
    Stockfish::Position& position, 
    Stockfish::Move move, 
    Stockfish::StateListPtr& states, 
    DSquareInfoProvider& toProvider
    )
    : m_position(position), m_move(move), m_states(states), m_toProvider(toProvider)
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
Stockfish::StateListPtr& DMoveInfoProvider::States() const
{
    return m_states;
}
//-----------------------------------
DSquareInfoProvider DMoveInfoProvider::ToSquare() const
{
    return m_toProvider;
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
//-----------------------------------
double DMoveInfoProvider::CentipawnChange() const
{
    Stockfish::StateInfo st;
    Stockfish::Value before = Stockfish::Eval::evaluate(Position());
    Position().do_move(Move(), st);
    Stockfish::Value after = Stockfish::Eval::evaluate(Position());
    Position().undo_move(Move());
    return -(double(after) * 100 / Stockfish::PawnValueEg) + (double(before) * 100 / Stockfish::PawnValueEg); // Sign on the eval value changes depending on side to move
}
//-----------------------------------
bool DMoveInfoProvider::CapturesHangingPiece() const
{
    return IsCapture() && ToSquare().IsHanging();
}

} // namespace DSpace
