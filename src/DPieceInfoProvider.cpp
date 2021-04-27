#include <iostream>
#include "DPieceInfoProvider.h"
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
DPieceInfoProvider::DPieceInfoProvider(Stockfish::Position& position, Stockfish::Square pieceSquare)
    : m_position(position), m_pieceSquare(pieceSquare) 
{
    if (m_position.piece_on(m_pieceSquare) == Stockfish::NO_PIECE)
    {
        throw std::runtime_error("There is no piece on " + this->PieceSquareString());
    }
    Stockfish::Color color = Stockfish::color_of(m_position.piece_on(m_pieceSquare));
    if (color != m_position.side_to_move())
    {
        string colorStr = color == Stockfish::WHITE ? "white" : "black"; 
        throw std::runtime_error("There is a " + colorStr + " piece on " + this->PieceSquareString() + " but it is not " + colorStr + "'s turn to move." );
    }
}
//-----------------------------------
Stockfish::Position& DPieceInfoProvider::Position() const
{
    return this->m_position;
}
//-----------------------------------
Stockfish::Square DPieceInfoProvider::PieceSquare() const
{
    return this->m_pieceSquare;
}
//-----------------------------------
string DPieceInfoProvider::PieceSquareString() const
{
    return Stockfish::UCI::square(this->m_pieceSquare);
}
//-----------------------------------
string DPieceInfoProvider::PieceNameOn(Stockfish::Square sq) const
{
    return pieceNames[type_of(this->Position().piece_on(sq))];
}
//-----------------------------------
string DPieceInfoProvider::PieceName() const
{
    return this->PieceNameOn(this->PieceSquare());
}
//-----------------------------------
vector<Stockfish::Square> DPieceInfoProvider::LegalMoves() const
{
    vector<Stockfish::Square> moves;
    for (const auto& m : Stockfish::MoveList<Stockfish::LEGAL>(this->Position()))
    {
        if (Stockfish::from_sq(m) == this->PieceSquare())
        {
            moves.push_back(Stockfish::to_sq(m));
        }
    }
    return moves;
}
//-----------------------------------
string DPieceInfoProvider::LegalMovesString() const
{
    string str;
    vector<Stockfish::Square> moves = LegalMoves();
    for (auto to : moves)
    {
        str += Stockfish::UCI::square(to) + ", ";
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
vector<Stockfish::Square> DPieceInfoProvider::CaptureMoves() const
{
    vector<Stockfish::Square> moves;
    for (const auto& m : Stockfish::MoveList<Stockfish::CAPTURES>(this->Position()))
    {
        if (Stockfish::from_sq(m) == this->PieceSquare())
        {
            if (this->Position().legal(m)) // Need to make sure we're not moving out of a king pin or that our king is already in check
            {
                moves.push_back(Stockfish::to_sq(m));
            }
        }
    }
    return moves;
}
//-----------------------------------
string DPieceInfoProvider::CaptureMovesString() const
{
    string str;
    vector<Stockfish::Square> moves = CaptureMoves();
    for (auto to : moves)
    {
        str += Stockfish::UCI::square(to) + ", ";
    }
    str = str.substr(0, str.size()-2); // Remove trailing comma
    if (str.size())
    {
        str = "The " + this->PieceName() + " on " + this->PieceSquareString() + " can make " + to_string(moves.size()) + " capture(s): " + str + ".";
    }
    else
    {
        str = "The " + this->PieceName() + " on " + this->PieceSquareString() + " cannot capture any pieces.";
    }
    return str;
}
//-----------------------------------
Stockfish::Square DPieceInfoProvider::ActivePinner(Stockfish::Bitboard snipers) const
{
    Stockfish::Square sniperSq = Stockfish::SQ_NONE;
    assert(snipers);
    do
    {
        sniperSq = Stockfish::pop_lsb(snipers);
    }
    while (!this->Position().legal(make_move(sniperSq, this->PieceSquare())));
    return sniperSq;
}
//-----------------------------------
bool DPieceInfoProvider::IsPinnedToSquare(Stockfish::Square attackedSq, Stockfish::Square& by, Stockfish::Square& to) const
{
    bool isPinned = false;
    Stockfish::Square pinnedSq = this->PieceSquare();
    Stockfish::Position& pos = this->Position();
    Stockfish::Color pieceColor = Stockfish::color_of(pos.piece_on(pinnedSq));
    Stockfish::Bitboard pinners = 0;
    if (!pos.empty(attackedSq) && Stockfish::color_of(pos.piece_on(attackedSq)) == pieceColor)
    {
        Stockfish::Bitboard blockers = pos.slider_blockers(pos.pieces(~pieceColor), attackedSq, pinners);
        if (pinnedSq & blockers)
        {
            Stockfish::Square pinner = this->ActivePinner(pinners); 
            to = attackedSq;
            by = pinner;
            if (this->Position().square<Stockfish::KING>(pieceColor) == attackedSq)
            {
                isPinned = true;
            }
            else
            {
                if (pinner != Stockfish::SQ_NONE) // If false, it means there's a pinner but that cannot legally make the capture
                {
                    Stockfish::Piece pinnedPiece = pos.piece_on(pinnedSq);
                    pos.remove_piece(pinnedSq); /* Temporarily remove pinned piece to check SEE. 
                                                 Ignores the fact that the pinned could be 
                                                 a knight that provides defense to the attacked piece
                                                 after moving out of the pin but I had to time cap.*/
                    isPinned = pos.see_ge(make_move(pinner, attackedSq), Stockfish::Value(1));
                    pos.put_piece(pinnedPiece, pinnedSq); 
                }
            }
        }
    }
    return isPinned;
}
//-----------------------------------
bool DPieceInfoProvider::IsPinned(Stockfish::Square& by, Stockfish::Square& to) const
{
    bool isPinned = false;
    Stockfish::Square sq = Stockfish::SQ_A1;
    while (!isPinned && sq <= Stockfish::SQ_H8)
    {
        isPinned = this->IsPinnedToSquare(sq, by, to);
        ++sq;
    }
    return isPinned;
}
//-----------------------------------
string DPieceInfoProvider::IsPinnedString() const
{
    string str = "The " + this->PieceName() + " on " + this->PieceSquareString();
    Stockfish::Square by = Stockfish::SQ_NONE;
    Stockfish::Square to = Stockfish::SQ_NONE;
    bool isPinned = this->IsPinned(by, to);
    if (isPinned)
    {
        string toPieceStr = this->PieceNameOn(to);
        string byPieceStr = this->PieceNameOn(by);
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


} // namespace DSpace
