//
//  TTTEngine.cpp
//  BasicMarkovBrainTemplate
//
//  Created by Douglas Kirkpatrick on 3/30/16.
//  Copyright (c) 2016 Douglas Kirkpatrick. All rights reserved.
//

#include "TTTEngine.h"
#include <sstream>
#include <math.h>

// Defaults to a standard 3x3 board
TTTGame::TTTGame() {
	TTTGame(3);
}

// Allows for different board sizes, sets up board and last play
// Default for turn restrictions is false 
TTTGame::TTTGame(int boardSize) {
	myWhoWon = 0;
	myNumPlay = 1;
	turnRestrictions = false;
	myBoardSize = boardSize;
	myLastPlay = std::vector<int>();
	for (int i = 0; i < 3; i++) {
		myLastPlay.push_back(0);
	}
	myBoard = std::vector<std::vector<int>>();
	for (int i = 0; i < myBoardSize; i++) {
		std::vector<int> temp = std::vector<int>();
		for (int j = 0; j < myBoardSize; j++) {
			temp.push_back(0);
		}
		myBoard.push_back(temp);
	}
	std::vector<int> mask1 = { 0, 3, 6, 1, 4, 7, 2, 5, 8 };
	std::vector<int> mask2 = { 8, 7, 6, 5, 4, 3, 2, 1, 0 };
	std::vector<int> mask3 = { 2, 1, 0, 5, 4, 3, 8, 7, 6 };
	std::vector<int> mask4 = { 8, 5, 2, 7, 4, 1, 6, 3, 0 };
	std::vector<int> mask5 = { 6, 7, 8, 3, 4, 5, 0, 1, 2 };
	std::vector<int> mask6 = { 6, 3, 0, 7, 4, 1, 8, 5, 2 };
	std::vector<int> mask7 = { 2, 5, 8, 1, 4, 7, 0, 3, 6 };
	myMasks.push_back(mask1);
	myMasks.push_back(mask2);
	myMasks.push_back(mask3);
	myMasks.push_back(mask4);
	myMasks.push_back(mask5);
	myMasks.push_back(mask6);
	myMasks.push_back(mask7);

}

// Copy constructor
TTTGame::TTTGame(const TTTGame & mGame) {
	this->myWhoWon = mGame.myWhoWon;
	this->myNumPlay = mGame.myNumPlay;
	this->myBoardSize = mGame.myBoardSize;
	this->myLastPlay = std::vector<int>();
	this->turnRestrictions = mGame.turnRestrictions;
	for (int i = 0; i < 3; i++) {
		this->myLastPlay.push_back(mGame.myLastPlay[i]);
	}
	myBoard = std::vector<std::vector<int>>();
	for (int i = 0; i < myBoardSize; i++) {
		std::vector<int> temp = std::vector<int>();
		for (int j = 0; j < myBoardSize; j++) {
			temp.push_back(mGame.myBoard[i][j]);
		}
		myBoard.push_back(temp);
	}

	for (auto m : mGame.myMasks) {
		this->myMasks.push_back(m);
	}
}

// Handles the plays made by either player
// Checks if play is valid before allowing play
bool TTTGame::play(int xPos, int yPos, int player) {
	if (validPlay(xPos, yPos, player) && (myWhoWon == 0)) {
		myBoard[xPos][yPos] = player;
		myNumPlay++;
		myLastPlay[0] = xPos;
		myLastPlay[1] = yPos;
		myLastPlay[2] = player;
		updateWon(xPos, yPos, player);
		return true;
	}
	return false;
}

bool TTTGame::play(int val, int player) {
	return play(val / 3, val % 3, player);
}

// Returns the board for outside use 
std::vector<std::vector<int>> TTTGame::getBoard() {
	return myBoard;
}

// Returns a list of positions on the board that are empty
std::vector<std::vector<int>> TTTGame::getValidPlays() {
	std::vector<std::vector<int>> tempList = std::vector<std::vector<int>>();
	for (int i = 0; i < myBoardSize; i++) {
		for (int j = 0; j < myBoardSize; j++) {
			if (myBoard[i][j] == 0) {
				std::vector<int> tempPos = std::vector<int>();
				tempPos.push_back(i);
				tempPos.push_back(j);
				tempList.push_back(tempPos);
			}
		}
	}
	return tempList;
}

// Returns the last play made on the board 
std::vector<int> TTTGame::getLastPlay() {
	return myLastPlay;
}

// Returns who has won (Player number or 0 if neither has won)
int TTTGame::gameWon() {
	return myWhoWon;
}

// Returns true if the game is a draw
bool TTTGame::gameDraw() {
	return (myNumPlay - 1 == (myBoardSize*myBoardSize)) && (gameWon() == 0);
}

// Returns true if the potential play is valid (with turn restrictions if active)
bool TTTGame::validPlay(int xPos, int yPos, int player) {
	if (xPos < myBoardSize && xPos >= 0 && yPos < myBoardSize && yPos >= 0) {
		if (turnRestrictions) {
			return (myBoard[xPos][yPos] == 0) && (player % 2 == myNumPlay % 2);
		}
		else {
			return (myBoard[xPos][yPos] == 0);
		}

	}
	return false;
}

// Checks the row, column, and diagonals that were played in
// sets whoWon if the player made a winning move 
void TTTGame::updateWon(int xPos, int yPos, int player) {
	bool wonRow = true, wonCol = true, wonDiag = true, wonAltDiag = true;
	for (int i = 0; i < myBoardSize; i++) {
		if (myBoard[xPos][i] != player) {
			wonRow = false;
		}
		if (myBoard[i][yPos] != player) {
			wonCol = false;
		}
		if (myBoard[i][(myBoardSize - 1) - i] != player) {
			wonAltDiag = false;
		}
		if (myBoard[i][i] != player) {
			wonDiag = false;
		}
	}
	if (wonDiag || wonRow || wonCol || wonAltDiag) {
		myWhoWon = player;
	}
}

// Prints the board to cout
void TTTGame::printBoard() {
	printOtherBoard(myBoard);
}

// Prints a board to cout from a base 3 representation
void TTTGame::printBase3Board(int base3Board) {
	printOtherBoard(boardFromBase3(base3Board));
}

// Converts the current board to a base 3 representation
int TTTGame::base3Board() {
	int result = 0;
	int place = 0;
	for (int i = 0; i < myBoardSize; i++) {
		for (int j = 0; j < myBoardSize; j++) {
			result += myBoard[i][j] * pow(3, place);
			place++;
		}
	}
	return result;
}


// Converts the current board to a base 3 representation
int TTTGame::base3OtherBoard(std::vector<std::vector<int>> board) {
	int result = 0;
	int place = 0;
	for (int i = 0; i < myBoardSize; i++) {
		for (int j = 0; j < myBoardSize; j++) {
			result += board[i][j] * pow(3, place);
			place++;
		}
	}
	return result;
}

int TTTGame::base3OtherBoardFrom1D(std::vector<int> board) {
	int result = 0;
	int place = 0;
	for (int i = 0; i < myBoardSize*myBoardSize; i++) {
		result += board[i] * pow(3, place);
		place++;
	}
	return result;
}

// Converts a board from base 3 to a vector of vector of ints 
std::vector<std::vector<int>> TTTGame::boardFromBase3(int base3Board) {
	std::vector<std::vector<int>> tempBoard = std::vector<std::vector<int>>(myBoardSize, std::vector<int>(myBoardSize, 0));
	int tempVal = base3Board;
	for (int i = 0; i < myBoardSize; i++) {
		for (int j = 0; j < myBoardSize; j++) {
			tempBoard[i][j] = tempVal % 3;
			tempVal = tempVal / 3;
		}
	}
	return tempBoard;
}

// Prints a board to cout with formatting 
void TTTGame::printOtherBoard(std::vector<std::vector<int>> board) {
	std::cout << std::endl;
	for (int i = 0; i < myBoardSize; i++) {
		std::cout << " ";
		for (int k = 0; k < myBoardSize * 2; k++) {
			std::cout << "--";
		}
		std::cout << "-";
		std::cout << std::endl;
		for (int j = 0; j < myBoardSize; j++) {
			std::cout << " | " << board[j][i];
		}
		std::cout << " | " << std::endl;
	}
	std::cout << " ";
	for (int i = 0; i < myBoardSize * 2; i++) {
		std::cout << "--";
	}
	std::cout << "-";
	std::cout << std::endl;
}

// Returns a nicely formmated version of the last play 
std::string TTTGame::getLastPlayString() {
	std::stringstream ss;
	ss << " ( " << myLastPlay.at(0) << " , " << myLastPlay.at(1) << " ) by player " << myLastPlay.at(2) << " ";
	return ss.str();
}


std::vector<int> TTTGame::get1DBoard(std::vector<std::vector<int>> inBoard) {
	std::vector<int> temp = std::vector<int>();
	for (int i = 0; i < myBoardSize; i++) {
		for (int j = 0; j < myBoardSize; j++) {
			temp.push_back(inBoard[i][j]);
		}
	}
	return temp;
}

std::vector<std::vector<int>> TTTGame::get2DBoard(std::vector<int> inBoard) {
	std::vector<std::vector<int>> myBoard;
	myBoard = std::vector<std::vector<int>>();
	int k = 0;
	for (int i = 0; i < myBoardSize; i++) {
		std::vector<int> temp = std::vector<int>();
		for (int j = 0; j < myBoardSize; j++) {
			temp.push_back(inBoard[k]);
			k++;
		}
		myBoard.push_back(temp);
	}
	return myBoard;
}


std::vector<int> TTTGame::getFlipedBoard(std::vector<int> myBoard) {
	std::vector<int> screen = { 6,7,8,3,4,5,0,1,2 };
	std::vector<int> temp = std::vector<int>(9, 0);
	for (int i = 0; i < 9; i++) {
		temp[i] = myBoard[screen[i]];
	}
	return temp;
}

bool TTTGame::boardEquality(std::vector<int> board1, std::vector<int> board2) {
	for (int i = 0; i < 9; i++) {
		if (board1[i] != board2[i]) {
			return false;
		}
	}
	return true;
}

std::vector<int> TTTGame::getRotatedBoard(std::vector<int> myBoard) {
	std::vector<int> screen = { 2,5,8,1,4,7,0,3,6 };
	std::vector<int> temp = std::vector<int>(9, 0);
	for (int i = 0; i < 9; i++) {
		temp[i] = myBoard[screen[i]];
	}
	return temp;
}

std::vector<int> TTTGame::getPermutedBoard(std::vector<int> inBoard, std::vector<int> inScreen) {
	std::vector<int> temp = std::vector<int>(9, 0);
	for (int i = 0; i < 9; i++) {
		temp[i] = inBoard[inScreen[i]];
	}
	return temp;
}

std::vector<int> TTTGame::getInversePermutation(std::vector<int> inPermutation)
{
	std::vector<int> mask6 = { 6, 3, 0, 7, 4, 1, 8, 5, 2 };
	std::vector<int> mask7 = { 2, 5, 8, 1, 4, 7, 0, 3, 6 };
	if (boardEquality(mask6, inPermutation)) {
		return mask7; 
	}
	if (boardEquality(mask7, inPermutation)) {
		return mask6;
	}
	return std::vector<int>(inPermutation);
}

std::pair<int, std::vector<int>> TTTGame::getOtherMinimumBoardAndPermutation(std::vector<int> inBoard)
{
	int minValue = base3OtherBoard(get2DBoard(inBoard));
	std::vector<int> outMask = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
	for (auto v : myMasks) {
		int test = base3OtherBoard(get2DBoard(getPermutedBoard(inBoard, v)));
		if (test < minValue) {
			minValue = test; 
			outMask = v; 
		}
	}
	return std::pair<int, std::vector<int>>(minValue, outMask);
}

std::pair<int, std::vector<int>> TTTGame::getOtherMinimumBoardAndPermutationUsingSubset(std::vector<int> inBoard, std::vector<int> whichPermutations)
{
	int minValue = base3OtherBoard(get2DBoard(inBoard));
	std::vector<int> outMask = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
	for (auto v : whichPermutations) {
		int test = base3OtherBoard(get2DBoard(getPermutedBoard(inBoard, myMasks[v])));
		if (test < minValue) {
			minValue = test;
			outMask = myMasks[v];
		}
	}
	return std::pair<int, std::vector<int>>(minValue, outMask);
}

std::pair<int, std::vector<int>> TTTGame::getMinimumBoardAndPermutation()
{
	std::vector<int> tempBoard = get1DBoard(myBoard); 
	int minValue = base3OtherBoardFrom1D(tempBoard);
	std::vector<int> outMask = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
	for (auto v : myMasks) {
		int test = base3OtherBoardFrom1D(getPermutedBoard(tempBoard, v));
		if (test < minValue) {
			minValue = test;
			outMask = v;
		}
	}
	return std::pair<int, std::vector<int>>(minValue, outMask);
}

std::pair<int, std::vector<int>> TTTGame::getMinimumBoardAndPermutationUsingSubset(std::vector<int> whichPermutations)
{
	std::vector<int> tempBoard = get1DBoard(myBoard);
	int minValue = base3OtherBoardFrom1D(tempBoard);
	std::vector<int> outMask = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
	for (auto v : whichPermutations) {
		int test = base3OtherBoardFrom1D(getPermutedBoard(tempBoard, myMasks[v]));
		if (test < minValue) {
			minValue = test;
			outMask = myMasks[v];
		}
	}
	return std::pair<int, std::vector<int>>(minValue, outMask);
}