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
                destinations &= destinations - 1;
            }
            knightsCopy &= knightsCopy - 1;
        }

    }

    return moves;

}
