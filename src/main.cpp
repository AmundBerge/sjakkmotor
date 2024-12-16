#include "../include/game.h"
#include "../include/board.h"
#include "../include/utils.h"


int main(){
    Game game = Game();
    printBitboard(game.board.whitePawns);
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
}