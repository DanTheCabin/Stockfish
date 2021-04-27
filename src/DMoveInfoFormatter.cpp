#include <iostream>
#include "DMoveInfoProvider.h"
#include "DMoveInfoFormatter.h"
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
    return Stockfish::UCI::move(Provider().Move(), false);
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
    string str = "The move " + direction +
        to_string(cpChange) + " centipawns.";
    return str;
}
//-----------------------------------
string DMoveInfoFormatter::CapturesHangingPieceString() const
{
    string str = "The move ";
    if (Provider().CapturesHangingPiece())
    {
        str += " captures a hanging piece (it may have defenders, but the SEE will be positive).";
    }
    else
    {
        str += " does not capture a hanging piece.";
    }
    return str;
}

} // namespace DSpace
