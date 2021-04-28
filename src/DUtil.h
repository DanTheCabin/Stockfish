#ifndef DUTIL_H_INCLUDED
#define DUTIL_H_INCLUDED

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
	namespace Util
	{
		Stockfish::Square to_square(std::string sqStr);	
		std::string piece_name_on(Stockfish::Position& pos, Stockfish::Square sq);
		std::vector<Stockfish::Move> all_see_positive_captures(Stockfish::Position& pos);
	}
}


#endif // #ifndef DUTIL_H_INCLUDED