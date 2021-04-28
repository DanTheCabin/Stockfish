#include <iostream>
#include "position.h"
#include "movegen.h"
#include "types.h"
#include <string>
#include "uci.h"
#include "search.h"
#include "thread.h"
#include "DUtil.h"

using namespace std;

namespace DSpace {
//-----------------------------------
namespace Util 
{
// Square to_square(string): Helper function that would actually belong in uci.cpp where "to_move" is a related function, but I put it here for separation
//-----------------------------------
Stockfish::Square to_square(string sqStr)
{
    Stockfish::Square sq = Stockfish::SQ_NONE;
    for (Stockfish::Square sqi = Stockfish::SQ_A1; sqi <= Stockfish::SQ_H8; ++sqi)
    {
        if (sqStr == Stockfish::UCI::square(sqi))
        {
            sq = sqi;
            break;
        }
    }
    return sq;
}
//----------------------------------
string square(Stockfish::Square s) {
  return string{ char('a' + Stockfish::file_of(s)), char('1' + Stockfish::rank_of(s)) };
}
//----------------------------------
Stockfish::Move to_move(const Stockfish::Position& pos, string str) {

  if (str.length() == 5) 
      str[4] = char(tolower(str[4]));

  for (const auto& m : Stockfish::MoveList<Stockfish::LEGAL>(pos))
      if (str == from_move(m))
          return m;

  return Stockfish::MOVE_NONE;
}
//----------------------------------
string from_move(Stockfish::Move m) {

  Stockfish::Square from = Stockfish::from_sq(m);
  Stockfish::Square to = Stockfish::to_sq(m);

  if (m == Stockfish::MOVE_NONE)
      return "(none)";

  if (m == Stockfish::MOVE_NULL)
      return "0000";

  if (Stockfish::type_of(m) == Stockfish::CASTLING)
      to = Stockfish::make_square(to > from ? Stockfish::FILE_G : Stockfish::FILE_C, Stockfish::rank_of(from));

  string move = square(from) + square(to);

  if (Stockfish::type_of(m) == Stockfish::PROMOTION)
      move += " pnbrqk"[Stockfish::promotion_type(m)];

  return move;
}
//----------------------------------------
string piece_name_on(Stockfish::Position& pos, Stockfish::Square sq)
{
    const string pieceNames[7] = {"", "pawn", "knight", "bishop", "rook", "queen", "king"};
    return pieceNames[Stockfish::type_of(pos.piece_on(sq))];
}
//----------------------------------
vector<Stockfish::Move> all_see_positive_captures(Stockfish::Position& pos)
{
    vector<Stockfish::Move> moves;
    for (const auto& m : Stockfish::MoveList<Stockfish::CAPTURES>(pos))
    {
        if (pos.legal(m) && pos.see_ge(m, Stockfish::Value(1)))
        {
            moves.push_back(m);
        }
    }
    return moves;
}
} // namespace Util

/// 

}
 // namespace DSpace
