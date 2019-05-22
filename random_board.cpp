//Kevin Peterson
//CSCI 4350
//Random Board
//This program reads an 8 board from std input 
//and outputs a shuffled board to std output
#include <iostream>
#include <cstdlib>
using namespace std;


int main(int argc, char *argv[])
{
	//prototypes
	void moveZero(int &x, int &y, int board[3][3], int moveDir);
	void printBoard(int board[3][3]);
	
	
	int board[3][3];           //represents the board
	int zeroCoordinate[2] = {0,0};  //coordinate of the zero
	int zeroX = 0;
	int zeroY = 0;
	
	//setup random number generator
	int seed = atoi(argv[1]);
	int numMoves = atoi(argv[2]);
	srandom(seed);
	
	//read in board
	for (int r = 0; r < 3; r++)
	{
		for (int c = 0; c < 3; c++)
		{
			cin >> board[r][c];
		}
	}

	//make moves
	for (int i = 0; i < numMoves; i++) 
	{
		int moveDir = int((4.0*random()) / (RAND_MAX + 1.0)); //will return 0(up), 1 (right), 2(down), or 3(left)
		moveZero(zeroX, zeroY, board, moveDir);
	}
	
	printBoard(board);
	return 0;
}

void moveZero(int &x, int &y, int board[3][3], int moveDir)
{
	
	//cout << "moveDir: " << moveDir << endl;    // DEBUG		
	//if the moveDirection would take us off the grid, then move the opposite direction
	if (moveDir == 0 && x == 0)
		moveDir = 2;
	else if (moveDir == 1 && y == 2)
		moveDir = 3;
	else if (moveDir == 2 && x == 2)
		moveDir = 0;
	else if (moveDir == 3 && y == 0)
		moveDir = 1;
		
		if (moveDir == 0){            //up
			int temp = board[x-1][y];
			board[x-1][y] = 0;   // set square above 0 to 0
			board[x][y] = temp; //set original 0 location to temp
			x--;
		}
		else if (moveDir == 1){       //right
			int temp = board[x][y+1];
			board[x][y+1] = 0;
			board[x][y] = temp;
			y++;
		}
		else if (moveDir == 2){      //down
			int temp = board[x+1][y];
			board[x+1][y] = 0;   // set square above 0 to 0
			board[x][y] = temp; //set original 0 location to temp
			x++;
		}
		else if (moveDir == 3){      //left
			int temp = board[x][y-1];
			board[x][y-1] = 0;
			board[x][y] = temp;
			y--;
		}
}


void printBoard(int board[3][3])
{
	//print final board
	for (int r = 0; r < 3; r++)
	{
		for (int c = 0; c < 3; c++)
		{
			cout << board[r][c] << " ";
		}
		cout << endl;
	}
}