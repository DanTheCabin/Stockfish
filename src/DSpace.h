#ifndef DSPACE_H_INCLUDED
#define DSPACE_H_INCLUDED

#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include "position.h"

namespace Stockfish {
  class Position;
  enum Square : int;
}

namespace DSpace {
	Stockfish::Square to_square(std::string sqStr);	
	void pieceinfo(Stockfish::Position& pos, std::istringstream& is);
	void moveinfo(Stockfish::Position& pos, std::istringstream& is, Stockfish::StateListPtr& states);
	void bestmoves(Stockfish::Position& pos, std::istringstream& is, Stockfish::StateListPtr& states);
	}


#endif // #ifndef DSPACE_H_INCLUDED