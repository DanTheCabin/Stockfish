#include <iostream>
#include "DPositionInfoProvider.h"
#include "position.h"
#include "search.h"
#include "bitboard.h"
#include "movegen.h"
#include "types.h"
#include "thread.h"
#include "uci.h"
#include <vector>

using namespace std;

namespace DSpace {


//-----------------------------------
DPositionInfoProvider::DPositionInfoProvider(Stockfish::Position& position, Stockfish::StateListPtr& states)
    : m_position(position), m_states(states) 
{
}
//-----------------------------------
Stockfish::Position& DPositionInfoProvider::Position() const
{
    return m_position;
}
//-----------------------------------
Stockfish::StateListPtr& DPositionInfoProvider::States() const
{
    return m_states;
}
//-----------------------------------
vector<Stockfish::Move> DPositionInfoProvider::BestMoves(int numberOfMoves)
{
    vector<Stockfish::Move> bestmoves;

    Stockfish::Search::LimitsType limits;
    limits.startTime = Stockfish::now();
    limits.movetime = 4000;

    Stockfish::Threads.start_thinking(Position(), States(), limits, false);
    Stockfish::Threads.main()->wait_for_search_finished();

    vector<Stockfish::Search::RootMove> rootMoves = Stockfish::Threads.main()->rootMoves;
    std::for_each(rootMoves.begin(), rootMoves.begin() + numberOfMoves,
        [&] (Stockfish::Search::RootMove& element) { bestmoves.push_back(element.pv[0]); } );

    return bestmoves;
}
//-----------------------------------
string DPositionInfoProvider::BestMovesString(int numberOfMoves)
{
    string str = "The best " + to_string(numberOfMoves) + " moves are:";
    vector<Stockfish::Move> bestmoves = BestMoves(numberOfMoves);
    for (size_t i=0; i < bestmoves.size(); ++i)
    {
        Stockfish::Move m = bestmoves[i];
        str += " " + to_string(i+1) + ". " + Stockfish::UCI::move(m, false);
    }
    return str;
}

} // namespace DSpace
