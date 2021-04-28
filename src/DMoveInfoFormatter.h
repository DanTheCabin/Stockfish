#ifndef DMOVEINFOFORMATTER_H_INCLUDED
#define DMOVEINFOFORMATTER_H_INCLUDED


#include "movegen.h"
#include "position.h"
#include "types.h"
#include <vector>
#include <iostream>


namespace Stockfish {
  class Position;
  enum Square : int;
}

namespace DSpace {

class DMoveInfoFormatter {

public:
  DMoveInfoFormatter(DMoveInfoProvider& provider);
  DMoveInfoProvider& Provider() const;
  std::string MoveName() const;
  std::string PieceName(Stockfish::Square sq) const;
  std::string SquareName(Stockfish::Square sq) const;
  std::string IsSEEPositiveCaptureString() const;
  std::string CentipawnChangeString() const;
  std::string CapturesHangingPieceString() const;
  std::string SEEPositiveCapturesAfterMoveString() const;
  
private:
  DMoveInfoProvider& m_provider;


};

} // namespace DSpace

#endif // #ifndef DMOVEINFOFORMATTER_H_INCLUDED
