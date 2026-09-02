#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "../include/game.h"
#include "../include/board.h"


#include <vector> 
#include <cstdint> 

std::vector<uint32_t> getPlayerMoves(const GameState& board); 
std::vector<uint32_t> getLegalMoves(const GameState& board);

std::vector<uint32_t> knightMoves(const GameState& board, int sq);

uint64_t slidingPieceAttacks(const GameState& board, int pc, int sq);
std::vector<uint32_t> bishopMoves(const GameState& board, int sq);
std::vector<uint32_t> rookMoves(const GameState& board, int sq);
std::vector<uint32_t> queenMoves(const GameState& board, int sq);
std::vector<uint32_t> kingMoves(const GameState& board, int sq);

uint64_t whiteSinglePawnPushTargets(const GameState& board);
uint64_t whiteDoublePawnPushTargets(const GameState& board);
uint64_t blackSinglePawnPushTargets(const GameState& board);
uint64_t blackDoublePawnPushTargets(const GameState& board);
uint64_t whitePawnsLeftAttackTargets(const GameState& board);
uint64_t whitePawnsRightAttackTargets(const GameState& board);
uint64_t blackPawnsLeftAttackTargets(const GameState& board);
uint64_t blackPawnsRightAttackTargets(const GameState& board);

uint64_t attackedSquares(const GameState& board, bool whiteToMove); 

std::vector<uint32_t> whitePawnMoves(const GameState& board);
std::vector<uint32_t> blackPawnMoves(const GameState& board);

bool applyMove(GameState& board, uint32_t move);

bool checkPositionEquality(const Position& pos1, const Position& pos2);
bool checkThreefoldRepetition(const GameState& board);

enum GameResult { ACTIVE, CHECKMATE, STALEMATE, MOVE50, THREEFOLD_REPETITION};
GameResult getGameResult(const GameState& board);

#endif