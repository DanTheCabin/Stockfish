#ifndef DMOVEINFOPROVIDER_H_INCLUDED
#define DMOVEINFOPROVIDER_H_INCLUDED


#include "movegen.h"
#include "position.h"
#include "types.h"
#include <vector>


namespace Stockfish {
  class Position;
  enum Square : int;
}

namespace DSpace {

class DMoveInfoProvider {

public:
  DMoveInfoProvider(Stockfish::Position& position, Stockfish::Move move);
  bool IsCapture() const;
  bool IsSEEPositiveCapture() const;
  Stockfish::Position& Position() const;
  Stockfish::Move Move() const;
  
private:
  Stockfish::Position& m_position;
  Stockfish::Move m_move;


};

} // namespace DSpace

#endif // #ifndef DMOVEINFOPROVIDER_H_INCLUDED
