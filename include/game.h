#ifndef GAME_H
#define GAME_H

#include "board.h"

#include <cstdint> 

class Game {
    private: 
        // GameState board;
    public: 
        Game();
        void initializeBoard();
        GameState board;
        bool makeMove(uint32_t move);
        bool isValidMove(uint32_t move);

        void updateBitboards();
};  



#endif