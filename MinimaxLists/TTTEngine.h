//
//  TTTEngine.h
//
//  Created by Douglas Kirkpatrick on  3/30/16.
//  Copyright (c) 2016 Douglas Kirkpatrick. All rights reserved.
//

#ifndef __TTTEngine__
#define __TTTEngine__

#include<vector>
#include<iostream> 
#include<string>
class TTTGame {
public:
	TTTGame();
	TTTGame(int boardSize);
	TTTGame(const TTTGame & mGame);
	bool play(int xPos, int yPos, int player);
	bool play(int val, int player);
	std::vector<std::vector<int>> getBoard();
	std::vector<int> getLastPlay();
	std::vector<std::vector<int>> getValidPlays();
	int gameWon();
	bool gameDraw();
	bool validPlay(int xPos, int yPos, int player);
	void printBoard();
	void printOtherBoard(std::vector<std::vector<int>> board);
	void printBase3Board(int base3Board);
	int base3Board();
	int base3OtherBoard(std::vector<std::vector<int>> board);
	int base3OtherBoardFrom1D(std::vector<int> board);
	std::vector<std::vector<int>> boardFromBase3(int base3Board);
	std::string getLastPlayString();

	std::vector<int> get1DBoard(std::vector<std::vector<int>> inBoard);
	std::vector<std::vector<int>> get2DBoard(std::vector<int> inBoard); 
	std::vector<int> getFlipedBoard(std::vector<int> myBoard);
	bool boardEquality(std::vector<int> board1, std::vector<int> board2);
	std::vector<int> getRotatedBoard(std::vector<int> myBoard);
	std::vector<int> getPermutedBoard(std::vector<int> myBoard, std::vector<int> myScreen);
	std::vector<int> getInversePermutation(std::vector<int> inPermutation);
	std::pair<int, std::vector<int>> getOtherMinimumBoardAndPermutation(std::vector<int> myBoard);
	std::pair<int, std::vector<int>> getMinimumBoardAndPermutation(); 
	std::pair<int, std::vector<int>> getOtherMinimumBoardAndPermutationUsingSubset(std::vector<int> inBoard, std::vector<int> whichPermutations); 
	std::pair<int, std::vector<int>> getMinimumBoardAndPermutationUsingSubset(std::vector<int> whichPermutations);
	// sets the turn restrictions to a given value
	void setTurnRestrictions(bool value) {
		turnRestrictions = value;
	}
private:
	void updateWon(int xPos, int yPos, int player);
	int myWhoWon;
	int myNumPlay;
	int myBoardSize;
	bool turnRestrictions;
	std::vector<int> myLastPlay;
	std::vector<std::vector<int>> myBoard;
	std::vector<std::vector<int>> myMasks;
};

#endif 