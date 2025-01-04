#include "../include/board.h"
#include "../include/game.h"

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

void Game::makeMove(const Move& move){

}