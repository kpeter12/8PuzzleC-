//Kevin Peterson
//CSCI 4350
//A-Star.cpp
//c++ program that uses A* search to solve an 8board puzzle that is inputted
//through standard input. Will output statistics of the A* search including
//the optimal path, number of nodes visited, branching factor, and depth of solution
#include <iostream>
#include <set>
#include <cmath>
using namespace std;

//prototype
int heuristic(int arg, const int board[][3], int zeroRow, int zeroCol); //h(n)

//Will hold the nodes used in Astar search to represent states of the board along with their depth and stats
struct node {
  int id;  //ID number of the node, will start at 0 and increment 1 every time
  int g;  //g(n)
  int h;  //h(n)
  int f;  //f(n)
  int zeroRow; // row location of the blank space
  int zeroCol;  //col location of the plank space
  int board[3][3];
  int parentID;
  //constructor
  node(int nodeID, int gX, int blankRow, int blankCol, const int state[3][3], int harg, int parID) {
    id=nodeID;
	g = gX;
	h = heuristic(harg, state, blankRow, blankCol);
    f= g + h;
	zeroRow = blankRow;
	zeroCol = blankCol;
	parentID = parID;
	for (int r = 0; r < 3; r++){
		for (int c = 0; c < 3; c++){
			board[r][c] = state[r][c];
		}
	}
  }  
  //constructor
  node(const node &other) 
  { 
	id = other.id;
	g = other.g;
	h = other.h;
	f = other.f;
	zeroRow = other.zeroRow;
	zeroCol = other.zeroCol;
	parentID = other.parentID;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			board[i][j] = other.board[i][j];
		}
	}
	
  }
  
  //compare first by f, use id for tiebreaker
  bool operator<(const node& Rhs) const {
    if (f < Rhs.f)       //value compares which node is <
      return true;
    else if (f > Rhs.f)
      return false;
    if (id > Rhs.id)       //if value is same, take greater id first as tiebreaker
      return true;
    else
      return false;
  }
  
  bool operator==(const node& Rhs) const {
    for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j<3; j++)
		{
			if (board[i][j] != Rhs.board[i][j])
				return false;
		}
	}
	return true;
  }
  //print function for nodes
  void print() const {
    cout << "Node - id=" << id << " g=" << g << " h=" << h << " f=" << f << endl <<
		"ZeroRow=" << zeroRow << " ZeroCol=" << zeroCol << endl;
		
	for (int r = 0; r < 3; r++){
		for (int c = 0; c < 3; c++){
			cout << board[r][c] << " ";
		}
		cout << endl;
	}
	cout <<"__" << endl;
  }
};

//Comparator for closed list
struct nodecomp {
  bool operator() (const node lhs, const node rhs) const
  {
	   for (int i = 0; i < 3; i++)
	   {
		   for (int j = 0; j < 3; j++)
		   {
			   if (lhs.board[i][j] > rhs.board[i][j])           //lhs is a greater board, so return false
				   return false;
				else if (lhs.board[i][j] < rhs.board[i][j])        //lhs is a less board, so return true
					return true;
		   }
	   }
	   return false;    //boards are equal
  }
  
  
  
};

int goalBoard[3][3] = {{0,1,2},{3,4,5},{6,7,8}};   //what the final board should look like,
int negativeBoard[3][3] = {{-1,-1,-1},{-1,-1,-1},{-1,-1,-1}};   //board of -1's used for computation
node goalNode(-1, -1, 0, 0, goalBoard, 0, -1);        //a node that holds a goalBoard for a goal to compare to
int goalRow(int num);     //prototype  
int goalCol(int num);     //prototype


int main(int argc, char *argv[])
{
	int heuristicArg = atoi(argv[1]);    //decides which heuristic to use
	set<node> openList;               //set that holds open list
	set<node,nodecomp> closedList;    //set that holds closed list
	int initialZeroRow;   //row coordinate of initial inputBoard
	int initialZeroCol;   //col coordinate of initial inputBoard
	//prototypes
	void expandNode(set<node,nodecomp>::iterator n, set<node> &openList, set<node,nodecomp>&closedList, int &idValue, int harg);
	void addToClosedList(node n, set<node,nodecomp> &closedList);
	void printLists(set<node> openList,set<node,nodecomp> closedList);
	node findNodeByID(int id, set<node,nodecomp> &closedList);
	
	//read in board from standard input
	int inputBoard[3][3];
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			cin >> inputBoard[i][j];
			if (inputBoard[i][j] == 0)
			{
				initialZeroRow = i;
				initialZeroCol = j;
			}
		}

	}
	
	int v = 0;  //total nodes visited
	int rootID = 0;
	node root(0, 0, initialZeroRow, initialZeroCol, inputBoard, heuristicArg, -1);   //add inital state to start tree
	openList.insert(root);
	while (!openList.empty())
	{
		//if current node is the goal node
		if ((*openList.begin()) == goalNode)
		{
			//printing out other stats
			cout << "V=" << v << endl;												//num of nodes expanded
			cout << "N=" << openList.size() + closedList.size() << endl;					//max num of nodes stored in memory
			cout << "d=" << openList.begin()->g << endl;                           		//depth              
			cout << "b=" << pow(openList.size() + closedList.size(), 1.0/ openList.begin()->g) << endl << endl;  //branching factor
			
			
			//print Path
			addToClosedList(*openList.begin(), closedList);  //add the final node to the closed list for printing path
			node pathNode = *openList.begin();
			int arr[openList.begin()->g + 10];   //declare array the size of the depth to store path in
			int iter = 0;                          //iterator
			//store all path in an array
			while (pathNode.id != 0) 
			{
				arr[iter] = pathNode.id;
				pathNode = findNodeByID(pathNode.parentID, closedList);
				iter++;
			}
			arr[iter] = pathNode.id;
			//print array backwards (will be path of optimal solution from top of tree to bottom)
			for (int i = iter; i >= 0; i--)
			{
				pathNode = findNodeByID(arr[i], closedList);
				for (int j = 0; j<3; j++)
				{
					for (int k = 0; k < 3; k++)
					{
						cout << pathNode.board[j][k] << " ";
					}
					cout << endl;
				}
				cout << endl;
			}
			
			
			
			return 0;
		}

		//if current node is not goal node, we add it to closed list, then take it off open list, then expand from closed listx
		addToClosedList(*openList.begin(), closedList);
		node closedNode = *openList.begin();
		openList.erase(openList.begin());     //pop first value off of the open list
		v++;                                    //increment # of Nodes visited
		expandNode(closedList.find(closedNode), openList, closedList, rootID, heuristicArg);      //problem is closed list is resorting b4 this
	}
	return 0;
}

//function that returns a h(x) value based off of a command line arguement and the board its given
int heuristic(int arg, const int board[][3], int zeroRow, int zeroCol) //h(n)
{
	//will result in uninformed cost search
	if (arg == 0)
	{
		return 0;    
	}
	//number of tiles  displaced from their goal location
	else if (arg == 1)
	{
		int numTilesDisplaced = 0;
		//find the coordinates of the goal
		for (int r = 0; r < 3; r++)
		{
			for (int c = 0; c < 3; c++)
			{
				if (goalBoard[r][c] != board[r][c])
				{
					numTilesDisplaced++;
				}
			}
		}
		
		return numTilesDisplaced;
	}
	//manhattan distance
	else if (arg == 2)
	{
		int sum = 0;
		//for every tile, calculate how far it is from its goal spot, then add it to running total
		for (int r = 0; r < 3; r++)
		{
			for (int c = 0; c < 3; c++)
			{
				sum += (abs(r - goalRow(board[r][c])) + abs(c - goalCol(board[r][c]))); 
			}
		}
		
		return sum;
	}
	
	else if (arg == 3)
	{
		int sum = 0;
		//for every tile, calculate how far it is from its goal spot, then add it to running total
		for (int r = 0; r < 3; r++)
		{
			for (int c = 0; c < 3; c++)
			{
				if ((abs(r - goalRow(board[r][c])) + abs(c - goalCol(board[r][c]))) > 0)
					sum += (abs(zeroRow - goalRow(board[r][c])) + abs(zeroCol - goalCol(board[r][c]))); 
			}
		}
		
		return sum;
	}
}

//function to quickly find the row of the goal spot
int goalRow(int num)
{
	if (num == 0 || num == 1 || num == 2){
		return 0;
	}
	else if (num == 3 || num == 4 || num == 5){
		return 1;
	}
	else if (num == 6 || num == 7 || num == 8){
		return 2;
	}
	else
		return 9999;
}
//function to quickly fund the column of the goal spot
int goalCol(int num)
{
	if (num == 0 || num == 3 || num == 6){
		return 0;
	}
	else if (num == 1 || num == 4 || num == 7){
		return 1;
	}
	else if (num == 2 || num == 5 || num == 8){
		return 2;
	}
	else
		return 9999;
}
//true if node in is in the closed list
bool inClosedList(node n, set<node,nodecomp> &closedList)
{	 
	 if (closedList.find(n) != closedList.end())
	 {
		 return true;
	 }
	 else 
		 return false;
		
	
}
//prints current open and closed lists
void printLists(set<node> openList,set<node,nodecomp> closedList)
{
	cout << endl << "OPEN LIST" << endl;
	for (set<node>::iterator itr = openList.begin(); itr != openList.end(); itr++)
	{
		itr->print(); 
	}
	 
	cout << endl << "CLOSED LIST" << endl;
	for (set<node,nodecomp>::iterator itr = closedList.begin(); itr != closedList.end(); itr++)
	{
		itr->print(); 
	}
	 
}
//expands a node given its iterator in the closed list and adds its children to the  open list, if they arent on the cloased list.
void expandNode(const set<node,nodecomp>::iterator n, set<node> &openList, set<node,nodecomp> &closedList, int &idValue, int harg)
{
	int gValue = n->g;
	node* child;
	//if not on top row, we can swap up
	if (n->zeroRow > 0)
	{
		child = new node(++idValue, gValue + 1, n->zeroRow, n->zeroCol, n->board, harg, n->id);
		int temp = child->board[child->zeroRow -1][child->zeroCol];      //perform a swap of the blank space up
		child->board[child->zeroRow -1][child->zeroCol] = 0;
		child->board[child->zeroRow][child->zeroCol] = temp;
		child->zeroRow--;
		if (inClosedList(*(child), closedList) == false)
		{
			//cout << "added id: " << child->id << " to frontier " << endl;
			openList.insert(*(child));
		}
		else
		{
			//cout << "Node id: " << child->id << " in closed list, not added to frontier " << endl;
		}
		
	}
	
	//if not on bottom row, we can swap zero down
	if (n->zeroRow < 2)
	{
		child = new node(++idValue, gValue + 1, n->zeroRow, n->zeroCol, n->board, harg, n->id);
		int temp = child->board[child->zeroRow +1][child->zeroCol];      //perform a swap of the blank space up
		child->board[child->zeroRow +1][child->zeroCol] = 0;
		child->board[child->zeroRow][child->zeroCol] = temp;
		child->zeroRow++;
		if (inClosedList(*(child), closedList) == false)
		{
			//cout << "added id: " << child->id << " to frontier " << endl;
			openList.insert(*(child));
			
		}
		else
		{
			//cout << "Node id: " << child->id << " in closed list, not added to frontier " << endl;
		}
	}
	
	//if not on right column, we can swap zero right
	if (n->zeroCol < 2)
	{
		child = new node(++idValue, gValue + 1, n->zeroRow, n->zeroCol, n->board, harg, n->id);
		int temp = child->board[child->zeroRow][child->zeroCol + 1];      //perform a swap of the blank space up
		child->board[child->zeroRow][child->zeroCol + 1] = 0;
		child->board[child->zeroRow][child->zeroCol] = temp;
		child->zeroCol++;
		if (inClosedList(*(child), closedList) == false)
		{
			//cout << "added id: " << child->id << " to frontier " << endl;
			openList.insert(*(child));
		}
		else
		{
			//cout << "Node id: " << child->id << " in closed list, not added to frontier " << endl;
		}
	}
	//if not on left column, we can swap zero left
	if (n->zeroCol > 0)
	{
		child = new node(++idValue, gValue + 1, n->zeroRow, n->zeroCol, n->board, harg, n->id);
		int temp = child->board[child->zeroRow][child->zeroCol - 1];      //perform a swap of the blank space up
		child->board[child->zeroRow][child->zeroCol - 1] = 0;
		child->board[child->zeroRow][child->zeroCol] = temp;
		child->zeroCol--;
		if (inClosedList(*(child), closedList) == false)
		{
			///cout << "added id: " << child->id << " to frontier " << endl;
			openList.insert(*(child));
		}
		else
		{
			//cout << "Node id: " << child->id << " in closed list, not added to frontier " << endl;
		}
	}	
}
//adds node to closed list
void addToClosedList(node n, set<node,nodecomp> &closedList)
{
	closedList.insert(n);
}
//returns a node given an id from the closed list, returns the node at the end of the closed list if id is not found
node findNodeByID(int id, set<node,nodecomp> &closedList)
{
	for (set<node,nodecomp>::iterator itr = closedList.begin(); itr != closedList.end(); itr++)
	{
		
		if (itr->id == id)
			return *itr;
	}
	cout << "ID not found" << endl;
	return *closedList.end();
	 
}
