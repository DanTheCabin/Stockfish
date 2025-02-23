#include <iostream>
#include "DSquareInfoProvider.h"
#include "DSquareInfoFormatter.h"
#include "DUtil.h"
#include "position.h"
#include "bitboard.h"
#include "movegen.h"
#include "types.h"
#include <vector>

using namespace std;

namespace DSpace {

//-----------------------------------
DSquareInfoFormatter::DSquareInfoFormatter(DSquareInfoProvider& provider)
    : m_provider(provider)
{
}
//-----------------------------------
DSquareInfoProvider& DSquareInfoFormatter::Provider() const
{
    return m_provider;
}
//-----------------------------------
Stockfish::Position& DSquareInfoFormatter::Position() const
{
    return Provider().Position();
}
//-----------------------------------
string DSquareInfoFormatter::SquareName() const
{
    return Util::square(Provider().Square());
}
//-----------------------------------
string DSquareInfoFormatter::PieceNameOn(Stockfish::Square sq) const
{
    return Util::piece_name_on(Position(), sq);
}
//-----------------------------------
string DSquareInfoFormatter::PieceName() const
{
    return PieceNameOn(Provider().Square());
}
//-----------------------------------
string DSquareInfoFormatter::PieceColorName() const
{
    return Provider().PieceColor() == Stockfish::WHITE ? "white" : "black";
}
//-----------------------------------
string DSquareInfoFormatter::LegalMovesString() const
{
    string str;
    if (Provider().PieceColor() != Position().side_to_move())
    {
        str = "The " + PieceColorName() + " " + PieceName() + " on " + SquareName() + 
            " has no legal moves since it is not " + PieceColorName() + "'s turn to move.";
    }
    else
    {
        vector<Stockfish::Move> moves = Provider().LegalMoves();
        for (auto m : moves)
        {
            str += Util::from_move(m) + ", ";
        }
        str = str.substr(0, str.size()-2); // Remove trailing comma
        if (str.size())
        {
            str = "There are " + to_string(moves.size()) + " legal move(s) for the " + PieceName() + " on " + SquareName() + ": " + str + ".";
        }
        else
        {
            str = "There are no legal moves for the " + PieceName() + " on " + SquareName() + ".";
        }
    }
    return str;
}
//-----------------------------------
string DSquareInfoFormatter::CaptureMovesString() const
{
    string str;
    if (Provider().PieceColor() != Position().side_to_move())
    {
        str = "The " + PieceColorName() + " " + PieceName() + " on " + SquareName() + 
            " has no capture moves since it is not " + PieceColorName() + "'s turn to move.";
    }
    else
    {
        vector<Stockfish::Move> moves = Provider().CaptureMoves();
        for (auto m : moves)
        {
            str += Util::from_move(m) + ", ";
        }
        str = str.substr(0, str.size()-2); // Remove trailing comma
        if (str.size())
        {
            str = "The " + PieceName() + " on " + SquareName() + " can make " + to_string(moves.size()) + " capture(s): " + str + ".";
        }
        else
        {
            str = "The " + PieceName() + " on " + SquareName() + " cannot capture any pieces.";
        }
    }
    return str;
}
//-----------------------------------
string DSquareInfoFormatter::IsPinnedString() const
{
    string str = "The " + PieceName() + " on " + SquareName();
    Stockfish::Square by = Stockfish::SQ_NONE;
    Stockfish::Square to = Stockfish::SQ_NONE;
    bool isPinned = Provider().IsPinned(by, to);
    if (isPinned)
    {
        string toPieceStr = PieceNameOn(to);
        string byPieceStr = PieceNameOn(by);
        string toSquareStr = Util::square(to);
        string bySquareStr = Util::square(by);
        str += " is pinned to the " + 
            toPieceStr + " on " + toSquareStr + " by the " + byPieceStr + " on " + bySquareStr + ".";
    }
    else
    {
        str += " is not pinned.";
    }
    return str;
}
//-----------------------------------
string DSquareInfoFormatter::IsHangingString() const
{
    string str = "The " + PieceName() + " on " + SquareName();
    
    bool isHanging = Provider().IsHanging();
    if (isHanging)
    {
        Stockfish::Square leastValuableAttacker = Provider().LeastValuableAttacker();
        string attackerPieceStr = PieceNameOn(leastValuableAttacker);
        string attackerSquareStr = Util::square(leastValuableAttacker);
        str += " is hanging as it can be SEE-negatively caputred by the " + 
            attackerPieceStr + " on " + attackerSquareStr + ".";
    }
    else
    {
        str += " is not hanging.";
    }
    return str;
}
//-----------------------------------


} // namespace DSpace
