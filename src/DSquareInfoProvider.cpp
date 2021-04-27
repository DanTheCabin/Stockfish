#include <iostream>
#include "DSquareInfoProvider.h"
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
DSquareInfoProvider::DSquareInfoProvider(Stockfish::Position& position, Stockfish::Square pieceSquare)
    : m_position(position), m_square(pieceSquare) 
{
}
//-----------------------------------
Stockfish::Position& DSquareInfoProvider::Position() const
{
    return m_position;
}
//-----------------------------------
Stockfish::Square DSquareInfoProvider::Square() const
{
    return m_square;
}
//-----------------------------------
Stockfish::Color DSquareInfoProvider::PieceColor() const
{
    return Stockfish::color_of(Piece());
}
//-----------------------------------
Stockfish::Piece DSquareInfoProvider::Piece() const
{
    return Position().piece_on(Square());
}
//-----------------------------------
vector<Stockfish::Square> DSquareInfoProvider::LegalMoves() const
{
    vector<Stockfish::Square> moves;
    for (const auto& m : Stockfish::MoveList<Stockfish::LEGAL>(Position()))
    {
        if (Stockfish::from_sq(m) == Square())
        {
            moves.push_back(Stockfish::to_sq(m));
        }
    }
    return moves;
}
//-----------------------------------
vector<Stockfish::Square> DSquareInfoProvider::CaptureMoves() const
{
    vector<Stockfish::Square> moves;
    for (const auto& m : Stockfish::MoveList<Stockfish::CAPTURES>(Position()))
    {
        if (Stockfish::from_sq(m) == Square())
        {
            if (Position().legal(m)) // Need to make sure we're not moving out of a king pin or that our king is already in check
            {
                moves.push_back(Stockfish::to_sq(m));
            }
        }
    }
    return moves;
}
//-----------------------------------
Stockfish::Square DSquareInfoProvider::ActivePinner(Stockfish::Bitboard snipers) const
{
    Stockfish::Square sniperSq = Stockfish::SQ_NONE;
    assert(snipers);
    do
    {
        sniperSq = Stockfish::pop_lsb(snipers);
    }
    while (!Position().legal(make_move(sniperSq, Square())));
    return sniperSq;
}
//-----------------------------------
bool DSquareInfoProvider::IsPinnedToSquare(Stockfish::Square attackedSq, Stockfish::Square& by, Stockfish::Square& to) const
{
    bool isPinned = false;
    Stockfish::Square pinnedSq = Square();
    Stockfish::Position& pos = Position();
    Stockfish::Color pieceColor = Stockfish::color_of(pos.piece_on(pinnedSq));
    Stockfish::Bitboard pinners = 0;
    if (!pos.empty(attackedSq) && Stockfish::color_of(pos.piece_on(attackedSq)) == pieceColor)
    {
        Stockfish::Bitboard blockers = pos.slider_blockers(pos.pieces(~pieceColor), attackedSq, pinners);
        if (pinnedSq & blockers)
        {
            Stockfish::Square pinner = ActivePinner(pinners); 
            to = attackedSq;
            by = pinner;
            if (Position().square<Stockfish::KING>(pieceColor) == attackedSq)
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
bool DSquareInfoProvider::IsPinned(Stockfish::Square& by, Stockfish::Square& to) const
{
    bool isPinned = false;
    Stockfish::Square sq = Stockfish::SQ_A1;
    while (!isPinned && sq <= Stockfish::SQ_H8)
    {
        isPinned = IsPinnedToSquare(sq, by, to);
        ++sq;
    }
    return isPinned;
}
//-----------------------------------
Stockfish::Square DSquareInfoProvider::LeastValuableAttacker() const
{
    Stockfish::Square lva = Stockfish::SQ_NONE;
    if (Piece() != Stockfish::NO_PIECE)
    {
        Stockfish::Bitboard attackers = Position().attackers_to(Square()) &
        Position().pieces(~PieceColor());
        if (attackers)
        {
            Stockfish::Bitboard bb;
            if ((bb = (attackers & Position().pieces(Stockfish::PAWN))))
            {
                lva = Stockfish::pop_lsb(bb);
            }
            else if ((bb = (attackers & Position().pieces(Stockfish::KNIGHT))))
            {
                lva = Stockfish::pop_lsb(bb);
            }
            else if ((bb = (attackers & Position().pieces(Stockfish::BISHOP))))
            {
                lva = Stockfish::pop_lsb(bb);
            }
            else if ((bb = (attackers & Position().pieces(Stockfish::ROOK))))
            {
                lva = Stockfish::pop_lsb(bb);
            }
            else if ((bb = (attackers & Position().pieces(Stockfish::QUEEN))))
            {
                lva = Stockfish::pop_lsb(bb);
            }
            else
            {
                lva = Position().square<Stockfish::KING>(~PieceColor());
            }
        }
    }
    
    return lva;
}
//-----------------------------------
bool DSquareInfoProvider::IsHanging() const
{
    bool isHanging = false;
    Stockfish::Square leastValuableAttacker = LeastValuableAttacker();
    if (leastValuableAttacker != Stockfish::SQ_NONE)
    {
        isHanging = Position().see_ge(make_move(leastValuableAttacker, Square()), Stockfish::Value(1));
    }
    return isHanging;
}
//-----------------------------------


} // namespace DSpace
