// Term Project by Kornelija G iifu-17.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "Header.h"


struct coordinate {
	int x, y;
};

enum playerState {
	alive, dead, won
};

playerState state = alive;

int points = 0;

class Board {
public:

	vector< vector<char> > boardVec = {
			{ '.', '.', '.', '.', '*', '.', '*', '.', '.', '.' },
			{ '.', '.', '*', '.', '.', '*', '.', '.', '.', '*' },
			{ '.', '.', '.', '.', '.', '.', 'T', '*', '.', '.' },
			{ '.', '.', '.', '*', '.', '*', '*', '.', '*', '.' },
			{ 'T', '*', '.', '.', '.', '.', '.', '.', '.', 'T' },
			{ '.', '.', '.', '*', '*', '.', 'T', '.', '.', '.' },
			{ '*', '.', '*', 'T', '.', '.', '*', '.', '.', 'X' }
	};

	void print() {
		for (int i = 0; i < 7; i++) {
			for (int j = 0; j < 10; j++) {
				cout << boardVec[i][j];
				if (j == 9) {
					cout << "\n";
				}
			}
		}
		cout << "\nPoints: " << points << " \n";
	}

	char moveSymbol(coordinate currentCoord, coordinate newCoord, char playerSymbol, char prevSymbol) {
		char saveCoord = boardVec[newCoord.y][newCoord.x];
		boardVec[currentCoord.y][currentCoord.x] = prevSymbol;

		if (playerSymbol == 'P') {
			if (boardVec[newCoord.y][newCoord.x] == '.') {
				boardVec[newCoord.y][newCoord.x] = playerSymbol;
			}
			else if (boardVec[newCoord.y][newCoord.x] == '*') {
				boardVec[newCoord.y][newCoord.x] = playerSymbol;
				saveCoord = '.';
				points++;
			}
		}
		else if (playerSymbol == 'E') {
			if (boardVec[newCoord.y][newCoord.x] == '.') {
				boardVec[newCoord.y][newCoord.x] = playerSymbol;
			}
			else if (boardVec[newCoord.y][newCoord.x] == 'P') {
				boardVec[newCoord.y][newCoord.x] = playerSymbol;
				cout << "\n\nOh no! An enemy caught you! You lose." << endl;
				state = dead;
			}
			else if (boardVec[newCoord.y][newCoord.x] == '*') {
				boardVec[newCoord.y][newCoord.x] = playerSymbol;
				saveCoord = '.';
			}
		}
		return saveCoord;
	}
};

class BoardController {
public:
	coordinate currentCoordinate, newCoordinate;
	Board * board = nullptr;
	char playerSymbol;
	char occupiedSymbol;
	char occupiedSymbolTemp;

	void moveLeft() {
		newCoordinate.x--;
		if (newCoordinate.x < 0) {
			newCoordinate.x = 0;
			return;
		}

		occupiedSymbol = board->moveSymbol(currentCoordinate, newCoordinate, playerSymbol, occupiedSymbol);
		currentCoordinate = newCoordinate;
	}
	void moveRight() {
		newCoordinate.x++;
		if (newCoordinate.x > 9) {
			newCoordinate.x = 9;
			return;
		}

		occupiedSymbol = board->moveSymbol(currentCoordinate, newCoordinate, playerSymbol, occupiedSymbol);
		currentCoordinate = newCoordinate;
	}
	void moveUp() {
		newCoordinate.y--;
		if (newCoordinate.y < 0) {
			newCoordinate.y = 0;
			return;
		}

		occupiedSymbol = board->moveSymbol(currentCoordinate, newCoordinate, playerSymbol, occupiedSymbol);
		currentCoordinate = newCoordinate;
	}
	void moveDown() {
		newCoordinate.y++;
		if (newCoordinate.y > 6) {
			newCoordinate.y = 6;
			return;
		}

		occupiedSymbol = board->moveSymbol(currentCoordinate, newCoordinate, playerSymbol, occupiedSymbol);
		currentCoordinate = newCoordinate;
	}

	void spawn(int x, int y, char newPlayerSymbol) {
		currentCoordinate.x = x;
		currentCoordinate.y = y;
		newCoordinate.x = x;
		newCoordinate.y = y;
		playerSymbol = newPlayerSymbol;
		occupiedSymbol = board->boardVec[y][x];
		board->boardVec[y][x] = playerSymbol;
	}
};

class Player : public BoardController {

public:
	char di;

	void checkPlayerState() {
		if (occupiedSymbol == 'T') {
			cout << "\n\nOh no! You stepped on a trap! You lose." << endl;
			state = dead;
		}
		if (occupiedSymbol == 'E') {
			cout << "\n\nOh no! The enemy caught you! You lose." << endl;
			state = dead;
		}
		else if (occupiedSymbol == 'X') {
			cout << "\n\nReached the treasure succesfully! Congrats - you win!" << endl;
			state = won;
		}
	}

	virtual void move() {
		moveAI();
	}

	void moveAI() {
		int di = rand() % 4;
		switch (di) {
		case 0: moveLeft();
			break;
		case 1: moveRight();
			break;
		case 2: moveUp();
			break;
		case 3: moveDown();
			break;
		}
	}

	void moveHuman() {
		cin >> di;
		cout << "\n";
		switch (di) {
		case 'a': moveLeft();
			break;
		case 'd': moveRight();
			break;
		case 'w': moveUp();
			break;
		case 's': moveDown();
			break;
		}
		system("CLS");
	}
};

class Human : public Player {
public:

	void move() {
		moveHuman();
	}
	void operator ++()
	{
		move();
		checkPlayerState();
	}
};

class Enemy : public Player {
public:
	void operator ++()
	{
		move();
	}

};


int main()
{
	Board *board = new Board;

	Human human;
	human.board = board;

	Enemy enemy1, enemy2;
	enemy1.board = board;
	enemy2.board = board;

	enemy1.spawn(0, 0, 'E');
	enemy2.spawn(rand() % 7, rand() % 10, 'E');
	human.spawn(1, 1, 'P');


	cout << "Controls: w - up, s - down, a - left, d - right. \n\n";
	cout << "Instructions:\n- You are the player, P.\n- If you step on trap T - you lose.\n- Es are enemies - don't get caught!\n- Collect gems that look like * before enemies steal them. More gems mean more points!\n- To win - reach treasure marked by X.\n\n";
	board->print();
	while (state == alive) {
		++human; // human player movement
		++enemy1; // enemy movement
		++enemy2; 
		board->print();
	}
	cout << "\nGame over.\n";

	return 0;

}