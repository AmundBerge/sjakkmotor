#include "../include/movegen.h"
#include "../include/board.h"
#include "../include/game.h"
#include "../include/utils.h"

#include <cstdint>
#include <vector> 
#include <iostream>

std::vector<uint32_t> getPlayerMoves(GameState board){
    std::vector<uint32_t> moves;
    uint64_t knights; 
    uint64_t bishops; 
    uint64_t rooks; 
    uint64_t queens; 
    uint64_t king;
    if (board.whiteToMove){
        std::vector<uint32_t> pawnMoves = whitePawnMoves(board);
        moves.insert(moves.end(), pawnMoves.begin(), pawnMoves.end());
        knights = board.whiteKnights;
        bishops = board.whiteBishops;
        rooks = board.whiteRooks;
        queens = board.whiteQueens;
        king = board.whiteKing;
    } else {
        std::vector<uint32_t> pawnMoves = blackPawnMoves(board);
        moves.insert(moves.end(), pawnMoves.begin(), pawnMoves.end());
        knights = board.blackKnights;
        bishops = board.blackBishops;
        rooks = board.blackRooks;
        queens = board.blackQueens;
        king = board.blackKing; 
    }
    for (uint64_t b = knights; b != 0; b &= (b - 1)){
        int p = __builtin_ctzll(b);
        std::vector<uint32_t> mvs = knightMoves(board, p);
        moves.insert(moves.end(), mvs.begin(), mvs.end());
    }
    for (uint64_t b = bishops; b != 0; b &= (b - 1)){
        int p = __builtin_ctzll(b);
        std::vector<uint32_t> mvs = bishopMoves(board, p);
        moves.insert(moves.end(), mvs.begin(), mvs.end());
    }
    for (uint64_t b = rooks; b != 0; b &= (b - 1)){
        int p = __builtin_ctzll(b);
        std::vector<uint32_t> mvs = rookMoves(board, p);
        moves.insert(moves.end(), mvs.begin(), mvs.end());
    }
    for (uint64_t b = queens; b != 0; b &= (b - 1)){
        int p = __builtin_ctzll(b);
        std::vector<uint32_t> mvs = queenMoves(board, p);
        moves.insert(moves.end(), mvs.begin(), mvs.end());
    }
    int q = __builtin_ctzll(king);
    std::vector<uint32_t> mvs = kingMoves(board, q);
    moves.insert(moves.end(), mvs.begin(), mvs.end());

    return moves;
}

std::vector<uint32_t> knightMoves(GameState board, int square){
    std::vector<uint32_t> moves;
    int pccl = getColorBySquare(board, square);
    uint64_t ts = pieceAttacks[1][square];
    for (uint64_t bb = ts; bb != 0; bb &= (bb - 1)){
        int sq = __builtin_ctzll(bb);
        int cl = getColorBySquare(board, sq);
        if (pccl == cl){
            continue;
        }
        uint32_t mv = 0x00000000;
        int pc = getPieceBySquare(board, sq);
        if (pc == -1){
            pc = 7; 
        }
        mv = mv & ~(0x3F << 26) | ((square & 0x3F) << 26);
        mv = mv & ~(0x3F << 20) | ((sq & 0x3F) << 20);
        mv = mv & ~(0x7 << 17) | ((pc & 0x7) << 17);
        moves.push_back(mv);
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
// b, r, q moves kan skrives som én funksjon
std::vector<uint32_t> bishopMoves(GameState board, int square){ 
    std::vector<uint32_t> moves;
    int pccl = getColorBySquare(board, square);
    uint64_t ts = slidingPieceAttacks(board, 2, square);
    for (uint64_t b = ts; b != 0; b &= (b - 1)){
        int sq = __builtin_ctzll(b);
        int cl = getColorBySquare(board, sq);
        if (pccl == cl){
            continue;
        }
        uint32_t mv = 0x00000000;
        int pc = getPieceBySquare(board, sq);
        if (pc == -1){
            pc = 7; 
        }
        mv = mv & ~(0x3F << 26) | ((square & 0x3F) << 26);
        mv = mv & ~(0x3F << 20) | ((sq & 0x3F) << 20);
        mv = mv & ~(0x7 << 17) | ((pc & 0x7) << 17);
        moves.push_back(mv);
    }
    return moves;
}

std::vector<uint32_t> rookMoves(GameState board, int square){
    std::vector<uint32_t> moves;
    int pccl = getColorBySquare(board, square);
    uint64_t ts = slidingPieceAttacks(board, 3, square);
    for (uint64_t b = ts; b != 0; b &= (b - 1)){
        int sq = __builtin_ctzll(b);
        int cl = getColorBySquare(board, sq);
        if (pccl == cl){
            continue;
        }
        uint32_t mv = 0x00000000;
        int pc = getPieceBySquare(board, sq);
        if (pc == -1){
            pc = 7;
        }
        mv = mv & ~(0x3F << 26) | ((square & 0x3F) << 26);
        mv = mv & ~(0x3F << 20) | ((sq & 0x3F) << 20);
        mv = mv & ~(0x7 << 17) | ((pc & 0x7) << 17);
        moves.push_back(mv);
    }
    return moves;
}

std::vector<uint32_t> queenMoves(GameState board, int square){
    std::vector<uint32_t> moves;
    int pccl = getColorBySquare(board, square);
    uint64_t ts = slidingPieceAttacks(board, 4, square);
    for (uint64_t b = ts; b != 0; b &= (b - 1)){
        int sq = __builtin_ctzll(b);
        int cl = getColorBySquare(board, sq);
        if (pccl == cl){
            continue;
        }
        uint32_t mv = 0x00000000;
        int pc = getPieceBySquare(board, sq);
        if (pc == -1){
            pc = 7;
        }
        mv = mv & ~(0x3F << 26) | ((square & 0x3F) << 26);
        mv = mv & ~(0x3F << 20) | ((sq & 0x3F) << 20);
        mv = mv & ~(0x7 << 17) | ((pc & 0x7) << 17);
        moves.push_back(mv);
    }
    return moves;
}

std::vector<uint32_t> kingMoves(GameState board, int square){
    std::vector<uint32_t> moves;
    uint64_t kingTargets = pieceAttacks[5][square];
    int pccl = getColorBySquare(board, square);
    for (uint64_t b = kingTargets; b != 0; b &= (b - 1)){
        int sq = __builtin_ctzll(b);
        int cl = getColorBySquare(board, sq);
        if (pccl == cl){
            continue;
        }
        uint32_t mv = 0x00000000;
        int pc = getPieceBySquare(board, sq);
        if (pc == -1){
            pc = 7;
        }
        mv = mv & ~(0x3F << 26) | ((square & 0x3f) << 26);
        mv = mv & ~(0x3F << 20) | ((sq & 0x3F) << 20);
        mv = mv & ~(0x7 << 17) | ((pc & 0x7) << 17);
        moves.push_back(mv);
    }
    return moves;
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
    return (board.whitePawns << 7) & 0x7F7F7F7F7F7F7F7F;
}

uint64_t whitePawnsRightAttackTargets(GameState board){
    return (board.whitePawns << 9) & 0xFEFEFEFEFEFEFEFE;
}

uint64_t blackPawnsLeftAttackTargets(GameState board){
    return (board.blackPawns >> 7) & 0xFEFEFEFEFEFEFEFE;
}

uint64_t blackPawnsRightAttackTargets(GameState board){
    return (board.blackPawns >> 9) & 0x7F7F7F7F7F7F7F7F;
}

std::vector<uint32_t> whitePawnMoves(GameState board){
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
        mv = mv & ~(0x7 << 17) | ((7 & 0x7) << 17);
        if (p >= 56){
            for (int i = 1; i < 5; i++){
                uint32_t m = mv | ((i & 0x3) << 15);
                moves.push_back(m);
            }
        } else {
            moves.push_back(mv);
        }
    }

    for (uint64_t b = doublePushes; b != 0; b &= (b - 1)){
        uint32_t mv = 0x00000000;
        int p = __builtin_ctzll(b);
        mv = mv & ~(0x3F << 26) | (((p - 16) & 0x3F) << 26);
        mv = mv & ~(0x3F << 20) | ((p & 0x3F) << 20);
        mv = mv & ~(0x7 << 17) | ((7 & 0x7) << 17);
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
        mv = mv & ~(0x7 << 17) | ((pc & 0x7) << 17);
        if (p >= 56){
            for (int i = 1; i < 5; i++){
                uint32_t m = mv | ((i & 0x3) << 15);
                moves.push_back(m);
            }
        } else {
            moves.push_back(mv);
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
        mv = mv & ~(0x3F << 26) | (((p - 9) & 0x3F) << 26);
        mv = mv & ~(0x3F << 20) | ((p & 0x3F) << 20);
        mv = mv & ~(0x7 << 17) | ((pc & 0x7) << 17);
        if (p >= 56){
            for (int i = 1; i < 5; i++){
                uint32_t m = mv | ((i & 0x3) << 15);
                moves.push_back(m);
            }
        } else {
            moves.push_back(mv);
        }
    }

    return moves;
}

std::vector<uint32_t> blackPawnMoves(GameState board){
    uint64_t singlePushes = blackSinglePawnPushTargets(board);
    uint64_t doublePushes = blackDoublePawnPushTargets(board);
    uint64_t blackPawnsLeft = blackPawnsLeftAttackTargets(board);
    uint64_t blackPawnsRight = blackPawnsRightAttackTargets(board);

    std::vector<uint32_t> moves;

    for (uint64_t b = singlePushes; b != 0; b &= (b - 1)){
        uint32_t mv = 0x00000000;
        int p = __builtin_ctzll(b);
        mv = mv & ~(0x3F << 26) | (((p + 8) & 0x3F) << 26);
        mv = mv & ~(0x3F << 20) | ((p & 0x3F) << 20);
        mv = mv & ~(0x7 << 17) | ((7 & 0x7) << 17);
        if (p >= 56){
            for (int i = 1; i < 5; i++){
                uint32_t m = mv | ((i & 0x3) << 15);
                moves.push_back(m);
            }
        } else {
            moves.push_back(mv);
        }
    }

    for (uint64_t b = doublePushes; b != 0; b &= (b - 1)){
        uint32_t mv = 0x00000000;
        int p = __builtin_ctzll(b);
        mv = mv & ~(0x3F << 26) | (((p + 16) & 0x3F) << 26);
        mv = mv & ~(0x3F << 20) | ((p & 0x3F) << 20);
        mv = mv & ~(0x7 << 17) | ((7 & 0x7) << 17);
        moves.push_back(mv);
    }

    for (uint64_t b = blackPawnsLeft; b != 0; b &= (b - 1)){
        int p = __builtin_ctzll(b);
        int cl = getColorBySquare(board, p);
        if (cl <= 0){
            continue;
        }
        int pc = getPieceBySquare(board, p);
        uint32_t mv = 0x00000000;
        mv = mv & ~(0x3F << 26) | (((p + 7) & 0x3F) << 26);
        mv = mv & ~(0x3F << 20) | ((p & 0x3F) << 20);
        mv = mv & ~(0x7 << 17) | ((pc & 0x7) << 17);
        if (p < 8){
            for (int i = 1; i < 5; i++){
                uint32_t m = mv | ((i & 0x3) << 15);
                moves.push_back(m);
            }
        } else {
            moves.push_back(mv);
        }
    }

    for (uint64_t b = blackPawnsRight; b != 0; b &= (b - 1)){
        int p = __builtin_ctzll(b);
        int cl = getColorBySquare(board, p);
        if (cl <= 0){
            continue;
        }
        int pc = getPieceBySquare(board, p);
        uint32_t mv = 0x00000000;
        mv = mv & ~(0x3F << 26) | (((p + 9) & 0x3F) << 26);
        mv = mv & ~(0x3F << 20) | ((p & 0x3F) << 20);
        mv = mv & ~(0x7 << 17) | ((pc & 0x7) << 17);
        if (p < 8){
            for (int i = 1; i < 5; i++){
                uint32_t m = mv | ((i & 0x3) << 15);
                moves.push_back(m);
            }
        } else {
            moves.push_back(mv);
        }
    }

    return moves;
}
