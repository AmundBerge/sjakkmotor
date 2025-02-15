#include "../include/game.h"
#include "../include/board.h"
#include "../include/utils.h"
#include "../include/movegen.h"

#include <vector> 
#include <cstdint> 
#include <iostream>
#include <bitset>
#include <string> 
#include <utility> 

int main(){
    Game game = Game();

    initializePieceAttacks();
    initializeBlockers();
    initializeBehind();

    std::string str; 

    while (true){
        printChessBoard(game.board);
        std::cin >> str;
        if (str == "stop"){
            break;
        }
        if (str == "player"){
            std::cout << (game.board.whiteToMove ? "white" : "black") << std::endl;
            continue;
        }
        if (str == "moves"){
            std::vector<uint32_t> moves = getPlayerMoves(game.board);
            for (int i = 0; i < moves.size(); i++){
                printMoveSquares(moves[i]);
            }
            std::cout << "Antall trekk: " << moves.size() << std::endl;
            continue;
        }
        std::pair<int, int> sqrs = textMoveToSquares(game.board, str);
        uint32_t mv = 0x00000000;
        mv = mv & ~(0x3F << 26) | ((sqrs.first & 0x3F) << 26);
        mv = mv & ~(0x3F << 20) | ((sqrs.second & 0x3F) << 20);
        bool test = game.makeMove(mv);
        if (test){
            std::cout << "move success?" << std::endl;
        } else {
            std::cout << "move failure?" << std::endl;
        }
    }
   
    return 0;
}