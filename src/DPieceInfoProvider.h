#ifndef DPIECEINFOPROVIDER_H_INCLUDED
#define DPIECEINFOPROVIDER_H_INCLUDED


#include "movegen.h"
#include "position.h"
#include "types.h"
#include <vector>


namespace Stockfish {
  class Position;
  enum Square : int;
}

namespace DSpace {

class DPieceInfoProvider {

public:
  DPieceInfoProvider(Stockfish::Position& position, Stockfish::Square pieceSquare);
  Stockfish::Position& Position() const;
  Stockfish::Square PieceSquare() const;
  Stockfish::Piece Piece() const;
  Stockfish::Color PieceColor() const;
  std::vector<Stockfish::Square> LegalMoves() const;
  std::vector<Stockfish::Square> CaptureMoves() const;
  bool IsPinned(Stockfish::Square& by, Stockfish::Square& to) const;
  Stockfish::Square LeastValuableAttacker() const;
  bool IsHanging(Stockfish::Square& by) const;

private:
  Stockfish::Position& m_position;
  Stockfish::Square m_pieceSquare;

  Stockfish::Square ActivePinner(Stockfish::Bitboard snipers) const;
  bool IsPinnedToSquare(Stockfish::Square attackedSq, Stockfish::Square& by, Stockfish::Square& to) const;

};

} // namespace DSpace

#endif // #ifndef DPIECEINFOPROVIDER_H_INCLUDED
