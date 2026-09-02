#include "../include/board.h"
#include "../include/movegen.h"
#include "../include/evaluation.h"

#include <cstdint> 
#include <vector>
#include <random>

uint32_t randomMove(const std::vector<uint32_t> moves){
    std::random_device rd; 
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, moves.size() - 1);
    uint32_t mv = moves[dist(gen)];
    return mv;
}

int negamax(GameState board, int depth, int alpha, int beta){
    std::vector<uint32_t> legalMoves = getLegalMoves(board);
    if (legalMoves.empty()) return evaluateNoMoves(board);
    if (depth == 0) return evaluatePosition(board);
    int best = -100000;
    for (int i = 0; i < legalMoves.size(); i++){
        GameState child = board;
        applyMove(child, legalMoves[i]);
        int value = -negamax(child, depth - 1, -beta, -alpha);
        best = std::max(best, value);
        alpha = std::max(alpha, value);
        if (alpha >= beta){
            break;
        }
    }
    return best; 
}

uint32_t findMove(const GameState& board, int depth){
    std::vector<uint32_t> moves = getLegalMoves(board);
    uint32_t bestMove = moves[0];
    int best = -100000;
    for (int i = 0; i < moves.size(); i++){
        GameState child = board; 
        applyMove(child, moves[i]);
        int value = -negamax(child, depth - 1, -100000, 100000);
        if (value > best){
            best = value; 
            bestMove = moves[i];
        }
    }
    return bestMove;
}
