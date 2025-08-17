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
	
	bool isJunction(int x, int y, const int direction)
	{
		//0 = up, 1 = right, 2 = left, 3 = down
		if (1 == direction || 2 == direction)	//If moving horizontally, check only vertical neighbours.
		{
			return !(isObstacle(x - 1, y) && isObstacle(x + 1, y));	//If either neighbour is empty, we are at a junction.
		}
		else	//If moving vertically, check only horizontal neighbours.
		{
			return !(isObstacle(x, y - 1) && isObstacle(x, y + 1));	//If either neighbour is empty, we are at a junction.
		}
	}

	void rma(int sX, int sY, int fX, int fY)
	{
		int x = sX, y = sY;
		int direction = rand() % 4;	//0 = up, 1 = right, 2 = left, 3 = down

		cout << "S: " << x << ", " << y << ": " << maze[x][y] << "\n";
		Point next;
	
		while (!(x == fX && y == fY))
		{
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
			
			if (!isObstacle(next.x, next.y))
			{
				x = next.x; y = next.y;
				cout << c << ": " << x << ", " << y << ": " << maze[x][y] << "\n";
				
				if (isJunction(x, y, direction))
				{
//					cout << c << ", at junction\n";
					direction = rand() % 4;	//0 = up, 1 = right, 2 = left, 3 = down
				}
			}
			else
			{
//				cout << "Encountered obstacle\n";
				direction = rand() % 4;	//0 = up, 1 = right, 2 = left, 3 = down
			}
		}
		
		cout << "F: " << x << ", " << y << ": " << maze[x][y] << "\n";
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

	void rma(int sX, int sY, int fX, int fY, char* filename)
	{
		int x = sX, y = sY;
		int direction = rand() % 4;	//0 = up, 1 = right, 2 = left, 3 = down

		cout << "S: " << x << ", " << y << ": " << maze[x][y] << "\n";
		Point next;

		generateLaTeX(filename, x, y, 'S');

		while (!(x == fX && y == fY))
		{
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
			
			generateLaTeX(filename, x, y, getCharDirection(direction));
			
			if (!isObstacle(next.x, next.y))
			{
				x = next.x; y = next.y;
				cout << c << ": " << x << ", " << y << ": " << maze[x][y] << "\n";
				
				if (isJunction(x, y, direction))
				{
//					cout << c << ", at junction\n";
					direction = rand() % 4;	//0 = up, 1 = right, 2 = left, 3 = down
				}
			}
			else
			{
//				cout << "Encountered obstacle\n";
				direction = rand() % 4;	//0 = up, 1 = right, 2 = left, 3 = down
			}
		}
		
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
			}
			str += "|}\n\t\\hline\n";
			
			for (int i = 0; i < noOfRows; i++)
			{
				str += "\t";
				for (int j = 0; j < noOfCols; j++)
				{
					if (i == x && j == y)
					{
						str += "\\cellcolor[rgb]{0, 1, 1}{";
						str += c;
						str += "}";	//(0, 255, 255)
//						str += "\\cellcolor[rgb]{0, 1, 1}";	//(0, 255, 255)
					}
					else if (0 == maze[i][j]) str += "\\texttt{}";
					else if (1 == maze[i][j]) str += "\\cellcolor{black}";
					
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
//	srand(time(0));
	char filename[] = "table.txt";
	char filenameAppended[] = "appended.txt";
	int noOfRows = 3, noOfCols = 3;
	Maze a(noOfRows, noOfCols);
//	a.print();
	a.generate();
	a.print();
	a.generateLaTeX(filename);
//	a.rma(6, 0, 0, 6);
//	a.rma(noOfRows - 1, 0, 0, noOfCols - 1);
	a.rma(noOfRows - 1, 0, 0, noOfCols - 1, filenameAppended);
	
	//pdflatex --max-print-line=10000, --shell-escape, -synctex=1, -interaction=nonstopmode, -file-line-error, "d:/STM/GU Tech/Misc LaTeX/misc.tex"
	//pdflatex -output-directory=d:/STM/"GU Tech"/"Misc LaTeX"/ --max-print-line=10000, --shell-escape, -synctex=1, -interaction=nonstopmode, -file-line-error, d:/STM/"GU Tech"/"Misc LaTeX"/misc.tex
	
	return 0;
}
