#include <windows.h>
#include <chrono>
#include <stdint.h>
#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>

#define CmdWidth 1200
#define CmdHeight 650

#define MainMenuLineColor 9
#define MainMenuChoosenColor 240
#define InitColor 7
#define BlackColor 128
#define RedColor 132
#define BoardColor 240
#define BoardNumColor 253
#define eibleColor 16
#define movableColor 128
#define HintColor 3

#define UpArrowKey 38
#define LeftArrowKey 37
#define RightArrowKey 39
#define DownArrowKey 40
#define EnterKey 13
#define EscKey 27
#define OKey 79
#define PKey 80

#define MainMenuTextInitX 63
#define MainMenuTextInitY 12
#define BoardInitX 38
#define BoardInitY 2
#define WindowBottomY 29

#define BoardRow 10
#define BoardCol 9
using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
bool running = true;
bool gameRunning = true;
bool gameMenuOpen = false;
bool holdChess = false;
int mainMenuPosition = 0;
int gameMenuPosition = 0;
const string mainMenuText[3] = { "開新遊戲", "讀取存檔", "離開遊戲" };


DWORD getKey()
{ // get user keydown
	INPUT_RECORD irInput;
	DWORD InputsRead = 0;
	ReadConsoleInput(hInput, &irInput, 1, &InputsRead);
	if (irInput.Event.KeyEvent.bKeyDown)
		return irInput.Event.KeyEvent.wVirtualKeyCode;
	else
		return 0;
}
void GoToXY(int column, int line)
{
	COORD coord;
	coord.X = column;
	coord.Y = line;

	if (!SetConsoleCursorPosition(hConsole, coord))
	{
		cout << "移動游標error";
	}
}
void colorMainMenuCursor()
{ // color menu's cursor
	SetConsoleTextAttribute(hConsole, MainMenuLineColor);
	for (int i = MainMenuTextInitY, j = 0; i <= MainMenuTextInitY + 4; i += 2, j++)
	{
		GoToXY(MainMenuTextInitX, i);
		cout << mainMenuText[j];
	}

	SetConsoleTextAttribute(hConsole, MainMenuChoosenColor);
	GoToXY(MainMenuTextInitX, MainMenuTextInitY + mainMenuPosition * 2);
	cout << mainMenuText[mainMenuPosition];
	GoToXY(0, WindowBottomY);
}
void init()
{
	SetConsoleTextAttribute(hConsole, InitColor);

	ios_base::sync_with_stdio(false);
	cin.tie(0);

	// system("chcp 65001");
	system("CHCP 950"); // BIG-5
}

void initCmdWindow(int width = CmdWidth, int height = CmdHeight)
{ // set cmd window's width and height
	HWND console = GetConsoleWindow();
	RECT r;

	GetWindowRect(console, &r);

	MoveWindow(console, 100, 100, width, height, TRUE);
}

void drawMainMenu()
{
	int index = mainMenuPosition;
	int space = 30;
	int blank_row = 11;

	system("CLS");
	SetConsoleTextAttribute(hConsole, MainMenuLineColor);

	for (int i = 0; i < blank_row; i++)
		cout << "\n";

	for (int i = 0; i < space; i++)
		cout << "　";

	cout << "┌─────┐\n";
	for (int i = 0; i < space; i++)
		cout << "　";

	cout << "│ " << mainMenuText[0] << " │\n";
	for (int i = 0; i < space; i++)
		cout << "　";

	cout << "├─────┤\n";
	for (int i = 0; i < space; i++)
		cout << "　";

	cout << "│ " << mainMenuText[1] << " │\n";
	for (int i = 0; i < space; i++)
		cout << "　";

	cout << "├─────┤\n";
	for (int i = 0; i < space; i++)
		cout << "　";

	cout << "│ " << mainMenuText[2] << " │\n";
	for (int i = 0; i < space; i++)
		cout << "　";

	cout << "└─────┘\n";
	for (int i = 0; i < blank_row; i++)
		cout << "\n";
	colorMainMenuCursor();
}
#define mainMenuChose 4
int main() 
{
	bool gameRuning = 0, mainMenuRuning = 1, gameMenuRuning = 0;
	init();
	initCmdWindow();
	drawMainMenu();
	static	int coordChose[2] = { 0 };
	while (true)
	{
		int keydown = getKey();
		if (mainMenuRuning = 1)
		{
			if (keydown == UpArrowKey)
			{
				coordChose[0] -=1;
				if (coordChose[0] < 0)
				{
					coordChose[0] += mainMenuChose;
				}
			}
			else if (keydown == DownArrowKey)
			{
				coordChose[0] -= 1;
				coordChose[0] %= mainMenuChose;
			}
			else if (keydown == EnterKey)
			{
				//呼叫太陽的FUNTION
				coordChose[0] = 0;
				coordChose[1] = 0;
			}
		}
		else if (gameMenuRuning = 1)
		{
			if (keydown == LeftArrowKey)
			{

			}
			else if (keydown == RightArrowKey)
			{

			}
			else if (keydown == EnterKey)
			{
				//呼叫太陽的FUNTION
				coordChose[0] = 0;
				coordChose[1] = 0;
			}
		}
		else if (gameRuning = 1)
		{
			if (keydown == LeftArrowKey)
			{

			}
			else if (keydown == RightArrowKey)
			{

			}
			else if (keydown == EnterKey)
			{
				//呼叫太陽的FUNTION
				coordChose[0] = 0;
				coordChose[1] = 0;
			}
		}
		else if (true)
		{

		}
		else
		{

		}
	}
	return 0;
}