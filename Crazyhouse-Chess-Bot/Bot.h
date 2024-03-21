#ifndef BOT_H
#define BOT_H
#include <bits/stdc++.h>
#include "PlaySide.h"
#include "Move.h"
#include "Piece.h"

using namespace std;

class Bot {
 private:
  static const std::string BOT_NAME;

 public:
  /* Declare custom fields below */
  PlaySide engineSide;
  PlaySide sideToMove;
  bool startGame;

  /* Declare custom fields above */
  Bot(bool _startGame);

  Bot();

  /**
   * Record move (either by enemy in normal mode, or by either side
   * in force mode) in custom structures
   * @param move received move
   * @param sideToMode side to move
   */
  void recordMove(Move* &move, PlaySide sideToMove);

  /**
   * Calculates next move, in response to enemyMove
   * @param enemyMove the enemy's last move
   *                  null if this is the opening move, or previous
   *                  move has been recorded in force mode
   * @return your move
   */
  Move* calculateNextMove(PlaySide sideToMove);

  static std::string getBotName();

  /* converting  functions */
  string convertFileAndRank(int file_idx, int rank_idx);
  int convertPiece(enum Piece piece, PlaySide side);
  enum pieces transformPiece(enum pieces piece);

  /* checking functions */
  bool isKingInCheck(Move *move, bool makeMove, struct state currentState, PlaySide side);
  bool isValidMove(int i, int j);
  bool isValidPawnMove(int i);
  bool isCastling(int srcRank, int srcFile, int destRank, int destFile, int board_game[8][8]);
  bool isEnPassant(int srcRank, int srcFile, int destRank, int destFile, int board_game[8][8]);

  /* generating functions */
  void generateKingMoves(vector<Move *> &possibleMoves, int rank, int file, PlaySide side, struct state currentState);
  void generateRookMoves(vector<Move *> &possibleMoves, int rank, int file, PlaySide side, struct state currentState);
  void generatePawnMoves(vector<Move *> &possibleMoves, int rank, int file, PlaySide side, struct state currentState);
  void generateKnightMoves(vector<Move *> &possibleMoves, int rank, int file, PlaySide side, struct state currentState);
  void generateBishopMoves(vector<Move *> &possibleMoves, int rank, int file, PlaySide side, struct state currentState);
  void generateQueenMoves(vector<Move *> &possibleMoves, int rank, int file, PlaySide side, struct state currentState);
  void generateCrazyhouseMoves(vector<Move *> &possibleMoves, int rank, int file, PlaySide side, struct state currentState);
  vector<Move *> generateMoves(vector<Move *>& possibleMoves, PlaySide side, struct state currentState);
  void generateCastlingMoves(vector<Move *> &possibleMoves, PlaySide side, struct state currentState);
  void generatePromotionMoves(vector<Move*> &possibleMoves, int rank, int file, PlaySide side, struct state currentState);
  void generateEnPassantMoves(vector<Move *> &possibleMoves, int rank, int file, PlaySide side, struct state currentState);

  /* record functions */
  void capturePiece(int destRank, int destFile, PlaySide sideToMove, struct state& currentState);
  void recordDropIn(Move* move, int destRank, int destFile, PlaySide sideToMove, struct state& currentState);
  void recordPromotion(Move* move, int destRank, int destFile, int srcRank, int srcFile, PlaySide sideToMove, struct state& currentState);
  void setFlags(int srcRank, int srcFile, int destRank, int destFile, PlaySide sideToMove, struct state& currentState);
  void recordNormalMove(Move* move, int destRank, int destFile, int srcRank, int srcFile, PlaySide sideToMove, struct state& currentState);
  void recordEnPassant(int destRank, int destFile, int srcRank, int srcFile, PlaySide sideToMove, struct state& currentState);
  void recordCastling(int destFile, PlaySide sideToMove, struct state& currentState);
  void recordAdvance(int destRank, int destFile, int srcRank, int srcFile, struct state& currentState);
  void recordNormalCapture(int destRank, int destFile, int srcRank, int srcFile, PlaySide sideToMove, struct state& currentState);

  void simulateRecordMove(Move* move, PlaySide sideToMove, int board[8][8]);
  void recordStatus(Move* &move, PlaySide sideToMove, struct state& currentState);

  /* negamax functions */
  Move* chooseBestMove(int depth, PlaySide side, struct state currentState);
  void cloneState(struct state& newState, struct state& oldState);
  int alphabeta_negamax(struct state& currentState, int depth, int alpha, int beta, PlaySide side);
  int evaluateBoard(struct state currentState, PlaySide side);
  int getValueForPieces(struct state currentState, PlaySide side);

};
#endif
