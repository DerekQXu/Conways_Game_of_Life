#include <iostream>
#include <string>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <windows.h>

using namespace std;

const int SPACE_WIDTH = 50;
const int SPACE_HEIGHT = 20;
//SPACE_WIDTH and SPACE_HEIGHT must be the same to catch errors when user inputs points
const int NUM_OF_INIT_LIFE = 20;
int timeStart;

void getInitPoints(const string input, int output[]);
void newPoints(char spaceArray[][SPACE_HEIGHT]);
void randomGenerated(char spaceArray[][SPACE_HEIGHT]);
string printArray(const char spaceArray[][SPACE_HEIGHT]);
void ShowConsoleCursor(bool showFlag);

int main(){
	char spaceArray[20][20];
	for (int i = 0; i < SPACE_WIDTH; ++i)
		for (int j = 0; j < SPACE_HEIGHT; ++j)
			spaceArray[i][j] = ' ';

	cout << "Welcome to John Conway's Game of Life!" << endl
		<< "--------------------------------------------------------------------------------" << endl
		<< "The rules of this game are simple:" << endl
		<< "if there are 4 or more surrounding cells, the middle cell dies of overpopulation" << endl
		<< "if there are 2 or less surrounding cells, the middle cell dies of isolation" << endl
		<< "if there are exactly 3 neighboring cells, a space can spawn a new cell" << endl;
	cout << "--------------------------------------------------------------------------------" << endl
		<< "please enter in up to 20 coordinates as follows: \"(5,6),(2,3), ... ,(x,y)\", or \"random\" for random generation" << endl;
	
	string initPointsInput;
	cin >> initPointsInput;
	if (initPointsInput == "random")
		randomGenerated(spaceArray);
	else 
	{
		int initPoints[NUM_OF_INIT_LIFE * 2] = { 0 };
		getInitPoints(initPointsInput, initPoints);

		//initializes first life
		for (int n = 0; n < NUM_OF_INIT_LIFE * 2; n += 2) {
			spaceArray[initPoints[n]][initPoints[n + 1]] = 'o';
		}
	}

	//ask how much time to run
	cout << "how many seconds should I run the program?" << endl;
	int timeEnd;
	cin >> timeEnd;
	timeStart = (int) time(0);

	string output;
	system("cls");

	ShowConsoleCursor(false);
	while (time(0) < timeStart + timeEnd) {
		COORD coord;
		coord.X = 0;
		coord.Y = 0;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
		output = printArray(spaceArray);
		cout << output;
		cout << endl << "time running: " << (time(0) - timeStart) << endl;
		newPoints(spaceArray);
	}
	ShowConsoleCursor(true);
	cout << "simulation complete";
	cin >> timeEnd;
	return 0;
}

void newPoints(char spaceArray[][SPACE_HEIGHT]) 
{
	int numberOfProximalCells;
	int location[8][2] = { {0,1}, {1,0}, {0,-1}, {-1,0}, {1,1}, {1,-1}, {-1,-1}, {-1,1} };
	int newLocation[2] = { 0 };
	//find birth points
	for (int i = 0; i < SPACE_WIDTH; ++i) 
	{
		for (int j = 0; j < SPACE_HEIGHT; ++j) {
			if (spaceArray[i][j] != 'o') {
				numberOfProximalCells = 0;
				for (int n = 0; n < 8; ++n)
				{
					newLocation[0] = i + location[n][0];
					newLocation[1] = j + location[n][1];

					if (newLocation[0] < 0 || newLocation[1] < 0)
						continue;
					if (newLocation[0] >= SPACE_WIDTH || newLocation[1] >= SPACE_HEIGHT)
						continue;

					if (spaceArray[newLocation[0]][newLocation[1]] == 'o') {
						++numberOfProximalCells;
					}
				}
				if (numberOfProximalCells == 3) {
					spaceArray[i][j] = 'x';
#if 0
					cout << "cell (" << i << ", " << j << ") has been born!" << endl;
#endif
				}
			}
		}
	}

	//find dead cells
	for (int i = 0; i < SPACE_WIDTH; ++i)
	{
		for (int j = 0; j < SPACE_HEIGHT; ++j) {
			if (spaceArray[i][j] == 'o') {
				numberOfProximalCells = 0;
				for (int n = 0; n < 8; ++n)
				{
					newLocation[0] = i + location[n][0];
					newLocation[1] = j + location[n][1];

					if (newLocation[0] < 0 || newLocation[1] < 0)
						continue;
					if (newLocation[0] >= SPACE_WIDTH || newLocation[1] >= SPACE_HEIGHT)
						continue;

					if (spaceArray[newLocation[0]][newLocation[1]] == 'o' || spaceArray[newLocation[0]][newLocation[1]] == 'd') {
						++numberOfProximalCells;
					}
				}
				if (numberOfProximalCells >= 4 || numberOfProximalCells <= 1) {
					spaceArray[i][j] = 'd';
#if 0
					cout << "cell (" << i << ", " << j << ") has died!" << endl;
#endif
				}
			}
		}
	}
	//makes all points 'o' or ' '
	for (int i = 0; i < SPACE_WIDTH; ++i)
		for (int j = 0; j < SPACE_HEIGHT; ++j) {
			if (spaceArray[i][j] == 'x')
				spaceArray[i][j] = 'o';
			if (spaceArray[i][j] == 'd')
				spaceArray[i][j] = ' ';
		}
}

void getInitPoints(const string input, int output[])
{
	int j = 0;
	for (int i = 0; i < input.size(); ++i) {
		int coordinate = 0;
		if (isdigit(input[i])) {
			while (isdigit(input[i]))
			{
				coordinate *= 10;
				coordinate += input[i] - '0';
				++i;
			}
			output[j] = coordinate-1;
			if (j >= NUM_OF_INIT_LIFE * 2)
			{
				cerr << "Invalid input: too many input points" << endl;
				return;
			}
			if (output[j] >= SPACE_WIDTH || output[j] >= SPACE_HEIGHT)
			{
				cerr << "Bad input points" << endl;
				return;
			}
			++j;
		}
	}
}

void randomGenerated(char spaceArray[][SPACE_HEIGHT]) {
	int more_generation = 0;
	srand(time(0));
	while (more_generation < SPACE_WIDTH*SPACE_HEIGHT / 2) {
		spaceArray[rand() % SPACE_WIDTH][rand() % SPACE_HEIGHT] = 'o';
		++more_generation;
	}
}

string printArray(const char spaceArray[][SPACE_HEIGHT])
{
	string graph;
	graph+= "+";
	for (int i = 0; i < SPACE_WIDTH; ++i)
		graph+= "-";
	graph+= "+\n";
	for (int j = SPACE_HEIGHT - 1; j > -1; --j) {
		graph+= "|";
		for (int i = 0; i < SPACE_WIDTH; ++i)
			graph+= spaceArray[i][j];
		graph+= "|\n";
	}
	graph+= "+";
	for (int i = 0; i < SPACE_WIDTH; ++i)
		graph+= "-";
	graph+= "+";
	return graph;
}

void ShowConsoleCursor(bool showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}