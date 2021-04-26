#ifndef DSPACE_H_INCLUDED
#define DSPACE_H_INCLUDED

#include <iostream>
#include <sstream>
#include <string>
#include <utility>

namespace Stockfish {

class Position;
enum Square : int;

namespace DSpace {
	void pieceinfo(Position& pos, std::istringstream& is);
	Square to_square(std::string sqStr);

}

}

#endif // #ifndef DSPACE_H_INCLUDED