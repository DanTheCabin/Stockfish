#ifndef DSQUAREINFOFORMATTER_H_INCLUDED
#define DSQUAREINFOFORMATTER_H_INCLUDED


#include "movegen.h"
#include "position.h"
#include "types.h"
#include <vector>

class DSquareInfoProvider;

namespace Stockfish {
  class Position;
  enum Square : int;
}

namespace DSpace {

class DSquareInfoFormatter {

public:
  DSquareInfoFormatter(DSquareInfoProvider& provider);
  DSquareInfoProvider& Provider() const;
  Stockfish::Position& Position() const;
  std::string SquareName() const;
  std::string PieceName() const;
  std::string PieceNameOn(Stockfish::Square sq) const;
  std::string PieceColorName() const;
  std::string LegalMovesString() const;
  std::string CaptureMovesString() const;
  std::string IsPinnedString() const;
  std::string IsHangingString() const;

private:
  DSquareInfoProvider& m_provider;

};

} // namespace DSpace

#endif // #ifndef DSQUAREINFOFORMATTER_H_INCLUDED
