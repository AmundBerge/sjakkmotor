#ifndef UTILS_H
#define UTILS_H

#include <cstdint> 
#include <array> 
#include "../include/board.h"


void printBitboard(uint64_t bitboard);

uint64_t northRay(int square);
uint64_t southRay(int square);
uint64_t eastRay(int square);
uint64_t westRay(int square);

uint64_t northEastRay(int square);
uint64_t northWestRay(int square);
uint64_t southEastRay(int square);
uint64_t southWestRay(int square);

uint64_t knightAttacks(int square);
uint64_t kingAttacks(int square);

uint64_t bishopBlockers(int square);
uint64_t rookBlockers(int square);

extern std::array<std::array<uint64_t, 64>, 6> pieceAttacks; // 1=N, 2=B, 3=R, 4=Q, 5=K
extern std::array<std::array<uint64_t, 64>, 6> blockers;
extern std::array<std::array<uint64_t, 64>, 64> behind;

void initializePieceAttacks(); 
void initializeBlockers();
void initializeBehind();

int getPieceBySquare(GameState board, int square);
int getColorBySquare(GameState board, int square);

#endif