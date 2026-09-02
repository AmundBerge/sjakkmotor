#include "../include/board.h"
#include "../include/game.h"
#include "../include/utils.h"
#include "../include/movegen.h"

#include <iostream>
#include <cstdint> 
#include <cstdlib>
#include <vector>

Game::Game(){
    initializeBoard();
}

void Game::initializeBoard(){
    board.whitePawns = 0x000000000000FF00;
    board.whiteKnights = 0x0000000000000042;
    board.whiteBishops = 0x00000000000024;
    board.whiteRooks = 0x0000000000000081;
    board.whiteQueens = 0x0000000000000008;
    board.whiteKing = 0x0000000000000010;
    board.blackPawns = 0x00FF000000000000; 
    board.blackKnights = 0x4200000000000000;
    board.blackBishops = 0x2400000000000000;
    board.blackRooks = 0x8100000000000000;
    board.blackQueens = 0x0800000000000000;
    board.blackKing = 0x1000000000000000;

    board.whitePieces = board.whitePawns | board.whiteKnights | board.whiteBishops | board.whiteRooks | board.whiteQueens | board.whiteKing;
    board.blackPieces = board.blackPawns | board.blackKnights | board.blackBishops | board.blackRooks | board.blackQueens | board.blackKing;

    board.occupiedSquares = board.whitePieces | board.blackPieces; 
    
    board.whiteToMove = true;

    board.whiteCanCastleShort = true;
    board.whiteCanCastleLong = true;
    board.blackCanCastleShort = true;
    board.blackCanCastleLong = true;

    board.enPassantSquare = 0x0000000000000000;

    board.halfMoveCounter = 0; 
    board.fullMoveCounter = 1;
}

bool Game::readUserMove(uint32_t move){
    std::vector<uint32_t> moves = getPlayerMoves(board);
        for (int i = 0; i < moves.size(); i++){
            uint32_t mv;
            if ((moves[i] & 0xfff00000) == (move & 0xfff00000)){
                mv = moves[i]; 
                if (mv & (1u << 12)){
                    int promotedPiece;
                    std::cout << "0 = knight, 3 = queen" << std::endl;
                    std::cin >> promotedPiece;
                    if (promotedPiece > 3 || promotedPiece < 0){
                        std::cout << "invalid piece" << std::endl;
                    }
                    mv |= (promotedPiece & 0x3) << 15;
                    std::cout << "promoting this pawn yo" << std::endl;
                    printInt32(mv);
                }  
                return makeMove(mv);
            }
        }
    std::cout << "test" << std::endl;
    return false;
}

bool Game::isValidMove(uint32_t move){
    std::vector<uint32_t> moves = getPlayerMoves(board);
    for (int i = 0; i < moves.size(); i++){
        uint32_t mv = moves[i];
        if ((move & 0xFFF00000) == (mv & 0xFFF00000)){
            return true;
        }
    }
    return false;
}

bool Game::makeMove(uint32_t mv){
    
    std::vector<uint32_t> legalMoves = getLegalMoves(board);

    for (int i = 0; i < legalMoves.size(); i++){
        if (mv == legalMoves[i]){
            applyMove(board, mv);
            board.moveHistory.push_back(mv);
            
            Position position; 
            position.whitePawns = board.whitePawns;
            position.whiteKnights = board.whiteKnights; 
            position.whiteBishops = board.whiteBishops;
            position.whiteRooks = board.whiteRooks; 
            position.whiteQueens = board.whiteQueens; 
            position.whiteKing = board.whiteKing; 
            position.blackPawns = board.blackPawns; 
            position.blackKnights = board.blackKnights; 
            position.blackBishops = board.blackBishops; 
            position.blackRooks = board.blackRooks; 
            position.blackQueens = board.blackQueens; 
            position.blackKing = board.blackKing; 
            position.whiteCanCastleShort = board.whiteCanCastleShort; 
            position.whiteCanCastleLong = board.whiteCanCastleLong; 
            position.blackCanCastleShort = board.blackCanCastleShort;
            position.blackCanCastleLong = board.blackCanCastleLong;
            position.whiteToMove = board.whiteToMove;
            position.enPassantSquare = board.enPassantSquare;

            board.positionHistory.push_back(position);
            return true;
        }
    }

    return false;
}

void Game::updateBitboards(){
    board.whitePieces = board.whitePawns | board.whiteKnights | board.whiteBishops | board.whiteRooks | board.whiteQueens | board.whiteKing;
    board.blackPieces = board.blackPawns | board.blackKnights | board.blackBishops | board.blackRooks | board.blackQueens | board.blackKing;
    board.occupiedSquares = board.whitePieces | board.blackPieces; 
}