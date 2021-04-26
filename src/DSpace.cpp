#include <iostream>
#include "DSpace.h"
#include "DPieceInfoProvider.h"
#include "position.h"
#include "movegen.h"
#include "types.h"
#include <string>
#include "uci.h"

using namespace std;

namespace Stockfish {

namespace DSpace {
//-----------------------------------
void pieceinfo(Position& pos, istringstream& is)
{
    string token;
    is >> token;
    Square pieceSquare = to_square(token);
    try
    {
        DPieceInfoProvider pieceInfo = DPieceInfoProvider(pos, pieceSquare);
        cout << pieceInfo.LegalMovesString() << endl;
        cout << pieceInfo.CaptureMovesString() << endl;
    }
    catch (const invalid_argument& e)
    {
        cout << e.what() << endl;
    }
}
//-----------------------------------
Square to_square(string sqStr)
{
    Square sq = SQ_NONE;
    for (sq = SQ_A1; sq != SQ_NONE; sq=(Square)(sq+1))
      if (sqStr == UCI::square(sq))
        return sq;
    return sq;
}
//-----------------------------------
}

} // namespace Stockfish
