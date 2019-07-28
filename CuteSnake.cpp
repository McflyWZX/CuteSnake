// CuteSnake.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <windows.h>

void cursorGoto(int x, int y);//定位光标位置到指定坐标
void HideCursor();//隐藏光标
void printIn(int x, int y, const char* c);//控制台在指定位置输出
int UIMenu(int numLines, const char** pageStrings);//菜单库
int startGame(Vector2 mapSize);
int UIMainMenu();

typedef struct {
	int x, y;
} Vector2;
//Woshizhuzhu
Vector2 mapSize = { 32, 16 };
char direction[2][4] = { {0, 1, 0, -1}, {1, 0, -1, 0} };

int main()
{
	HideCursor();
	while (UIMainMenu());

}

void UIGameSettings()
{
	const char* gameSettingsStr[] = { "地图大小", "背景颜色", "背景音乐","返回" };
	switch (UIMenu(4, gameSettingsStr))
	{
	case 0:

		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		return;
		break;
	}
}

int UIMainMenu()
{
	const char* mainMenuStr[] = { "开始游戏", "游戏设置", "退出游戏" };
	int alt = 1;
	int score;
	switch (UIMenu(3, mainMenuStr))
	{
	case 0:

	start:
		score = startGame(mapSize);
		char scoreInfo[20];
		sprintf_s(scoreInfo, "你的得分是: %d", score);
		while (alt)
		{

			const char* gameOverStr[] = { scoreInfo,"重新开始","返回菜单" };
			switch (UIMenu(3, gameOverStr))
			{
			case 0:
				alt = 1;
				break;
			case 1:
				goto start;
				break;
			case 2:
				return 1;
				break;
			}
		}

		break;
	case 1:
		UIGameSettings();

		break;
	case 2:
		return 0;
		break;
	}
	return 1;
}

int startGame(Vector2 mapSize)
{
	system("cls");
	char gameOver = 0;
	//随机数发生器初始化
	srand((int)time(0));
	//创建地图数组
	char** map = new char* [mapSize.y];
	int** snakeMap = new int* [mapSize.y];
	for (int i = 0; i < mapSize.y; i++)
	{
		map[i] = new char[mapSize.x];
		snakeMap[i] = new int[mapSize.x];
	}

	//绘制地图内容
	for (int i = 0; i < mapSize.y; i++)
	{
		for (int j = 0; j < mapSize.x; j++)
		{
			if (i == 0 || j == 0 || i == mapSize.y - 1 || j == mapSize.x - 1)
			{
				map[i][j] = '#';
			}
			else
			{
				map[i][j] = ' ';
			}
			printf("%c", map[i][j]);
			snakeMap[i][j] = 0;
		}
		printf("\n");
	}

	//小蛇的参数
	int score = 0;
	int headX = 1, headY = 1;
	int tailX = 1, tailY = 1;
	map[headY][headX] = '@';
	int velocityHeadX = 1, velocityHeadY = 0;
	unsigned int snakeLengthCount = 1;
	snakeMap[headY][headX] = snakeLengthCount++;

	//用于时间控制的变量
	clock_t start, foodT = 0;
	//游戏逻辑
	while (!(GetAsyncKeyState(VK_ESCAPE) && 0x8000) && !gameOver)
	{
		start = clock();

		if (GetAsyncKeyState('A') && 0x8000)
		{
			velocityHeadX = -1;
			velocityHeadY = 0;
		}
		else if (GetAsyncKeyState('D') && 0x8000)
		{
			velocityHeadX = 1;
			velocityHeadY = 0;
		}
		else if (GetAsyncKeyState('W') && 0x8000)
		{
			velocityHeadX = 0;
			velocityHeadY = -1;
		}
		else if (GetAsyncKeyState('S') && 0x8000)
		{
			velocityHeadX = 0;
			velocityHeadY = 1;
		}

		//每九帧生成一个食物
		if (foodT++ == 9)
		{
			foodT = 0;

			int foodX, foodY;
			foodX = rand() % (mapSize.x - 2) + 1;
			foodY = rand() % (mapSize.y - 2) + 1;
			//如果当前随机位置上面已经有东西,就再生成一个随机位置
			while (map[foodY][foodX] != ' ')
			{
				foodX = rand() % (mapSize.x - 2) + 1;
				foodY = rand() % (mapSize.y - 2) + 1;
			}
			map[foodY][foodX] = '*';
			printIn(foodX, foodY, "*");

		}

		headX += velocityHeadX;
		headY += velocityHeadY;
		snakeMap[headY][headX] = snakeLengthCount++;

		if (map[headY][headX] != '*')
		{
			for (int i = 0; i < 4; i++)
			{
				if (snakeMap[tailY + direction[0][i]][tailX + direction[1][i]] == snakeMap[tailY][tailX] + 1)
				{
					printIn(tailX, tailY, " ");
					snakeMap[tailY][tailX] = 0;
					map[tailY][tailX] = ' ';
					tailY += direction[0][i];
					tailX += direction[1][i];
					break;
				}
			}
		}
		else {
			score++;
		}


		if (map[headY][headX] != ' ' && map[headY][headX] != '*')
			gameOver = 1;

		map[headY][headX] = '@';
		printIn(headX, headY, "@");
		//当当前时间-起始时间 < 300ms时在此等待,也就是达到了每300ms才更新一次画面的效果
		while ((clock() - start) * 1000 / CLOCKS_PER_SEC < 300);
	}
	return score;
}

int UIMenu(int numLines, const char** pageStrings)
{
	system("cls");
	int start;
	const int lineJump = 2;
	int select = 0;
	for (int i = 0; i < numLines; i++)
	{
		printIn(4, lineJump * (i + 1), pageStrings[i]);
	}
	while (1)
	{
		start = clock();
		if (GetAsyncKeyState('W') && 0x8000)
		{
			printIn(1, lineJump * (select + 1), "  ");
			select = (numLines + select - 1) % numLines;
		}
		else if (GetAsyncKeyState('S') && 0x8000)
		{
			printIn(1, lineJump * (select + 1), "  ");
			select = (numLines + select + 1) % numLines;
		}
		else if (GetAsyncKeyState(VK_RETURN) && 0x8000)
		{
			return select;
		}
		printIn(1, lineJump * (select + 1), ">>");
		while ((clock() - start) * 1000 / CLOCKS_PER_SEC < 300);
	}
}

void printIn(int x, int y, const char* c)
{
	cursorGoto(x, y);
	printf(c);
}


void cursorGoto(int x, int y)//定位光标位置到指定坐标
{
	HANDLE hOut;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { x,y };
	SetConsoleCursorPosition(hOut, pos);
}

void HideCursor()
{
	CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}


// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
