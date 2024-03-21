
 ```_
    ___ _                   ____         _    
  / ___| |__   ___  ___ ___| __ )  ___ | |_  
 | |   | '_ \ / _ \/ __/ __|  _ \ / _ \| __| 
 | |___| | | |  __/\__ \__ \ |_) | (_) | |_  
  \____|_| |_|\___||___/___/____/ \___/ \__| 
  _ _ __  __                  _          _ _ 
 ( | )  \/  | __ _ _   _ _ __(_) ___ ___( | )
  V V| |\/| |/ _` | | | | '__| |/ __/ _ \V V 
     | |  | | (_| | |_| | |  | | (_|  __/    
     |_|  |_|\__,_|\__,_|_|  |_|\___\___|

```

### **Copyright**: Dobrica Nicoleta-Adriana, Mirza Ana-Maria, Popa Bianca, Andrei Calin-Mihail 321CA

---

## **Description**:

&rarr; This projects creates a Crazyhouse Chess Bot. Aside from the usual chess rules, this variant adds four new modifications to the game. 

> In Crazyhouse, captured pieces can be dropped back onto the game table, becoming your own.

&rarr; All the user interface, game visualization and the chess playing process was done with Xboard. The bot, Maurice, communicates directly with Xboard and interacts or interprets certain commands.

## **Installation**:

&rarr; In order to run the program, you need to have Xboard installed on your computer.

&rarr; Compile the program with the following command:

```
> make
```

&rarr; Load the bot in Xboard:
```
> Engine -> Load new 1st engine -> ./Main
```

&rarr; [Optional]: Mode -> Machine White/ Machine Black if you want a certain side to play first.

## **Board representation**:

&rarr; In order to have an internal representation of the board, so that we can easily maintain all the chess positions when searching or evaluating new moves, we used a `square centric` board representation, with an 8x8 board of two-dimensional arrays of integers.

&rarr; The 8x8 board contains a simple piece and empty square encoding, per convention:

> P, R, B, N, K, Q: **white pieces**

> p, r, b, n, k, q: **black pieces**

> e: **empty square**

&rarr; When searching for moves or empty squares, or sending certain moves for the bot, we still need to convert our file and rank indexes, because a chess board's rank index starts from 8 to 1, and its file index from `a` to `h`. 

## **Bot structure and move generation:**

&rarr; After creating the board representation, we implemented our most important feature in the game: Maurice, the chess bot.

 > This class has many custom fields so that we could easily keep track of certain changes in the board table, how to respond to them and how to keep track of certain conditions in order to perform chess-specific moves (castling, en passant etc). 

&rarr; The entire program flow of the chess board consists of the bot, which responds to the user's moves accordingly. Based on the surrounding enemy moves or empty squares, the bot calculates firstly all its pseudo-legal moves. Afterwards, we need just one move to send to the bot, so now it comes down to checking whether the generated pseudo-legal moves are legal and don't leave the king in check, and if they are, they can be used further in the game. The used move for the bot from the list of legal ones is chosen randomly for now.

> The main idea behind the generation of pseudo-legal moves is to generate all the possible moves available at the given moment for all of the pieces from the bot's side. 

&rarr; For the pawns, we check if they are in the starting position, and if they are, we can move them two squares forward. Afterwards, we check if the pawn can move one square forward, or if it can capture an enemy piece on the diagonals. 

&rarr; For the knights, we check if they can move in any of the 8 possible directions, and if they can, we generate the moves.

&rarr; For the bishops, we check if they can move in any of the 4 diagonal directions, and if they can, we generate the moves.

&rarr; For the rooks, we check if they can move in any of the 4 horizontal or vertical directions, and if they can, we generate the moves.

&rarr; For the queens, we check if they can move in any of the 8 possible directions, and if they can, we generate the moves.

&rarr; For the kings, we check if they can move in any of the 8 possible directions, and if they can, we generate the moves.

> After generating all the possible moves for each piece, we check if the move is legal, and if it is, we add it to the list of legal moves. A move is legal if the king is not in check after the move is executed.

> The **move generation algorithms** for each piece are similar, because we used directions arrays for them, which contain the possible directions in which the piece can move, similar to Lee's algorithm.

&rarr; The bot also keeps track of en passant, castling, and promotion moves. 

> Only a pawn can perform the en passant move. After checking whether the pawn has moved three squares forward, we also check the diagonals for enemies in order to perform the en passant move and the last move of the enemy.

> For castling, which can only be performed by the king and the rook, we check if the king and the rook have not previously moved. Only if the squares between them are empty and the king is not in check, we can perform the castling move.

> For promotion, if the pawn has reached the last rank, where the target square is empty, it can be promoted to a queen, rook, bishop or knight. It can also promote diagonally, if the target square is occupied by an enemy piece.

&rarr; **Crazyhouse moves:** Crazyhouse allows for captured pieces to be dropped back into the game table, their color becoming the one of the side who captured them. When generating possible crazyhouse moves, we added the possible moves for the dropped in pieces, except for the moves where the pawn is on the first or last rank, where it can be promoted.

## **Evaluation function:**

&rarr; The evaluation function used for this bot is a simplified version, where each piece gets a table of points (they can either get penalties for their current position on the table, 0 points or bonus points). Also, each piece gets a corresponding value based on their importance in the game. Depending on each piece's position on the board, the overall weight of the current game is a sum of all the pieces' points and their corresponding value, minus the opponent's sum.

## **Alpha-beta negamax algorithm:**

&rarr; For this algorithm, we first created an array of possible moves, and filtered them based on whether they would leave the king in check or not. If the depth of the search is 0, then the evaluation score will be returned. Otherwise, for efficiency, we are going to sort the moves based on their evaluation score. This is being done by creating an array of states. Each new state has a side, a move, and a score based on that specific move. 
   > **state** is a structure that contains the board game, a table of promoted pieces, and other arrays or flags that are used throughout the game. This structure basically represents the current state of the game. For the negamax algorithm, three additional fields were added: the side, a move, and the move's score.

&rarr; Then, for each possible move, we are going recursively through the negamax algorithm (alternating between the sides), until we find the best score.

&rarr; With the best score obtained by the negamax algorithm, we are going to choose the next best move for the bot that has the highest score, using a depth of 4.


## **Time & space complexities**:

&rarr; Calculate next move:
- Time: **O(n)**, where n is the number of possible legal moves, and is also the branching factor (the number of possible moves at each position)
- Space: **O(n)**

&rarr; Negamax algorithm:
- Time: **O(b^(d/2))**, where b - the branching factor, d - our depth (which is 4).

## **Individual work:**

> - Board representation: **Dobrica Nicoleta-Adriana**
> - Move generation: **Dobrica Nicoleta-Adriana**, **Mirza Ana-Maria**, **Popa Bianca**, **Andrei Calin-Mihail**
> - Bot structure: **Dobrica Nicoleta-Adriana**, **Mirza Ana-Maria**, **Popa Bianca**, **Andrei Calin-Mihail**
> - Crazyhouse moves: **Mirza Ana-Maria**, **Andrei Calin-Mihail**
> - Castling: **Mirza Ana-Maria**, **Andrei Calin-Mihail**
> - En passant: **Mirza Ana-Maria**, **Andrei Calin-Mihail**
> - Promotion: **Mirza Ana-Maria**, **Andrei Calin-Mihail**
> - Move recording: **Mirza Ana-Maria**
> - Evaluation function: **Dobrica Nicoleta-Adriana**
> - Negamax algorithm: **Mirza Ana-Maria**

## **References:**

> - [Chess Programming Wiki](https://www.chessprogramming.org/Main_Page)
> - [En Passant](https://en.wikipedia.org/wiki/En_passant)
> - [Castling](https://en.wikipedia.org/wiki/Castling)
> - [Promotion](https://en.wikipedia.org/wiki/Promotion_(chess))
> - [Crazyhouse](https://en.wikipedia.org/wiki/Crazyhouse)
> - [Xboard](https://www.gnu.org/software/xboard/)
> - [Board representation](https://www.chessprogramming.org/Board_Representation)
> - [Move generation](https://www.chessprogramming.org/Move_Generation)
> - [Xboard communication](https://www.gnu.org/software/xboard/engine-intf.html)
> - [Review of different board representations](https://lichess.org/@/likeawizard/blog/review-of-different-board-representations-in-computer-chess/S9eQCAWa)
> - [Simplified Evaluation function](https://www.chessprogramming.org/Simplified_Evaluation_Function)
> - [Negamax algorithm](https://en.wikipedia.org/wiki/Negamax)
