#include <iostream>
#include "DUci.h"
#include "DUtil.h"
#include "DSquareInfoProvider.h"
#include "DSquareInfoFormatter.h"
#include "DPositionInfoProvider.h"
#include "DPositionInfoFormatter.h"
#include "DMoveInfoProvider.h"
#include "DMoveInfoFormatter.h"
#include "position.h"
#include "movegen.h"
#include "types.h"
#include <string>
#include "uci.h"
#include "search.h"
#include "thread.h"

using namespace std;

namespace DSpace {
//-----------------------------------
void bestmoves(Stockfish::Position& pos, istringstream& is, Stockfish::StateListPtr& states)
{
    string numberOfMovesStr;
    int numberOfMoves;
    is >> numberOfMovesStr;
    string fen = pos.fen();
    try
    {
        numberOfMoves = std::stoi(numberOfMovesStr);
    }
    catch (std::exception& e)
    {
        numberOfMoves = 3;
    }

    Stockfish::Options["MultiPV"] = to_string(numberOfMoves);
    DPositionInfoProvider provider = DPositionInfoProvider(pos, states);
    DPositionInfoFormatter positionInfo = DPositionInfoFormatter(provider);
    cout << positionInfo.BestMovesString(numberOfMoves) << endl;

    // Restore state
    states = Stockfish::StateListPtr(new std::deque<Stockfish::StateInfo>(1)); // Drop old and create a new one
    pos.set(fen, Stockfish::Options["UCI_Chess960"], &states->back(), Stockfish::Threads.main());
}
//-----------------------------------
void pieceinfo(Stockfish::Position& pos, istringstream& is)
{
    string token;
    is >> token;
    Stockfish::Square square = Util::to_square(token);
    if (pos.piece_on(square) != Stockfish::NO_PIECE)
    {
        DSquareInfoProvider provider = DSquareInfoProvider(pos, square);
        DSquareInfoFormatter pieceInfo = DSquareInfoFormatter(provider);
        cout << pieceInfo.LegalMovesString() << endl;
        cout << pieceInfo.CaptureMovesString() << endl;
        cout << pieceInfo.IsPinnedString() << endl;
        cout << pieceInfo.IsHangingString() << endl;        
    }
    else 
    {
        cout << "There is no piece on that square." << endl;
    }
}
//-----------------------------------
void moveinfo(Stockfish::Position& pos, istringstream& is, Stockfish::StateListPtr& states)
{
    string token;
    is >> token;
    string off = "Off";
    Stockfish::Options["Analysis Contempt"] = off;
    Stockfish::Move move = Stockfish::UCI::to_move(pos, token);
    if (move != Stockfish::MOVE_NONE)
    {
        DMoveInfoProvider provider = DMoveInfoProvider(pos, move, states);
        DMoveInfoFormatter moveInfo = DMoveInfoFormatter(provider);
        cout << moveInfo.IsSEEPositiveCaptureString() << endl;
        cout << moveInfo.CapturesHangingPieceString() << endl;
        cout << moveInfo.SEEPositiveCapturesAfterMoveString() << endl;
        cout << moveInfo.CentipawnChangeString() << endl;
    }
    else
    {
        cout << token + " is not a legal move" << endl;
    }
}
//-----------------------------------

/// 

}
 // namespace DSpace
