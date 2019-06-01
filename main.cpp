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

#define MainMenuTextInitX 63
#define MainMenuTextInitY 12
#define BoardInitX 38
#define BoardInitY 2
#define WindowBottomY 29

#define BoardRow 10
#define BoardCol 9

using namespace std;
namespace fs = std::experimental::filesystem;

const string savePath = "./save";
const string chessText = "將士象車馬包卒帥仕相車傌炮兵";
const string mainMenuText[3] = { "開新遊戲", "讀取存檔", "離開遊戲" };
const string gameMenuText[4] = { "繼續遊戲", "保存遊戲", "重新開始", "退出遊戲" };
//const string BoardText[8] = {"１２３４５６７８９", "車馬象士將士象馬車", "砲", "卒", "兵", "炮", "車傌相仕帥仕相傌車", "九八七六五四三二一" };
string *Board = new string[23];

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
bool running = true;
bool gameRunning = true;
bool gameMenuOpen = false;
bool holdChess = false;
int mainMenuPosition = 0;
int gameMenuPosition = 0;


void init();
void delay(int);
DWORD getKey();
void storeSavedFilename(int &, vector<string> &);
void initCmdWindow(int, int);
void colorBoard(ChessBoard);
void colorMainMenuCursor();
void drawMainMenu();
void moveMainMenu(int);
void loadGame();
void showHoldHint(int);
void drawGameMenu(Game &);
void printGameFormat();
void moveCursor(int);
void startNewGame();
void mainMenuAction();
void gameMenuAction(Game &);
void testColor();

void init()
{
	SetConsoleTextAttribute(hConsole, InitColor);

	ios_base::sync_with_stdio(false);
	cin.tie(0);

	// system("chcp 65001");
	system("CHCP 950"); // BIG-5
}



void delay(int time)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(time));
}

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


void initCmdWindow(int width = CmdWidth, int height = CmdHeight)
{ // set cmd window's width and height
	HWND console = GetConsoleWindow();
	RECT r;

	GetWindowRect(console, &r);

	MoveWindow(console, 100, 100, width, height, TRUE);
}


void showReport(vector<string> reports)
{
	const int report_index_x = 20;
	int size = reports.size();
	int start_index = (reports.size()) > 19 ? reports.size() - 19 : 0;
	//while (reports.size() > 19)
	 //   reports.erase(reports.begin());
	for (int i = 0, j = start_index; i < 19 && j < size; i++, j++)
	{
		GoToXY(10, i + 2);
		SetConsoleTextAttribute(hConsole, InitColor);
		cout << j + 1 << " ";
		if (j % 2 == 1)
		{
			SetConsoleTextAttribute(hConsole, BlackColor);
			cout << "黑";
		}
		else
		{
			SetConsoleTextAttribute(hConsole, RedColor);
			cout << "紅";
		}
		SetConsoleTextAttribute(hConsole, InitColor);
		cout << "：" << reports[j];
	}
}

void drawPrompt(vector<Coord> move, vector<Coord> capture, Coord origin)
{
	int start_index_x = 37;
	int end_x = 70;
	int start_y = BoardInitY;
	int end_y = BoardInitY + BoardRow;

	Coord coo;

	SetConsoleTextAttribute(hConsole, movableColor);
	for (vector<Coord>::iterator it = move.begin(); it != move.end(); ++it)
	{
		GoToXY(BoardInitX - 1 + it->x * 4, BoardInitY + it->y * 2);
		cout << "  ";
	}

	SetConsoleTextAttribute(hConsole, eibleColor);
	for (vector<Coord>::iterator it = capture.begin(); it != capture.end(); ++it)
	{
		GoToXY(BoardInitX - 1 + it->x * 4, BoardInitY + it->y * 2);
		cout << "  ";
	}

	GoToXY(4 * origin.x + BoardInitX, 2 * origin.y + BoardInitY);
}

void showHint(unsigned short player) {
	int x = 87;
	int y = 4;

	GoToXY(x, y);
	if (player == PLAYER_BLACK)
	{
		SetConsoleTextAttribute(hConsole, BlackColor);
		cout << "黑色方";
	}
	else
	{
		SetConsoleTextAttribute(hConsole, RedColor);
		cout << "紅色方";
	}
}

void colorBoard(ChessBoard obj)
{ // color Board
	int start_index_x = 37;
	int end_x = 70;
	int start_y = BoardInitY;
	int end_y = BoardInitY + BoardRow;

	Coord coo;
	int kindCode;

	//GoToXY()
	SetConsoleTextAttribute(hConsole, BoardNumColor);

	GoToXY(BoardInitX - 1, BoardInitY - 1);
	SetConsoleTextAttribute(hConsole, BoardNumColor);
	for (int i = start_index_x; i <= end_x; i++)
		cout << Board[1][i];

	for (int i = 2, y = 0; i < 21; i++)
	{
		GoToXY(BoardInitX - 1, BoardInitY + i - 2);

		for (int j = start_index_x, x = 0; j <= end_x; j += 4)
		{
			coo.x = x;
			coo.y = y;

			kindCode = obj.getChess(coo).getKind();

			if (kindCode >= 1 && kindCode <= 7) // black
				SetConsoleTextAttribute(hConsole, BlackColor);
			else if (kindCode >= 8 && kindCode <= 14) // red
				SetConsoleTextAttribute(hConsole, RedColor);
			else
				SetConsoleTextAttribute(hConsole, BoardColor);

			if (i % 2 == 1)
				SetConsoleTextAttribute(hConsole, BoardColor);

			if (kindCode != -1 && i % 2 == 0)
			{
				for (int k = 2 * (kindCode - 1); k < 2 * kindCode; k++)
					cout << chessText[k];
			}
			else
			{
				for (int k = j; k < j + 2; k++)
					cout << Board[i][k];
			}

			SetConsoleTextAttribute(hConsole, BoardColor);
			for (int k = j + 2; k < j + 4 && k <= end_x; k++)
				cout << Board[i][k];

			if (i != 1 && i != 21 && i % 2 == 0)
				x++;
		}
		if (i % 2 == 0)
		{
			y++;
		}
		GoToXY(BoardInitX - 1, BoardInitY + 19);
		SetConsoleTextAttribute(hConsole, BoardNumColor);
		for (int i = start_index_x; i <= end_x; i++)
			cout << Board[21][i];
	}

	GoToXY(BoardInitX, BoardInitY);
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
void colorGameMenuCursor()
{ // color game menu's cursor
	SetConsoleTextAttribute(hConsole, MainMenuLineColor);
	for (int i = MainMenuTextInitY, j = 0; i <= MainMenuTextInitY + 4; i += 2, j++)
	{
		GoToXY(MainMenuTextInitX, i);
		cout << gameMenuText[j];
	}

	SetConsoleTextAttribute(hConsole, MainMenuChoosenColor);
	GoToXY(MainMenuTextInitX, MainMenuTextInitY + mainMenuPosition * 2);
	cout << gameMenuText[gameMenuPosition];
	GoToXY(0, WindowBottomY);
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
void moveMainMenu(int keydown)
{ // move menu's cursor
	if (keydown == UpArrowKey)
	{
		mainMenuPosition--;
	}
	if (keydown == DownArrowKey)
	{
		mainMenuPosition++;
	}
	if (mainMenuPosition > 2)
		mainMenuPosition = 0;
	if (mainMenuPosition < 0)
		mainMenuPosition = 2;
	colorMainMenuCursor();
}
void moveGameMenu(int keydown)
{ // move game menu's cursor
	if (keydown == UpArrowKey)
	{
		gameMenuPosition--;
	}
	if (keydown == DownArrowKey)
	{
		gameMenuPosition++;
	}
	if (gameMenuPosition > 3)
		gameMenuPosition = 0;
	if (gameMenuPosition < 0)
		gameMenuPosition = 3;
	colorGameMenuCursor();
}

void loadGame()
{
	int counter = 0;
	vector<string> filename;
	string cmd = "";

	storeSavedFilename(counter, filename);

	if (counter > 0)
	{
		while (atoi(cmd.c_str()) < 1 || atoi(cmd.c_str()) > counter)
		{
			cout << "請輸入編號(輸入 0 離開) : ";
			cin >> cmd;
			if (cmd[0] == '0' && cmd.length() == 1)
			{
				return;
			}
		}

		system("cls");
		cout << "載入 \"" << filename[atoi(cmd.c_str()) - 1] << "\" 中 ...... \n";
		delay(1000);

		// call gaming process
		Game newGame(filename[atoi(cmd.c_str()) - 1]);
		ChessBoard &board = newGame.getboard();
		system("cls");

		cursor.x = 0;
		cursor.y = 0;

		printGameFormat();
		colorBoard(board);
		showHint(newGame.playerNow());

		while (gameRunning)
		{
			GoToXY(4 * cursor.x + BoardInitX, 2 * cursor.y + BoardInitY);
			int keydown = getKey();
			Coord select;
			if (keydown != 0)
			{
				// move the cursor by arrow key
				if (keydown >= LeftArrowKey && keydown <= DownArrowKey)
					moveCursor(keydown);

				else if (keydown == EnterKey && !holdChess)
				{
					int kindCode = board.getChess(cursor).getKind();
					if (kindCode != -1) // not null chess
					{
						if ((newGame.playerNow() == PLAYER_BLACK && kindCode >= 1 && kindCode <= 7) ||
							(newGame.playerNow() == PLAYER_RED && kindCode >= 8 && kindCode <= 14))
						{
							select.x = cursor.x;
							select.y = cursor.y;
							vector<Coord> temp = newGame.promptMovement(select);
							vector<Coord> temp2 = newGame.promptCapture(select);
							drawPrompt(temp, temp2, select);
							showHoldHint(kindCode);
							holdChess = true;
						}
					}
				}
				// has chess -> move
				else if (keydown == EnterKey && holdChess)
				{
					Chess& c = board.getChess(select);
					if (select.x == cursor.x && select.y == cursor.y)
					{
						holdChess = false;
						colorBoard(board);
						showHoldHint(-1);
					}
					else if (board.isMovable(cursor, select, newGame.playerNow()))
					{
						if (board.moveChess(select, cursor, newGame.playerNow()))
						{
							holdChess = false;
							newGame.writeHistory(board.getArea());
							newGame.writeReport(select, cursor);
							colorBoard(board);
							newGame.switchPlayer();
							showHint(newGame.playerNow());
							showHoldHint(-1);
							showReport(newGame.getReport());
						}
					}
				}
				else if (keydown == EscKey)
				{
					drawGameMenu(newGame);
				}
			}
		}
	}
	else
	{
		cout << "無存檔！";
		delay(3000);
		drawMainMenu();
	}
}

void drawGameMenu(Game &g)
{
	int index = gameMenuPosition;
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

	cout << "│ "
		<< "繼續遊戲"
		<< " │\n";
	for (int i = 0; i < space; i++)
		cout << "　";

	cout << "├─────┤\n";
	for (int i = 0; i < space; i++)
		cout << "　";

	cout << "│ "
		<< "保存遊戲"
		<< " │\n";
	for (int i = 0; i < space; i++)
		cout << "　";

	cout << "├─────┤\n";
	for (int i = 0; i < space; i++)
		cout << "　";

	cout << "│ "
		<< "重新開始"
		<< " │\n";
	for (int i = 0; i < space; i++)
		cout << "　";

	cout << "│ "
		<< "退出遊戲"
		<< " │\n";
	for (int i = 0; i < space; i++)
		cout << "　";

	cout << "└─────┘\n";
	for (int i = 0; i < blank_row; i++)
		cout << "\n";
	colorGameMenuCursor();

	gameMenuOpen = true;
	while (gameMenuOpen)
	{
		int keydown = getKey();

		if (keydown != 0)
		{
			if (keydown == UpArrowKey || keydown == DownArrowKey)
			{
				moveGameMenu(keydown);
			}
			else if (keydown == EnterKey)
			{
				gameMenuAction(g);
			}
		}
	}
}

void gameMenuAction(Game &g)
{
	gameMenuOpen = false;
	// get system time
	time_t now = time(0);
	string fileName = to_string(now);
	if (gameMenuPosition == 0) // continue
		return;
	else if (gameMenuPosition == 1) // save
		g.saveFile(fileName + ".txt");
	else if (gameMenuPosition == 2) // restart
		startNewGame();
	else if (gameMenuPosition == 3) // quit
	{
		g.saveFile(fileName + ".txt");
		exit(0);
	}
}

void printGameFormat()
{
	ifstream in;
	in.open("./Format/blank.ya");
	string line;
	for (int i = 0; getline(in, line); i++)
	{
		Board[i] = line;
		cout << line << "\n";
	}
}

void moveCursor(int keydown)
{
	// int max_x = BoardCol * 2 + BoardInitX;
	// int max_y = BoardRow * 2 + BoardInitY;
	if (keydown == UpArrowKey)
	{
		Game::controll(0, -1);
	}
	else if (keydown == DownArrowKey)
	{
		Game::controll(0, 1);
	}
	else if (keydown == LeftArrowKey)
	{
		Game::controll(-1, 0);
	}
	else if (keydown == RightArrowKey)
	{
		Game::controll(1, 0);
	}

	cursor = Game::getController();

	if (cursor.x < 0)
		Game::controll(1, 0);
	if (cursor.x >= BoardCol)
		Game::controll(-1, 0);
	if (cursor.y < 0)
		Game::controll(0, 1);
	if (cursor.y >= BoardRow)
		Game::controll(0, -1);

	cursor = Game::getController();

	GoToXY(4 * cursor.x + BoardInitX, 2 * cursor.y + BoardInitY);
}

void showHoldHint(int kindCode) {
	int x = 84;
	int y = 6;

	GoToXY(x, y);
	SetConsoleTextAttribute(hConsole, InitColor);
	if (kindCode == -1) {
		cout << "            ";
	}
	else
	{
		cout << "您選擇了 ";
		if (kindCode >= 1 && kindCode <= 8)
		{
			SetConsoleTextAttribute(hConsole, BlackColor);
		}
		else if (kindCode >= 9 && kindCode <= 14)
		{
			SetConsoleTextAttribute(hConsole, RedColor);
		}
		for (int i = 2 * (kindCode - 1); i < 2 * kindCode; i++)
			cout << chessText[i];
	}
}

// main gaming process
void startNewGame()
{
	GoToXY(MainMenuTextInitX, MainMenuTextInitY);
	cout << "Game Start！\n";
	Game newGame("./save/Initial.txt");
	ChessBoard &board = newGame.getboard();
	delay(1000);
	system("cls");

	cursor.x = 0;
	cursor.y = 0;

	printGameFormat();
	colorBoard(board);
	showHint(newGame.playerNow());

	while (gameRunning)
	{
		GoToXY(4 * cursor.x + BoardInitX, 2 * cursor.y + BoardInitY);
		int keydown = getKey();
		Coord select;
		if (keydown != 0)
		{
			// move the cursor by arrow key
			if (keydown >= LeftArrowKey && keydown <= DownArrowKey)
				moveCursor(keydown);

			else if (keydown == EnterKey && !holdChess)
			{
				int kindCode = board.getChess(cursor).getKind();
				if (kindCode != -1) // not null chess
				{
					if ((newGame.playerNow() == PLAYER_BLACK && kindCode >= 1 && kindCode <= 7) ||
						(newGame.playerNow() == PLAYER_RED && kindCode >= 8 && kindCode <= 14))
					{
						select.x = cursor.x;
						select.y = cursor.y;
						vector<Coord> temp = newGame.promptMovement(select);
						vector<Coord> temp2 = newGame.promptCapture(select);
						drawPrompt(temp, temp2, select);
						showHoldHint(kindCode);
						holdChess = true;
					}
				}
			}
			// has chess -> move
			else if (keydown == EnterKey && holdChess)
			{
				Chess &c = board.getChess(select);
				if (select.x == cursor.x && select.y == cursor.y)
				{
					holdChess = false;
					colorBoard(board);
					showHoldHint(-1);
				}
				else if (board.isMovable(cursor, select, newGame.playerNow()))
				{
					if (board.moveChess(select, cursor, newGame.playerNow()))
					{
						holdChess = false;
						newGame.writeHistory(board.getArea());
						newGame.writeReport(select, cursor);
						colorBoard(board);
						newGame.switchPlayer();
						showHint(newGame.playerNow());
						showHoldHint(-1);
						showReport(newGame.getReport());
						showWinning(isCheckmate());
					}
				}
			}
			else if (keydown == EscKey)
			{
				drawGameMenu(newGame);
			}
			else if (keydown == OKey)
			{
				newGame.undo();
				newGame.switchPlayer();
			}
		}
	}
}

void mainMenuAction()
{
	SetConsoleTextAttribute(hConsole, InitColor);
	system("CLS");
	if (mainMenuPosition == 0)
	{
		startNewGame();
	}
	else if (mainMenuPosition == 1)
	{
		loadGame();
	}
	else if (mainMenuPosition == 2)
	{
		exit(0);
	}
	drawMainMenu();
}

void testColor()
{
	for (int i = 1; i <= 255; i++)
	{
		SetConsoleTextAttribute(hConsole, i);
		cout << i << " : weqweqweqwe\n";
	}
}

int main()
{
	init();
	initCmdWindow();
	drawMainMenu();
	//testColor();
	while (running)
	{
		int keydown = getKey();

		if (keydown != 0)
		{
			if (keydown == UpArrowKey || keydown == DownArrowKey)
			{
				moveMainMenu(keydown);
			}
			else if (keydown == EnterKey)
			{
				mainMenuAction();
			}
		}
	}
	return 0;
}