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
        throw std::runtime_error("There is no piece on " + PieceSquareName());
    }
}
//-----------------------------------
Stockfish::Position& DPieceInfoProvider::Position() const
{
    return m_position;
}
//-----------------------------------
Stockfish::Square DPieceInfoProvider::PieceSquare() const
{
    return m_pieceSquare;
}
//-----------------------------------
string DPieceInfoProvider::PieceSquareName() const
{
    return Stockfish::UCI::square(m_pieceSquare);
}
//-----------------------------------
Stockfish::Color DPieceInfoProvider::PieceColor() const
{
    return Stockfish::color_of(Piece());
}
//-----------------------------------
Stockfish::Piece DPieceInfoProvider::Piece() const
{
    return Position().piece_on(PieceSquare());
}
//-----------------------------------
string DPieceInfoProvider::PieceNameOn(Stockfish::Square sq) const
{
    return pieceNames[type_of(Position().piece_on(sq))];
}
//-----------------------------------
string DPieceInfoProvider::PieceName() const
{
    return PieceNameOn(PieceSquare());
}
//-----------------------------------
vector<Stockfish::Square> DPieceInfoProvider::LegalMoves() const
{
    vector<Stockfish::Square> moves;
    for (const auto& m : Stockfish::MoveList<Stockfish::LEGAL>(Position()))
    {
        if (Stockfish::from_sq(m) == PieceSquare())
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
    if (PieceColor() != Position().side_to_move())
    {
        string colorStr = PieceColor() == Stockfish::WHITE ? "white" : "black"; 
        str = "The " + colorStr + " " + PieceName() + " on " + PieceSquareName() + 
            " has no legal moves since it is not " + colorStr + "'s turn to move.";
    }
    else
    {
        vector<Stockfish::Square> moves = LegalMoves();
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
vector<Stockfish::Square> DPieceInfoProvider::CaptureMoves() const
{
    vector<Stockfish::Square> moves;
    for (const auto& m : Stockfish::MoveList<Stockfish::CAPTURES>(Position()))
    {
        if (Stockfish::from_sq(m) == PieceSquare())
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
string DPieceInfoProvider::CaptureMovesString() const
{
    string str;
    if (PieceColor() != Position().side_to_move())
    {
        string colorStr = PieceColor() == Stockfish::WHITE ? "white" : "black"; 
        str = "The " + colorStr + " " + PieceName() + " on " + PieceSquareName() + 
            " has no capture moves since it is not " + colorStr + "'s turn to move.";
    }
    else
    {
        vector<Stockfish::Square> moves = CaptureMoves();
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
Stockfish::Square DPieceInfoProvider::ActivePinner(Stockfish::Bitboard snipers) const
{
    Stockfish::Square sniperSq = Stockfish::SQ_NONE;
    assert(snipers);
    do
    {
        sniperSq = Stockfish::pop_lsb(snipers);
    }
    while (!Position().legal(make_move(sniperSq, PieceSquare())));
    return sniperSq;
}
//-----------------------------------
bool DPieceInfoProvider::IsPinnedToSquare(Stockfish::Square attackedSq, Stockfish::Square& by, Stockfish::Square& to) const
{
    bool isPinned = false;
    Stockfish::Square pinnedSq = PieceSquare();
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
bool DPieceInfoProvider::IsPinned(Stockfish::Square& by, Stockfish::Square& to) const
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
string DPieceInfoProvider::IsPinnedString() const
{
    string str = "The " + PieceName() + " on " + PieceSquareName();
    Stockfish::Square by = Stockfish::SQ_NONE;
    Stockfish::Square to = Stockfish::SQ_NONE;
    bool isPinned = IsPinned(by, to);
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
Stockfish::Square DPieceInfoProvider::LeastValuableAttacker() const
{
    Stockfish::Square lva = Stockfish::SQ_NONE;
    Stockfish::Bitboard attackers = Position().attackers_to(PieceSquare()) &
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
    return lva;
}
//-----------------------------------
bool DPieceInfoProvider::IsHanging(Stockfish::Square& leastValuableAttacker) const
{
    bool isHanging = false;
    leastValuableAttacker = LeastValuableAttacker();
    if (leastValuableAttacker != Stockfish::SQ_NONE)
    {
        isHanging = Position().see_ge(make_move(leastValuableAttacker, PieceSquare()), Stockfish::Value(1));
    }
    return isHanging;
}
//-----------------------------------
string DPieceInfoProvider::IsHangingString() const
{
    string str = "The " + PieceName() + " on " + PieceSquareName();
    Stockfish::Square leastValuableAttacker = Stockfish::SQ_NONE;
    bool isHanging = IsHanging(leastValuableAttacker);
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
