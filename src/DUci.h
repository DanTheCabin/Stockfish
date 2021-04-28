#ifndef DUCI_H_INCLUDED
#define DUCI_H_INCLUDED

#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include "position.h"

namespace Stockfish {
  class Position;
  enum Square : int;
}

namespace DSpace
{
	void pieceinfo(Stockfish::Position& pos, std::istringstream& is);
	void moveinfo(Stockfish::Position& pos, std::istringstream& is, Stockfish::StateListPtr& states);
	void bestmoves(Stockfish::Position& pos, std::istringstream& is, Stockfish::StateListPtr& states);
}


#endif // #ifndef DUCI_H_INCLUDED