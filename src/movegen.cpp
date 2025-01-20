#include "../include/movegen.h"
#include "../include/board.h"
#include "../include/game.h"
#include "../include/utils.h"

#include <cstdint>
#include <vector> 

std::vector<uint16_t> getPlayerMoves(GameState board){
    std::vector<uint16_t> moves;

    if (board.whiteToMove){
        uint64_t knightsCopy = board.whiteKnights;
        while (knightsCopy){
            int p = __builtin_ctzll(knightsCopy);
            uint64_t destinations = pieceAttacks[1][p];
            destinations &= ~board.whitePieces;
            while (destinations){
                int q = __builtin_ctzll(destinations);
                int start = p & 0x3F;
                int target = q & 0x3F;
                uint16_t move = (start << 10) | (target << 4) | 0x0;
                moves.push_back(move);
                destinations &= destinations - 1; // kan skrives som for-lokke 
            }
            knightsCopy &= knightsCopy - 1;
        }

    }

    return moves;

}

uint64_t slidingPieceAttacks(GameState board, int piece, int square){ 
    uint64_t ts = pieceAttacks[piece][square];
    for (uint64_t bb = board.occupiedSquares & blockers[piece][square]; bb != 0; bb &= (bb - 1)){
        int target = __builtin_ctzll(bb);
        ts &= ~behind[square][target];
    }
    return ts;
}

uint64_t whiteSinglePawnPushTargets(GameState board){
    return (board.whitePawns << 8) & ~board.occupiedSquares;
}

uint64_t whiteDoublePawnPushTargets(GameState board){
    const uint64_t rank4 = 0x00000000FF000000;
    uint64_t singlePushes = whiteSinglePawnPushTargets(board);
    return (singlePushes << 8) & ~board.occupiedSquares & rank4;
}

uint64_t blackSinglePawnPushTargets(GameState board){
    return (board.blackPawns >> 8) & ~board.occupiedSquares;
}

uint64_t blackDoublePawnPushTargets(GameState board){
    const uint64_t rank4 = 0x000000FF00000000;
    uint64_t singlePushes = blackSinglePawnPushTargets(board);
    return (singlePushes >> 8) & ~board.occupiedSquares & rank4;   
}