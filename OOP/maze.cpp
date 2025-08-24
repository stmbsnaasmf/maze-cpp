#include <iostream>
#include <ctime>
#include <fstream>
using namespace std;

class MazeSolver
{
	int** maze;
	int noOfRows = 0, noOfCols = 0;

public:

	virtual void solve (const int beginX, const int beginY, 
						const int finishX, const int finishY, 
						const char* LaTeX_file, const char* preambleFile) const = 0;

	MazeSolver(char* filename, int noOfRows, int noOfCols)
	{
		this->noOfRows = noOfRows; this->noOfCols = noOfCols;
		maze = new int*[noOfRows];
		for (int i = 0; i < noOfRows; i++)
		{
			maze[i] = new int[noOfCols];
		}

		ifstream myFile(filename);
		if (myFile.is_open())
		{
			for (int i = 0; i < noOfRows; i++)
			{
				for (int j = 0; j < noOfCols; j++)
				{
					myFile >> maze[i][j];
				}
			}
			myFile.close();
		}
		else
		{
			cout << "Error opening file in MazeSolver(char*)\n";
		}
	}

	class Point
	{
	public:
		int x, y;
		Point() {}
		Point(int x, int y) : x(x), y(y) {}
	};

	enum Direction
	{
		//north = 0, east = 1, west = 2, south = 3
		north = 0,
		east = 1,
		west = 2,
		south = 3
	};

    MazeSolver (const MazeSolver& other)
	{
        noOfRows = other.noOfRows;
        noOfCols = other.noOfCols;

		maze = new int*[noOfRows];
		for (int i = 0; i < noOfRows; i++)
		{
			maze[i] = new int[noOfCols];
		}

		for (int i = 0; i < noOfRows; i++)
		{
			for (int j = 0; j < noOfCols; j++)
			{
				maze[i][j] = other.maze[i][j];
			}
		}
    }

	MazeSolver& operator = (const MazeSolver& other)
	{
		if (this == &other)
		{
            return *this;
        }

        for (int i = 0; i < noOfRows; i++)
		{
			delete[] maze[i];
		}
		delete[] maze;

        noOfRows = other.noOfRows;
        noOfCols = other.noOfCols;

		maze = new int*[noOfRows];
		for (int i = 0; i < noOfRows; i++)
		{
			maze[i] = new int[noOfCols];
		}

		for (int i = 0; i < noOfRows; i++)
		{
			for (int j = 0; j < noOfCols; j++)
			{
				maze[i][j] = other.maze[i][j];
			}
		}

        return *this;
    }

	int getCellValue(const int x, const int y) const
	{
		return maze[x][y];
	}

	bool isObstacle(const int x, const int y) const
	{
		if ( (x < 0 || x >= noOfRows) || (y < 0 || y >= noOfCols) )
		{
			return true;
		}
		else if (1 == maze[x][y])
		{
			return true;
		}
		else
		{
			return false;			
		}
	}
	
	bool isJunction(const int x, const int y) const
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

	char getCharDirection(Direction direction) const
	{
		char c;
		if (north == direction)	//Up
		{
			c = 'N';
		}
		if (east == direction)	//Right
		{
			c = 'E';
		}
		if (west == direction)	//Left
		{
			c = 'W';
		}
		if (south == direction)	//Down
		{
			c = 'S';
		}
		
		return c;
	}

	bool isRightCellWall(const int x, const int y, const Direction direction) const
	{
		//north = 0, east = 1, west = 2, south = 3
		int rightX, rightY;
		if (north == direction)
		{
			rightX = x; rightY = y + 1;
		}
		else if (east == direction)
		{
			rightX = x + 1; rightY = y;
		}
		else if (west == direction)
		{
			rightX = x - 1; rightY = y;
		}
		else if (south == direction)
		{
			rightX = x; rightY = y - 1;
		}
		
		return isObstacle(rightX, rightY);
	}

	bool isForwardCelltWall(const int x, const int y, const Direction direction) const
	{
		//north = 0, east = 1, west = 2, south = 3
		int frontX, frontY;
		if (north == direction)
		{
			frontX = x - 1; frontY = y;
		}
		else if (east == direction)
		{
			frontX = x; frontY = y + 1;
		}
		else if (west == direction)
		{
			frontX = x; frontY = y - 1;
		}
		else if (south == direction)
		{
			frontX = x + 1; frontY = y;
		}
		
		return isObstacle(frontX, frontY);
	}

	Point getForward(const int x, const int y, const int direction) const
	{
		//north = 0, east = 1, west = 2, south = 3
		int frontX, frontY;
		if (north == direction)
		{
			frontX = x - 1; frontY = y;
		}
		else if (east == direction)
		{
			frontX = x; frontY = y + 1;
		}
		else if (west == direction)
		{
			frontX = x; frontY = y - 1;
		}
		else if (south == direction)
		{
			frontX = x + 1; frontY = y;
		}

		return Point(frontX, frontY);
	}

	Point getRight(const int x, const int y, const Direction direction) const
	{
		//north = 0, east = 1, west = 2, south = 3
		int rightX, rightY;
		if (north == direction)
		{
			rightX = x; rightY = y + 1;
		}
		else if (east == direction)
		{
			rightX = x + 1; rightY = y;
		}
		else if (west == direction)
		{
			rightX = x - 1; rightY = y;
		}
		else if (south == direction)
		{
			rightX = x; rightY = y - 1;
		}

		return Point(rightX, rightY);
	}

	Direction getRightDirection(const Direction previousDirection) const
	{
		//north = 0, east = 1, west = 2, south = 3
		Direction newDirection;
		if (north == previousDirection)
		{
			newDirection = east;
		}
		else if (east == previousDirection)
		{
			newDirection = south;
		}
		else if (west == previousDirection)
		{
			newDirection = north;
		}
		else if (south == previousDirection)
		{
			newDirection = west;
		}

		return newDirection;
	}

	Direction getLeftDirection(const Direction previousDirection) const
	{
		//north = 0, east = 1, west = 2, south = 3
		Direction newDirection;
		if (north == previousDirection)
		{
			newDirection = west;
		}
		else if (east == previousDirection)
		{
			newDirection = north;
		}
		else if (west == previousDirection)
		{
			newDirection = south;
		}
		else if (south == previousDirection)
		{
			newDirection = east;
		}

		return newDirection;
	}

	Direction getRandomDirection() const
	{
		return (Direction)(rand() % 4);
	}

	int getNoOfRows() const
	{
		return noOfRows;
	}

	int getNoOfCols() const
	{
		return noOfCols;
	}

	void print() const
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

	int copyFile(const char* destinationFile, const char* sourceFile) const
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

	int addBeginDocument(const char* filename) const
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

	int getNoOfColsForLaTeX() const
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

	int addBeginMulicol(const char* filename, const int multiColsNumber) const
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

	int addEndMulicol(const char* filename) const
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

	int addEndDocument(const char* filename) const
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

	int generateLaTeX(const char* filename, const int x, const int y, const char c) const
	{
		int status = 0;
		if (isObstacle(x, y))
		{
			// cout << "(" << x << ", " << y << "): Obstacle in int generateLaTeX(char* filename, int x, int y, char dir)\n";
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
			
			// cout << str;
			
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

	virtual ~MazeSolver()
	{
		for (int i = 0; i < noOfRows; i++)
		{
			delete[] maze[i];
		}
		delete[] maze;
	}
};

class RandomMouseAlgorithm : public MazeSolver
{
public:

	RandomMouseAlgorithm(char* filename, int noOfRows, int noOfCols) : MazeSolver(filename, noOfRows, noOfCols) {}

	RandomMouseAlgorithm(const RandomMouseAlgorithm& other) : MazeSolver(other) {}

	RandomMouseAlgorithm& operator = (const RandomMouseAlgorithm& other)
	{
		if (this == &other)
		{
			return *this;
		}

		MazeSolver::operator = (other); 

        return *this;
    }

	void solve(	const int beginX, const int beginY, 
				const int finishX, const int finishY, 
				const char* LaTeX_file, const char* preambleFile) const override
	{
		int x = beginX, y = beginY;
		Direction direction = getRandomDirection();

		bool isMulticolAdded = false;
		copyFile(LaTeX_file, preambleFile);
		addBeginDocument(LaTeX_file);
		if (getNoOfColsForLaTeX()>= 2)
		{
			addBeginMulicol(LaTeX_file, getNoOfColsForLaTeX());
			isMulticolAdded = true;
		}
		generateLaTeX(LaTeX_file, x, y, 'B');
		generateLaTeX(LaTeX_file, x, y, getCharDirection(direction));
		cout << "B: " << x << ", " << y << ": " << getCellValue(x, y) << "\n";

		if (isJunction(x, y))
		{
			direction = getRandomDirection();
			generateLaTeX(LaTeX_file, x, y, getCharDirection(direction));
			cout << getCharDirection(direction) << ": " << x << ", " << y << ": " << getCellValue(x, y) << "\n";
		}

		while (!(x == finishX && y == finishY))
		{
			if (!isForwardCelltWall(x, y, direction))
			{
				Point forward = getForward(x, y, direction);
				x = forward.x; y = forward.y;
				generateLaTeX(LaTeX_file, x, y, getCharDirection(direction));
				cout << getCharDirection(direction) << ": " << x << ", " << y << ": " << getCellValue(x, y) << "\n";

				if (isJunction(x, y))
				{
					direction = getRandomDirection();
					generateLaTeX(LaTeX_file, x, y, getCharDirection(direction));
					cout << getCharDirection(direction) << ": " << x << ", " << y << ": " << getCellValue(x, y) << "\n";
				}
			}
			else
			{
				direction = getRandomDirection();
				generateLaTeX(LaTeX_file, x, y, getCharDirection(direction));
				cout << getCharDirection(direction) << ": " << x << ", " << y << ": " << getCellValue(x, y) << "\n";
			}
		}
	
		generateLaTeX(LaTeX_file, x, y, 'F');
		if (isMulticolAdded)
		{
			addEndMulicol(LaTeX_file);
		}
		addEndDocument(LaTeX_file);
		cout << "F: " << x << ", " << y << ": " << getCellValue(x, y) << "\n";
	}

	~RandomMouseAlgorithm() override {}
};

class HandOnWallAlgorithm : public MazeSolver
{
public:

	HandOnWallAlgorithm(char* filename, int noOfRows, int noOfCols) : MazeSolver(filename, noOfRows, noOfCols) {}

	HandOnWallAlgorithm(const HandOnWallAlgorithm& other) : MazeSolver(other) {}

	HandOnWallAlgorithm& operator = (const HandOnWallAlgorithm& other)
	{
		if (this == &other)
		{
			return *this;
		}

		MazeSolver::operator = (other); 

        return *this;
    }

	// void solve(	const int beginX, const int beginY, 
				// const int finishX, const int finishY, 
				// const char* LaTeX_file, const char* preambleFile) const = 0;
		
	virtual ~HandOnWallAlgorithm() override {}
};

class RightHandOnWallAlgorithm : public HandOnWallAlgorithm
{
public:

	RightHandOnWallAlgorithm(char* filename, int noOfRows, int noOfCols) : HandOnWallAlgorithm(filename, noOfRows, noOfCols) {}

	RightHandOnWallAlgorithm(const RightHandOnWallAlgorithm& other) : HandOnWallAlgorithm(other) {}

	RightHandOnWallAlgorithm& operator = (const RightHandOnWallAlgorithm& other)
	{
		if (this == &other)
		{
			return *this;
		}

		HandOnWallAlgorithm::operator = (other); 

        return *this;
    }

		void solve(	const int beginX, const int beginY, 
				const int finishX, const int finishY, 
				const char* LaTeX_file, const char* preambleFile) const override
		{
			cout << "RHoW\n";
		}


	~RightHandOnWallAlgorithm() override {}
};

class LeftHandOnWallAlgorithm : public HandOnWallAlgorithm
{
public:

	LeftHandOnWallAlgorithm(char* filename, int noOfRows, int noOfCols) : HandOnWallAlgorithm(filename, noOfRows, noOfCols) {}

	LeftHandOnWallAlgorithm(const LeftHandOnWallAlgorithm& other) : HandOnWallAlgorithm(other) {}

	LeftHandOnWallAlgorithm& operator = (const LeftHandOnWallAlgorithm& other)
	{
		if (this == &other)
		{
			return *this;
		}

		HandOnWallAlgorithm::operator = (other); 

        return *this;
    }

		void solve(	const int beginX, const int beginY, 
				const int finishX, const int finishY, 
				const char* LaTeX_file, const char* preambleFile) const override
		{
			cout << "RHoW\n";
		}


	~LeftHandOnWallAlgorithm() override {}
};


int main(void)
{
	// srand(time(0));
	srand(420);
	char preamble[] = "preamble.txt";
	char mazeLaTeX[] = "maze.tex";
	char small_maze[] = "small_maze.txt";
	int small_maze_rows = 5, small_maze_cols = 5;

	MazeSolver* rma = new RandomMouseAlgorithm(small_maze, small_maze_rows, small_maze_cols);
	MazeSolver* rhow = new RightHandOnWallAlgorithm(small_maze, small_maze_rows, small_maze_cols);
	MazeSolver* lhow = new LeftHandOnWallAlgorithm(small_maze, small_maze_rows, small_maze_cols);
	
	// rma->print();
	// rma->solve(rma->getNoOfRows() - 1, 0, 0, rma->getNoOfCols() - 1, mazeLaTeX, preamble);

	rhow->solve(rma->getNoOfRows() - 1, 0, 0, rma->getNoOfCols() - 1, mazeLaTeX, preamble);

	char str[] = "pdflatex -output-directory=d:/STM/\"GU Tech\"/\"OOP Lab Spring 2025\"/\"OOP Lab Final Exam\"/Solution/MazeSolver/PDF/ --max-print-line=10000 --shell-escape -synctex=1 -interaction=nonstopmode -file-line-error d:/STM/\"GU Tech\"/\"OOP Lab Spring 2025\"/\"OOP Lab Final Exam\"/Solution/MazeSolver/maze.tex";
	system(str);

	delete rma;

	return 0;
}
