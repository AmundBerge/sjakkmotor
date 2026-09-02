#include "../include/game.h"
#include "../include/board.h"
#include "../include/utils.h"
#include "../include/movegen.h"
#include "../include/search.h"

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
    std::cin >> str; 

    if (str == "engine"){
        std::string color; 
        std::cin >> color; 
        printChessBoard(game.board);
        if (color == "white"){
            std::cin >> str; 
            std::pair<int, int> sqrs = textMoveToSquares(game.board, str);
            uint32_t mv = 0x00000000;
            mv = mv & ~(0x3F << 26) | ((sqrs.first & 0x3F) << 26);
            mv = mv & ~(0x3F << 20) | ((sqrs.second & 0x3F) << 20);
            bool test = game.readUserMove(mv);
            if (!test) std::cout << "ERROR" << std::endl;
        }
        while (true){
            uint32_t engineMove = findMove(game.board, 4);
            game.makeMove(engineMove);
            printChessBoard(game.board);
            GameResult result1 = getGameResult(game.board);
            if (result1 == CHECKMATE){
                std::cout << (game.board.whiteToMove ? "Black" : "White") << " wins!" << std::endl;
                break;
            }
            if (result1 == STALEMATE){
                std::cout << "Stalemate!" << std::endl;
                break; 
            }
            if (result1 == MOVE50){
                std::cout << "Draw by fifty-move rule!" << std::endl;
                break;
            }
            if (result1 == THREEFOLD_REPETITION){
                std::cout << "Draw by threefold-repetition!" << std::endl;
                break;
            }
            bool moveSuccess = false; 
            while (!moveSuccess){
                std::cin >> str; 
                std::pair<int, int> sqrs = textMoveToSquares(game.board, str);
                uint32_t mv = 0x00000000;
                mv = mv & ~(0x3F << 26) | ((sqrs.first & 0x3F) << 26);
                mv = mv & ~(0x3F << 20) | ((sqrs.second & 0x3F) << 20);
                moveSuccess = game.readUserMove(mv);
                if (!moveSuccess) std::cout << "Invalid move." << std::endl;
            }
            printChessBoard(game.board);
            GameResult result2 = getGameResult(game.board);
            if (result2 == CHECKMATE){
                std::cout << (game.board.whiteToMove ? "Black" : "White") << " wins!" << std::endl;
                break;
            }
            if (result2 == STALEMATE){
                std::cout << "Stalemate!" << std::endl;
                break; 
            }
            if (result2 == MOVE50){
                std::cout << "Draw by fifty-move rule!" << std::endl;
                break;
            }
            if (result2 == THREEFOLD_REPETITION){
                std::cout << "Draw by threefold-repetition!" << std::endl;
                break;
            }
        }
    }

    while (true){
        printChessBoard(game.board);
        GameResult result = getGameResult(game.board);
        if (result == CHECKMATE){
            std::cout << (game.board.whiteToMove ? "Black" : "White") << " wins!" << std::endl;
            break;
        }
        if (result == STALEMATE){
            std::cout << "Stalemate!" << std::endl;
            break; 
        }
        if (result == MOVE50){
            std::cout << "Draw by fifty-move rule!" << std::endl;
            break;
        }
        if (result == THREEFOLD_REPETITION){
            std::cout << "Draw by threefold-repetition!" << std::endl;
            break;
        }
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
        bool test = game.readUserMove(mv);
        if (test){
            //
        } else {
            std::cout << "ERROR: move failure main.cpp" << std::endl;
        }
    }
   
    return 0;
}