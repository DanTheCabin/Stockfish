#include <iostream>
#include "DMoveInfoProvider.h"
#include "DSquareInfoProvider.h"
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
DMoveInfoProvider::DMoveInfoProvider(
    Stockfish::Position& position, 
    Stockfish::Move move, 
    Stockfish::StateListPtr& states
    )
    : m_position(position), m_move(move), m_states(states)
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
    States()->emplace_back();
    Stockfish::Value before = Stockfish::Eval::evaluate(Position());
    Position().do_move(Move(), States()->back());
    Stockfish::Value after = Stockfish::Eval::evaluate(Position());
    Position().undo_move(Move());
    return -(double(after) * 100 / Stockfish::PawnValueEg) + (double(before) * 100 / Stockfish::PawnValueEg); // Sign on the eval value changes depending on side to move
}
//-----------------------------------
bool DMoveInfoProvider::CapturesHangingPiece() const
{
    bool isHanging = DSquareInfoProvider(Position(), Stockfish::to_sq(Move())).IsHanging();
    return IsCapture() && isHanging;
}
//------------------------------------
vector<Stockfish::Move> DMoveInfoProvider::AllSEEPositiveCaptures() const
{
    return Util::all_see_positive_captures(Position());
}
//------------------------------------
vector<Stockfish::Move> DMoveInfoProvider::SEEPositiveCapturesAfterMove() const
{
    States()->emplace_back();
    Position().do_move(Move(), States()->back());
    vector<Stockfish::Move> seePositiveCaptures = AllSEEPositiveCaptures();
    Position().undo_move(Move());
    return seePositiveCaptures;
}
//-------------------------------------
} // namespace DSpace
