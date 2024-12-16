#ifndef GAME_H
#define GAME_H

#include "board.h"

class Game {
    private: 
        // GameState board;
    public: 
        Game();
        void initializeBoard();
        void makeMove(const Move& move);
        void undoMove();
        GameState board;
};  



#endif