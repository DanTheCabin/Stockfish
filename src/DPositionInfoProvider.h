#ifndef DPOSITIONINFOPROVIDER_H_INCLUDED
#define DPOSITIONINFOPROVIDER_H_INCLUDED


#include "movegen.h"
#include "position.h"
#include "types.h"
#include <vector>


namespace Stockfish {
  class Position;
  enum Square : int;
}

namespace DSpace {

class DPositionInfoProvider {

public:
  DPositionInfoProvider(Stockfish::Position& position, Stockfish::StateListPtr& states);
  Stockfish::Position& Position() const;
  Stockfish::StateListPtr& States() const;
  std::vector<Stockfish::Move> BestMoves(int numberOfMoves);
  std::string BestMovesString(int numberOfMoves);
  
private:
  Stockfish::Position& m_position;
  Stockfish::StateListPtr& m_states;


};

} // namespace DSpace

#endif // #ifndef DPOSITIONINFOPROVIDER_H_INCLUDED
