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

class DSquareInfoProvider;

class DMoveInfoProvider {

public:
  DMoveInfoProvider(
  	Stockfish::Position& position, 
  	Stockfish::Move move, 
  	Stockfish::StateListPtr& states,
  	DSquareInfoProvider& toProvider
  );
  Stockfish::Position& Position() const;
  Stockfish::Move Move() const;
  Stockfish::StateListPtr& States() const;
  DSquareInfoProvider ToSquare() const;
  double CentipawnChange() const;
  bool IsCapture() const;
  bool IsSEEPositiveCapture() const;
  bool CapturesHangingPiece() const;
  
private:
  Stockfish::Position& m_position;
  Stockfish::Move m_move;
  Stockfish::StateListPtr& m_states;
  DSquareInfoProvider& m_toProvider;

};

} // namespace DSpace

#endif // #ifndef DMOVEINFOPROVIDER_H_INCLUDED
