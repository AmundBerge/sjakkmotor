#include "../include/board.h"
#include "../include/game.h"
#include "../include/utils.h"
#include "../include/movegen.h"

#include <iostream>
#include <cstdint> 
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

bool Game::isValidMove(uint32_t move){
    std::vector<uint32_t> moves = getPlayerMoves(board);
    std::cout << "moves.size(): " << moves.size() << std::endl;
    for (int i = 0; i < moves.size(); i++){
        uint32_t mv = moves[i];
        if ((move & 0xFFF00000) == (mv & 0xFFF00000)){
            return true;
        }
    }
    return false;
}

bool Game::makeMove(uint32_t move){
    if (!isValidMove(move)){
        std::cout << "not valid move" << std::endl;
        return false;
    }
    int startSquare = (move & 0xFC000000) >> 26;
    int endSquare = (move & 0x03F00000) >> 20;
    int cl = getColorBySquare(board, startSquare);
    int pc = getPieceBySquare(board, startSquare);
    std::cout << "color of moving piece" << pc << std::endl;
    if (board.whiteToMove){
        if (cl != 1){
            std::cout << "piece type error" << std::endl;
        }
        switch (pc){
            case 0:
                board.whitePawns &= ~(1ULL << startSquare);
                board.whitePawns |= 1ULL << endSquare;
                break;
            case 1:
                board.whiteKnights &= ~(1ULL << startSquare);
                board.whiteKnights |= 1ULL << endSquare;
                break;
            case 2:
                board.whiteBishops &= ~(1ULL << startSquare);
                board.whiteBishops |= 1ULL << endSquare;
                break;
            case 3:
                board.whiteRooks &= ~(1ULL << startSquare);
                board.whiteRooks |= 1ULL << endSquare;
                break;
            case 4:
                board.whiteQueens &= ~(1ULL << startSquare);
                board.whiteQueens |= 1ULL << endSquare;
                break;
            case 5:
                board.whiteKing = 1ULL << endSquare;
                break;
            case 6:
                std::cout << "empty square m8" << std::endl;
                break;
            default:
                std::cout << "what??" << std::endl;
                break;
        }
    } else {
        if (cl != -1){
            std::cout << "what is happening?" << std::endl;
        }
        switch (pc){
            case 0:
                board.blackPawns &= ~(1ULL << startSquare);
                board.blackPawns |= 1ULL << endSquare;
                break;
            case 1:
                board.blackKnights &= ~(1ULL << startSquare);
                board.blackKnights |= 1ULL << endSquare;
                break;
            case 2:
                board.blackBishops &= ~(1ULL << startSquare);
                board.blackBishops |= 1ULL << endSquare;
                break;
            case 3:
                board.blackRooks &= ~(1ULL << startSquare);
                board.blackRooks |= 1ULL << endSquare;
                break;
            case 4:
                board.blackQueens &= ~(1ULL << startSquare);
                board.blackQueens |= 1ULL << endSquare;
                break;
            case 5:
                board.blackKing = 1ULL << endSquare;
                break;
            case 6:
                std::cout << "empty square m8" << std::endl;
                break;
            default:
                std::cout << "what??" << std::endl;
                break;
        }
    }

    int capturedPiece = (0x000E0000 & move) >> 17; 
    std::cout << capturedPiece << " piece" << std::endl;

    // capture

    switch (capturedPiece){
        case 0: 
            board.whiteToMove ? board.blackPawns &= ~(1ULL << endSquare) : board.whitePawns &= ~(1ULL << endSquare);
            break;
        case 1: 
            board.whiteToMove ? board.blackKnights &= ~(1ULL << endSquare) : board.whiteKnights &= ~(1ULL << endSquare);
            break;
        case 2: 
            board.whiteToMove ? board.blackBishops &= ~(1ULL << endSquare) : board.whiteBishops &= ~(1ULL << endSquare);
            break;
        case 3: 
            board.whiteToMove ? board.blackRooks &= ~(1ULL << endSquare) : board.whiteRooks &= ~(1ULL << endSquare);
            break;
        case 4: 
            board.whiteToMove ? board.blackQueens &= ~(1ULL << endSquare) : board.whiteQueens &= ~(1ULL << endSquare);
            break;
        case 5: 
            board.whiteToMove ? board.blackKing &= ~(1ULL << endSquare) : board.whiteKing &= ~(1ULL << endSquare);
            break;
        default: 
            std::cerr << "what in the world is going on??" << std::endl;
    }

    board.whiteToMove = !board.whiteToMove;

    updateBitboards();

    return true;
}

void Game::updateBitboards(){
    board.whitePieces = board.whitePawns | board.whiteKnights | board.whiteBishops | board.whiteRooks | board.whiteQueens | board.whiteKing;
    board.blackPieces = board.blackPawns | board.blackKnights | board.blackBishops | board.blackRooks | board.blackQueens | board.blackKing;
    board.occupiedSquares = board.whitePieces | board.blackPieces; 
}