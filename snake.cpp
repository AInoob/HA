#include "stdafx.h"
using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

boolean ask(string question)
{
	char response = 'x';
	cout << question << "enter y or n to choose" << endl;
	cin >> response;
	response = tolower(response);
	while (!(response == 'y' || response == 'n'))
	{
		cout << "invalid input, enter y or n to choose" << endl;
		cin >> response;
		response = tolower(response);
	}
	if (response == 'y')
	{
		return true;
	}
	return false;
}

int ask(int min, int max, string question)
{
	int r;
	string input;
	cout << question << endl;
	char *temp;
	cin >> input;
	temp = const_cast<char*>(input.c_str());
	r = atoi(temp);
	while (r < min || r>max)
	{
		cout << "invalid input, try again" << endl;
		cin >> input;
		temp = const_cast<char*>(input.c_str());
		r = atoi(temp);
	}
	return r;
}

COORD** matrix(int x, int y, COORD value)
{
	COORD **m = new COORD*[x];
	for (int i = 0; i < x; i++)
	{
		m[i] = new COORD[y];
	}
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			m[i][j] = value;
		}
	}
	return m;
}

void changeSize(int a, int b)
{
	SMALL_RECT windowSize = { 0, 0, a, b };
	SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
}

void gotoxy(int x, int y)
{
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(hConsole, pos);
}

void show(int x, int y, string show)
{
	gotoxy(x * 2, y);
	cout << show;
}

class Snake
{
	COORD **map;
	int MAX_W = 39, MAX_H = 24;
	int times = 0;
	int point = 0;
	int length = 0;
	char direction;
	boolean dead = false;
	COORD head;
	COORD tail;
	COORD food;
public:
	Snake()
	{
		show(3, 5, "■");
		show(3, 4, "■");
		head = { 3, 5 };
		tail = { 3, 4 };
	}
	boolean isDead()
	{
		return dead;
	}
	void setWH(int a, int b)
	{
		MAX_W = a;
		MAX_H = b;
		COORD temp;
		temp = { -1, -1 };
		map = matrix(a+1, b+1,temp);
		map[head.X][head.Y] = tail;
		length = 2;
	}
	void end()
	{
		delete map;
	}
	boolean paintItSelf(int x, int y)
	{
		if (map[x][y].X == -1)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	boolean eatItSelf(int x, int y)
	{
		if (paintItSelf(x, y))
		{
			dead = true;
			return true;
		}
		else
		{
			return false;
		}
	}
	COORD getTail()
	{
		COORD current=head;
		for (int i = 0; i < length-1; i++)
		{
			current = map[current.X][current.Y];
		}
		return current;
	}
	COORD getTailPrev()
	{
		COORD current = head;
		for (int i = 0; i < length - 2; i++)
		{
			current = map[current.X][current.Y];
		}
		return current;
	}
	void makeFood()
	{
		times++;
		point = point + times;
		int r = rand() % (MAX_W*MAX_H-length);
		for (int i = 0; i < MAX_W; i++)
		{
			for (int j = 0; j < MAX_H; j++)
			{
				if (map[i][j].X == -1)
				{
					r--;
					if (r == -1)
					{
						food.X = i;
						food.Y = j;
						show(food.X, food.Y, "★");
						return;
					}
				}
			}
		}
		return;
	}
	boolean hitWall(int x, int y)
	{
		if (x<0 || y<0 || x>MAX_W || y>MAX_H)
		{
			dead = true;
			return true;
		}
		return false;
	}
	void move(int x, int y)
	{
		show(0, 0, "Score: ");
		cout <<point;
		COORD newHead = {x,y};
		map[newHead.X][newHead.Y] = head;
		head = newHead;
		show(head.X, head.Y, "■");
		if ((food.X == x) && (food.Y == y))
		{
			makeFood();
			length++;
		}
		else
		{
			COORD temp = getTail();
			if (!paintItSelf(tail.X, tail.Y))
			{
				show(tail.X, tail.Y, "  ");
				map[temp.X][temp.Y] = { -1, -1 };
			}
			tail = temp;
		}
	}
	void headTo(char d)
	{
		direction = d;
		COORD headPos;
		headPos = head;
		switch (direction)
		{
		case 'w':
			headPos.Y = headPos.Y - 1;
			break;
		case 's':
			headPos.Y = headPos.Y + 1;
			break;
		case 'a':
			headPos.X = headPos.X - 1;
			break;
		case 'd':
			headPos.X = headPos.X + 1;
			break;
		}
		if ((!hitWall(headPos.X, headPos.Y) && (!eatItSelf(headPos.X, headPos.Y))))
		{
			move(headPos.X, headPos.Y);
		}
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	SetConsoleTitle(L"SNAKE");
	int w, h, temp = 0;
	srand((int)time(NULL));
	double speed = 100;
	gotoxy(0, 0);
	if (ask("Do you want to customize settings?"))
	{
		cout << "input width(max 39, min 10) and height(max 24, min 10)" << endl;
		w=ask(10, 39, "input the width");
		h=ask(10, 24, "input the height");
		changeSize(w*2+1, h);
	}
	else
	{
		w = 39;
		h = 24;
	}
	cout << "j and k can change the speed of snake" << endl << "enter space to pause, then press any move key to resume" << endl;
	cout << "enter q to exit" << endl;
	while (temp < 3)
	{
		Sleep(1000);
		temp++;
		cout << 4 - temp << endl;
	}
	system("cls");
	while (true)
	{
		Snake s;
		s.setWH(w, h);
		char direction = 'd';
		char newDirection = 'x';
		s.makeFood();
		while (!s.isDead())
		{
			gotoxy(0, 1);
			cout << "Speed: " << speed;
			if (_kbhit())
				newDirection = _getch();
			while (newDirection == ' ')
			{
				if (_kbhit())
					newDirection = _getch();
			}
			if (newDirection == 224 || newDirection == 0 || newDirection == -32)
			{
				newDirection = _getch();
				switch (newDirection)
				{
				case 72:
					newDirection = 'w';
					break;
				case 80:
					newDirection = 's';
					break;
				case 75:
					newDirection = 'a';
					break;
				case 77:
					newDirection = 'd';
					break;
				}
			}

			newDirection = (char)towlower(newDirection);
			if (newDirection == 'j')
			{
				speed = 1.1*speed;
				newDirection = 'x';
			}
			else if (newDirection == 'k')
			{
				if (speed >= 3)
					speed = speed / 1.1;
				newDirection = 'x';
			}
			else if (newDirection == 'q')
			{
				return 0;
			}
			else if (newDirection == 'w' || newDirection == 'a' || newDirection == 's' || newDirection == 'd')
			{
				if ((direction == 'w'&&newDirection != 's') || (direction == 's'&&newDirection != 'w') || (direction == 'a'&&newDirection != 'd') || (direction == 'd'&&newDirection != 'a'))
				{
					direction = newDirection;
				}
			}
			s.headTo(direction);
			Sleep((int)speed);
		}
		gotoxy(0, 6);
		s.end();
		cout << "Game over! Press enter to restart";
		while (newDirection != 13)
		{
			if (_kbhit())
				newDirection = _getch();
		}
		system("cls");
	}
	return 0;
}
