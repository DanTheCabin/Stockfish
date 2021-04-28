#include <iostream>
#include "DMoveInfoProvider.h"
#include "DMoveInfoFormatter.h"
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
DMoveInfoFormatter::DMoveInfoFormatter(DMoveInfoProvider& provider)
    : m_provider(provider)
{
}
//-----------------------------------
DMoveInfoProvider& DMoveInfoFormatter::Provider() const
{
    return m_provider;
}
//-----------------------------------
string DMoveInfoFormatter::MoveName() const
{
    return Util::from_move(Provider().Move());
}
//-----------------------------------
string DMoveInfoFormatter::PieceName(Stockfish::Square sq) const
{
    return Util::piece_name_on(Provider().Position(), sq);
}
//-----------------------------------
string DMoveInfoFormatter::SquareName(Stockfish::Square sq) const
{
    return Util::square(sq);
}
//-----------------------------------
string DMoveInfoFormatter::IsSEEPositiveCaptureString() const
{
    string str = "The move is";
    bool isSEEPositiveCap = Provider().IsSEEPositiveCapture();
    if (!isSEEPositiveCap)
    {
        str += " not";
    }
    str += " a SEE-positive capture.";
    return str;
}
//-----------------------------------
string DMoveInfoFormatter::CentipawnChangeString() const
{
    double cpChange = Provider().CentipawnChange();
    string direction = cpChange > 0 ? " wins " : " loses ";
    string str = "(Questionable) The move supposedly" + direction +
        to_string(abs(cpChange)) + " centipawns (static evaluation).";
    return str;
}
//-----------------------------------
string DMoveInfoFormatter::CapturesHangingPieceString() const
{
    string str = "The move ";
    if (Provider().CapturesHangingPiece())
    {
        str += "captures a hanging " + PieceName(Stockfish::to_sq(Provider().Move())) + " (it may have defenders, but the SEE will be positive).";
    }
    else
    {
        str += "does not capture a hanging piece.";
    }
    return str;
}
//-----------------------------------
string DMoveInfoFormatter::SEEPositiveCapturesAfterMoveString() const
{
    ostringstream str;
    str << "The move allows ";
    vector<Stockfish::Move> captures = Provider().SEEPositiveCapturesAfterMove();
    Provider().States()->emplace_back();
    Provider().Position().do_move(Provider().Move(), Provider().States()->back());
    if (captures.size())
    {
        str << captures.size() << " SEE-positive capture moves:";
        for (auto& m : captures)
        {
            str << endl << "\t The " << PieceName(Stockfish::from_sq(m)) << " on " << SquareName(Stockfish::from_sq(m)) 
            << " captures the " << PieceName(Stockfish::to_sq(m)) << " on " << SquareName(Stockfish::to_sq(m)) << ".";
        }
    }
    else
    {
        str << "no SEE-positive capture moves.";
    }
    Provider().Position().undo_move(Provider().Move());
    return str.str();
}
} // namespace DSpace
