#ifndef BOARD_H
#define BOARD_H

#include <cstdint> 
#include <vector> 

enum Piece {
    None,
    Pawn, 
    Knight,
    Bishop,
    Rook,
    Queen,
    King
};

enum MoveType {
    Normal,
    Promotion,
    ShortCastle,
    LongCastle, 
    EnPassant
};


struct Move {
    int from;
    int to; 
    MoveType moveType; 
    Piece movedPiece;
    Piece capturedPiece;
    Piece promotedPiece; 
};

struct GameState {
    uint64_t whitePawns;
    uint64_t blackPawns;
    uint64_t whiteKnights;
    uint64_t blackKnights;
    uint64_t whiteBishops;
    uint64_t blackBishops;
    uint64_t whiteRooks; 
    uint64_t blackRooks; 
    uint64_t whiteQueens; 
    uint64_t blackQueens; 
    uint64_t whiteKing; 
    uint64_t blackKing;

    uint64_t whitePieces; 
    uint64_t blackPieces; 

    uint64_t occupiedSquares;

    bool whiteToMove; 

    bool whiteCanCastleShort; 
    bool whiteCanCastleLong; 
    bool blackCanCastleShort; 
    bool blackCanCastleLong; 

    uint64_t enPassantSquare; 

    int halfMoveCounter; 
    int fullMoveCounter; 

    std::vector<uint16_t> moveHistory; 

};

#endif 