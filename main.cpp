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
const string mainMenuText[4] = { "開新遊戲", "讀取存檔", "關於我們" , "離開遊戲" };


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
void colorMainMenuCursor(int y)
{ // color menu's cursor
	SetConsoleTextAttribute(hConsole, MainMenuLineColor);
	for (int i = MainMenuTextInitY, j = 0; i <= MainMenuTextInitY + 6; i += 2, j++)
	{
		GoToXY(MainMenuTextInitX, i);
		cout << mainMenuText[j];
	}

	SetConsoleTextAttribute(hConsole, MainMenuChoosenColor);
	GoToXY(MainMenuTextInitX, MainMenuTextInitY + y * 2);
	cout << mainMenuText[y];
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

	cout << "├─────┤\n";
	for (int i = 0; i < space; i++)
		cout << "　";

	cout << "│ " << mainMenuText[3] << " │\n";
	for (int i = 0; i < space; i++)
		cout << "　";

	cout << "└─────┘\n";
	for (int i = 0; i < blank_row; i++)
		cout << "\n";
}
#define mainMenuChose 4
int main() 
{
	static	int coordChose[2] = { 0 };  //coordcChose[0] = X, coordcChose[1] = Y
	bool gameRunning = 0, mainMenuRunning = 1, gameMenuRunning = 0;
	init();
	initCmdWindow();
	drawMainMenu();
	colorMainMenuCursor(0);
	while (true)
	{
		int keydown = getKey();
		if (mainMenuRunning)
		{
			if (keydown == UpArrowKey)
			{
				coordChose[1] --;
				if (coordChose[1] < 0)
				{
					coordChose[1] = mainMenuChose - 1;
				}
				colorMainMenuCursor(coordChose[1]);
			}
			else if (keydown == DownArrowKey)
			{
				coordChose[1] ++;
				coordChose[1] %= mainMenuChose;
				colorMainMenuCursor(coordChose[1]);
			}
			else if (keydown == EnterKey)
			{
				/*switch (int) {
				case 符合數字或字元:
					陳述句一;
					break;
				case 符合數字或字元:
					陳述句二;
					break;
				default:
					陳述三;
				}*/

				coordChose[0] = 0;
				coordChose[1] = 0;
			}
		}
		else if (gameMenuRunning)
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
		else if (gameRunning)
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