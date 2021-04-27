#include <iostream>
#include "DPieceInfoProvider.h"
#include "DPieceInfoFormatter.h"
#include "position.h"
#include "bitboard.h"
#include "movegen.h"
#include "types.h"
#include "uci.h"
#include <vector>

using namespace std;

namespace DSpace {

const string pieceNames[7] = {"", "pawn", "knight", "bishop", "rook", "queen", "king"};

//-----------------------------------
DPieceInfoFormatter::DPieceInfoFormatter(DPieceInfoProvider& provider)
    : m_provider(provider)
{
}
//-----------------------------------
DPieceInfoProvider& DPieceInfoFormatter::Provider() const
{
    return m_provider;
}
//-----------------------------------
Stockfish::Position& DPieceInfoFormatter::Position() const
{
    return Provider().Position();
}
//-----------------------------------
string DPieceInfoFormatter::PieceSquareName() const
{
    return Stockfish::UCI::square(Provider().PieceSquare());
}
//-----------------------------------
string DPieceInfoFormatter::PieceNameOn(Stockfish::Square sq) const
{
    return pieceNames[type_of(Position().piece_on(sq))];
}
//-----------------------------------
string DPieceInfoFormatter::PieceName() const
{
    return PieceNameOn(Provider().PieceSquare());
}
//-----------------------------------
string DPieceInfoFormatter::PieceColorName() const
{
    return Provider().PieceColor() == Stockfish::WHITE ? "white" : "black";
}
//-----------------------------------
string DPieceInfoFormatter::LegalMovesString() const
{
    string str;
    if (Provider().PieceColor() != Position().side_to_move())
    {
        str = "The " + PieceColorName() + " " + PieceName() + " on " + PieceSquareName() + 
            " has no legal moves since it is not " + PieceColorName() + "'s turn to move.";
    }
    else
    {
        vector<Stockfish::Square> moves = Provider().LegalMoves();
        for (auto to : moves)
        {
            str += Stockfish::UCI::square(to) + ", ";
        }
        str = str.substr(0, str.size()-2); // Remove trailing comma
        if (str.size())
        {
            str = "There are " + to_string(moves.size()) + " legal move(s) for the " + PieceName() + " on " + PieceSquareName() + ": " + str + ".";
        }
        else
        {
            str = "There are no legal moves for the " + PieceName() + " on " + PieceSquareName() + ".";
        }
    }
    return str;
}
//-----------------------------------
string DPieceInfoFormatter::CaptureMovesString() const
{
    string str;
    if (Provider().PieceColor() != Position().side_to_move())
    {
        str = "The " + PieceColorName() + " " + PieceName() + " on " + PieceSquareName() + 
            " has no capture moves since it is not " + PieceColorName() + "'s turn to move.";
    }
    else
    {
        vector<Stockfish::Square> moves = Provider().CaptureMoves();
        for (auto to : moves)
        {
            str += Stockfish::UCI::square(to) + ", ";
        }
        str = str.substr(0, str.size()-2); // Remove trailing comma
        if (str.size())
        {
            str = "The " + PieceName() + " on " + PieceSquareName() + " can make " + to_string(moves.size()) + " capture(s): " + str + ".";
        }
        else
        {
            str = "The " + PieceName() + " on " + PieceSquareName() + " cannot capture any pieces.";
        }
    }
    return str;
}
//-----------------------------------
string DPieceInfoFormatter::IsPinnedString() const
{
    string str = "The " + PieceName() + " on " + PieceSquareName();
    Stockfish::Square by = Stockfish::SQ_NONE;
    Stockfish::Square to = Stockfish::SQ_NONE;
    bool isPinned = Provider().IsPinned(by, to);
    if (isPinned)
    {
        string toPieceStr = PieceNameOn(to);
        string byPieceStr = PieceNameOn(by);
        string toSquareStr = Stockfish::UCI::square(to);
        string bySquareStr = Stockfish::UCI::square(by);
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
string DPieceInfoFormatter::IsHangingString() const
{
    string str = "The " + PieceName() + " on " + PieceSquareName();
    Stockfish::Square leastValuableAttacker = Stockfish::SQ_NONE;
    bool isHanging = Provider().IsHanging(leastValuableAttacker);
    if (isHanging)
    {
        string attackerPieceStr = PieceNameOn(leastValuableAttacker);
        string attackerSquareStr = Stockfish::UCI::square(leastValuableAttacker);
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
