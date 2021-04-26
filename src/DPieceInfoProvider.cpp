#include <iostream>
#include "DPieceInfoProvider.h"
#include "position.h"
#include "movegen.h"
#include "types.h"
#include "uci.h"
#include <vector>

using namespace std;

const string pieceNames[7] = {"", "pawn", "knight", "bishop", "rook", "queen", "king"};

namespace Stockfish {
//-----------------------------------
DPieceInfoProvider::DPieceInfoProvider(Stockfish::Position& position, Square pieceSquare)
    : m_position(position), m_pieceSquare(pieceSquare) 
{
    if (type_of(m_position.piece_on(m_pieceSquare)) == NO_PIECE_TYPE)
    {
        throw invalid_argument("There is no piece on " + this->PieceSquareString());
    }
}
//-----------------------------------
Position& DPieceInfoProvider::Position() const
{
    return this->m_position;
}
//-----------------------------------
Square DPieceInfoProvider::PieceSquare() const
{
    return this->m_pieceSquare;
}
//-----------------------------------
string DPieceInfoProvider::PieceSquareString() const
{
    return UCI::square(this->m_pieceSquare);
}
//-----------------------------------
string DPieceInfoProvider::PieceName() const
{
    return pieceNames[type_of(this->Position().piece_on(this->PieceSquare()))];
}
//-----------------------------------
vector<Square> DPieceInfoProvider::LegalMoves() const
{
    vector<Square> moves;
    for (const auto& m : MoveList<LEGAL>(this->Position()))
    {
        if (from_sq(m) == this->PieceSquare())
        {
            moves.push_back(to_sq(m));
        }
    }
    return moves;
}
//-----------------------------------
string DPieceInfoProvider::LegalMovesString() const
{
    string str;
    vector<Square> moves = LegalMoves();
    for (auto to : moves)
    {
        str += UCI::square(to) + ", ";
    }
    str = str.substr(0, str.size()-2); // Remove trailing comma
    if (str.size())
    {
        str = "There are " + to_string(moves.size()) + " legal moves for the " + this->PieceName() + " on " + this->PieceSquareString() + ": " + str + ".";
    }
    else
    {
        str = "There are no legal moves for the " + this->PieceName() + " on " + this->PieceSquareString() + ".";
    }
    return str;
}
//-----------------------------------
vector<Square> DPieceInfoProvider::CaptureMoves() const
{
    vector<Square> moves;
    for (const auto& m : MoveList<CAPTURES>(this->Position()))
    {
        if (from_sq(m) == this->PieceSquare())
        {
            if (this->Position()->legal(m)) // Need to make sure we're not moving out of a pin or that our king is already in check
            {
                moves.push_back(to_sq(m));
            }
        }
    }
    return moves;
}
//-----------------------------------
string DPieceInfoProvider::CaptureMovesString() const
{
    string str;
    vector<Square> moves = CaptureMoves();
    for (auto to : moves)
    {
        str += UCI::square(to) + ", ";
    }
    str = str.substr(0, str.size()-2); // Remove trailing comma
    if (str.size())
    {
        str = "The " + this->PieceName() + " on " + this->PieceSquareString() + " can make " + to_string(moves.size()) + " captures: " + str + ".";
    }
    else
    {
        str = "The " + this->PieceName() + " on " + this->PieceSquareString() + " cannot capture any pieces.";
    }
    return str;
}
//-----------------------------------

} // namespace Stockfish
