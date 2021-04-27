#ifndef DPIECEINFOFORMATTER_H_INCLUDED
#define DPIECEINFOFORMATTER_H_INCLUDED


#include "movegen.h"
#include "position.h"
#include "types.h"
#include <vector>

class DPieceInfoProvider;

namespace Stockfish {
  class Position;
  enum Square : int;
}

namespace DSpace {

class DPieceInfoFormatter {

public:
  DPieceInfoFormatter(DPieceInfoProvider& provider);
  DPieceInfoProvider& Provider() const;
  Stockfish::Position& Position() const;
  std::string PieceSquareName() const;
  std::string PieceName() const;
  std::string PieceNameOn(Stockfish::Square sq) const;
  std::string PieceColorName() const;
  std::string LegalMovesString() const;
  std::string CaptureMovesString() const;
  std::string IsPinnedString() const;
  std::string IsHangingString() const;

private:
  DPieceInfoProvider& m_provider;

};

} // namespace DSpace

#endif // #ifndef DPIECEINFOFORMATTER_H_INCLUDED
