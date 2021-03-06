#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <cstdlib>

enum VIEW
{
	MENU = 0, GAME, END
};
enum DIRECTION
{
	UP = 0, RIGHT, DOWN, LEFT
};
const int scoreToWin = 50;

void setup();
void finishGame();
void logic();
void draw();
void toMenu();
void toGame();
bool isTailCoords(int x, int y);
void createPoint();
void addMoveTail(int x, int y);
void moveTail(int x, int y);

bool endGame;
int mapWidth;
int mapHeight;
VIEW currentView;

int speedSnake;//co ile ms zmienia pozycje
float speedMultiplier;//co ile ms zmienia pozycje
DIRECTION direction;
int score;
int xSnake;
int ySnake;

int timeLastSnakeChangePosition;
int startTime;

int xsTails[scoreToWin];
int ysTails[scoreToWin];
int cTails;

int xPoint;
int yPoint;

int main()
{
	srand(time(0));
	setup();
	while (!endGame)
	{
		system("cls");
		logic();
		draw();
		Sleep(50);
	}
}

void setup()
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(handle, &info);
	COORD new_size =
	{
		info.srWindow.Right - info.srWindow.Left + 1,
		info.srWindow.Bottom - info.srWindow.Top + 1
	};
	SetConsoleScreenBufferSize(handle, new_size);

	endGame = false;
	mapWidth = 30;
	mapHeight = 15;
	currentView = MENU;
	score = -1;
	direction = UP;
}

void finishGame()
{
	endGame = true;
}

void logic()
{
	if (currentView == MENU)
	{
		while (_kbhit())
		{
			switch ((int)_getch())
			{
			case 49:
				toGame();
				break;
			case 50:
				finishGame();
				break;
			case 27:
				finishGame();
				break;
			}
		}
		return;
	}
	if (currentView == GAME)
	{
		int xSnakeFrom = xSnake;
		int ySnakeFrom = ySnake;
		while (_kbhit())
		{
			switch ((int) _getch())
			{
			case 72:    // key up
				direction = UP;
				break;
			case 80:    // key down
				direction = DOWN;
				break;
			case 77:    // key right
				direction = RIGHT;
				break;
			case 75:    // key left
				direction = LEFT;
				break;
			case 27:    // key esc
				toMenu();
				break;
			}
		}
		int timeNow = clock();
		if (timeNow-timeLastSnakeChangePosition >= speedSnake/speedMultiplier)
		{
			switch (direction)
			{
			case UP:    // key up
				ySnake -= 1;
				break;
			case DOWN:    // key down
				ySnake += 1;
				break;
			case RIGHT:    // key right
				xSnake += 1;
				break;
			case LEFT:    // key left
				xSnake -= 1;
				break;
			}
			if (ySnake <= 1)
			{
				//ySnake = mapHeight - 1;
				toMenu();
				return;
			}
			else if (ySnake >= mapHeight)
			{
				//ySnake = 2;
				toMenu();
				return;
			}
			else if (xSnake <= 1)
			{
				//xSnake = mapWidth - 1;
				toMenu();
				return;
			}
			else if (xSnake >= mapWidth)
			{
				//xSnake = 2;
				toMenu();
				return;
			}
			if (isTailCoords(xSnake, ySnake)) {
				toMenu();
				return;
			}
			if (xSnake == xPoint && ySnake == yPoint)
			{
				score++;
				speedMultiplier = (float)score * 4 / (float)scoreToWin*0.9 + 1;
				createPoint();
				addMoveTail(xSnakeFrom, ySnakeFrom);
			}
			else
			{
				moveTail(xSnakeFrom, ySnakeFrom);
			}
			if (score == scoreToWin)
			{
				toMenu();
				return;
			}
			timeLastSnakeChangePosition = timeNow;
		}
		return;
	}
	if (currentView == END)
	{
		finishGame();
		return;
	}
}

void draw()
{
	if (currentView == MENU)
	{
		printf("Witaj w SnakeConsole by Huczeq\n\n");
		if (score < 0)
		{
			printf("MENU: (wybierz na klawiaturze)\n");
			printf("1. Rozpocznij gre\n");
		}
		else
		{
			if (score == scoreToWin)
				printf("Gratulacje, wygrales :)\nW czasie %is\n\n", startTime);
			else
				printf("Przegrales :(\nZdobyles %i punktow w czasie %is\n\n", score, startTime);
			printf("MENU: (wybierz na klawiaturze)\n");
			printf("1. Zagraj od nowa\n");
		}
		printf("2/ESC. Zakoncz gre\n");
		return;
	}
	if (currentView == GAME)
	{
		printf("Zbierz %i punktow(gwiazdek), uwazaj na swoj ogon i ogrodzenie\n\n", scoreToWin);
		for (int hi = 1; hi <= mapHeight; hi++)
		{
			for (int wi = 1; wi <= mapWidth; wi++)
			{
				if (hi == 1 || hi == mapHeight || wi == 1 || wi == mapWidth)
				{
					printf("#");
					continue;
				}
				if (hi == ySnake && wi == xSnake)
				{
					printf("@");
					continue;
				}
				if (isTailCoords(wi, hi))
				{
					printf("$");
					continue;
				}
				if (hi == yPoint && wi == xPoint)
				{
					printf("*");
					continue;
				}
				printf(" ");
			}
			printf("\n");
		}
		printf("\nPunkty: %i\n", score);
		printf("Czas: %is\n", time(0) - startTime);
		printf("Point: x:%i y:%i\n", xPoint, yPoint);
		printf("Snake: x:%i y:%i\n", xSnake, ySnake);
		printf("Speed: %i\n", speedSnake);
		printf("SpeedM: %g\n", speedMultiplier);
		printf("Speed/M: %g\n", (float)speedSnake / speedMultiplier);
		printf("\nMozesz wcisnac ESC aby wrocic do menu");
		/*
		int consoleWidth = 120;
		int consoleHeight = 30;

		int leftM = consoleWidth / 2 - mapWidth / 2;
		int rightM = consoleWidth / 2 + mapWidth / 2;
		int topM = consoleHeight / 2 - mapHeight / 2;
		int bottomM = consoleHeight / 2 + mapHeight / 2;

		for (int hi = 1; hi <= consoleHeight; hi++)
		{
			for (int wi = 1; wi <= consoleWidth; wi++)
			{
				if (hi == topM || hi == bottomM)
				{
					if (wi > leftM && wi < rightM)
						printf("#");
					else
						printf(" ");
				}
				else if(hi > topM && hi < bottomM)
				{
					if (wi == leftM || wi == rightM)
						printf("#");
					else
						printf(" ");
				}
				else
				{
					printf(" ");
				}
			}
			printf("\n");
		}*/
		return;
	}
}

void toMenu()
{
	currentView = MENU;
	startTime = time(0) - startTime;
}

void toGame()
{
	currentView = GAME;
	score = 0;
	direction = UP;
	xSnake = mapWidth / 2;
	ySnake = mapHeight / 2;
	timeLastSnakeChangePosition = clock();
	startTime = time(0);
	cTails = 0;
	createPoint();
	speedMultiplier = 1;
	speedSnake = 400;
}

bool isTailCoords(int x, int y)
{
	for (int i = 0; i < cTails; i++)
	{
		if (xsTails[i] == x && ysTails[i] == y)
			return true;
	}
	return false;
}

void createPoint()
{
	xPoint = rand() % (mapWidth - 2) + 2;
	yPoint = rand() % (mapHeight - 2) + 2;
}

void addMoveTail(int x, int y)
{
	for (int i = cTails; i > 0; i--)
	{
		xsTails[i] = xsTails[i - 1];
		ysTails[i] = ysTails[i - 1];
	}
	cTails++;
	xsTails[0] = x;
	ysTails[0] = y;
}

void moveTail(int x, int y)
{
	for (int i = cTails-1; i > 0; i--)
	{
		xsTails[i] = xsTails[i - 1];
		ysTails[i] = ysTails[i - 1];
	}
	xsTails[0] = x;
	ysTails[0] = y;
}
