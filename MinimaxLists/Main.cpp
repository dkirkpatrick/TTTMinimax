#include "TTTEngine.h"
#include "Random.h"
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std; 

map<int, vector<int>> p1MinimaxPlay = map<int, vector<int>>();
map<int, vector<int>> p2MinimaxPlay = map<int, vector<int>>();
map<int, int> p1MinimaxVal = map<int, int>();
map<int, int> p2MinimaxVal = map<int, int>();
map<int, map<vector<int>, int>> p1PlayValues = map<int, map<vector<int>, int>>();
map<int, map<vector<int>, int>> p2PlayValues = map<int, map<vector<int>, int>>();
map<int, pair<int,vector<int>>> boardMinimumTransformations = map<int, pair<int, vector<int>>>();
map<int, int> boardOverview = map<int, int>(); 
int outercount = 0;

int getOtherPlayer(int player) {
	if (player == 1) {
		return 2; 
	}
	else if (player == 2) {
		return 1; 
	}
	else {
		return -1; 
	}
}

int minusOneAbs(int i) {
	if (i < 0) {
		return i+1; 
	} else if (i > 0) {
		return i - 1;
	} else {
		return 0; 
	}
}

int minimax(TTTGame game, int player, int x, int y, int depth, std::vector<int>& whichPermutations) {
	game.play(x, y, getOtherPlayer(player));
	if (player == 1 && p1MinimaxVal.count(game.base3Board()) > 0) {
		return p1MinimaxVal.at(game.base3Board());
	}
	if (player == 2 && p2MinimaxVal.count(game.base3Board()) > 0) {
		return p2MinimaxVal.at(game.base3Board());
	}
	if (game.gameWon() == 2) {
		return -20;
	}
	if (game.gameWon() == 1) {
		return 20;
	}
	if (depth == 18 || game.gameDraw()) {
		return 0;
	}
	vector<vector<int>> myPlays = game.getValidPlays();
	map<vector<int>, int> values;
	for (auto p : myPlays) {
		TTTGame newGame(game); 
		int score = minimax(newGame, getOtherPlayer(player), p.at(0), p.at(1), depth + 1, whichPermutations);
		values.emplace(p, minusOneAbs(score));
	}
	TTTGame tempGame(game);
	int nullscore = minimax(tempGame, getOtherPlayer(player), -1, -1, depth + 1);
	auto b = values.rbegin();
	int value = b->second;

	if (boardMinimumTransformations.count(game.base3Board()) <= 0) {
		if (whichPermutations.size() == 0) {
			auto p = game.getMinimumBoardAndPermutation();
		}
		else {
			auto p = game.getMinimumBoardAndPermutationUsingSubset(whichPermutations);
		}
		boardMinimumTransformations.emplace(game.base3Board(), p);
		if (boardOverview.count(p.first) <= 0) {
			boardOverview.emplace(p.first, outercount);
			outercount++;
		}
	}

	vector<int> play = b->first;
	if (player == 1) {
		for (auto v : values) {
			if (v.second > value) {
				value = v.second;
				play = v.first;
			}
		}
		p1MinimaxPlay.emplace(game.base3Board(), play);
		p1MinimaxVal.emplace(game.base3Board(), value);
		p1PlayValues.emplace(game.base3Board(), values);
	}
	if (player == 2) {
		for (auto v : values) {
			if (v.second < value) {
				value = v.second;
				play = v.first;
			}
		}
		p2MinimaxPlay.emplace(game.base3Board(), play);
		p2MinimaxVal.emplace(game.base3Board(), value);
		p2PlayValues.emplace(game.base3Board(), values);
	}
	return value;
}

void playGame() {
	int myPlayer = -1; 
	while (myPlayer < 1 || myPlayer > 2) {
		cout << "Do you want to play as player 1 or player 2?" << endl; 
		cout << "(enter 1 or 2)" << endl;
		cin >> myPlayer; 
	}
	int computerPlayer = getOtherPlayer(myPlayer); 
	TTTGame myGame = TTTGame(3);
	bool computerPlays = (computerPlayer == 1); 
	while (myGame.gameWon() == 0 && !myGame.gameDraw()) {
		if (computerPlays) {
			vector<int> myPlay; 
			if (computerPlayer == 1) {
				myPlay = p1MinimaxPlay.at(myGame.base3Board()); 
			}
			if (computerPlayer == 2) {
				myPlay = p2MinimaxPlay.at(myGame.base3Board());
			}
			myGame.play(myPlay.at(0), myPlay.at(1), computerPlayer); 
			computerPlays = false;
		}
		else {
			cout << "You are player " << myPlayer << ". The last play was at " << myGame.getLastPlayString() << endl; 
			cout << "The current board is: " << endl; 
			myGame.printBoard(); 
			int x = -1;
			int y = -1;
			cout << "What is the x position of the play you want to make (0,1,2)?"; 
			cin >> x; 
			cout << "What is the y position of the play you want to make (0,1,2)?";
			cin >> y;
			myGame.play(x, y, myPlayer); 
			computerPlays = true; 
		}
	}
	if (myGame.gameDraw()) {
		cout << "The game ended in a draw!" << endl; 
	} else if (myGame.gameWon() == computerPlayer) {
		cout << "The computer won the game!" << endl; 
	} else {
		cout << "You won the game!" << endl; 
	}
}

int main() {
	TTTGame mGame = TTTGame(3); 
	int i, j; 
	
	i = minimax(mGame, 1, -1, -1, 0);
	
	cout << boardOverview.size() << endl;
	cout << outercount << endl;

	string transformationOutput = "C:\\Source\\transformationOutputDictionary.txt";
	ofstream transformationOutputStream(transformationOutput);



	transformationOutputStream << "\tstd::vector<int> myTempVector = std::vector<int>();" << endl;
	transformationOutputStream << "\tstd::pair<int, std::vector<int>> myTempPair = std::pair<int, std::vector<int>>(); " << endl;
	transformationOutputStream << "\tstd::map<int, pair<int, vector<int>>> transformationDictionary = std::map<int, pair<int, vector<int>>>();" << endl;
	for (auto e : boardMinimumTransformations) {
		vector<int> i = e.second.second; 
		transformationOutputStream << "\tmyTempVector = "; 
		transformationOutputStream << "{ " << i[0] << ", " << i[1] << ", " << i[2] << ", " << i[3] << ", " << i[4] << ", " << i[5] << ", " << i[6] << ", " << i[7] << ", " << i[8] << " };" << endl;
		transformationOutputStream << "\tstd::pair<int, std::vector<int>> myTempPair = std::pair<int, std::vector<int>>( " << e.second.first << ", myTempVector);" << endl;
		transformationOutputStream << "\ttransformationDictionary.emplace( " << e.first << " , myTempMap);" << endl;
	}

	/*
	p1MinimaxPlay;

	char c = 'y'; 
	cout << "Do you want to play versus the optimal opponent?" << endl;
	cout << "(enter y or n)" << endl;
	cin >> c;
	while (tolower(c) == 'y') {
		playGame();
		cout << "Do you want to play versus the optimal opponent?" << endl; 
		cout << "(enter y or n)" << endl;
		cin >> c; 
	}

	string p1Output = "C:\\Source\\player1OutputDictionary.txt";
	string p2Output = "C:\\Source\\player2OutputDictionary.txt";
	ofstream p1OStream(p1Output);
	ofstream p2OStream(p2Output);
	
	p1OStream << "\tstd::vector<int> myTempVector = std::vector<int>()" << endl;
	p1OStream << "\tstd::map<int, map<vector<int>, int>> playDictionary = std::map<int, map<vector<int>, int>>();" << endl;
	for (auto e : p1PlayValues) {
		p1OStream << "\tstd::map<std::vector<int>,int> myTempMap = std::map<std::vector<int>,int>()" << endl; 
		for (auto e2 : e.second) {
			p1OStream << "\tmyTempVector = { " << e2.first.at(0) << " , " << e2.first.at(1) << " };"; 
			p1OStream << "\tmyTempMap.emplace( myTempVector, " << e2.second << " );" << endl;
		}
		p1OStream << "\tplayDictionary.emplace( " << e.first << " , myTempMap);" << endl; 
	}
	*/
	cin >> i; 
	return 0; 
}