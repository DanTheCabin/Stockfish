
Build using the MakeFile in src/.

Launch the binary.and enter a position at the command line (example: position fen r3k2r/1pp3p1/p3pn1p/8/1N3N2/P4BP1/1P1K1P1P/7R b kq - 0 18)

Then use any of these three new "uci" commands:

pieceinfo [square] (example: pieceinfo e2) 
    gives information about the piece (if any) on the given square. information includes:
        - legal moves 
        - captures
        - if the piece is SEE-negatively pinned (not only to the king)
        - if the piece is SEE-negatively hanging

moveinfo [move] (example: moveinfo e2e3)
    gives information about the given move. information includes whether the move:
        - is legal
        - is a SEE-positive capture
        - captures a SEE-negatively hanging piece
        - allows any SEE-positive (negative for side that made the move) captures (and if so which ones)
    the number of centipawns lost is also given but likely incorrect.

bestmoves [N] (example: bestmoves 5)
    gives the N best moves (note: does spam the log while searching, not the most beautiful solution, but seemingly works!).


Example case:

position fen position fen rn1qkb2/pppbpp1r/5np1/1N1p3p/B2P1N2/4PQ2/PPP2PPP/R1B1K2R b KQq - 11 12
moveinfo h5h4
    The move is not a SEE-positive capture.
    The move does not capture a hanging piece.
    The move allows 1 SEE-positive capture moves:
        The knight on f4 captures the pawn on d5.
pieceinfo d5
    There are no legal moves for the pawn on d5.
    The pawn on d5 cannot capture any pieces.
    The pawn on d5 is pinned to the pawn on b7 by the queen on f3.
    The pawn on d5 is hanging as it can be SEE-negatively caputred by the knight on f4.


Comments:
    I tried to understand and use existing Stockfish code to the best of my ability, but likely some things could have been done more efficiently or in a more stringent fashion. 

    The choice of class names, structure, and separation of concern is questionable, but had to be accepted because of time constraints. Some of the functionality added would have fit better in already existing Stockfish classes, but I wanted to avoid changing existing code more than necessary.
    The division into Providers and Formatters was a small attempt at separating logics from viewing; the Providers doing the business logics and the Formatters representing some rudimentary form of viewers that deals with producing nice messages for user. 

    Time constraints did not allow for rigorous testing, mostly "happy path" cases have been tried. 

    Namespace "Util" has some overlapping code with existing Stockfish code, necessary to avoid cyclic includes. 

    While the DSquareInfoProvider::IsPinned method finds pins that are not just to the king, it does not consider the fact
    that a pinned knight could, after moving, become a defender of the piece it is pinned to. It also doesn't consider 
    potential checks possible by the pinned piece in order to escape the pin. 

Files edited:
    uci.cpp
    position.h (a terrible act of making two private methods public)
    MakeFile
Files containing my code:
    DUci.cpp (.h)
    DUtil.cpp (.h)
    DPositionInfoProvider.cpp (.h)
    DPositionInfoFormatter.cpp (.h)
    DMoveInfoProvider.cpp (.h)
    DMoveInfoFormatter.cpp (.h)
    DSquareInfoProvider.cpp (.h)
    DSquareInfoFormatter.cpp (.h) 