#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "../include/game.h"
#include "../include/board.h"


#include <vector> 
#include <cstdint> 


std::vector<uint16_t> getPlayerMoves(GameState board); 

uint64_t slidingPieceAttacks(GameState board, int pc, int sq);
std::vector<uint32_t> bishopMoves(GameState board, int sq);
std::vector<uint32_t> rookMoves(GameState board, int sq);
std::vector<uint32_t> queenMoves(GameState board, int sq);
std::vector<uint32_t> kingMoves(GameState board, int sq);

uint64_t whiteSinglePawnPushTargets(GameState board);
uint64_t whiteDoublePawnPushTargets(GameState board);
uint64_t blackSinglePawnPushTargets(GameState board);
uint64_t blackDoublePawnPushTargets(GameState board);
uint64_t whitePawnsLeftAttackTargets(GameState board);
uint64_t whitePawnsRightAttackTargets(GameState board);
uint64_t blackPawnsLeftAttackTargets(GameState board);
uint64_t blackPawnsRightAttackTargets(GameState board);

std::vector<uint32_t> whitePawnsMoves(GameState board);

#endif