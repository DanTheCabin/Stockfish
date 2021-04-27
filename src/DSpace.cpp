#include <iostream>
#include "DSpace.h"
#include "DPieceInfoProvider.h"
#include "position.h"
#include "movegen.h"
#include "types.h"
#include <string>
#include "uci.h"

using namespace std;

namespace DSpace {
//-----------------------------------
void pieceinfo(Stockfish::Position& pos, istringstream& is)
{
    string token;
    is >> token;
    Stockfish::Square pieceSquare = to_square(token);
    try
    {
        DPieceInfoProvider pieceInfo = DPieceInfoProvider(pos, pieceSquare);
        cout << pieceInfo.LegalMovesString() << endl;
        cout << pieceInfo.CaptureMovesString() << endl;
        cout << pieceInfo.IsPinnedString() << endl;
        cout << pieceInfo.IsHangingString() << endl;        
    }
    catch (const std::runtime_error& e)
    {
        cout << e.what() << endl;
    }
}
//-----------------------------------
// Helper function that would actually belong in uci.cpp where "to_move" is a related function, but I put it here for separation
Stockfish::Square to_square(string sqStr)
{
    Stockfish::Square sq = Stockfish::SQ_NONE;
    for (Stockfish::Square sqi = Stockfish::SQ_A1; sqi <= Stockfish::SQ_H8; ++sqi)
    {
        if (sqStr == Stockfish::UCI::square(sqi))
        {
            sq = sqi;
            break;
        }
    }
    return sq;
}
//-----------------------------------
/// 

}
 // namespace DSpace
