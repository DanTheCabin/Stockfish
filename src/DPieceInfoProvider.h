#ifndef DPIECEINFOPROVIDER_H_INCLUDED
#define DPIECEINFOPROVIDER_H_INCLUDED


#include "movegen.h"
#include "position.h"
#include "types.h"
#include <vector>


enum Square : int;


namespace Stockfish {

class Position;

class DPieceInfoProvider {

public:
  DPieceInfoProvider(Position& position, Square pieceSquare);
  Position& Position() const;
  Square PieceSquare() const;
  std::string PieceName() const;
  std::string PieceSquareString() const;
  std::vector<Square> LegalMoves() const;
  std::string LegalMovesString() const;
  std::vector<Square> CaptureMoves() const;
  std::string CaptureMovesString() const;

private:
  Stockfish::Position& m_position;
  Stockfish::Square m_pieceSquare;

};

} // namespace Stockfish

#endif // #ifndef DPIECEINFOPROVIDER_H_INCLUDED
