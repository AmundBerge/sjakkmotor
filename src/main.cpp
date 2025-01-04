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

    std::vector<uint16_t> knightMoves = getPlayerMoves(game.board);

    for (int i = 0; i < knightMoves.size(); i++){
        std::cout << std::bitset<16>(knightMoves[i]) << std::endl;
    }
    

    return 0;
}