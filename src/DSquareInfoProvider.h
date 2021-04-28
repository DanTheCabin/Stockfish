#ifndef DSQUAREINFOPROVIDER_H_INCLUDED
#define DSQUAREINFOPROVIDER_H_INCLUDED


#include "movegen.h"
#include "position.h"
#include "types.h"
#include <vector>


namespace Stockfish {
  class Position;
  enum Square : int;
}

namespace DSpace {

class DSquareInfoProvider {

public:
  DSquareInfoProvider(Stockfish::Position& position, Stockfish::Square pieceSquare);
  Stockfish::Position& Position() const;
  Stockfish::Square Square() const;
  Stockfish::Piece Piece() const;
  Stockfish::Color PieceColor() const;
  std::vector<Stockfish::Move> LegalMoves() const;
  std::vector<Stockfish::Move> CaptureMoves() const;
  bool IsPinned(Stockfish::Square& by, Stockfish::Square& to) const;
  Stockfish::Square LeastValuableAttacker() const;
  bool IsHanging() const;

private:
  Stockfish::Position& m_position;
  Stockfish::Square m_square;

  Stockfish::Square ActivePinner(Stockfish::Bitboard snipers) const;
  bool IsPinnedToSquare(Stockfish::Square attackedSq, Stockfish::Square& by, Stockfish::Square& to) const;

};

} // namespace DSpace

#endif // #ifndef DSQUAREINFOPROVIDER_H_INCLUDED
