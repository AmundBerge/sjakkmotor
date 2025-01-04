#include "../include/game.h"
#include "../include/board.h"
#include "../include/utils.h"
#include "../include/movegen.h"

#include <vector> 
#include <cstdint> 
#include <iostream>

int main(){
    Game game = Game();

    initializePieceAttacks();
    initializeBlockers();

    std::vector<uint16_t> knightMoves = getPlayerMoves(game.board);

    for (int i = 0; i < knightMoves.size(); i++){
        std::cout << std::bitset<16>(knightMoves[i]) << std::endl;
    }

    for (int i = 0; i < 64; i++){
        printBitboard(blockers[4][i]);
    }
    

    return 0;
}