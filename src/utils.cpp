#include "../include/utils.h"
#include "../include/game.h"

#include <cstdint> 
#include <iostream> 
#include <array>


void printBitboard(uint64_t bitboard){
    for (int i = 7; i >= 0; --i){
        for (int j = 0; j <= 7; ++j){
            std::cout << (bitboard >> (8 * i + j) & 1) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

uint64_t northRay(int square){
    uint64_t ray = 0ULL;
    for (int r = square + 8; r < 64; r += 8){
        ray |= 1ULL << r;
    }
    return ray; 
}

uint64_t southRay(int square){
    uint64_t ray = 0ULL;
    for (int r = square - 8; r >= 0; r -= 8){
        ray |= 1ULL << r;
    }
    return ray; 
}

uint64_t eastRay(int square){
    uint64_t ray = 0ULL;
    int file = square % 8;
    for (int f = file + 1; f < 8; f++){
        ray |= 1ULL << (square + f - file);
    }
    return ray; 
}

uint64_t westRay(int square){
    uint64_t ray = 0ULL;
    int file = square % 8;
    for (int f = file - 1; f >= 0; f--){
        ray |= 1ULL << (square + f - file);
    }
    return ray; 
}

uint64_t northEastRay(int square){
    uint64_t ray = 0ULL;
    int file = square % 8; 
    for (int r = square + 8, f = file + 1; r < 64 && f < 8; r += 8, f++){
        ray |= 1ULL << (r + f - file);
    }
    return ray;
}

uint64_t northWestRay(int square){
    uint64_t ray = 0ULL;
    int file = square % 8; 
    for (int r = square + 8, f = file - 1; r < 64 && f >= 0; r += 8, f--){
        ray |= 1ULL << (r + f - file);
    }
    return ray;
}

uint64_t southEastRay(int square){
    uint64_t ray = 0ULL;
    int file = square % 8; 
    for (int r = square - 8, f = file + 1; r >= 0 && f < 8; r -= 8, f++){
        ray |= 1ULL << (r + f - file);
    }
    return ray;
}

uint64_t southWestRay(int square){
    uint64_t ray = 0ULL;
    int file = square % 8; 
    for (int r = square - 8, f = file - 1; r >= 0 && f >= 0; r -= 8, f--){
        ray |= 1ULL << (r + f - file);
    }
    return ray;
}

uint64_t knightAttacks(int square){
    uint64_t attacks = 0ULL;
    static const int knightOffsets[8] = {17, 10, -6, -15, -17, -10, 6, 15};
    int rank = square / 8;
    int file = square % 8; 

    for (int i = 0; i < sizeof(knightOffsets) / sizeof(knightOffsets[0]); ++i){
        int offset = knightOffsets[i];
        int destinationSquare = square + offset;
        int destinationRank = destinationSquare / 8;
        int destinationFile = destinationSquare % 8; 
        if (std::abs(destinationRank - rank) <= 2 && std::abs(destinationFile - file) <= 2 && destinationSquare >= 0 && destinationSquare < 64){
            attacks |= 1ULL << (square + offset);
        }
    }

    return attacks;
    
}

uint64_t kingAttacks(int square){
    uint64_t attacks = 0ULL;
    static const int kingOffsets[8] = {1, 7, 8, 9, -1, -7, -8, -9};
    int rank = square / 8;
    int file = square % 8; 

    for (int i = 0; i < sizeof(kingOffsets) / sizeof(kingOffsets[0]); ++i){
        int offset = kingOffsets[i];
        int destinationSquare = square + offset;
        int destinationRank = destinationSquare / 8;
        int destinationFile = destinationSquare % 8; 
        if (std::abs(destinationRank - rank) <= 1 && std::abs(destinationFile - file) <= 1 && destinationSquare >= 0 && destinationSquare < 64){
            attacks |= 1ULL << (square + offset);
        }
    }

    return attacks;
    
}

std::array<std::array<uint64_t, 64>, 6> pieceAttacks;

void initializePieceAttacks(){ 
    for (int i = 0; i < 64; i++){
        pieceAttacks[1][i] = knightAttacks(i);
        pieceAttacks[2][i] = northEastRay(i) | northWestRay(i) | southEastRay(i) | southWestRay(i);
        pieceAttacks[3][i] = northRay(i) | southRay(i) | eastRay(i) | westRay(i); 
        pieceAttacks[4][i] = pieceAttacks[2][i] | pieceAttacks[3][i];
        pieceAttacks[5][i] = kingAttacks(i);
    }
}

std::array<std::array<uint64_t, 64>, 6> blockers;

uint64_t bishopBlockers(int square){
    uint64_t ray = 0ULL;
    int file = square % 8;
    for (int r = square + 8, f = file + 1; r < 56 && f < 7; r += 8, f++){
        ray |= 1ULL << (r + f - file);
    }
    for (int r = square - 8, f = file - 1; r >= 8 && f >= 1; r -= 8, f--){
        ray |= 1ULL << (r + f - file);
    }
    for (int r = square + 8, f = file - 1; r < 56 && f >= 1; r += 8, f--){
        ray |= 1ULL << (r + f - file);
    }
    for (int r = square - 8, f = file + 1; r >= 8 && f < 7; r -= 8, f++){
        ray |= 1ULL << (r + f - file);
    }
    return ray; 
}

uint64_t rookBlockers(int square){
    uint64_t ray = 0ULL;
    int rank = square / 8;
    int file = square % 8;
    for (int r = rank + 1; r < 7; r++){
        ray |= 1ULL << (square + (r - rank) * 8);
    }
    for (int r = rank - 1; r > 0; r--){
        ray |= 1ULL << (square + (r - rank) * 8);
    }
    for (int f = file + 1; f < 7; f++){
        ray |= 1ULL << (square + f - file);
    }
    for (int f = file - 1; f > 0; f--){
        ray |= 1ULL << (square + f - file);
    }
    return ray;
}

void initializeBlockers(){
    for (int i = 0; i < 64; i++){
        blockers[2][i] = bishopBlockers(i);
        blockers[3][i] = rookBlockers(i);
        blockers[4][i] = blockers[2][i] | blockers[3][i];
    }
}

std::array<std::array<uint64_t, 64>, 64> behind;

void initializeBehind(){
    for (int i = 0; i < 64; i++){
        for (int j = 0; j < 64; j++){
            behind[i][j] = 0ULL;
            if (i == j){
                continue;
            }
            int i_r = i / 8;
            int i_f = i % 8;
            int j_r = j / 8;
            int j_f = j % 8;
            if (i_r == j_r){
                if (i_f > j_f){
                    for (int k = j_f - 1; k >= 0; k--){
                        behind[i][j] |= 1ULL << (j + j_f - k);
                    }
                } else {
                    for (int k = j_f + 1; k < 8; k++){
                        behind[i][j] |= 1ULL << (j - j_f + k);
                    }
                }
            } else if (i_f == j_f){
                if (i_r > j_r){
                    for (int k = j_r - 1; k >= 0; k--){
                        behind[i][j] |= 1ULL << (j - (j_r - k) * 8);
                    }
                } else {
                    for (int k = j_r + 1; k < 8; k++){
                        behind[i][j] |= 1ULL << (j + (k - j_r) * 8);
                    }
                }
            } else if (abs(i_r - j_r) == abs(i_f - j_f)){
                int r_d = (i_r > j_r) ? -1 : 1;
                int f_d = (i_f > j_f) ? -1 : 1;
                for (int r = j_r + r_d, f = j_f + f_d; r >= 0 && r < 8 && f >= 0 && f < 8; r = r + r_d, f = f + f_d){
                    behind[i][j] |= 1ULL << (r * 8 + f);
                }
            }
        }
    }
}

int getPieceBySquare(GameState board, int square){
    uint64_t b = 1ULL << square;

    if (board.whitePawns & b || board.blackPawns & b){
        return 0;
    }
    if (board.whiteKnights & b || board.blackKnights & b){
        return 1; 
    }
    if (board.whiteBishops & b || board.blackBishops & b){
        return 2;
    }
    if (board.whiteRooks & b || board.blackRooks & b){
        return 3;
    }
    if (board.whiteQueens & b || board.blackQueens & b){
        return 4;
    }
    if (board.whiteKing & b || board.blackKing & b){
        return 5;
    }
    return 6;
}

int getColorBySquare(GameState board, int square){
    uint64_t b = 1ULL << square;

    if (board.whitePieces & b){
        return 1;
    }
    if (board.blackPieces & b){
        return -1;
    }
    return 0;
}