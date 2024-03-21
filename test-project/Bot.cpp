#include "Bot.h"
#include <bits/stdc++.h>

using namespace std;

#define PAWN_VALUE 100
#define KNIGHT_VALUE 310
#define BISHOP_VALUE 320
#define ROOK_VALUE 500
#define QUEEN_VALUE 900
#define KING_VALUE 20000

const std::string Bot::BOT_NAME = "Maurice"; /* Edit this, escaped characters are forbidden */

/* RANK = LINE              FILE = COLUMN */

Bot::Bot(bool _startGame)
    :startGame(_startGame) {}

struct state {
    int board_game[8][8] = {
        {r, n, b, q, k, b, n, r},
        {p, p, p, p, p, p, p, p},
        {e, e, e, e, e, e, e, e},
        {e, e, e, e, e, e, e, e},
        {e, e, e, e, e, e, e, e},
        {e, e, e, e, e, e, e, e},
        {P, P, P, P, P, P, P, P},
        {R, N, B, Q, K, B, N, R},
    };

    bool promoted_pieces[8][8] = {
        {false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false},
    };

    Move* blackLastMove;
    Move* whiteLastMove;

    int kingWhiteFile;
    int kingWhiteRank;
    int kingBlackFile;
    int kingBlackRank;

    bool whiteKingMoved;
    bool blackKingMoved;
    bool whiteLeftRookMoved;
    bool blackLeftRookMoved;
    bool whiteRightRookMoved;
    bool blackRightRookMoved;

    std::vector<enum pieces> capturedWhitePieces;
    std::vector<enum pieces> capturedBlackPieces;

    /* negamax necesities */
    PlaySide sideTmp;
    int value;
    Move *move;
};

struct state botState;

string Bot::convertFileAndRank(int file_idx, int rank_idx)
{
    string coordinate;
    map<int, char> fileToIndex;
    fileToIndex.insert({0, 'a'});
    fileToIndex.insert({1, 'b'});
    fileToIndex.insert({2, 'c'});
    fileToIndex.insert({3, 'd'});
    fileToIndex.insert({4, 'e'});
    fileToIndex.insert({5, 'f'});
    fileToIndex.insert({6, 'g'});
    fileToIndex.insert({7, 'h'});

    auto it = fileToIndex.find(file_idx);
    coordinate += it->second;
    rank_idx++;
    string rank = to_string(rank_idx);
    coordinate += rank;

    return coordinate;
}

void Bot::generateEnPassantMoves(vector<Move *> &possibleMoves, int rank, int file, PlaySide side, struct state currentState)
{   Move *enemyMove;

    // ofstream fout;
    // fout.open("drop.out", std::ios_base::app);
    // fout << "GENERATE ENPASSANTTTTTTTTTTTTTTT" << endl;

    enemyMove = currentState.whiteLastMove;
    if (side == WHITE) {
        enemyMove = currentState.blackLastMove;
        // fout << "WHITE" << endl;
    }
    // fout << "AICI INTRA" << endl;

    // if (enemyMove == NULL)
    //     fout << "enemy is null" << endl;
    // if (currentState.blackLastMove == NULL)
    //     fout << "black last move is null" << endl;
    // if (currentState.whiteLastMove == NULL)
    //     fout << "white last move is null" << endl;

    // fout << enemyMove->getDestination().has_value() << "\n";
    // fout.flush();
    // fout << "is promotion: " << enemyMove->isPromotion() << endl;
    // fout << "is drop in: " << enemyMove->isDropIn() << endl;

    /* if last move of enemy is not normal move continue */
    if (enemyMove->isPromotion() || enemyMove->isDropIn())
        return;

    // fout << "DADADADA //////" << endl;
    // fout << enemyMove->getDestination().value() << enemyMove->getSource().value() << endl;
    int srcRank = 8 - (enemyMove->getSource().value()[1] - '0');
    // fout << "srcRank: " << srcRank << endl;
    int destRank = 8 - (enemyMove->getDestination().value()[1] - '0');
    // fout << "destRank: " << destRank << endl;
    int destFile = enemyMove->getDestination().value()[0] - 'a';
    // fout << "destFile: " << destFile << endl;
    // fout << "SI AICI INTRA" << endl;

    // if (!enemyMove->isDropIn())
    //     fout << "ENEMY LAST MOVE: " << enemyMove->getSource().value() << " " << enemyMove->getDestination().value() << endl;
    // else
    //     fout << "ENEMY LAST MOVE: " << enemyMove->getDestination().value() << " " << enemyMove->getReplacement().value() << endl;

    if (side == WHITE && currentState.board_game[destRank][destFile] == p && srcRank - destRank == -2) {
        if (currentState.board_game[rank][file] == P && destFile == file - 1 &&
            currentState.board_game[rank - 1][file - 1] == e)
            possibleMoves.push_back(Move::moveTo(convertFileAndRank(file, 7 - rank),
                                    convertFileAndRank(file - 1, 7 - (rank - 1))));
        if (currentState.board_game[rank][file] == P && destFile == file + 1 &&
            currentState.board_game[rank - 1][file + 1] == e)
            possibleMoves.push_back(Move::moveTo(convertFileAndRank(file, 7 - rank), 
                                    convertFileAndRank(file + 1, 7 - (rank - 1))));
    }

    if (side == BLACK && currentState.board_game[destRank][destFile] == P && srcRank - destRank == 2) {
        if (currentState.board_game[rank][file] == p && destFile == file - 1 &&
            currentState.board_game[rank + 1][file - 1] == e)
            possibleMoves.push_back(Move::moveTo(convertFileAndRank(file, 7 - rank),
                                    convertFileAndRank(file - 1, 7 - (rank + 1))));
        if (currentState.board_game[rank][file] == p && destFile == file + 1 &&
            currentState.board_game[rank + 1][file + 1] == e)
            possibleMoves.push_back(Move::moveTo(convertFileAndRank(file, 7 - rank), 
                                    convertFileAndRank(file + 1, 7 - (rank + 1))));
    }
}

void Bot::generatePromotionMoves(vector<Move *> &possibleMoves, int rank, int file, PlaySide side, struct state currentState) {
    //////// DEBUG ////////
    // ofstream fout;
    // fout.open("drop.out", std::ios_base::app);
    // fout << "////// GENERATE PROMOTION MOVES //////\n";
    // fout.close();
    //////////////////////

    /* check forward move */
    if (isValidMove(rank - 1, file) && side == WHITE && currentState.board_game[rank - 1][file] == e)
        for (enum Piece i : {QUEEN, ROOK, BISHOP, KNIGHT})
            possibleMoves.push_back(Move::promote(convertFileAndRank(file, 7 - rank),
                                    convertFileAndRank(file, 7 - (rank - 1)), i));
    if (isValidMove(rank + 1, file) && side == BLACK && currentState.board_game[rank + 1][file] == e)
        for (enum Piece i : {QUEEN, ROOK, BISHOP, KNIGHT})
            possibleMoves.push_back(Move::promote(convertFileAndRank(file, 7 - rank),
                                    convertFileAndRank(file, 7 - (rank + 1)), i));

    /* check diagonal move */
    if (isValidMove(rank - 1, file - 1) && side == WHITE && currentState.board_game[rank - 1][file - 1] != e && currentState.board_game[rank - 1][file - 1] <= p)
        for (enum Piece i : {QUEEN, ROOK, BISHOP, KNIGHT})
            possibleMoves.push_back(Move::promote(convertFileAndRank(file, 7 - rank), 
                                    convertFileAndRank(file - 1, 7 - (rank - 1)), i));
    if (isValidMove(rank + 1, file - 1) && side == BLACK && currentState.board_game[rank + 1][file - 1] != e && currentState.board_game[rank + 1][file - 1] >= R)
        for (enum Piece i : {QUEEN, ROOK, BISHOP, KNIGHT})
            possibleMoves.push_back(Move::promote(convertFileAndRank(file, 7 - rank), 
                                    convertFileAndRank(file - 1, 7 - (rank + 1)), i));
    if (isValidMove(rank - 1, file + 1) && side == WHITE && currentState.board_game[rank - 1][file + 1] != e && currentState.board_game[rank - 1][file + 1] <= p)
        for (enum Piece i : {QUEEN, ROOK, BISHOP, KNIGHT})
            possibleMoves.push_back(Move::promote(convertFileAndRank(file, 7 - rank), 
                                    convertFileAndRank(file + 1, 7 - (rank - 1)), i));
    if (isValidMove(rank + 1, file + 1) && side == BLACK && currentState.board_game[rank + 1][file + 1] != e && currentState.board_game[rank + 1][file + 1] >= R)
        for (enum Piece i : {QUEEN, ROOK, BISHOP, KNIGHT})
            possibleMoves.push_back(Move::promote(convertFileAndRank(file, 7 - rank), 
                                    convertFileAndRank(file + 1, 7 - (rank + 1)), i));
}

void Bot::simulateRecordMove(int& auxDest, Move* move, PlaySide sideToMove, int board[8][8]) {
    int srcRank, srcFile;
    if (!move->isDropIn()) {
        srcRank = 8 - (move->getSource().value()[1] - '0');
        srcFile = move->getSource().value()[0] - 'a';
    }
    int destRank = 8 - (move->getDestination().value()[1] - '0');
    int destFile = move->getDestination().value()[0] - 'a';

    // ofstream fout;
    // fout.open("drop.out", std::ios_base::app);
    // fout << "SIMULATING MOVE" << endl;
    // fout << "SIMULATING RECORD MOVE: ENPASSANT FLAG = " << enPassant << " CASTLING FLAG = " << castling << endl;
    // fout << "MOVE" << move->destination.value() << endl;
    // fout << "DEST: " << destRank << " " << destFile << endl;

    /* Normal move */
    if (move->isNormal()) {
        /* en passant move */
        // fout << "SIMULATE NORMAL MOVE" << endl;
        if (isEnPassant(srcRank, srcFile, destRank, destFile, board)) {
            // fout << "SIMULATE ENPASSANT MOVE" << endl;
            if (sideToMove == WHITE) {
                /* move pawn */
                board[destRank][destFile] = board[srcRank][srcFile];
                board[srcRank][srcFile] = e;
                /* remove captured pawn */
                board[destRank + 1][destFile] = e;
            } else {
                /* move pawn */
                board[destRank][destFile] = board[srcRank][srcFile];
                board[srcRank][srcFile] = e;
                /* remove captured pawn */
                board[destRank - 1][destFile] = e;
            }
        } else if (isCastling(srcRank, srcFile, destRank, destFile, board)) {
            /* castling move */
            // fout << "SIMULATE CASTLING MOVE" << endl;

            /* castling move */
            if (sideToMove == WHITE) {
                if (destFile == 6) {
                    /* move rook: small castling */
                    board[7][5] = board[7][7];
                    board[7][7] = e;
                    /* move king: small casling */
                    board[7][6] = board[7][4];
                    board[7][4] = e;
                } else {
                    /* move rook: big castling */
                    board[7][3] = board[7][0];
                    board[7][0] = e;
                    /* move king: big castling */
                    board[7][2] = board[7][4];
                    board[7][4] = e;
                }
            } else {
                /* black side of the board */
                if (destFile == 6) {
                    /* move rook: small castling */
                    board[0][5] = board[0][7];
                    board[0][7] = e;
                    /* move king: small castling */
                    board[0][6] = board[0][4];
                    board[0][4] = e;
                } else {
                    /* move rook: big castling */
                    board[0][3] = board[0][0];
                    board[0][0] = e;
                    /* move king:big castling */
                    board[0][2] = board[0][4];
                    board[0][4] = e;
                }
            }
        } else {
            /* capturing or empty square move */
            // fout << "SIMULATING CAPTURING MOVE" << endl;
            board[destRank][destFile] = board[srcRank][srcFile];
            board[srcRank][srcFile] = e;
        }
    } else if (move->isPromotion()) {
        /* Promotion move */
        // fout << "SIMULATING PROMOTION MOVE" << endl;
        // fout << convertPiece(move->getReplacement().value(), sideToMove) << endl;
        board[destRank][destFile] = convertPiece(move->getReplacement().value(), sideToMove);
        board[srcRank][srcFile] = e;
    } else if (move->isDropIn()) {
        /* Drop-in move */
        // fout << "SIMULATING DROP-IN MOVE" << endl;
        board[destRank][destFile] = convertPiece(move->getReplacement().value(), sideToMove);
    }

    // fout << "SIMULATING RECORD MOVE ENDING: ENPASSANT FLAG = " << enPassant << " CASTLING FLAG = " << castling << endl;
}

string getPiece(enum pieces piece);

void printBoard(int board_game[8][8]) {
    ofstream fout;
    fout.open("drop.out", std::ios_base::app);

    /* printing board */
    fout << "\n";
    fout<<"printing board:\n";
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            fout << getPiece((enum pieces)board_game[i][j]) << " " << flush;
        }
        fout << "\n";
    }
    fout << endl;
}

bool Bot::isKingInCheck(Move *move, bool makeMove, struct state currentState, PlaySide side) {
    //////// DEBUG ////////
    // ofstream fout;
    // fout.open("drop.out", std::ios_base::app);
    // fout << "////// IS KING IN CHECK MOVES //////\n";
    //////////////////////

    /* get source and destination positions of move */
    int srcRank, srcFile;
    if (!move->isDropIn()) {
        srcRank = 8 - (move->getSource().value()[1] - '0');
        srcFile = move->getSource().value()[0] - 'a';
    }
    int destRank = 8 - (move->getDestination().value()[1] - '0');
    int destFile = move->getDestination().value()[0] - 'a';

    // if (move->isDropIn())
    //     fout << "drop in move " << move->getDestination().value() << move->getReplacement().value() << endl;
    // else if (move->isNormal())
    //     fout << "normal move " << move->getSource().value() << move->getDestination().value() << endl;
    // else if (move->isPromotion())
    //     fout << "promotion move " << move->getSource().value() << move->getDestination().value() <<
                                    // move->getReplacement().value() << endl;

    /* make move on separate board */
    int board[8][8];
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            board[i][j] = currentState.board_game[i][j];
        }
    }

    int auxDest = e;
    if (makeMove) {
        simulateRecordMove(auxDest, move, sideToMove, board);
    }

    // fout << "auxDest in simulation " << auxDest << endl;
    // fout << "table with in simulation" << endl;
    // printBoard(board);

    int newKingFile;
    int newKingRank;

    /* get current king position */
    if (side == WHITE) {
        newKingFile = currentState.kingWhiteFile;
        newKingRank = currentState.kingWhiteRank;
        // fout << "king position before move: " << currentState.kingWhiteRank << " " << currentState.kingWhiteFile << endl;
    } else {
        newKingFile = currentState.kingBlackFile;
        newKingRank = currentState.kingBlackRank;
        // fout << "king position before move: " << currentState.kingBlackRank << " " << currentState.kingBlackFile << endl;
    }

    /* update king position */
    if (!move->isDropIn() && srcFile == newKingFile && srcRank == newKingRank && makeMove) {
        newKingRank = destRank;
        newKingFile = destFile;
    }
    // fout << "new king position: " << newKingRank << " " << newKingFile << endl;

    /* check diagonal */
    vector<int> di = {-1, -1, 1, 1};
    vector<int> dj = {-1, 1, -1, 1};

    for (int i = 0; i < 4; ++i) {
        int newRank = newKingRank + di[i];
        int newFile = newKingFile + dj[i];

        /* check for queen and bishop */
        while (isValidMove(newRank, newFile)) {
            if (board[newRank][newFile] == e) {
                newRank += di[i];
                newFile += dj[i];
            } else if ((side == WHITE && (board[newRank][newFile] == q ||
                        board[newRank][newFile] == b)) ||
                    (side == BLACK && (board[newRank][newFile] == Q ||
                     board[newRank][newFile] == B))) {

                return true;
            } else {
                break;
            }
        }
    }
    // fout << "checked diagonal" << endl;

    /* check cross */
    di = {-1, 0, 0, 1};
    dj = {0, -1, 1, 0};

    for (int i = 0; i < 4; ++i) {
        int newRank = newKingRank + di[i];
        int newFile = newKingFile + dj[i];

        /* check for queen and rook */
        while (isValidMove(newRank, newFile)) {
            if (board[newRank][newFile] == e) {
                newRank += di[i];
                newFile += dj[i];

                // fout << "checking for rook and queen: " << newRank << " " << newFile << endl;
            } else if ((side == WHITE && (board[newRank][newFile] == q ||
                        board[newRank][newFile] == r)) ||
                    (side == BLACK && (board[newRank][newFile] == Q ||
                     board[newRank][newFile] == R))) {
                // fout << "found rook or queen" << endl;

                return true;
            } else {
                break;
            }
        }
    }
    // fout << "checked cross" << endl;

    /* check horse moves */
    di = {-2, -2, -1, -1, 1, 1, 2, 2};
    dj = {-1, 1, -2, 2, -2, 2, -1, 1};

    for (int i = 0; i < 8; ++i) {
        int newRank = newKingRank + di[i];
        int newFile = newKingFile + dj[i];

        if (isValidMove(newRank, newFile)) {
            if ((side == WHITE && (board[newRank][newFile] == n)) ||
                (side == BLACK && (board[newRank][newFile] == N))) {

                return true;
            }
        }
    }
    // fout << "checked horse" << endl;

    /* check pawn moves */
    vector<int> diWhite = {-1, -1};
    vector<int> djWhite = {-1, 1};
    di = {1, 1};
    dj = {-1, 1};

    if (side == WHITE) {
        di = diWhite;
        dj = djWhite;
    }

    for (int i = 0; i < 2; ++i) {
        int newRank = newKingRank + di[i];
        int newFile = newKingFile + dj[i];

        if (isValidPawnMove(newRank)) {
            if ((side == WHITE && (board[newRank][newFile] == p)) ||
                (side == BLACK && (board[newRank][newFile] == P))) {

                return true;
            }
        }
    }
    // fout << "checked pawn" << endl;

    /* check king moves */
    di = {-1, -1, -1, 0, 0, 1, 1, 1};
    dj = {-1, 0, 1, -1, 1, -1, 0, 1};

    for (int i = 0; i < 8; ++i) {
        int newRank = newKingRank + di[i];
        int newFile = newKingFile + dj[i];

        if (isValidMove(newRank, newFile)) {
            if ((side == WHITE && (board[newRank][newFile] == k)) ||
                (side == BLACK && (board[newRank][newFile] == K))) {

                return true;
            }
        }
    }
    // fout << "checked king" << endl;


    /* check king position */
    // fout << "KING POSITION CHECK AFTER UNDO MOVE" << endl;
    // if (side == WHITE) {
    //     fout << "white king position: " << currentState.kingWhiteRank << " " << currentState.kingWhiteFile << endl;
    // } else {
    //     fout << "black king position: " << currentState.kingBlackRank << " " << currentState.kingBlackFile << endl;
    // }
    // fout.close();
    return false;
}

void Bot::generateCastlingMoves(vector<Move *> &possibleMoves, PlaySide side, struct state currentState)
{
    //////// DEBUG ////////
    // ofstream fout;
    // fout.open("drop.out", std::ios_base::app);
    // fout << "////// GENERATE CASTLING MOVES //////\n";
    // fout.close();
    //////////////////////

    /* white castling moves */
    if (side == WHITE) {
        /* check for big castling: squares are empty and king is not in check */
        std::optional<std::string> src = convertFileAndRank(currentState.kingWhiteFile, 7 - currentState.kingWhiteRank);
        if (currentState.whiteKingMoved == false && currentState.whiteLeftRookMoved == false) {
            if (currentState.board_game[7][1] == e && currentState.board_game[7][2] == e && currentState.board_game[7][3] == e &&
                !isKingInCheck(Move::moveTo(src, convertFileAndRank(3, 7 - 7)), true, currentState, side) &&
                !isKingInCheck(Move::moveTo(src, convertFileAndRank(2, 7 - 7)), false, currentState, side)) {
                possibleMoves.push_back(Move::moveTo(src, convertFileAndRank(2, 7 - 7)));
            }
        }

        /* check for small castling: squares are empty and king is not in check */
        if (currentState.whiteKingMoved == false && currentState.whiteRightRookMoved == false) {
            if (currentState.board_game[7][5] == e && currentState.board_game[7][6] == e &&
                !isKingInCheck(Move::moveTo(src, convertFileAndRank(5, 7 - 7)), true, currentState, side) &&
                !isKingInCheck(Move::moveTo(src, convertFileAndRank(6, 7 - 7)), false, currentState, side)) {
                possibleMoves.push_back(Move::moveTo(convertFileAndRank(currentState.kingWhiteFile,
                                        7 - currentState.kingWhiteRank), convertFileAndRank(6, 7 - 7)));
            }
        }
    } else {
        /* black castling moves */
        std::optional<std::string> src = convertFileAndRank(currentState.kingBlackFile, 7 - currentState.kingBlackRank);
        if (currentState.blackKingMoved == false && currentState.blackLeftRookMoved == false) {
            /* check for big castling: squares are empty and king is not in check */
            if (currentState.board_game[0][1] == e && currentState.board_game[0][2] == e && currentState.board_game[0][3] == e &&
                !isKingInCheck(Move::moveTo(src, convertFileAndRank(3, 7 - 0)), true, currentState, side) &&
                !isKingInCheck(Move::moveTo(src, convertFileAndRank(2, 7 - 0)), false, currentState, side)) {
                possibleMoves.push_back(Move::moveTo(src, convertFileAndRank(2, 7 - 0)));
            }
        }

        /* check for small castling: squares are empty and king is not check */
        if (currentState.blackKingMoved == false && currentState.blackRightRookMoved == false) {
            if (currentState.board_game[0][5] == e && currentState.board_game[0][6] == e && 
                !isKingInCheck(Move::moveTo(src, convertFileAndRank(5, 7 - 0)), true, currentState, side) &&
                !isKingInCheck(Move::moveTo(src, convertFileAndRank(6, 7 - 0)), false, currentState, side)) {
                possibleMoves.push_back(Move::moveTo(src, convertFileAndRank(6, 7 - 0)));
            }
        }
    }
}

bool Bot::isValidMove(int i, int j) {
    return (i >= 0 && i <= 7 && j >= 0 && j <= 7);
}

bool Bot::isValidPawnMove(int i) {
    return (i >= 0 && i <= 7);
}

void Bot::generateKingMoves(vector<Move *> &possibleMoves, int rank, int file, PlaySide side, struct state currentState) {
    //////// DEBUG ////////
    // ofstream fout;
    // fout.open("drop.out", std::ios_base::app);
    // fout << "////// GENERATE KING MOVES //////\n";
    // fout.close();
    //////////////////////

    vector<int> di = {-1, -1, -1, 0, 0, 1, 1, 1};
    vector<int> dj = {-1, 0, 1, -1, 1, -1, 0, 1};

    for (int k = 0; k < 8; ++k) {
        int newRank = rank + di[k];
        int newFile = file + dj[k];

        if (isValidMove(newRank, newFile)) {
            // if the possible next move is empty, then we add it to our possible moves
            if (currentState.board_game[newRank][newFile] == e) {
                possibleMoves.push_back(Move::moveTo(convertFileAndRank(file, 7 - rank),
                                                     convertFileAndRank(newFile, 7 - newRank)));
            } else if ((side == WHITE && currentState.board_game[newRank][newFile] <= p) ||
                     (side == BLACK && currentState.board_game[newRank][newFile] >= R)) {
                possibleMoves.push_back(Move::moveTo(convertFileAndRank(file, 7 - rank),
                                                     convertFileAndRank(newFile, 7 - newRank)));
            }
        }
    }
}

void Bot::generateRookMoves(vector<Move *> &possibleMoves, int rank, int file, PlaySide side, struct state currentState)
{

    //////// DEBUG ////////
    // ofstream fout;
    // fout.open("drop.out", std::ios_base::app);
    // fout << "////// GENERATE ROOK MOVES //////\n";
    // fout.close();
    //////////////////////

    vector<int> di = {-1, 0, 0, 1};
    vector<int> dj = {0, -1, 1, 0};

    for (int i = 0; i < 4; i++) {
        int newRank = rank + di[i];
        int newFile = file + dj[i];

        while (isValidMove(newRank, newFile)) {
            if (currentState.board_game[newRank][newFile] == e) {
                possibleMoves.push_back(Move::moveTo(convertFileAndRank(file, 7 - rank),
                                                     convertFileAndRank(newFile, 7 - newRank)));
            } else if ((side == WHITE && currentState.board_game[newRank][newFile] <= p) ||
                     (side == BLACK && currentState.board_game[newRank][newFile] >= R)) {
                possibleMoves.push_back(Move::moveTo(convertFileAndRank(file, 7 - rank),
                                                     convertFileAndRank(newFile, 7 - newRank)));
                break;
            } else {
                break;
            }

            newRank += di[i];
            newFile += dj[i];
        }
    }
}

void Bot::generatePawnMoves(vector<Move *> &possibleMoves, int rank, int file, PlaySide side, struct state currentState) {
    //////// DEBUG ////////
    // ofstream fout;
    // fout.open("drop.out", std::ios_base::app);
    // fout << "////// GENERATE PAWN MOVES //////\n";
    // fout.close();
    //////////////////////

    int startingRow, direction, oneRow;

    if (side == BLACK) {
        startingRow = 1;
        direction = 1;
    } else {
        startingRow = 6;
        direction = -1;
    }

    oneRow = rank + direction;

    if (isValidPawnMove(oneRow) && currentState.board_game[oneRow][file] == e) {
        /* if the pawn has not moved yet we can jump 2 squares */
        if (rank == startingRow) {
            /* adding the move where the pawn advances two squares */
            if (currentState.board_game[rank + 2 * direction][file] == e) {
                int newRow = rank + 2 * direction;
                possibleMoves.push_back(Move::moveTo(convertFileAndRank(file, 7 - rank),
                                                 convertFileAndRank(file, 7 - newRow)));
            }
        }

        /* adding standard movement */
        possibleMoves.push_back(Move::moveTo(convertFileAndRank(file, 7 - rank),
                                             convertFileAndRank(file, 7 - oneRow)));
    }

    /* check for diagonals */
    vector<int> di = {-1, -1};
    vector<int> dj = {-1, 1};

    if (side == BLACK) {
        di[0] = 1;
        di[1] = 1;
    }

    for (int i = 0; i < 2; i++) {
        int newRank = rank + di[i];
        int newFile = file + dj[i];

        if (isValidMove(newRank, newFile) && currentState.board_game[newRank][newFile] != e) {
            if ((side == WHITE && currentState.board_game[newRank][newFile] <= p) ||
                (side == BLACK && currentState.board_game[newRank][newFile] >= R)) {
                possibleMoves.push_back(Move::moveTo(convertFileAndRank(file, 7 - rank),
                                                     convertFileAndRank(newFile, 7 - newRank)));
            }
        }
    }
}

void Bot::generateKnightMoves(vector<Move *> &possibleMoves, int rank, int file, PlaySide side, struct state currentState)
{
    vector<int> di = {-2, -2, -1, -1, 1, 1, 2, 2};
    vector<int> dj = {-1, 1, -2, 2, -2, 2, -1, 1};

    for (int i = 0; i < 8; ++i) {
        int newRank = rank + di[i];
        int newFile = file + dj[i];

        if (isValidMove(newRank, newFile)) {
            /* square is empty or occupied by enemy */
            if (currentState.board_game[newRank][newFile] == e ||
                (side == WHITE && currentState.board_game[newRank][newFile] <= p) ||
                (side == BLACK && currentState.board_game[newRank][newFile] >= R)) {
                possibleMoves.push_back(Move::moveTo(convertFileAndRank(file, 7 - rank),
                                                     convertFileAndRank(newFile, 7 - newRank)));
            }
        }
    }
}

void Bot::generateBishopMoves(vector<Move *> &possibleMoves, int rank, int file, PlaySide side, struct state currentState) {
    //////// DEBUG ////////
    // ofstream fout;
    // fout.open("drop.out", std::ios_base::app);
    // fout << "////// GENERATE BISHOP MOVES //////\n";
    // fout.close();
    //////////////////////

    vector<int> di = {-1, -1, 1, 1};
    vector<int> dj = {-1, 1, -1, 1};

    for (int i = 0; i < 4; ++i) {
        int newRank = rank + di[i];
        int newFile = file + dj[i];

        while (isValidMove(newRank, newFile)) {
            if (currentState.board_game[newRank][newFile] == e) {
                possibleMoves.push_back(Move::moveTo(convertFileAndRank(file, 7 - rank),
                                                     convertFileAndRank(newFile, 7 - newRank)));
            }
            else if ((side == WHITE && currentState.board_game[newRank][newFile] <= p) ||
                     (side == BLACK && currentState.board_game[newRank][newFile] >= R)) {
                possibleMoves.push_back(Move::moveTo(convertFileAndRank(file, 7 - rank),
                                                     convertFileAndRank(newFile, 7 - newRank)));
                break;
            } else {
                break;
            }

            newRank += di[i];
            newFile += dj[i];
        }
    }
}

void Bot::generateQueenMoves(vector<Move *> &possibleMoves, int rank, int file, PlaySide side, struct state currentState)
{
    //////// DEBUG ////////
    // ofstream fout;
    // fout.open("drop.out", std::ios_base::app);
    // fout << "////// GENERATE QWEEN MOVES //////\n";
    // fout.close();
    //////////////////////
    generateRookMoves(possibleMoves, rank, file, side, currentState);
    generateBishopMoves(possibleMoves, rank, file, side, currentState);
}

enum Piece convertToPiece(enum pieces chr)
{
    switch (chr)
    {
    case p:
        return PAWN;
    case P:
        return PAWN;
    case r:
        return ROOK;
    case R:
        return ROOK;
    case n:
        return KNIGHT;
    case N:
        return KNIGHT;
    case b:
        return BISHOP;
    case B:
        return BISHOP;
    case q:
        return QUEEN;
    case Q:
        return QUEEN;
    default:
        return KING;
    }
}

void Bot::generateCrazyhouseMoves(vector<Move *> &possibleMoves, int rank, int file, PlaySide side, struct state currentState) {
    //////// DEBUG ////////
    // ofstream fout;
    // fout.open("drop.out", std::ios_base::app);
    // fout << "////// GENERATE CRAZYHOUSE MOVES //////\n";
    // fout.close();
    //////////////////////

    vector<enum pieces> capturedPieces = currentState.capturedBlackPieces;
    if (currentState.board_game[rank][file] != e)
        return;
    if(side == WHITE)
        capturedPieces = currentState.capturedWhitePieces;

    /* add all pieces captured */
    for (enum pieces piece : capturedPieces) {
        /* pawns cannot be placed on first or last tiles */
        if ((rank == 0 || rank == 7) && (piece == P || piece == p))
            continue;

        possibleMoves.push_back(Move::dropIn(convertFileAndRank(file, 7 - rank),
                                             convertToPiece((enum pieces)piece)));
    }

    return;
}

vector<Move *> Bot::generateMoves(vector<Move *>& possibleMoves, PlaySide side, struct state currentState) {
    for (int rank = 0; rank < 8; ++rank) {
        for (int file = 0; file < 8; ++file) {
            int square = currentState.board_game[rank][file];

            if (side == BLACK) {
                switch (square) {
                case k: {
                    currentState.kingBlackFile = file;
                    currentState.kingBlackRank = rank;

                    // ofstream fout;
                    // fout.open("drop.out", std::ios_base::app);
                    // fout << "TABLE BLACK KING POSITION: \n" << currentState.kingBlackFile << " " << currentState.kingBlackRank << "\n";

                    generateKingMoves(possibleMoves, rank, file, side, currentState);
                    // fout << "TABLE BLACK KING POSITION AFTER GENERATION: \n" << currentState.kingBlackFile << " " << currentState.kingBlackRank << "\n";

                    break;
                }

                case p: {
                    if(rank == 6)
                        generatePromotionMoves(possibleMoves, rank, file, side, currentState);
                    else
                        generatePawnMoves(possibleMoves, rank, file, side, currentState);
                    if(rank == 4) {
                        // ofstream fout;
                        // fout.open("drop.out", std::ios_base::app);
                        // fout << "RANK = 4 ////// ENPASSANT //////\n";
                        generateEnPassantMoves(possibleMoves, rank, file, side, currentState);
                    }

                    // ofstream fout;
                    // fout.open("drop.out", std::ios_base::app);
                    // fout << "ENDED ENPASSANT\n";
                    break;
                }

                case r: {
                    generateRookMoves(possibleMoves, rank, file, side, currentState);
                    break;
                }

                case n: {
                    generateKnightMoves(possibleMoves, rank, file, side, currentState);
                    break;
                }

                case b: {   
                    generateBishopMoves(possibleMoves, rank, file, side, currentState);
                    break;
                }

                case q: {   
                    generateQueenMoves(possibleMoves, rank, file, side, currentState);
                    break;
                }

                default: {
                    /* case "e" */
                    if (square == e)
                        generateCrazyhouseMoves(possibleMoves, rank, file, side, currentState);
                    break;
                }
                }
            } else {
                switch (square) {
                    case K: {
                        currentState.kingWhiteFile = file;
                        currentState.kingWhiteRank = rank;
                        generateKingMoves(possibleMoves, rank, file, side, currentState);
                        break;
                    }

                    case P: {
                        if(rank == 1)
                            generatePromotionMoves(possibleMoves, rank, file, side, currentState);
                        else
                            generatePawnMoves(possibleMoves, rank, file, side, currentState);
                        if(rank == 3)
                            generateEnPassantMoves(possibleMoves, rank, file, side, currentState);
                        
                        // ofstream fout;
                        // fout.open("drop.out", std::ios_base::app);
                        // fout << "ENDED ENPASSANT\n";
                        break;
                    }

                    case R: {  
                        generateRookMoves(possibleMoves, rank, file, side, currentState);
                        break;
                    }

                    case N: {
                        generateKnightMoves(possibleMoves, rank, file, side, currentState);
                        break;
                    }

                    case B: {   
                        generateBishopMoves(possibleMoves, rank, file, side, currentState);
                        break;
                    }

                    case Q: {   
                        generateQueenMoves(possibleMoves, rank, file, side, currentState);
                        break;
                    }

                    default: {
                        /* case "e" */
                        if (square == e)
                            generateCrazyhouseMoves(possibleMoves, rank, file, side, currentState);
                        break;
                    }
                }
            }
        }
    }

    return possibleMoves;
}

int Bot::convertPiece(enum Piece piece, PlaySide side) {
    switch (piece) {
    case PAWN:
        return side == WHITE ? P : p;
    case ROOK:
        return side == WHITE ? R : r;
    case KNIGHT:
        return side == WHITE ? N : n;
    case BISHOP:
        return side == WHITE ? B : b;
    case QUEEN:
        return side == WHITE ? Q : q;
    default:
        return side == WHITE ? K : k;
    }
}

/* transform captured piece */
enum pieces Bot::transformPiece(enum pieces piece) {
    switch (piece) {
    case P:
        return p;
    case p:
        return P;
    case R:
        return r;
    case r:
        return R;
    case N:
        return n;
    case n:
        return N;
    case B:
        return b;
    case b:
        return B;
    case Q:
        return q;
    case q:
        return Q;
    default:
        return e;
    }
}

bool Bot::isCastling(int srcRank, int srcFile, int destRank, int destFile, int board_game[8][8])
{
    if (board_game[srcRank][srcFile] == K)
        return (srcRank == 7 && srcFile == 4 && destRank == 7 && destFile == 6) ||
               (srcRank == 7 && srcFile == 4 && destRank == 7 && destFile == 2);
    else if (board_game[srcRank][srcFile] == k)
        return (srcRank == 0 && srcFile == 4 && destRank == 0 && destFile == 6) ||
               (srcRank == 0 && srcFile == 4 && destRank == 0 && destFile == 2);
    else
        return false;
}

void Bot::capturePiece(int destRank, int destFile, PlaySide sideToMove, struct state& currentState) {
    /* add captured piece to captured pieces */
    int capturedPiece = currentState.board_game[destRank][destFile];
    if (currentState.promoted_pieces[destRank][destFile] == true)
        capturedPiece = (sideToMove == WHITE ? p : P);

    if (sideToMove == WHITE)
        currentState.capturedWhitePieces.push_back(transformPiece((enum pieces)capturedPiece));
    else
        currentState.capturedBlackPieces.push_back(transformPiece((enum pieces)capturedPiece));
}

bool Bot::isEnPassant(int srcRank, int srcFile, int destRank, int destFile, int board_game[8][8])
{
    if (board_game[srcRank][srcFile] == P)
        return (srcRank == 3 && destRank == 2 && board_game[destRank][destFile] == e &&
                board_game[destRank + 1][destFile] == p);
    else if (board_game[srcRank][srcFile] == p)
        return (srcRank == 4 && destRank == 5 && board_game[destRank][destFile] == e &&
                board_game[destRank - 1][destFile] == P);
    else
        return false;
}

void Bot::recordDropIn(Move* move, int destRank, int destFile, PlaySide sideToMove, struct state& currentState) {
    currentState.board_game[destRank][destFile] = convertPiece(move->getReplacement().value(), sideToMove);
    currentState.promoted_pieces[destRank][destFile] = false;

    /* remove piece from list of captured pieces */
    if (sideToMove == WHITE) {
        for (size_t i = 0; i < currentState.capturedWhitePieces.size(); i++) {
            if (currentState.capturedWhitePieces[i] == currentState.board_game[destRank][destFile]) {
                currentState.capturedWhitePieces.erase(currentState.capturedWhitePieces.begin() + i);
                break;
            }
        }
    } else {
        for (size_t i = 0; i < currentState.capturedBlackPieces.size(); i++) {
            if (currentState.capturedBlackPieces[i] == currentState.board_game[destRank][destFile]) {
                currentState.capturedBlackPieces.erase(currentState.capturedBlackPieces.begin() + i);
                break;
            }
        }
    }
}

void Bot::recordPromotion(Move* move, int destRank, int destFile,
                          int srcRank, int srcFile, PlaySide sideToMove, struct state& currentState) {
    /* check for captured piece */
    if (currentState.board_game[destRank][destFile] != e) {
        capturePiece(destRank, destFile, sideToMove, currentState);
    }

    /* promotion move */
    currentState.board_game[destRank][destFile] = convertPiece(move->getReplacement().value(), sideToMove);
    currentState.board_game[srcRank][srcFile] = e;

    /* update promotion board */
    currentState.promoted_pieces[destRank][destFile] = true;
    currentState.promoted_pieces[srcRank][srcFile] = false;
}

void Bot::setFlags(int srcRank, int srcFile, int destRank, int destFile, PlaySide sideToMove, struct state& currentState) {
    if (sideToMove == WHITE) {
        /* check if white king or rook moved */
        if (srcRank == currentState.kingWhiteRank && srcFile == currentState.kingWhiteFile) {
            /* udpate king position */
            currentState.whiteKingMoved = true;
            currentState.kingWhiteRank = destRank;
            currentState.kingWhiteFile = destFile;
        }
        else if (currentState.board_game[srcRank][srcFile] == R && currentState.promoted_pieces[srcRank][srcFile] == false &&
                 srcRank == 7 && srcFile == 0)
            currentState.whiteLeftRookMoved = true;
        else if (currentState.board_game[srcRank][srcFile] == R && currentState.promoted_pieces[srcRank][srcFile] == false &&
                 srcRank == 7 && srcFile == 7)
            currentState.whiteRightRookMoved = true;
    } else {
        /* check if black king or rook moved */
        if (srcRank == currentState.kingBlackRank && srcFile == currentState.kingBlackFile) {
            /* update king postion */
            currentState.blackKingMoved = true;
            currentState.kingBlackRank = destRank;
            currentState.kingBlackFile = destFile;
        }
        else if (currentState.board_game[srcRank][srcFile] == r && currentState.promoted_pieces[srcRank][srcFile] == false &&
                 srcRank == 0 && srcFile == 0)
            currentState.blackLeftRookMoved = true;
        else if (currentState.board_game[srcRank][srcFile] == r && currentState.promoted_pieces[srcRank][srcFile] == false &&
                 srcRank == 0 && srcFile == 7)
            currentState.blackRightRookMoved = true;
    }
}

void Bot::recordEnPassant(int destRank, int destFile, int srcRank, int srcFile, PlaySide sideToMove, struct state& currentState) {
    if (sideToMove == WHITE) {
            /* move pawn */
            currentState.board_game[destRank][destFile] = currentState.board_game[srcRank][srcFile];
            currentState.board_game[srcRank][srcFile] = e;
            /* remove captured pawn */
            currentState.board_game[destRank + 1][destFile] = e;
    } else {
        /* move pawn */
        currentState.board_game[destRank][destFile] = currentState.board_game[srcRank][srcFile];
        currentState.board_game[srcRank][srcFile] = e;
        /* remove captured pawn */
        currentState.board_game[destRank - 1][destFile] = e;
    }

    /* set flag on promotion board */
    currentState.promoted_pieces[destRank][destFile] = false;
    currentState.promoted_pieces[srcRank][srcFile] = false;
}

void Bot::recordCastling(int destFile, PlaySide sideToMove, struct state& currentState) {
    /* castling move */
    if (sideToMove == WHITE) {
        if (destFile == 6) {
            /* move rook: small castling */
            currentState.board_game[7][5] = currentState.board_game[7][7];
            currentState.board_game[7][7] = e;
            /* move king: small casling */
            currentState.board_game[7][6] = currentState.board_game[7][4];
            currentState.board_game[7][4] = e;
        } else {
            /* move rook: big castling */
            currentState.board_game[7][3] = currentState.board_game[7][0];
            currentState.board_game[7][0] = e;
            /* move king: big castling */
            currentState.board_game[7][2] = currentState.board_game[7][4];
            currentState.board_game[7][4] = e;
        }
    } else {
        /* black side of the board */
        if (destFile == 6) {
            /* move rook: small castling */
            currentState.board_game[0][5] = currentState.board_game[0][7];
            currentState.board_game[0][7] = e;
            /* move king: small castling */
            currentState.board_game[0][6] = currentState.board_game[0][4];
            currentState.board_game[0][4] = e;
        } else {
            /* move rook: big castling */
            currentState.board_game[0][3] = currentState.board_game[0][0];
            currentState.board_game[0][0] = e;
            /* move king:big castling */
            currentState.board_game[0][2] = currentState.board_game[0][4];
            currentState.board_game[0][4] = e;
        }
    }
}

void Bot::recordAdvance(int destRank, int destFile, int srcRank, int srcFile, struct state& currentState) {
    /* advancing on empty square move */
    currentState.promoted_pieces[destRank][destFile] = currentState.promoted_pieces[srcRank][srcFile];
    currentState.promoted_pieces[srcRank][srcFile] = false;

    /* update pieces board*/
    currentState.board_game[destRank][destFile] = currentState.board_game[srcRank][srcFile];
    currentState.board_game[srcRank][srcFile] = e;
}

void Bot::recordNormalCapture(int destRank, int destFile, int srcRank, int srcFile, PlaySide sideToMove, struct state& currentState) {
    /* capturing move */
    capturePiece(destRank, destFile, sideToMove, currentState);

    /* update pieces board */
    currentState.board_game[destRank][destFile] = currentState.board_game[srcRank][srcFile];
    currentState.board_game[srcRank][srcFile] = e;

    /* update promotion board */
    currentState.promoted_pieces[destRank][destFile] = currentState.promoted_pieces[srcRank][srcFile];
    currentState.promoted_pieces[srcRank][srcFile] = false;
}

void Bot::recordNormalMove(Move* move, int destRank, int destFile,
                      int srcRank, int srcFile, PlaySide sideToMove, struct state& currentState) {
    /* set flags if king or rook moved */
    setFlags(srcRank, srcFile, destRank, destFile, sideToMove, currentState);

    /* en passant move */
    if (isEnPassant(srcRank, srcFile, destRank, destFile, currentState.board_game)) {
        recordEnPassant(destRank, destFile, srcRank, srcFile, sideToMove, currentState);

        /* capture pawn */
        if (sideToMove == WHITE)
            currentState.capturedWhitePieces.push_back(P);
        else
            currentState.capturedBlackPieces.push_back(p);
    } else if (isCastling(srcRank, srcFile, destRank, destFile, currentState.board_game)) {
        /* castling move */
        recordCastling(destFile, sideToMove, currentState);
    } else if (currentState.board_game[destRank][destFile] == e) {
        /* advancing on empty square move */
        recordAdvance(destRank, destFile, srcRank, srcFile, currentState);
    } else {
        /* capturing move */
        recordNormalCapture(destRank, destFile, srcRank, srcFile, sideToMove, currentState);
    }
}


string getPiece(enum pieces piece) {
    switch (piece) {
    case P:
        return "P";
    case p:
        return "p";
    case R:
        return "R";
    case r:
        return "r";
    case N:
        return "N";
    case n:
        return "n";
    case B:
        return "B";
    case b:
        return "b";
    case Q:
        return "Q";
    case q:
        return "q";
    case K:
        return "K";
    case k:
        return "k";
    case e:
        return "_";
    default:
        return "x";
    }
}

vector<string> unicode = {"♜", "♞", "♝", "♛", "♚", "♟", "♖", "♘", "♗", "♕", "♔", "♙", "_"};

void Bot::printStatus(PlaySide sideToMove, PlaySide side, Move* move, struct state currentState) {
    /* open file for append */
    ofstream fout;
    fout.open("drop.out", std::ios_base::app);

    /* printing sides */
    string sideToMoveStr = (side == WHITE) ? "WHITE" : "BLACK";
    string engineSideStr = (side == WHITE) ? "WHITE" : "BLACK";
    fout << "printing side to move: " << sideToMoveStr << endl;
    fout << "printing engineSide: " << engineSideStr << endl;
    if (currentState.whiteLastMove != NULL)
        fout << "WHITE LAST MOVE: " << currentState.whiteLastMove->getDestination().value() << endl;
    if (currentState.blackLastMove != NULL)
        fout << "BLACK LAST MOVE: " << currentState.blackLastMove->getDestination().value() << endl;
    
    /* capture move */
    if (move->isNormal()) {
        fout << "printing normal move src: " << move->getSource().value() << " " << "dest: " << move->getDestination().value() << endl;
    } else if (move->isDropIn()) {
        fout << "printing drop in move dest: " << move->getDestination().value() << " " << "replacement: " << move->getReplacement().value() << endl;
    } else if (move->isPromotion()) {
        fout << "printing promotion move src: " << move->getSource().value() << " " << "dest: " << move->getDestination().value() << " " << "replacement: " << move->getReplacement().value() << endl;
    }

    /* printing back captured pieces */
    fout << "printing captured black pieces: " << endl;
    for (size_t i = 0; i < currentState.capturedBlackPieces.size(); i++) {
        fout << getPiece(currentState.capturedBlackPieces[i]) << " ";
    }
    fout << endl;

    /* printing white captured pices */
    fout << "\n";
    fout << "printing captured white pieces: " << endl;
    for (size_t i = 0; i < currentState.capturedWhitePieces.size(); i++) {
        fout << getPiece(currentState.capturedWhitePieces[i]) << " ";
    }
    fout << endl;

    /* printing board */
    fout << "\n";
    fout<<"printing board:\n";
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            fout << getPiece((enum pieces)currentState.board_game[i][j]) << " " << flush;
        }
        fout << "\n";
    }
}

void Bot::recordStatus(Move* &move, PlaySide sideToMove, struct state& currentState) {
    int srcRank, srcFile;
    if (!move->isDropIn()) {
        srcRank = 8 - (move->getSource().value()[1] - '0');
        srcFile = move->getSource().value()[0] - 'a';
    }
    int destRank = 8 - (move->getDestination().value()[1] - '0');
    int destFile = move->getDestination().value()[0] - 'a';

    /* Normal move */
    if (move->isNormal()) {
        recordNormalMove(move, destRank, destFile, srcRank, srcFile, sideToMove, currentState);
    } else if (move->isPromotion()) {
        /* Promotion move */
        recordPromotion(move, destRank, destFile, srcRank, srcFile, sideToMove, currentState);
    } else if (move->isDropIn()) {
        /* Drop-in move */
        recordDropIn(move, destRank, destFile, sideToMove, currentState);
    }

    /* Record last move for current side */
    // ofstream fout;
    // fout.open("drop.out", std::ios_base::app);

    // if (move == NULL)
    //     fout << "recording last move to NULL !!! " << endl;
    if (sideToMove == WHITE) {
        if (move->isNormal())
            currentState.whiteLastMove = Move::moveTo(move->getSource(), move->getDestination());
        else if (move->isDropIn())
            currentState.whiteLastMove = Move::dropIn(move->getDestination(), move->getReplacement());
        else if (move->isPromotion())
            currentState.whiteLastMove = Move::promote(move->getSource(), move->getDestination(), move->getReplacement());
    } else {
        if (move->isNormal())
            currentState.blackLastMove = Move::moveTo(move->getSource(), move->getDestination());
        else if (move->isDropIn())
            currentState.blackLastMove = Move::dropIn(move->getDestination(), move->getReplacement());
        else if (move->isPromotion())
            currentState.blackLastMove = Move::promote(move->getSource(), move->getDestination(), move->getReplacement());
    }

    // fout << " PRINTING CURRENT STATE AFTER RECORD " << endl;
    // printStatus(sideToMove, engineSide, move, currentState);
}



/////////////////////////// MINIMAX ///////////////////////////

int pawns[8][8] = {
    {0,  0,  0,  0,  0,  0,  0,  0},
   {50, 50, 50, 50, 50, 50, 50, 50},
    {10, 10, 20, 30, 30, 20, 10, 10},
    {5,  5, 10, 25, 25, 10,  5,  5},
    {0,  0,  0, 20, 20,  0,  0,  0},
    {5, -5,-10,  0,  0,-10, -5,  5},
    {5, 10, 10,-20,-20, 10, 10,  5},
    {0,  0,  0,  0,  0,  0,  0,  0},
};

int black_pawns[8][8] = {
    {0,  0,  0,  0,  0,  0,  0,  0},
    {5, 10, 10, -20, -20, 10, 10, 5},
    {5, -5, -10, 0, 0, -10, -5, 5},
    {0, 0, 0, 20, 20, 0, 0, 0},
    {5, 5, 10, 25, 25, 10, 5, 5},
    {10, 10, 20, 30, 30, 20, 10, 10},
    {50, 50, 50, 50, 50, 50, 50, 50},
    {0, 0, 0, 0, 0, 0, 0, 0},
};

int knights[8][8] = {
    {-50,-40,-30,-30,-30,-30,-40,-50},
    {-40,-20,  0,  0,  0,  0,-20,-40},
    {-30,  0, 10, 15, 15, 10,  0,-30},
    {-30,  5, 15, 20, 20, 15,  5,-30},
    {-30,  0, 15, 20, 20, 15,  0,-30},
    {-30,  5, 10, 15, 15, 10,  5,-30},
    {-40,-20,  0,  5,  5,  0,-20,-40},
    {-50,-40,-30,-30,-30,-30,-40,-50},
};

int black_knights[8][8] = {
    {-50,-40,-30,-30,-30,-30,-40,-50},
    {-40,-20,  0,  5,  5,  0,-20,-40},
    {-30,  5, 10, 15, 15, 10,  5,-30},
    {-30,  0, 15, 20, 20, 15,  0,-30},
    {-30,  5, 15, 20, 20, 15,  5,-30},
    {-30,  0, 10, 15, 15, 10,  0,-30},
    {-40,-20,  0,  0,  0,  0,-20,-40},
    {-50,-40,-30,-30,-30,-30,-40,-50},
};

int bishops[8][8] = {
    {-20,-10,-10,-10,-10,-10,-10,-20},
    {-10,  0,  0,  0,  0,  0,  0,-10},
    {-10,  0,  5, 10, 10,  5,  0,-10},
    {-10,  5,  5, 10, 10,  5,  5,-10},
    {-10,  0, 10, 10, 10, 10,  0,-10},
    {-10, 10, 10, 10, 10, 10, 10,-10},
    {-10,  5,  0,  0,  0,  0,  5,-10},
    {-20,-10,-10,-10,-10,-10,-10,-20},
};

int black_bishops[8][8] = {
    {-20,-10,-10,-10,-10,-10,-10,-20},
    {-10,  5,  0,  0,  0,  0,  5,-10},
    {-10, 10, 10, 10, 10, 10, 10,-10},
    {-10,  0, 10, 10, 10, 10,  0,-10},
    {-10,  5,  5, 10, 10,  5,  5,-10},
    {-10,  0,  5, 10, 10,  5,  0,-10},
    {-10,  0,  0,  0,  0,  0,  0,-10},
    {-20,-10,-10,-10,-10,-10,-10,-20},
};

int rooks[8][8] = {
    {0,  0,  0,  0,  0,  0,  0,  0},
    {5, 10, 10, 10, 10, 10, 10,  5},
    {-5,  0,  0,  0,  0,  0,  0, -5},
    {-5,  0,  0,  0,  0,  0,  0, -5},
    {-5,  0,  0,  0,  0,  0,  0, -5},
    {-5,  0,  0,  0,  0,  0,  0, -5},
    {-5,  0,  0,  0,  0,  0,  0, -5},
    {0,  0,  0,  5,  5,  0,  0,  0},
};

int black_rooks[8][8] = {
    {0,  0,  0,  5,  5,  0,  0,  0},
    {-5,  0,  0,  0,  0,  0,  0, -5},
    {-5,  0,  0,  0,  0,  0,  0, -5},
    {-5,  0,  0,  0,  0,  0,  0, -5},
    {-5,  0,  0,  0,  0,  0,  0, -5},
    {-5,  0,  0,  0,  0,  0,  0, -5},
    {5, 10, 10, 10, 10, 10, 10,  5},
    {0,  0,  0,  0,  0,  0,  0,  0},
};

int queen[8][8] {
    {-20,-10,-10, -5, -5,-10,-10,-20},
    {-10,  0,  0,  0,  0,  0,  0,-10},
    {-10,  0,  5,  5,  5,  5,  0,-10},
    {-5,  0,  5,  5,  5,  5,  0, -5},
    {0,  0,  5,  5,  5,  5,  0, -5},
    {-10,  5,  5,  5,  5,  5,  0,-10},
    {-10,  0,  5,  0,  0,  0,  0,-10},
    {-20,-10,-10, -5, -5,-10,-10,-20},
};

int black_queen[8][8] = {
    {-20,-10,-10, -5, -5,-10,-10,-20},
    {-10,  0,  5,  0,  0,  0,  0,-10},
    {-10,  5,  5,  5,  5,  5,  0,-10},
    {0,  0,  5,  5,  5,  5,  0, -5},
    {-5,  0,  5,  5,  5,  5,  0, -5},
    {-10,  0,  5,  5,  5,  5,  0,-10},
    {-10,  0,  0,  0,  0,  0,  0,-10},
    {-20,-10,-10, -5, -5,-10,-10,-20},
};

int king_middle_game[8][8] = {
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-20,-30,-30,-40,-40,-30,-30,-20},
    {-10,-20,-20,-20,-20,-20,-20,-10},
    {20, 20,  0,  0,  0,  0, 20, 20},
    {20, 30, 10,  0,  0, 10, 30, 20},
};

int black_king_middle_game[8][8] = {
    {20, 30, 10,  0,  0, 10, 30, 20},
    {20, 20,  0,  0,  0,  0, 20, 20},
    {-10,-20,-20,-20,-20,-20,-20,-10},
    {-20,-30,-30,-40,-40,-30,-30,-20},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30},
};

int king_end_game[8][8] = {
    {-50,-40,-30,-20,-20,-30,-40,-50},
    {-30,-20,-10,  0,  0,-10,-20,-30},
    {-30,-10, 20, 30, 30, 20,-10,-30},
    {-30,-10, 30, 40, 40, 30,-10,-30},
    {-30,-10, 30, 40, 40, 30,-10,-30},
    {-30,-10, 20, 30, 30, 20,-10,-30},
    {-30,-30,  0,  0,  0,  0,-30,-30},
    {-50,-30,-30,-30,-30,-30,-30,-50},
};

int black_kings_end_game[8][8] = {
    {-50,-30,-30,-30,-30,-30,-30,-50},
    {-30,-30,  0,  0,  0,  0,-30,-30},
    {-30,-10, 20, 30, 30, 20,-10,-30},
    {-30,-10, 30, 40, 40, 30,-10,-30},
    {-30,-10, 30, 40, 40, 30,-10,-30},
    {-30,-10, 20, 30, 30, 20,-10,-30},
    {-30,-20,-10,  0,  0,-10,-20,-30},
    {-50,-40,-30,-20,-20,-30,-40,-50},
};

int Bot::getValueForPieces(struct state currentState, PlaySide side) {
    int botValue = 0;

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            /* each piece has a different weight */
            int piece = currentState.board_game[i][j];

            if (side == WHITE) {
                switch (piece) {
                    case P:
                        botValue += PAWN_VALUE + pawns[i][j];
                        break;
                    case N:
                        botValue += KNIGHT_VALUE + knights[i][j];
                        break;
                    case B:
                        botValue += BISHOP_VALUE + bishops[i][j];
                        break;
                    case R:
                        botValue += ROOK_VALUE + rooks[i][j];
                        break;
                    case Q:
                        botValue += QUEEN_VALUE + queen[i][j];
                        break;
                    case K:
                        botValue += KING_VALUE + king_middle_game[i][j];
                        break;
                    case e:
                        break;
                }
            } else {
                switch (piece) {
                    case p:
                        botValue += PAWN_VALUE + black_pawns[i][j];
                        break;
                    case n:
                        botValue += KNIGHT_VALUE + black_knights[i][j];
                        break;
                    case b:
                        botValue += BISHOP_VALUE + black_bishops[i][j];
                        break;
                    case r:
                        botValue += ROOK_VALUE + black_rooks[i][j];
                        break;
                    case q:
                        botValue += QUEEN_VALUE + black_queen[i][j];
                        break;
                    case k:
                        botValue += KING_VALUE + black_king_middle_game[i][j];
                        break;
                }
            }
        }
    }

    return botValue;
}

/* evaluation function for the positions of the pieces on the board;
    used a simplified evaluation function based on piece-square tables */

int Bot::evaluateBoard(struct state currentState, PlaySide side) {
    PlaySide opponentSide = (side == WHITE) ? BLACK : WHITE;

    int botSide = getValueForPieces(currentState, side);
    int opponent = getValueForPieces(currentState, opponentSide);

    /* TODO: add to the botSide/OpponentSide bonus points if castling has been done (maybe? idk) */

    return botSide - opponent;
}


void Bot::cloneState(struct state& newState, struct state& oldState) {
    /* copy boards */
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
                newState.board_game[i][j] = oldState.board_game[i][j];
                newState.promoted_pieces[i][j] = oldState.promoted_pieces[i][j];
        }
    }

    /* copy flags */
    newState.blackLastMove = oldState.blackLastMove;
    newState.whiteLastMove = oldState.whiteLastMove;

    newState.kingWhiteFile = oldState.kingWhiteFile;
    newState.kingWhiteRank = oldState.kingWhiteRank;
    newState.kingBlackFile = oldState.kingBlackFile;
    newState.kingBlackRank = oldState.kingBlackRank;

    newState.whiteKingMoved = oldState.whiteKingMoved;
    newState.blackKingMoved = oldState.blackKingMoved;
    newState.whiteLeftRookMoved = oldState.whiteLeftRookMoved;
    newState.blackLeftRookMoved = oldState.blackLeftRookMoved;
    newState.whiteRightRookMoved = oldState.whiteRightRookMoved;
    newState.blackRightRookMoved = oldState.blackRightRookMoved;

    while(newState.capturedBlackPieces.size() > 0)
        newState.capturedBlackPieces.pop_back();
    for (int i = 0; i < (int)oldState.capturedBlackPieces.size(); i++)
        newState.capturedBlackPieces.push_back(oldState.capturedBlackPieces[i]);

    while(newState.capturedWhitePieces.size() > 0)
        newState.capturedWhitePieces.pop_back();
    for (int i = 0; i < (int)oldState.capturedWhitePieces.size(); i++)
        newState.capturedWhitePieces.push_back(oldState.capturedWhitePieces[i]);
}

int Bot::alphabeta_negamax(struct state& currentState, int depth, int alpha, int beta, PlaySide side) {
    /* if depth is 0 or game is over, return the evaluation of the board */
    vector<Move *> possibleMoves;
    vector<Move *> movesAvailable =  generateMoves(possibleMoves, side, currentState);
    generateCastlingMoves(movesAvailable, side, currentState);

    vector<Move *> validMoves;
    for (Move* move: movesAvailable) {
        if (!isKingInCheck(move, true, currentState, side))
            validMoves.push_back(move);
    }

////////////////////////////
    // ofstream fout;
    // fout.open("drop.out", std::ios_base::app);
    // fout << " START negamax depth: " << depth << endl;
////////////////////////////

    if (depth == 0)
        return evaluateBoard(currentState, side);
    if (validMoves.size() == 0)
        return INT_MIN;
        
    /* sort moves based on evaluation */
    vector<struct state> states;
    for (Move *move : validMoves) {
        /* create array of states */
        struct state newState;
        cloneState(newState, currentState);
        recordStatus(move, side, newState);
        newState.sideTmp = side;
        newState.move = move;
        newState.value = evaluateBoard(newState, side);
        states.push_back(newState);
    }
    sort(states.begin(), states.end(), [](struct state state1, struct state state2){
        return state1.value > state2.value;
    });

    int maxEval = INT_MIN;

    /* for each possible move */
    for (struct state state1 : states) {
        // if (!state1.move->isDropIn())
        //     fout << "Negamax calculating MOVE: " << state1.move->getSource().value() << " " << state1.move->getDestination().value() << endl;
        // else
        //     fout << "Negamax calculating MOVE: "  << state1.move->getDestination().value() << state1.move->getReplacement().value() << endl;
    ////////////////////////////

        int eval = -alphabeta_negamax(state1, depth - 1, -beta, -alpha, (side == WHITE) ? BLACK : WHITE);

    ////////////////////////////
        // fout << "IN finished negamax eval = " << eval;
        // fout << " for move ";
        // if (state1.move->isDropIn()) {
        //     fout << state1.move->getDestination().value() << " " << state1.move->getReplacement().value() << endl;
        // } else {
        //     fout << state1.move->getSource().value() << " " << state1.move->getDestination().value() << endl;
        // }
    ////////////////////////////

        /* if eval is greater than maxEval, update maxEval */
        if (eval > maxEval)
            maxEval = eval;
        
        /* update alpha */
        if (maxEval > alpha)
            alpha = maxEval;
        
        /* if beta is less than or equal to alpha, break */
        if (beta <= alpha)
            break;
    }

    /* return maxEval */
//////////////////////////
    // fout << "returning maxEval = " << maxEval << endl;
    // if (!currentState.move->isDropIn())
    //     fout << " move " << currentState.move->getSource().value() << " " << currentState.move->getDestination().value() << endl;
    // else
    //     fout << " drop " << currentState.move->getDestination().value() << " " << currentState.move->getReplacement().value() << endl;
    // fout << "ENDED NEGAMAX depth: " << depth << endl;
/////////////////////////
    return maxEval;
}


Move *Bot::chooseBestMove(int depth, PlaySide side, struct state currentState) {
    vector<Move *> possibleMoves;
    vector<Move *> movesAvailable = generateMoves(possibleMoves, side, currentState);
    generateCastlingMoves(movesAvailable, side, currentState);

    // ofstream fout;
    // fout.open("drop.out", std::ios_base::app);
    
    /* find valid moves available */
    vector<Move *> validMoves;
    for (Move* move: movesAvailable) {
        if (!isKingInCheck(move, true, currentState, side)) {
            validMoves.push_back(move);
        }
    }

    /* if there are no moves available, return nullptr */
    if (validMoves.size() == 0)
        return nullptr;
    
    /* if there are more moves available, choose the best one */
    int maxEval = INT_MIN;
    Move *bestMove = nullptr;

    /* sort moves based on evaluation */
    vector<struct state> states;
    for (Move *move : validMoves) {
        /* create array of states */
        struct state newState;
        cloneState(newState, currentState);
        recordStatus(move, side, newState);
        newState.sideTmp = side;
        newState.value = evaluateBoard(newState, side);
        newState.move = move;
        states.push_back(newState);
    }
    sort(states.begin(), states.end(), [](struct state state1, struct state state2){
        return state1.value > state2.value;
    });

    /* for each possible move */
    struct state newState;
    int alpha = INT_MIN;
    int beta = INT_MAX;

/////////////////////////////
    // for (struct state state1 : states) {
    //     if (!state1.move->isDropIn())
    //         fout << "MOVE: " << state1.move->getSource().value() << " " << state1.move->getDestination().value() << endl;
    //     else
    //         fout << "MOVE: "  << state1.move->getDestination().value() << state1.move->getReplacement().value() << endl;
    // }
/////////////////////////////

    for (struct state state1 : states) {
        // if (!state1.move->isDropIn())
        //     fout << "CALCULATING MOVE: " << state1.move->getSource().value() << " " << state1.move->getDestination().value() << endl;
        // else
        //     fout << "CALCULATING MOVE: "  << state1.move->getDestination().value() << state1.move->getReplacement().value() << endl;

        int eval = -alphabeta_negamax(state1, depth - 1, -beta, -alpha, (side == WHITE) ? BLACK : WHITE);
        // fout << "FINISHED NEGAMAX FOR DEPTH 1 -- eval = " << eval << endl;

        if(eval > maxEval) {
            maxEval = eval;
            bestMove = state1.move;
        }

        if (maxEval > alpha) {
            alpha = maxEval;
        }

        if (alpha >= beta)
            break;
    }

    // if (!bestMove->isDropIn())
    // fout << " best move : " << bestMove->getSource().value_or("gol") << " " << bestMove->getDestination().value() << endl;
    // fout << "FINISHED  ALL\n";

    return bestMove;
}

///////////////////////////////////////////////////////////////




void Bot::recordMove(Move* &move, PlaySide sideToMove)
{
    recordStatus(move, sideToMove, botState);
}

Move *Bot::calculateNextMove(PlaySide sideToMove)
{
    /* Play move for the side the engine is playing (Hint: Main.getEngineSide())
     * Make sure to record your move in custom structures before returning.
     *
     * Return move that you are willing to submit
     * Move is to be constructed via one of the factory methods declared in Move.h */

    /* Set beginning flags */
    if (!startGame)
    {
        Bot::startGame = true;
        botState.whiteKingMoved = false;
        botState.blackKingMoved = false;
        botState.whiteLeftRookMoved = false;
        botState.whiteRightRookMoved = false;
        botState.blackLeftRookMoved = false;
        botState.blackRightRookMoved = false;
        enPassant = false;
        castling = false;
        
        botState.blackLastMove = NULL;
        botState.whiteLastMove = NULL;

        botState.kingBlackFile = 4;
        botState.kingBlackRank = 0;
        botState.kingWhiteFile = 4;
        botState.kingWhiteRank = 7;

        engineSide = sideToMove;
    }

    ofstream fout;
    fout.open("drop.out", std::ios_base::app);
    fout << "bot side " << sideToMove << endl;

/////////////////////////// NEGAMAX ///////////////////////////
    /* Chose move with highest score */
    int depth = 4;
    Move* bestMove = chooseBestMove(depth, sideToMove, botState);

///////////////////////////////////////////////////////////////

    /* Record move in custom structures */
    recordMove(bestMove, sideToMove);

    // printStatus(sideToMove, engineSide, bestMove, botState);
    return bestMove;
}

std::string Bot::getBotName() { return Bot::BOT_NAME;
}
