#ifndef DPOSITIONINFOFORMATTER_H_INCLUDED
#define DPOSITIONINFOFORMATTER_H_INCLUDED


#include "movegen.h"
#include "position.h"
#include "types.h"
#include <vector>


namespace Stockfish {
  class Position;
  enum Square : int;
}

namespace DSpace {

class DPositionInfoFormatter {

public:
  DPositionInfoFormatter(DPositionInfoProvider& provider);
  std::string BestMovesString(int numberOfMoves);
  DPositionInfoProvider& Provider() const;
  
private:
  DPositionInfoProvider& m_provider;


};

} // namespace DSpace

#endif // #ifndef DPOSITIONINFOFORMATTER_H_INCLUDED
