#include "../include/board.h"
#include "../include/movegen.h"

int evaluatePosition(const GameState& board){
    int whiteValue = __builtin_popcountll(board.whitePawns) * 100 + __builtin_popcountll(board.whiteKnights) * 300 + 
    __builtin_popcountll(board.whiteBishops) * 300 + __builtin_popcountll(board.whiteRooks) * 500 +
    __builtin_popcountll(board.whiteQueens) * 900;
    int blackValue = __builtin_popcountll(board.blackPawns) * 100 + __builtin_popcountll(board.blackKnights) * 300 +
    __builtin_popcountll(board.blackBishops) * 300 + __builtin_popcountll(board.blackRooks) * 500 + 
    __builtin_popcountll(board.blackQueens) * 900;
    return board.whiteToMove ? whiteValue - blackValue : blackValue - whiteValue;
}

int evaluateNoMoves(const GameState& board){
    uint64_t kingSquare = board.whiteToMove ? board.whiteKing : board.blackKing;
    if (kingSquare & attackedSquares(board, !board.whiteToMove)){
        return -100000;
    }
    return 0;
}
