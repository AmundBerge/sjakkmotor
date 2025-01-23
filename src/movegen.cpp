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

std::vector<uint32_t> bishopMoves(GameState board, int square){
    std::vector<uint32_t> moves;
    int pccl = getColorBySquare(board, square);
    uint64_t ts = slidingPieceAttacks(board, 2, square);
    for (uint64_t b = ts; b != 0; b &= (b - 1)){
        int sq = __builtin_ctzll(b);
        int cl = getColorBySquare(board, sq);
        if (pccl + cl != 0 || std::abs(cl) == 0){
            continue;
        }
        uint32_t mv = 0x00000000;
        int pc = getPieceBySquare(board, sq);
        mv = mv & ~(0x3F << 26) | ((square & 0x3F) << 26);
        mv = mv & ~(0x3F << 20) | ((sq & 0x3F) << 26);
        mv = mv & ~(0x7 << 17) | (((pc + 1) & 0x7) << 17);
        moves.push_back(mv);
    }
}

std::vector<uint32_t> rookMoves(GameState board, int square){
    std::vector<uint32_t> moves;
    int pccl = getColorBySquare(board, square);
    uint64_t ts = slidingPieceAttacks(board, 3, square);
    for (uint64_t b = ts; b != 0; b &= (b - 1)){
        int sq = __builtin_ctzll(b);
        int cl = getColorBySquare(board, sq);
        if (pccl + cl != 0 || std::abs(cl) == 0){
            continue;
        }
        uint32_t mv = 0x00000000;
        int pc = getPieceBySquare(board, sq);
        mv = mv & ~(0x3F << 26) | ((square & 0x3F) << 26);
        mv = mv & ~(0x3F << 20) | ((sq & 0x3F) << 26);
        mv = mv & ~(0x7 << 17) | (((pc + 1) & 0x7) << 17);
        moves.push_back(mv);
    }
}

std::vector<uint32_t> queenMoves(GameState board, int square){
    std::vector<uint32_t> moves;
    int pccl = getColorBySquare(board, square);
    uint64_t ts = slidingPieceAttacks(board, 4, square);
    for (uint64_t b = ts; b != 0; b &= (b - 1)){
        int sq = __builtin_ctzll(b);
        int cl = getColorBySquare(board, sq);
        if (pccl + cl != 0 || std::abs(cl) == 0){
            continue;
        }
        uint32_t mv = 0x00000000;
        int pc = getPieceBySquare(board, sq);
        mv = mv & ~(0x3F << 26) | ((square & 0x3F) << 26);
        mv = mv & ~(0x3F << 20) | ((sq & 0x3F) << 26);
        mv = mv & ~(0x7 << 17) | (((pc + 1) & 0x7) << 17);
        moves.push_back(mv);
    }
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
    const uint64_t rank5 = 0x000000FF00000000;
    uint64_t singlePushes = blackSinglePawnPushTargets(board);
    return (singlePushes >> 8) & ~board.occupiedSquares & rank5;   
}

uint64_t whitePawnsLeftAttackTargets(GameState board){
    return (board.whitePawns << 7) & 0xFEFEFEFEFEFEFEFE;
}

uint64_t whitePawnsRightAttackTargets(GameState board){
    return (board.whitePawns << 9) & 0x7F7F7F7F7F7F7F7F;
}

uint64_t blackPawnsLeftAttackTargets(GameState board){
    return (board.blackPawns >> 7) & 0x7F7F7F7F7F7F7F7F;
}

uint64_t blackPawnsRightAttackTargets(GameState board){
    return (board.blackPawns >> 9) & 0xFEFEFEFEFEFEFEFE;
}

std::vector<uint32_t> whitePawnsMoves(GameState board){
    uint64_t singlePushes = whiteSinglePawnPushTargets(board);
    uint64_t doublePushes = whiteDoublePawnPushTargets(board);
    uint64_t whitePawnsLeft = whitePawnsLeftAttackTargets(board);
    uint64_t whitePawnsRight = whitePawnsRightAttackTargets(board);

    std::vector<uint32_t> moves;

    for (uint64_t b = singlePushes; b != 0; b &= (b - 1)){
        uint32_t mv = 0x00000000;
        int p = __builtin_ctzll(b);
        mv = mv & ~(0x3F << 26) | (((p - 8) & 0x3F) << 26);
        mv = mv & ~(0x3F << 20) | ((p & 0x3F) << 20);
        moves.push_back(mv);
    }

    for (uint64_t b = doublePushes; b != 0; b &= (b - 1)){
        uint32_t mv = 0x00000000;
        int p = __builtin_ctzll(b);
        mv = mv & ~(0x3F << 26) | (((p - 16) & 0x3F) << 26);
        mv = mv & ~(0x3F << 20) | ((p & 0x3F) << 20);
        moves.push_back(mv);
    }

    for (uint64_t b = whitePawnsLeft; b != 0; b &= (b - 1)){
        int p = __builtin_ctzll(b);
        int cl = getColorBySquare(board, p);
        if (cl >= 0){
            continue;
        }
        int pc = getPieceBySquare(board, p);
        uint32_t mv = 0x00000000;
        mv = mv & ~(0x3F << 26) | (((p - 7) & 0x3F) << 26);
        mv = mv & ~(0x3F << 20) | ((p & 0x3F) << 20);
        mv = mv & ~(0x7 << 17) | (((pc + 1) & 0x7) << 17);
        moves.push_back(mv);
        if (p >= 56){
            for (int i = 1; i < 5; i++){
                uint32_t m = mv | ((i & 0x7) << 14);
                moves.push_back(m);
            }
        }
    }

    for (uint64_t b = whitePawnsRight; b != 0; b &= (b - 1)){
        int p = __builtin_ctzll(b);
        int cl = getColorBySquare(board, p);
        if (cl >= 0){
            continue;
        }
        int pc = getPieceBySquare(board, p);
        uint32_t mv = 0x00000000;
        mv = mv & ~(0x3F << 26) | (((p - 7) & 0x3F) << 26);
        mv = mv & ~(0x3F << 20) | ((p & 0x3F) << 20);
        mv = mv & ~(0x7 << 17) | (((pc + 1) & 0x7) << 17);
        moves.push_back(mv);
        if (p >= 56){
            for (int i = 1; i < 5; i++){
                uint32_t m = mv | ((i & 0x7) << 14);
                moves.push_back(m);
            }
        }
    }

    return moves;
}