#include "../include/movegen.h"
#include "../include/board.h"
#include "../include/game.h"
#include "../include/utils.h"


#include <cstdint>
#include <vector> 
#include <iostream>


bool applyMove(GameState& board, uint32_t move){

    int startSquare = (move & 0xFC000000) >> 26;
    int endSquare = (move & 0x03F00000) >> 20;
    int cl = getColorBySquare(board, startSquare);
    int pc = getPieceBySquare(board, startSquare);

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
                if (startSquare == 7){
                    board.whiteCanCastleShort = false; 
                } else if (startSquare == 0){
                    board.whiteCanCastleLong = false;
                }
                board.whiteRooks &= ~(1ULL << startSquare);
                board.whiteRooks |= 1ULL << endSquare;
                break;
            case 4:
                board.whiteQueens &= ~(1ULL << startSquare);
                board.whiteQueens |= 1ULL << endSquare;
                break;
            case 5:
                board.whiteCanCastleShort = board.whiteCanCastleLong = false; 
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
            printInt32(move);
            printChessBoard(board);
            std::cout << board.whiteToMove << std::endl;
            std::cout << board.whiteBishops << std::endl;
            std::cout << board.blackBishops << std::endl;
            std::cout << board.whiteRooks << std::endl;
            std::cout << board.whitePieces << std::endl;
            
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

                if (startSquare == 63){
                    board.blackCanCastleShort = false; 
                } else if (startSquare == 56){
                    board.blackCanCastleLong = false; 
                }

                board.blackRooks &= ~(1ULL << startSquare);
                board.blackRooks |= 1ULL << endSquare;
                break;
            case 4:
                board.blackQueens &= ~(1ULL << startSquare);
                board.blackQueens |= 1ULL << endSquare;
                break;
            case 5:
                board.blackCanCastleShort = board.blackCanCastleLong = false; 
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
            if (board.whiteToMove){
                if (endSquare == 63){
                    board.blackCanCastleShort = false; 
                } else if (endSquare == 56){
                    board.blackCanCastleLong = false;
                }
            } else {
                if (endSquare == 7){
                    board.whiteCanCastleShort = false;
                } else if (endSquare == 0){
                    board.whiteCanCastleLong = false;
                }
            }
            break;
        case 4: 
            board.whiteToMove ? board.blackQueens &= ~(1ULL << endSquare) : board.whiteQueens &= ~(1ULL << endSquare);
            break;
        case 5: 
        // gir ikke mening aa fjerne kongen
            board.whiteToMove ? board.blackKing &= ~(1ULL << endSquare) : board.whiteKing &= ~(1ULL << endSquare);
            break;
        case 7: 
            break;
        default: 
            std::cerr << "what in the world is going on??" << std::endl;
    }

    if (move & (1u << 12)){
        int promotedPiece = move >> 15 & 0x3;
        switch (promotedPiece){
            case 0:
                board.whiteToMove ? board.whiteKnights |= 1ULL << endSquare : board.blackKnights |= 1ULL << endSquare;
                break;
            case 1:
                board.whiteToMove ? board.whiteBishops |= 1ULL << endSquare : board.blackBishops |= 1ULL << endSquare;
                break;
            case 2:
                board.whiteToMove ? board.whiteRooks |= 1ULL << endSquare : board.blackRooks |= 1ULL << endSquare;
                break;
            case 3:
                board.whiteToMove ? board.whiteQueens |= 1ULL << endSquare : board.blackQueens |= 1ULL << endSquare;
                break;
            default:
                std::cout << "ERROR: FEIL FEIL FEIL HER" << std::endl;
                break;
        }
        if (board.whiteToMove){
            board.whitePawns &= ~(1ULL << endSquare);
        } else {
            board.blackPawns &= ~(1ULL << endSquare);
        } 
    }

    if (move & (1u << 13)){
        if (board.whiteToMove){
            board.blackPawns &= ~(1ULL << (board.enPassantSquare - 8));
        } else {
            board.whitePawns &= ~(1ULL << (board.enPassantSquare + 8));
        }
    }

    board.enPassantSquare = 0;
    if (pc == 0 && std::abs(endSquare - startSquare) == 16){
        board.enPassantSquare = (startSquare + endSquare) / 2;
    }

    if (move == 0x10604000){
        board.whiteRooks &= ~(1ULL << 7);
        board.whiteRooks |= 1ULL << 5;
    }

    if (move == 0x10204000){
        board.whiteRooks &= ~(1ULL);
        board.whiteRooks |= 1ULL << 3;
    }

    if (move == 0xf3e04000){
        board.blackRooks &= ~(1ULL << 63);
        board.blackRooks |= 1ULL << 61;
    }

    if (move == 0xf3a04000){
        board.blackRooks &= ~(1ULL << 56);
        board.blackRooks |= 1ULL << 59;
    }

    if (pc == 0 || capturedPiece != 7){
        board.halfMoveCounter = 0;
    } else {
        board.halfMoveCounter++;
    }

    board.whitePieces = board.whitePawns | board.whiteKnights | board.whiteBishops | board.whiteRooks | board.whiteQueens | board.whiteKing;
    board.blackPieces = board.blackPawns | board.blackKnights | board.blackBishops | board.blackRooks | board.blackQueens | board.blackKing;
    board.occupiedSquares = board.whitePieces | board.blackPieces;

    board.whiteToMove = !board.whiteToMove;

    return true;
}

std::vector<uint32_t> getPlayerMoves(const GameState& board){
    std::vector<uint32_t> moves;
    uint64_t knights; 
    uint64_t bishops; 
    uint64_t rooks; 
    uint64_t queens; 
    uint64_t king;
    if (board.whiteToMove){
        std::vector<uint32_t> pawnMoves = whitePawnMoves(board);
        moves.insert(moves.end(), pawnMoves.begin(), pawnMoves.end());
        knights = board.whiteKnights;
        bishops = board.whiteBishops;
        rooks = board.whiteRooks;
        queens = board.whiteQueens;
        king = board.whiteKing;
    } else {
        std::vector<uint32_t> pawnMoves = blackPawnMoves(board);
        moves.insert(moves.end(), pawnMoves.begin(), pawnMoves.end());
        knights = board.blackKnights;
        bishops = board.blackBishops;
        rooks = board.blackRooks;
        queens = board.blackQueens;
        king = board.blackKing; 
    }
    for (uint64_t b = knights; b != 0; b &= (b - 1)){
        int p = __builtin_ctzll(b);
        std::vector<uint32_t> mvs = knightMoves(board, p);
        moves.insert(moves.end(), mvs.begin(), mvs.end());
    }
    for (uint64_t b = bishops; b != 0; b &= (b - 1)){
        int p = __builtin_ctzll(b);
        std::vector<uint32_t> mvs = bishopMoves(board, p);
        moves.insert(moves.end(), mvs.begin(), mvs.end());
    }
    for (uint64_t b = rooks; b != 0; b &= (b - 1)){
        int p = __builtin_ctzll(b);
        std::vector<uint32_t> mvs = rookMoves(board, p);
        moves.insert(moves.end(), mvs.begin(), mvs.end());
    }
    for (uint64_t b = queens; b != 0; b &= (b - 1)){
        int p = __builtin_ctzll(b);
        std::vector<uint32_t> mvs = queenMoves(board, p);
        moves.insert(moves.end(), mvs.begin(), mvs.end());
    }
    int q = __builtin_ctzll(king);
    std::vector<uint32_t> mvs = kingMoves(board, q);
    moves.insert(moves.end(), mvs.begin(), mvs.end());
    
    return moves;
}

std::vector<uint32_t> getLegalMoves(const GameState& board){
    std::vector<uint32_t> pseudoMoves = getPlayerMoves(board);
    std::vector<uint32_t> legalMoves; 

    for (int i = 0; i < pseudoMoves.size(); i++){
        GameState boardCopy(board);
        uint32_t mv = pseudoMoves[i];
        applyMove(boardCopy, mv);
        uint64_t kingBoard = board.whiteToMove ? boardCopy.whiteKing : boardCopy.blackKing; 
        if (!(kingBoard & attackedSquares(boardCopy, boardCopy.whiteToMove))){
            legalMoves.push_back(mv);
        }
    }

    return legalMoves;
}

std::vector<uint32_t> knightMoves(const GameState& board, int square){
    std::vector<uint32_t> moves;
    int pccl = getColorBySquare(board, square);
    uint64_t ts = pieceAttacks[1][square];
    for (uint64_t bb = ts; bb != 0; bb &= (bb - 1)){
        int sq = __builtin_ctzll(bb);
        int cl = getColorBySquare(board, sq);
        if (pccl == cl){
            continue;
        }
        uint32_t mv = 0x00000000;
        int pc = getPieceBySquare(board, sq);
        if (pc == -1){
            pc = 7; 
        }
        mv = mv & ~(0x3F << 26) | ((square & 0x3F) << 26);
        mv = mv & ~(0x3F << 20) | ((sq & 0x3F) << 20);
        mv = mv & ~(0x7 << 17) | ((pc & 0x7) << 17);
        moves.push_back(mv);
    }
    return moves;
}

uint64_t slidingPieceAttacks(const GameState& board, int piece, int square){ 
    uint64_t ts = pieceAttacks[piece][square];
    for (uint64_t bb = board.occupiedSquares & blockers[piece][square]; bb != 0; bb &= (bb - 1)){
        int target = __builtin_ctzll(bb);
        ts &= ~behind[square][target];
    }
    return ts;
}
// b, r, q moves kan skrives som én funksjon
std::vector<uint32_t> bishopMoves(const GameState& board, int square){ 
    std::vector<uint32_t> moves;
    int pccl = getColorBySquare(board, square);
    uint64_t ts = slidingPieceAttacks(board, 2, square);
    for (uint64_t b = ts; b != 0; b &= (b - 1)){
        int sq = __builtin_ctzll(b);
        int cl = getColorBySquare(board, sq);
        if (pccl == cl){
            continue;
        }
        uint32_t mv = 0x00000000;
        int pc = getPieceBySquare(board, sq);
        if (pc == -1){
            pc = 7; 
        }
        mv = mv & ~(0x3F << 26) | ((square & 0x3F) << 26);
        mv = mv & ~(0x3F << 20) | ((sq & 0x3F) << 20);
        mv = mv & ~(0x7 << 17) | ((pc & 0x7) << 17);
        moves.push_back(mv);
    }
    return moves;
}

std::vector<uint32_t> rookMoves(const GameState& board, int square){
    std::vector<uint32_t> moves;
    int pccl = getColorBySquare(board, square);
    uint64_t ts = slidingPieceAttacks(board, 3, square);
    for (uint64_t b = ts; b != 0; b &= (b - 1)){
        int sq = __builtin_ctzll(b);
        int cl = getColorBySquare(board, sq);
        if (pccl == cl){
            continue;
        }
        uint32_t mv = 0x00000000;
        int pc = getPieceBySquare(board, sq);
        if (pc == -1){
            pc = 7;
        }
        mv = mv & ~(0x3F << 26) | ((square & 0x3F) << 26);
        mv = mv & ~(0x3F << 20) | ((sq & 0x3F) << 20);
        mv = mv & ~(0x7 << 17) | ((pc & 0x7) << 17);
        moves.push_back(mv);
    }
    return moves;
}

// q = b + r
std::vector<uint32_t> queenMoves(const GameState& board, int square){
    std::vector<uint32_t> moves;
    int pccl = getColorBySquare(board, square);
    uint64_t ts = slidingPieceAttacks(board, 4, square);
    for (uint64_t b = ts; b != 0; b &= (b - 1)){
        int sq = __builtin_ctzll(b);
        int cl = getColorBySquare(board, sq);
        if (pccl == cl){
            continue;
        }
        uint32_t mv = 0x00000000;
        int pc = getPieceBySquare(board, sq);
        if (pc == -1){
            pc = 7;
        }
        mv = mv & ~(0x3F << 26) | ((square & 0x3F) << 26);
        mv = mv & ~(0x3F << 20) | ((sq & 0x3F) << 20);
        mv = mv & ~(0x7 << 17) | ((pc & 0x7) << 17);
        moves.push_back(mv);
    }
    return moves;
}

std::vector<uint32_t> kingMoves(const GameState& board, int square){
    std::vector<uint32_t> moves;
    uint64_t kingTargets = pieceAttacks[5][square];
    int pccl = getColorBySquare(board, square);
    for (uint64_t b = kingTargets; b != 0; b &= (b - 1)){
        int sq = __builtin_ctzll(b);
        int cl = getColorBySquare(board, sq);
        if (pccl == cl){
            continue;
        }
        uint32_t mv = 0x00000000;
        int pc = getPieceBySquare(board, sq);
        if (pc == -1){
            pc = 7;
        }
        mv = mv & ~(0x3F << 26) | ((square & 0x3F) << 26);
        mv = mv & ~(0x3F << 20) | ((sq & 0x3F) << 20);
        mv = mv & ~(0x7 << 17) | ((pc & 0x7) << 17);
        moves.push_back(mv);
    }

    if (board.whiteToMove){
        if (board.whiteCanCastleShort){
            if (!(attackedSquares(board, false) & ((1ULL << 4) | (1ULL << 5) | (1ULL << 6))) && 
                (!(board.occupiedSquares & (1ULL << 5) || (board.occupiedSquares & (1ULL << 6))))
            ){
                moves.push_back(0x10604000);
            }
        }
        if (board.whiteCanCastleLong){
            if (!(attackedSquares(board, false) & ((1ULL << 4) | (1ULL << 3) | (1ULL << 2))) && 
                (!(board.occupiedSquares & (1ULL << 3) || (board.occupiedSquares & (1ULL << 2)) || (board.occupiedSquares & (1ULL << 1))))
            ){
                moves.push_back(0x10204000);
            }
        }
    }

    if (!board.whiteToMove){
        if (board.blackCanCastleShort){
            if (!(attackedSquares(board, true) & ((1ULL << 60) | (1ULL << 61) | (1ULL << 62))) &&
                (!(board.occupiedSquares & (1ULL << 61) || (board.occupiedSquares & (1ULL << 62))))
            ){
                moves.push_back(0xf3e04000);
            }
        }
        if (board.blackCanCastleLong){
            if (!(attackedSquares(board, true) & ((1ULL << 60) | (1ULL << 59) | (1ULL << 58))) &&
                (!(board.occupiedSquares & (1ULL << 59) || (board.occupiedSquares & (1ULL << 58)) || (board.occupiedSquares & (1ULL << 57))))
            ){
                moves.push_back(0xf3a04000);
            }
        }
    }
    return moves;
}

uint64_t whiteSinglePawnPushTargets(const GameState& board){
    return (board.whitePawns << 8) & ~board.occupiedSquares;
}

uint64_t whiteDoublePawnPushTargets(const GameState& board){
    const uint64_t rank4 = 0x00000000FF000000;
    uint64_t singlePushes = whiteSinglePawnPushTargets(board);
    return (singlePushes << 8) & ~board.occupiedSquares & rank4;
}

uint64_t blackSinglePawnPushTargets(const GameState& board){
    return (board.blackPawns >> 8) & ~board.occupiedSquares;
}

uint64_t blackDoublePawnPushTargets(const GameState& board){
    const uint64_t rank5 = 0x000000FF00000000;
    uint64_t singlePushes = blackSinglePawnPushTargets(board);
    return (singlePushes >> 8) & ~board.occupiedSquares & rank5;
}

uint64_t whitePawnsLeftAttackTargets(const GameState& board){
    return (board.whitePawns << 7) & 0x7F7F7F7F7F7F7F7F;
}

uint64_t whitePawnsRightAttackTargets(const GameState& board){
    return (board.whitePawns << 9) & 0xFEFEFEFEFEFEFEFE;
}

uint64_t blackPawnsLeftAttackTargets(const GameState& board){
    return (board.blackPawns >> 7) & 0xFEFEFEFEFEFEFEFE;
}

uint64_t blackPawnsRightAttackTargets(const GameState& board){
    return (board.blackPawns >> 9) & 0x7F7F7F7F7F7F7F7F;
}

std::vector<uint32_t> whitePawnMoves(const GameState& board){
    uint64_t singlePushes = whiteSinglePawnPushTargets(board);
    uint64_t doublePushes = whiteDoublePawnPushTargets(board);
    uint64_t whitePawnsLeft = whitePawnsLeftAttackTargets(board);
    uint64_t whitePawnsRight = whitePawnsRightAttackTargets(board);

    std::vector<uint32_t> moves;

    for (uint64_t b = singlePushes; b != 0; b &= (b - 1)){
        uint32_t mv = 0x00000000;
        int p = __builtin_ctzll(b);
        mv = mv & ~(0x3F << 26) | (((p - 8) & 0x3F) << 26);
        mv = mv & ~(0x3F << 20) | ((p & 0x3F) << 20);
        mv = mv & ~(0x7 << 17) | ((7 & 0x7) << 17);
        //kan skrive blokken under bedre
        if (p >= 56){
            for (int i = 0; i < 4; i++){
                uint32_t m = mv | (((i) & 0x3) << 15);
                m |= (1u << 12); 
                moves.push_back(m);
            }
        } else {
            moves.push_back(mv);
        }
    }

    for (uint64_t b = doublePushes; b != 0; b &= (b - 1)){
        uint32_t mv = 0x00000000;
        int p = __builtin_ctzll(b);
        mv = mv & ~(0x3F << 26) | (((p - 16) & 0x3F) << 26);
        mv = mv & ~(0x3F << 20) | ((p & 0x3F) << 20);
        mv = mv & ~(0x7 << 17) | ((7 & 0x7) << 17);
        moves.push_back(mv);
    }

    for (uint64_t b = whitePawnsLeft; b != 0; b &= (b - 1)){
        int p = __builtin_ctzll(b);
        if (p == board.enPassantSquare && board.enPassantSquare != 0){
            uint32_t mv = 0x00000000;
            mv = mv & ~(0x3F << 26) | (((p - 7) & 0x3F) << 26);
            mv = mv & ~(0x3F << 20) | ((p & 0x3F) << 20);
            mv = mv & ~(0x7 << 17) | ((7 & 0x7) << 17);
            mv |= (1u << 13);
            moves.push_back(mv);
            continue;
        }
        int cl = getColorBySquare(board, p);
        if (cl >= 0){
            continue;
        }
        int pc = getPieceBySquare(board, p);
        uint32_t mv = 0x00000000;
        mv = mv & ~(0x3F << 26) | (((p - 7) & 0x3F) << 26);
        mv = mv & ~(0x3F << 20) | ((p & 0x3F) << 20);
        mv = mv & ~(0x7 << 17) | ((pc & 0x7) << 17);
        //kan skrive blokken under bedre
        if (p >= 56){
            for (int i = 0; i < 4; i++){
                uint32_t m = mv | (((i) & 0x3) << 15);
                m |= (1u << 12); 
                moves.push_back(m);
            }
        } else {
            moves.push_back(mv);
        }
    }

    for (uint64_t b = whitePawnsRight; b != 0; b &= (b - 1)){
        int p = __builtin_ctzll(b);
        if (p == board.enPassantSquare && board.enPassantSquare != 0){
            uint32_t mv = 0x00000000;
            mv = mv & ~(0x3F << 26) | (((p - 9) & 0x3F) << 26);
            mv = mv & ~(0x3F << 20) | ((p & 0x3F) << 20);
            mv = mv & ~(0x7 << 17) | ((7 & 0x7) << 17);
            mv |= (1u << 13);
            moves.push_back(mv);
            continue;
        }
        int cl = getColorBySquare(board, p);
        if (cl >= 0){
            continue;
        }
        int pc = getPieceBySquare(board, p);
        uint32_t mv = 0x00000000;
        mv = mv & ~(0x3F << 26) | (((p - 9) & 0x3F) << 26);
        mv = mv & ~(0x3F << 20) | ((p & 0x3F) << 20);
        mv = mv & ~(0x7 << 17) | ((pc & 0x7) << 17);
        //kan skrive blokken under bedre
        if (p >= 56){
            for (int i = 0; i < 4; i++){
                uint32_t m = mv | (((i) & 0x3) << 15);
                m |= (1u << 12); 
                moves.push_back(m);
            }
        } else {
            moves.push_back(mv);
        }
    }

    return moves;
}

std::vector<uint32_t> blackPawnMoves(const GameState& board){
    uint64_t singlePushes = blackSinglePawnPushTargets(board);
    uint64_t doublePushes = blackDoublePawnPushTargets(board);
    uint64_t blackPawnsLeft = blackPawnsLeftAttackTargets(board);
    uint64_t blackPawnsRight = blackPawnsRightAttackTargets(board);

    std::vector<uint32_t> moves;

    for (uint64_t b = singlePushes; b != 0; b &= (b - 1)){
        uint32_t mv = 0x00000000;
        int p = __builtin_ctzll(b);
        mv = mv & ~(0x3F << 26) | (((p + 8) & 0x3F) << 26);
        mv = mv & ~(0x3F << 20) | ((p & 0x3F) << 20);
        mv = mv & ~(0x7 << 17) | ((7 & 0x7) << 17);
        //kan skrive blokken under bedre
        if (p < 8){
            for (int i = 0; i < 4; i++){
                uint32_t m = mv | (((i) & 0x3) << 15);
                m |= (1u << 12);
                moves.push_back(m);
            }
        } else {
            moves.push_back(mv);
        }
    }

    for (uint64_t b = doublePushes; b != 0; b &= (b - 1)){
        uint32_t mv = 0x00000000;
        int p = __builtin_ctzll(b);
        mv = mv & ~(0x3F << 26) | (((p + 16) & 0x3F) << 26);
        mv = mv & ~(0x3F << 20) | ((p & 0x3F) << 20);
        mv = mv & ~(0x7 << 17) | ((7 & 0x7) << 17);
        moves.push_back(mv);
    }

    for (uint64_t b = blackPawnsLeft; b != 0; b &= (b - 1)){
        int p = __builtin_ctzll(b);
        if (p == board.enPassantSquare && board.enPassantSquare != 0){
            uint32_t mv = 0x00000000;
            mv = mv & ~(0x3F << 26) | (((p + 7) & 0x3F) << 26);
            mv = mv & ~(0x3F << 20) | ((p & 0x3F) << 20);
            mv = mv & ~(0x7 << 17) | ((7 & 0x7) << 17);
            mv |= (1u << 13);
            moves.push_back(mv);
            continue;
        }
        int cl = getColorBySquare(board, p);
        if (cl <= 0){
            continue;
        }
        int pc = getPieceBySquare(board, p);
        uint32_t mv = 0x00000000;
        mv = mv & ~(0x3F << 26) | (((p + 7) & 0x3F) << 26);
        mv = mv & ~(0x3F << 20) | ((p & 0x3F) << 20);
        mv = mv & ~(0x7 << 17) | ((pc & 0x7) << 17);
        //kan skrive blokken under bedre
        if (p < 8){
            for (int i = 0; i < 4; i++){
                uint32_t m = mv | (((i) & 0x3) << 15);
                m |= (1u << 12); 
                moves.push_back(m);
            }
        } else {
            moves.push_back(mv);
        }
    }

    for (uint64_t b = blackPawnsRight; b != 0; b &= (b - 1)){
        int p = __builtin_ctzll(b);
        if (p == board.enPassantSquare && board.enPassantSquare != 0){
            uint32_t mv = 0x00000000;
            mv = mv & ~(0x3F << 26) | (((p + 9) & 0x3F) << 26);
            mv = mv & ~(0x3F << 20) | ((p & 0x3F) << 20);
            mv = mv & ~(0x7 << 17) | ((7 & 0x7) << 17);
            mv |= (1u << 13);
            moves.push_back(mv);
            continue;
        }
        int cl = getColorBySquare(board, p);
        if (cl <= 0){
            continue;
        }
        int pc = getPieceBySquare(board, p);
        uint32_t mv = 0x00000000;
        mv = mv & ~(0x3F << 26) | (((p + 9) & 0x3F) << 26);
        mv = mv & ~(0x3F << 20) | ((p & 0x3F) << 20);
        mv = mv & ~(0x7 << 17) | ((pc & 0x7) << 17);
        //kan skrive blokken under bedre
        if (p < 8){
            for (int i = 0; i < 4; i++){
                uint32_t m = mv | (((i) & 0x3) << 15);
                m |= (1u << 12); 
                moves.push_back(m);
            }
        } else {
            moves.push_back(mv);
        }
    }

    return moves;
}

uint64_t attackedSquares(const GameState& board, bool whiteToMove){
    uint64_t targets = 0x00000000;
    uint64_t knights; 
    uint64_t bishops; 
    uint64_t rooks; 
    uint64_t queens; 
    if (whiteToMove){
        targets |= whitePawnsLeftAttackTargets(board);
        targets |= whitePawnsRightAttackTargets(board);
        knights = board.whiteKnights;
        bishops = board.whiteBishops; 
        rooks = board.whiteRooks; 
        queens = board.whiteQueens; 
        targets |= pieceAttacks[5][__builtin_ctzll(board.whiteKing)];
    } else {
        targets |= blackPawnsLeftAttackTargets(board);
        targets |= blackPawnsRightAttackTargets(board);
        knights = board.blackKnights; 
        bishops = board.blackBishops;
        rooks = board.blackRooks; 
        queens = board.blackQueens; 
        targets |= pieceAttacks[5][__builtin_ctzll(board.blackKing)];
    }
    for (uint64_t bb = knights; bb != 0; bb &= (bb - 1)){
        int sq = __builtin_ctzll(bb);
        targets |= pieceAttacks[1][sq];
    }
    for (uint64_t bb = bishops; bb != 0; bb &= (bb - 1)){
        int sq = __builtin_ctzll(bb);
        targets |= slidingPieceAttacks(board, 2, sq);
    }
    for (uint64_t bb = rooks; bb != 0; bb &= (bb - 1)){
        int sq = __builtin_ctzll(bb);
        targets |= slidingPieceAttacks(board, 3, sq);
    }
    for (uint64_t bb = queens; bb != 0; bb &= (bb - 1)){
        int sq = __builtin_ctzll(bb);
        targets |= slidingPieceAttacks(board, 4, sq);
    }
    return targets; 
}

bool checkPositionEquality(const Position& pos1, const Position& pos2){
    return pos1.whitePawns == pos2.whitePawns && pos1.whiteKnights == pos2.whiteKnights && pos1.whiteBishops == pos2.whiteBishops &&
    pos1.whiteRooks == pos2.whiteRooks && pos1.whiteQueens == pos2.whiteQueens && pos1.whiteKing == pos2.whiteKing &&
    pos1.blackPawns == pos2.blackPawns && pos1.blackKnights == pos2.blackKnights && pos1.blackBishops == pos2.blackBishops &&
    pos1.blackRooks == pos2.blackRooks && pos1.blackQueens == pos2.blackQueens && pos1.blackKing == pos2.blackKing &&
    pos1.whiteCanCastleShort == pos2.whiteCanCastleShort && pos1.whiteCanCastleLong == pos2.whiteCanCastleLong &&
    pos1.blackCanCastleShort == pos2.blackCanCastleShort && pos1.blackCanCastleLong == pos2.blackCanCastleLong &&
    pos1.whiteToMove == pos2.whiteToMove && pos1.enPassantSquare == pos2.enPassantSquare;
}
// grov kode, huske å skifte til zobrist-hashing
bool checkThreefoldRepetition(const GameState& board){
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

    int count = 0; 
    for (int i = 0; i < board.positionHistory.size(); i++){
        if (checkPositionEquality(position, board.positionHistory[i])){
            count++;
        }
    }
    return count >= 3;
}

GameResult getGameResult(const GameState& board){
    std::vector<uint32_t> legalMoves = getLegalMoves(board);
    if (legalMoves.empty()){
        uint64_t kingBoard = board.whiteToMove ? board.whiteKing : board.blackKing;
        if (attackedSquares(board, !board.whiteToMove) & kingBoard) return CHECKMATE;
        return STALEMATE;
    }
    if (board.halfMoveCounter >= 100) return MOVE50; // maa sjekke litt mer opp i offisielle regler, bedre kode aa ha denne
    // foerst, men sjakkmatt paa trekk #100 skal vaere sjakkmatt, ikke remis.
    if (checkThreefoldRepetition(board)) return THREEFOLD_REPETITION; 
    return ACTIVE;
}