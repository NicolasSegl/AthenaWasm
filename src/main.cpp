#include <chrono>
#include <random>
#include <time.h>
#include <iostream>

#include "ChessGame.h"
#include "UCI.h"

// I LOVE YOU CHARLOTTE <3 I LOVE YOU MORE!!!!

int main(int argc, char** argv)
{
	std::srand(time(NULL));

    /*auto beforeTime = std::chrono::steady_clock::now();
    auto afterTime = std::chrono::steady_clock::now();

    Board board;
    board.setPositionFEN("r1b2rk1/pp1p1pp1/2pq3p/2bNp3/2BnP3/3P1N2/PPP2PPP/R2Q1RK1 w - - 0 11");
    int count = 0;
    
    // on average, around 1485838 times in a second
    while (std::chrono::duration<double>(afterTime - beforeTime).count() < 1)
    {
        count++;
        board.calculateSideMoves(SIDE_WHITE);
        afterTime = std::chrono::steady_clock::now();
    }
    std::cout << "number of times: " << count << std::endl;*/
	UCI uci;
	uci.run();
	return 0;
}

// TODO
// perft
// housecoat
// parallel search
// remove the ChessPosition class? doesn't really do anything ?
// insufficient material
// return Athena's caluclated move by reference ?
// get the engine to never use up more time than it has. keep a clock and every loop of negamax see if the time we have alloted it is up
// add some basic pawn structure stuff after uci
// implement SEE so that we can prune quiescence search
// static exchange evaluation
// TRANSPOSITION TABLE
// change the MoveData::Encoding bits flag so it's actually a bit flag
// rename zobristkeygenerator
// put more things in Constants.h?
// make structs/classes smaller in size
// add delta pruning

// and ENDGAME PLAY
// test the fifty move rule
// pawn hash table (hashing the pawn evaluations function)
// switch statement Board::promoteTo
// pawn structure https://www.chessprogramming.org/Pawn_Pattern_and_Properties
// move ordering
//  historic heuristic
//	consider also pawn promotions/advances
// futility pruning

// make all structures as small as possible. make movedata use bits set to get data
// refactor everything so that all functions are short and do only one thing, and that methods are in appropriate classes
// for the above maybe look to other chess engines online. some have classes only for Search and one for making Moves, etc
// null moves
// extensions
// search windows
//  one simple extension is to simply increase depth by 1 when under check
// reductions
// menus
// choosing what to upgrade a pawn to
