#include <iostream>
#include <fstream>
#include <stack>
#include <cstdlib>
#include <ctime>
using namespace std;

class Maze
{
	int noOfRows = 0, noOfCols = 0;
	int** maze = NULL;

	class Point
	{
	public:
		int x, y;
		Point() {}
		Point(int x, int y) : x(x), y(y) {}
	};

public:
	Maze(int noOfRows, int noOfCols)
	{
		if (noOfRows < 3) noOfRows = 3; if (noOfCols < 3) noOfCols = 3;	//Minimum grid

		this->noOfRows = noOfRows + (noOfRows - 1); this->noOfCols = noOfCols + (noOfCols - 1);
		
		maze = new int*[this->noOfRows];
		for (int i = 0; i < this->noOfRows; i++)
		{
			maze[i] = new int[this->noOfCols];
		}

		for (int i = 0; i < this->noOfRows; i++)
		{
			for (int j = 0; j < this->noOfCols; j++)
			{				
				if (i % 2 == 1 || j % 2 == 1 )
				{
					maze[i][j] = 1;	//Walls
				}
				else
				{
					maze[i][j] = 0;	//Empty cells (unvisited)
				}
			}
		}
	}

	int getNoOfRows() const
	{
		return noOfRows;
	}

	int getNoOfCols() const
	{
		return noOfCols;
	}

	void emptyMaze()
	{
		for (int i = 0; i < noOfRows; i++)
		{
			for (int j = 0; j < noOfCols; j++)
			{
				if (i % 2 == 1 || j % 2 == 1 )
				{
					maze[i][j] = 1;	//Walls
				}
				else
				{
					maze[i][j] = 0;	//Empty cells (unvisited)
				}
			}
		}
	}

	void markVisited(int r, int c)
	{
		maze[r][c] = 2;	//2 = visited, 0 = unvisisted.
	}
	
	bool isVisited(int r, int c)
	{
		if (maze[r][c] == 2) //2 = visited, 0 = unvisisted.
		{
			return true;
		}
		return false;
	}

	bool isCorner(int r, int c)
	{
		if ((r == 0 && c == 0) || (r == 0 && c == noOfCols - 1) || (r == noOfRows - 1 && c == 0) || (r == noOfRows - 1 && c == noOfCols - 1))
		{
//			cout << "(" << r << ", " << c << ") is corner\n";
			return true;
		}
		return false;
	}

	int isEdge(int r, int c)
	{
		if ( (0 == r && !(c == 0 || c == noOfCols - 1)) || (noOfRows - 1 == r && !(c == 0 || c == noOfCols - 1)) || 
			 (0 == c && !(r == 0 || r == noOfRows - 1)) || (noOfCols - 1 == c && !(r == 0 || r == noOfRows - 1)) )
		{
			return true;
		}
		return false;
	}

	int getNoOfNeighbours(int r, int c)
	{
		//In this context, neighnours are NOT immediately neighbouring indexes, but indexes that are (+-2, +-2).
		//That is because this function is used for maze generation ONLY, and at that point, immediately neighbouring indexes are treated as WALLS or removed walls.
		
		//Corner = 2, Edge = 3, Middle = 4
		if (isCorner(r, c))
		{
			return 2;
		}
		if (isEdge(r, c))
		{
			return 3;
		}
		
		return 4;
	}

	void getNeighbours(int r, int c, Point* neighbours)
	{
		//In this context, neighnours are NOT immediately neighbouring indexes, but indexes that are (+-2, +-2).
		//That is because this function is used for maze generation ONLY, and at that point, immediately neighbouring indexes are treated as WALLS or removed walls.
		
		//This function assumes that the array of neighbours is already appropriately sized.
		if (r == 0 && c == 0) //Top left corner
		{
			neighbours[0] = Point(0, 2);
			neighbours[1] = Point(2, 0);
		}
		else if (r == 0 && c == noOfCols - 1)	//Top right corner
		{
			neighbours[0] = Point(0, noOfCols - 1 - 2);
			neighbours[1] = Point(2, noOfCols - 1);			
		}
		else if (r == noOfRows - 1 && c == 0)	//Bottom left corner
		{
			neighbours[0] = Point(noOfRows - 1 - 2, 0);
			neighbours[1] = Point(noOfRows - 1, 2);
		}
		else if (r == noOfRows - 1 && c == noOfCols - 1)	//Bottom right corner
		{
			neighbours[0] = Point(noOfRows - 1 - 2, noOfCols - 1);
			neighbours[1] = Point(noOfRows - 1, noOfCols - 1 - 2);
		}
		else if (0 == r && !(c == 0 || c == noOfCols - 1))	//Top edge
		{
			neighbours[0] = Point(r, c - 2);
			neighbours[1] = Point(r, c + 2);
			neighbours[2] = Point(r + 2, c);
		}
		else if (noOfRows - 1 == r && !(c == 0 || c == noOfCols - 1))	//Bottom edge
		{
			neighbours[0] = Point(r, c - 2);
			neighbours[1] = Point(r, c + 2);
			neighbours[2] = Point(r - 2, c);
		}
		else if (0 == c && !(r == 0 || r == noOfRows - 1))	//Left edge
		{
			neighbours[0] = Point(r - 2, c);
			neighbours[2] = Point(r, c + 2);
			neighbours[1] = Point(r + 2, c);
		}
		else if (noOfCols - 1 == c && !(r == 0 || r == noOfRows - 1))	//Right edge
		{
			neighbours[0] = Point(r - 2, c);
			neighbours[2] = Point(r, c - 2);
			neighbours[1] = Point(r + 2, c);
		}
		else	//Middle
		{
			neighbours[0] = Point(r - 2, c);
			neighbours[1] = Point(r, c - 2);
			neighbours[2] = Point(r, c + 2);
			neighbours[3] = Point(r + 2, c);
		}
	}

	int min(int x, int y)
	{
		if (x <= y)
		{
			return x;
		}
		
		return y;
	}

	void removeWall(int r0, int c0, int r1, int c1)
	{
		//Given two cells separated by a wall, it turns the wall into an empty cell.
		
		if (r0 == r1)	//Same row
		{
			maze[r0][min(c0, c1) + 1] = 0;	//0 = empty cell, 1 = obstacle or wall
			//cout << "Removed wall between (" << r0 << ", " << c0 << ") and (" << r1 << ", " << c1 << "): (" << r0 << ", " << min(c0, c1) + 1 << ")\n";
		}
		else if (c0 == c1)
		{
			maze[min(r0, r1) + 1][c0] = 0;	//0 = empty cell, 1 = obstacle or wall
			//cout << "Removed wall between (" << r0 << ", " << c0 << ") and (" << r1 << ", " << c1 << "): (" << min(r0, r1) + 1 << ", " << c0 << ")\n";
		}
	}

	void generate()
	{
		emptyMaze();

/*
1. Choose the initial cell, mark it as visited and push it to the stack
2. While the stack is not empty 

	1. Pop a cell from the stack and make it a current cell
	2. If the current cell has any neighbours which have not been visited 

		1. Push the current cell to the stack
		2. Choose one of the unvisited neighbours
		3. Remove the wall between the current cell and the chosen cell
		4. Mark the chosen cell as visited and push it to the stack
*/

		int r = (rand() % (noOfRows / 2)) * 2, c = (rand() % (noOfCols / 2)) * 2;	//Pick a random cell.
		//Always generate an even r and even c, because all odd (r, c) values are walls and not cells.

		std::stack <Point> s;

		markVisited(r, c);
		s.push(Point(r, c));
		
		while (!s.empty())
		{
			Point current = s.top();
			s.pop();
			
			int noOfNeighbours = getNoOfNeighbours(current.x, current.y);
			Point neighbours[noOfNeighbours];
			getNeighbours(current.x, current.y, neighbours);
			
			int noOfUnvisitedNeighbours = 0;
			for (int i = 0; i < noOfNeighbours; i++)
			{
				Point p = neighbours[i];
				if (!isVisited(p.x, p.y))
				{
					noOfUnvisitedNeighbours++;
				}
			}
			
			if (noOfUnvisitedNeighbours >= 1)
			{
				s.push(current);
				
				Point unvisitedNeighbours[noOfUnvisitedNeighbours];
				int uvni = 0;	//unvisted_neighbours_index
				for (int i = 0; i < noOfNeighbours; i++)
				{
					Point p = neighbours[i];
					if (!isVisited(p.x, p.y))
					{
						unvisitedNeighbours[uvni++] = neighbours[i];
					}
				}
				
				int randomUnvisitedNeighbourNo = rand() % noOfUnvisitedNeighbours;
				Point n = unvisitedNeighbours[randomUnvisitedNeighbourNo];
				removeWall(current.x, current.y, n.x, n.y);
				markVisited(n.x, n.y);
				s.push(n);
			}
		}
		
		for (int i = 0; i < noOfRows; i++)
		{
			for (int j = 0; j < noOfCols; j++)
			{
				if (2 == maze[i][j])	//Denote all visited cells as empty cells. 
				{
					maze[i][j] = 0;
				}
			}
		}
	}

	void print()
	{
		for (int i = 0; i < noOfRows; i++)
		{
			for (int j = 0; j < noOfCols; j++)
			{
				cout << maze[i][j] << " ";
			}
			cout << "\n";
		}		
	}
	
	void generateLaTeX(char* filename)
	{
		string str = "\\begin{table}[H]\n\\makebox[\\linewidth][c]\n{\n\t\\begin{tabular}{| ";
		
		for (int i = 0; i < noOfRows; i++)
		{
			str += "c ";
		}
		str += "|}\n\t\\hline\n";
		
		for (int i = 0; i < noOfRows; i++)
		{
			str += "\t";
			for (int j = 0; j < noOfCols; j++)
			{
				if (0 == maze[i][j]) str += "\\texttt{}";
				else if (1 == maze[i][j]) str += "\\cellcolor{black}";
				
				if (j != noOfCols - 1) str += " & ";
				else str += " \\\\";
			}
			str += "\n";
		}

		str += "\t\\hline\n\t\\end{tabular}\n}\n% \\caption{}\n% \\label{}\n\\end{table}\n";
		
		cout << str;
		
		ofstream myFile(filename);
		if (myFile.is_open())
		{
			myFile << str;
			myFile.close();
		}
		else
		{
			cout << "Error opening the file: " << filename << "\n";
		}
	}
	
	bool isObstacle(int x, int y)
	{
		if ( (x < 0 || x >= noOfRows) || (y < 0 || y >= noOfCols) )
		{
//			cout << "Obstacle (out of bound): " << x << ", " << y << "\n";
			return true;
		}
		else if (1 == maze[x][y])
		{
//			cout << "Obstacle: " << x << ", " << y << "\n";
			return true;
		}
		else
		{
			return false;			
		}
	}
	
	// bool isJunction(int x, int y, const int direction)
	// {
	// 	//0 = up, 1 = right, 2 = left, 3 = down
	// 	if (1 == direction || 2 == direction)	//If moving horizontally, check only vertical neighbours.
	// 	{
	// 		return !(isObstacle(x - 1, y) && isObstacle(x + 1, y));	//If either neighbour is empty, we are at a junction.
	// 	}
	// 	else	//If moving vertically, check only horizontal neighbours.
	// 	{
	// 		return !(isObstacle(x, y - 1) && isObstacle(x, y + 1));	//If either neighbour is empty, we are at a junction.
	// 	}
	// }

	bool isJunction(const int x, const int y)
	{
		//A junction is a cell that has at least 3 empty neighbour cells.
		int noOfEmptyNeighbours = 0;
		
		if (!isObstacle(x - 1, y))
		{
			noOfEmptyNeighbours++;
		}
		
		if (!isObstacle(x + 1, y))
		{
			noOfEmptyNeighbours++;
		}
		
		if (!isObstacle(x, y - 1))
		{
			noOfEmptyNeighbours++;
		}
		
		if (!isObstacle(x, y + 1))
		{
			noOfEmptyNeighbours++;
		}


		if (noOfEmptyNeighbours >= 3)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	char getCharDirection(int direction)
	{
		char c;
		if (0 == direction)	//Up
		{
			c = 'U';
		}
		if (1 == direction)	//Right
		{
			c = 'R';
		}
		if (2 == direction)	//Left
		{
			c = 'L';
		}
		if (3 == direction)	//Down
		{
			c = 'D';
		}
		
		return c;
	}

	int copyFile(char* destinationFile, char* sourceFile)
	{
		int status = 0;
		ifstream source(sourceFile);
		if (source.is_open())
		{
			string str, line;
			while (std::getline(source, line))
			{
				str += line;
				str += "\n";
			}
			str += "\n";
			source.close();

			ofstream destination(destinationFile);
			if (destination.is_open())
			{
				destination << str;
				destination.close();
			}
			else
			{
				cout << "Error opening destination file in copyFile(char*, char*)\n";
				status = 2;
			}
		}
		else
		{
			cout << "Error opening source file in copyFile(char*, char*)\n";
			status = 1;
		}

		return status;
	}

	int addBeginMulicol(char* filename, int multiColsNumber)
	{
		int status = 0;
		string str = "\\begin{multicols}{";
		str += char(48 + multiColsNumber);	//ASCII-48 = 0
		str+= "}\n\n";

		ofstream myFile(filename, std::ios::app);
		if (myFile.is_open())
		{
			myFile << str;
			myFile.close();
		}
		else
		{
			cout << "Error opening the file: " << filename << "\n";
			status = 1;
		}
		return status;
	}

	int addBeginDocument(char* filename)
	{
		int status = 0;
		string str = "\\begin{document}\n\n";

		ofstream myFile(filename, std::ios::app);
		if (myFile.is_open())
		{
			myFile << str;
			myFile.close();
		}
		else
		{
			cout << "Error opening the file: " << filename << "\n";
			status = 1;
		}
		return status;
	}

	int addEndMulicol(char* filename)
	{
		int status = 0;
		string str = "\n\n\\end{multicols}\n";

		ofstream myFile(filename, std::ios::app);
		if (myFile.is_open())
		{
			myFile << str;
			myFile.close();
		}
		else
		{
			cout << "Error opening the file: " << filename << "\n";
			status = 1;
		}
		return status;
	}

	int addEndDocument(char* filename)
	{
		int status = 0;
		string str = "\n\n\\end{document}";

		ofstream myFile(filename, std::ios::app);
		if (myFile.is_open())
		{
			myFile << str;
			myFile.close();
		}
		else
		{
			cout << "Error opening the file: " << filename << "\n";
			status = 1;
		}
		return status;
	}

	int getNoOfColsForLaTeX()
	{
		// \begin{multicols}{n}
		int multi_col_number = 4;
		if (noOfRows > 5 && noOfRows <= 10)
		{
			multi_col_number = 3;
		}
		else if (noOfRows > 10 && noOfRows <= 12)
		{
			multi_col_number = 2;
		}
		else if (noOfRows > 12)
		{
			multi_col_number = 0;
		}
		
		return multi_col_number;
	}

	bool isRightCellWall(const int x, const int y, const int direction)
	{
		//0 = up, 1 = right, 2 = left, 3 = down
		int rightX, rightY;
		if (0 == direction)
		{
			rightX = x; rightY = y + 1;
		}
		else if (1 == direction)
		{
			rightX = x + 1; rightY = y;
		}
		else if (2 == direction)
		{
			rightX = x - 1; rightY = y;
		}
		else if (3 == direction)
		{
			rightX = x; rightY = y - 1;
		}
		
		return isObstacle(rightX, rightY);
	}

	// bool isRightDiagonalCellWall(const int x, const int y, const int direction)
	// {
	// 	//0 = up, 1 = right, 2 = left, 3 = down
	// 	int rightDiagonalX, rightDiagonalY;
	// 	if (0 == direction)
	// 	{
	// 		rightDiagonalX = x - 1; rightDiagonalY = y + 1;
	// 	}
	// 	else if (1 == direction)
	// 	{
	// 		rightDiagonalX = x + 1; rightDiagonalY = y + 1;
	// 	}
	// 	else if (2 == direction)
	// 	{
	// 		rightDiagonalX = x - 1; rightDiagonalY = y - 1;
	// 	}
	// 	else if (3 == direction)
	// 	{
	// 		rightDiagonalX = x + 1; rightDiagonalY = y - 1;
	// 	}
		
	// 	return isObstacle(rightDiagonalX, rightDiagonalY);
	// }

	bool isFronCelltWall(const int x, const int y, const int direction)
	{
		//0 = up, 1 = right, 2 = left, 3 = down
		int frontX, frontY;
		if (0 == direction)
		{
			frontX = x - 1; frontY = y;
		}
		else if (1 == direction)
		{
			frontX = x; frontY = y + 1;
		}
		else if (2 == direction)
		{
			frontX = x; frontY = y - 1;
		}
		else if (3 == direction)
		{
			frontX = x + 1; frontY = y;
		}
		
		return isObstacle(frontX, frontY);
	}

	Point getFront(const int x, const int y, const int direction)
	{
		//0 = up, 1 = right, 2 = left, 3 = down
		int frontX, frontY;
		if (0 == direction)
		{
			frontX = x - 1; frontY = y;
		}
		else if (1 == direction)
		{
			frontX = x; frontY = y + 1;
		}
		else if (2 == direction)
		{
			frontX = x; frontY = y - 1;
		}
		else if (3 == direction)
		{
			frontX = x + 1; frontY = y;
		}

		return Point(frontX, frontY);
	}

	Point getRight(const int x, const int y, const int direction)
	{
		//0 = up, 1 = right, 2 = left, 3 = down
		int rightX, rightY;
		if (0 == direction)
		{
			rightX = x; rightY = y + 1;
		}
		else if (1 == direction)
		{
			rightX = x + 1; rightY = y;
		}
		else if (2 == direction)
		{
			rightX = x - 1; rightY = y;
		}
		else if (3 == direction)
		{
			rightX = x; rightY = y - 1;
		}

		return Point(rightX, rightY);
	}

	// Point getRightDiagonal(const int x, const int y, const int direction)
	// {
	// 	//0 = up, 1 = right, 2 = left, 3 = down
	// 	int rightDiagonalX, rightDiagonalY;
	// 	if (0 == direction)
	// 	{
	// 		rightDiagonalX = x - 1; rightDiagonalY = y + 1;
	// 	}
	// 	else if (1 == direction)
	// 	{
	// 		rightDiagonalX = x + 1; rightDiagonalY = y + 1;
	// 	}
	// 	else if (2 == direction)
	// 	{
	// 		rightDiagonalX = x - 1; rightDiagonalY = y - 1;
	// 	}
	// 	else if (3 == direction)
	// 	{
	// 		rightDiagonalX = x + 1; rightDiagonalY = y - 1;
	// 	}
		
	// 	return Point(rightDiagonalX, rightDiagonalY);
	// }

	int getNewRightDirection(int previousDirection)
	{
		//0 = up, 1 = right, 2 = left, 3 = down
		int newDirection;
		if (0 == previousDirection)
		{
			newDirection = 1;
		}
		else if (1 == previousDirection)
		{
			newDirection = 3;
		}
		else if (2 == previousDirection)
		{
			newDirection = 0;
		}
		else if (3 == previousDirection)
		{
			newDirection = 2;
		}

		return newDirection;
	}

	int getNewLeftDirection(int previousDirection)
	{
		//0 = up, 1 = right, 2 = left, 3 = down
		int newDirection;
		if (0 == previousDirection)
		{
			newDirection = 2;
		}
		else if (1 == previousDirection)
		{
			newDirection = 0;
		}
		else if (2 == previousDirection)
		{
			newDirection = 3;
		}
		else if (3 == previousDirection)
		{
			newDirection = 1;
		}

		return newDirection;
	}

	void rhow(int startX, int startY, int finishX, int finishY, char* LaTeX_file, char* preambleFile)
	{
		int x = startX, y = startY;
		int direction = rand() % 4;	//0 = up, 1 = right, 2 = left, 3 = down
		cout << "S: " << x << ", " << y << ": " << maze[x][y] << "\n";
		
		bool isMulticolAdded = false;

		copyFile(LaTeX_file, preambleFile);
		addBeginDocument(LaTeX_file);

		if (getNoOfColsForLaTeX()>= 2)
		{
			addBeginMulicol(LaTeX_file, getNoOfColsForLaTeX());
			isMulticolAdded = true;
		}

		generateLaTeX(LaTeX_file, x, y, 'S');
		generateLaTeX(LaTeX_file, x, y, getCharDirection(direction));
		
		while (!isRightCellWall(x, y, direction))	//Change directions until a wall is on the right hand.
		{
			direction = (direction + 1) % 4;	//0 = up, 1 = right, 2 = left, 3 = down
			generateLaTeX(LaTeX_file, x, y, getCharDirection(direction));
			cout << "Changed direction: " << getCharDirection(direction) << "\n";
		}

		while (!(x == finishX && y == finishY))
		{
			if (!isRightCellWall(x, y, direction))	//If right cell is not wall, change direction to right, then move to the cell.
			{
				Point right = getRight(x, y, direction);
				direction = getNewRightDirection(direction);
				generateLaTeX(LaTeX_file, x, y, getCharDirection(direction));
				x = right.x; y = right.y;
				generateLaTeX(LaTeX_file, x, y, getCharDirection(direction));
			}
			else if (!isFronCelltWall(x, y, direction))	//If right is wall but front is not, move to front.
			{
				Point front = getFront(x, y, direction);
				x = front.x; y = front.y;
				generateLaTeX(LaTeX_file, x, y, getCharDirection(direction));
			}
			else //If right is wall and front is wall, change direction to left.
			{
				direction = getNewLeftDirection(direction);
				generateLaTeX(LaTeX_file, x, y, getCharDirection(direction));
			}
		}

		generateLaTeX(LaTeX_file, x, y, 'F');
		if (isMulticolAdded)
		{
			addEndMulicol(LaTeX_file);
		}
		addEndDocument(LaTeX_file);
	}

	void rma(int sX, int sY, int fX, int fY, char* LaTeX_file, char* preambleFile)
	{
		int x = sX, y = sY;
		int direction = rand() % 4;	//0 = up, 1 = right, 2 = left, 3 = down

		cout << "S: " << x << ", " << y << ": " << maze[x][y] << "\n";
		Point next;

		bool isMulticolAdded = false;
		copyFile(LaTeX_file, preambleFile);		
		addBeginDocument(LaTeX_file);
		if (getNoOfColsForLaTeX()>= 2)
		{
			addBeginMulicol(LaTeX_file, getNoOfColsForLaTeX());
			isMulticolAdded = true;
		}
		generateLaTeX(LaTeX_file, x, y, 'S');

		while (!(x == fX && y == fY))
		{
			//First find the next cell we will travel to if it is not an obstacle.
			
			char c;
			if (0 == direction)	//Up
			{
				next.x = x - 1, next.y = y;	// x = x - 1 because origin is at top left.
				c = 'U';
			}
			if (1 == direction)	//Right
			{
				next.x = x, next.y = y + 1;
				c = 'R';
			}
			if (2 == direction)	//Left
			{
				next.x = x, next.y = y - 1;
				c = 'L';
			}
			if (3 == direction)	//Down
			{
				next.x = x + 1, next.y = y;
				c = 'D';
			}
			
			generateLaTeX(LaTeX_file, x, y, getCharDirection(direction));
			
			if (!isObstacle(next.x, next.y))	//Check if it is not an obstacle
			{
				x = next.x; y = next.y;			//Actually move
				cout << c << ": " << x << ", " << y << ": " << maze[x][y] << "\n";
				
//				if (isJunction(x, y, direction))
				if (isJunction(x, y))
				{
					cout << "Junction: (" << x << ", " << y << ")\n";
//					cout << c << ", at junction\n";
					direction = rand() % 4;	//0 = up, 1 = right, 2 = left, 3 = down
				}
			}
			else	//If it is an obstacle, change direction. (And in the next iteration, find the next cell we will travel to if it is not an obstacle)
			{
//				cout << "Encountered obstacle\n";
				direction = rand() % 4;	//0 = up, 1 = right, 2 = left, 3 = down
			}
		}

		generateLaTeX(LaTeX_file, x, y, 'F');
		if (isMulticolAdded)
		{
			addEndMulicol(LaTeX_file);
		}
		addEndDocument(LaTeX_file);

		cout << "F: " << x << ", " << y << ": " << maze[x][y] << "\n";
	}


	int generateLaTeX(char* filename, int x, int y, char c)
	{
		int status = 0;
		if (isObstacle(x, y))
		{
			cout << "(" << x << ", " << y << "): Obstacle in int generateLaTeX(char* filename, int x, int y, char dir)\n";
			status = 1;
		}
		else
		{
			string str = "\\begin{table}[H]\n\\makebox[\\linewidth][c]\n{\n\t\\begin{tabular}{| ";
			
			for (int i = 0; i < noOfRows; i++)
			{
				str += "c ";
//				str += "p{1em} ";
			}
			str += "|}\n\t\\hline\n";
			
			for (int i = 0; i < noOfRows; i++)
			{
				str += "\t";
				for (int j = 0; j < noOfCols; j++)
				{
					if (i == x && j == y)
					{
						str += "\\cellcolor[rgb]{0, 1, 1}{\\tiny ";
						str += c;
						str += "}";	//(0, 255, 255)
//						str += "\\cellcolor[rgb]{0, 1, 1}";	//(0, 255, 255)
					}
					// else if (0 == maze[i][j]) str += "\\texttt{} \\ ";
					else if (0 == maze[i][j]) str += "\\";
					else if (1 == maze[i][j]) str += "\\cellcolor{black} \\";
					
					if (j != noOfCols - 1) str += " & ";
					else str += " \\\\";
				}
				str += "\n";
			}
	
			str += "\t\\hline\n\t\\end{tabular}\n}\n% \\caption{}\n% \\label{}\n\\end{table}\n\n";
			
			cout << str;
			
			ofstream myFile(filename, std::ios::app);
			if (myFile.is_open())
			{
				myFile << str;
				myFile.close();
			}
			else
			{
				cout << "Error opening the file: " << filename << "\n";
				status = 2;
			}
		}

		return status;
	}

	int printArray(char* filename)
	{
		int status = 0;
		string str;
		for (int i = 0 ; i < noOfRows; i++)
		{
			for (int j = 0; j < noOfCols; j++)
			{
				str += char(48 + maze[i][j]);	//ASCII-48 = 0
				str += " ";
			}
			str += "\n";
		}
		ofstream myFile(filename);
		if (myFile.is_open())
		{
			myFile << str;
			myFile.close();
		}
		else
		{
			status = 1;
			cout << "Error opening file: " << filename << "\n";
		}
		
		return status;
	}

	~Maze()
	{
		for (int i = 0; i < noOfRows; i++)
		{
			delete[] maze[i];
			maze[i] = NULL;
		}
		delete[] maze;
		maze = NULL;
	}
};

int main(void)
{
	srand(time(0));
	char filename[] = "table.txt";
	char preambleLaTeX[] = "preamble.txt";
	char mazeLaTeX[] = "maze.tex";
	char bitmapArrayFile[] = "bitmap.txt";
	int noOfRowsAndCols = 3;
	// int noOfRowsAndCols = 3;
	int noOfRows = noOfRowsAndCols, noOfCols = noOfRowsAndCols;
	Maze a(noOfRows, noOfCols);
	a.generate();
	a.print();
	// a.rma(a.getNoOfRows() - 1, 0, 0, a.getNoOfCols() - 1, mazeLaTeX, preambleLaTeX);
	a.printArray(bitmapArrayFile);
//	a.isJunction(4, 2);
	a.rhow(a.getNoOfRows() - 1, 0, 0, a.getNoOfCols() - 1, mazeLaTeX, preambleLaTeX);

	char str[] = "pdflatex -output-directory=d:/STM/\"GU Tech\"/\"PSPF Lab Spring 2025\"/\"PSPF Projects\"/Maze/PDF/ --max-print-line=10000 --shell-escape -synctex=1 -interaction=nonstopmode -file-line-error d:/STM/\"GU Tech\"/\"PSPF Lab Spring 2025\"/\"PSPF Projects\"/Maze/maze.tex";
	system(str);







	//pdflatex --max-print-line=10000, --shell-escape, -synctex=1, -interaction=nonstopmode, -file-line-error, "d:/STM/GU Tech/Misc LaTeX/misc.tex"
	//pdflatex -output-directory=d:/STM/"GU Tech"/"Misc LaTeX"/ --max-print-line=10000, --shell-escape, -synctex=1, -interaction=nonstopmode, -file-line-error, d:/STM/"GU Tech"/"Misc LaTeX"/misc.tex

	//pdflatex -output-directory=c:\Users\"Syed Taloot"\Desktop\"STM GU Tech Laptop"\"PSPF Lab Spring 2025"\"PSPF Projects"\Maze\MazeLaTeX\ --max-print-line=10000, --shell-escape, -synctex=1, -interaction=nonstopmode, -file-line-error, c:\Users\"Syed Taloot"\Desktop\"STM GU Tech Laptop"\"PSPF Lab Spring 2025"\"PSPF Projects"\Maze\"Maze Code"\maze.tex

	//pdflatex -output-directory=d:/STM/"GU Tech"/"PSPF Lab Spring 2025"/"PSPF Projects"/Maze/PDF/ --max-print-line=10000, --shell-escape, -synctex=1, -interaction=nonstopmode, -file-line-error, d:/STM/"GU Tech"/"PSPF Lab Spring 2025"/"PSPF Projects"/Maze/maze.tex


//	char str[] = "pdflatex -output-directory=d:/STM/\"GU Tech\"/\"PSPF Lab Spring 2025\"/\"PSPF Projects\"/Maze/PDF/ --max-print-line=10000, --shell-escape, -synctex=1, -interaction=nonstopmode, -file-line-error, d:/STM/\"GU Tech\"/\"PSPF Lab Spring 2025\"/\"PSPF Projects\"/Maze/maze.tex";
//	char str[] = "pdflatex -output-directory=d:/STM/\"GU Tech\"/\"PSPF Lab Spring 2025\"/\"PSPF Projects\"/Maze/PDF/ --max-print-line=10000, --shell-escape, -synctex=1, -interaction=nonstopmode, -file-line-error, d:/STM/\"GU Tech\"/\"PSPF Lab Spring 2025\"/\"PSPF Projects\"/Maze/maze.tex";
	

//	cout << "\n\n" << str << "\n\n";

//	system("pdflatex -output-directory=d:/STM/\"GU Tech\"/\"PSPF Lab Spring 2025\"/\"PSPF Projects\"/Maze/PDF/ --max-print-line=10000, --shell-escape, -synctex=1, -interaction=nonstopmode, -file-line-error, d:/STM/\"GU Tech\"/\"PSPF Lab Spring 2025\"/\"PSPF Projects\"/Maze/maze.tex");
//	system("pause");

	return 0;
}
