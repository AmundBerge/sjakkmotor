#ifndef UTILS_H
#define UTILS_H

#include <cstdint> 
#include <array> 


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

extern std::array<std::array<uint64_t, 64>, 6> pieceAttacks; // 1=N, 2=B, 3=R, 4=Q, 5=K

void initializePieceAttacks(); 

#endif