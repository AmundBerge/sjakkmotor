#include "../include/game.h"
#include "../include/board.h"
#include "../include/utils.h"


int main(){
    Game game = Game();
    /* printBitboard(game.board.whitePawns);
    printBitboard(game.board.whiteKnights);
    printBitboard(game.board.whiteBishops);
    printBitboard(game.board.whiteRooks);
    printBitboard(game.board.whiteQueens);
    printBitboard(game.board.whiteKing);

    printBitboard(game.board.blackPawns);
    printBitboard(game.board.blackKnights);
    printBitboard(game.board.blackBishops);
    printBitboard(game.board.blackRooks);
    printBitboard(game.board.blackQueens);
    printBitboard(game.board.blackKing);

    printBitboard(game.board.whitePieces);
    printBitboard(game.board.blackPieces);

    printBitboard(game.board.occupiedSquares); */

    /* uint64_t d5_rook = northRay(35) | southRay(35) | eastRay(35) | westRay(35);
    printBitboard(d5_rook);



    uint64_t d5_bishop = northEastRay(35) | northWestRay(35) | southEastRay(35) | southWestRay(35);
    printBitboard(d5_bishop);

    uint64_t d5_queen = d5_rook | d5_bishop;
    printBitboard(d5_queen); */

    initializePieceAttacks();
    for (int i = 0; i < 64; i++){
        printBitboard(pieceAttacks[4][i]);
    }
}