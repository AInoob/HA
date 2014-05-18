#include "stdafx.h"

using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
COORD pos;

boolean ask(string question)
{
	char response='x';
	cout << question << "enter y or n to choose" << endl;
	cin >> response;
	response = tolower(response);
	while (!(response=='y'||response=='n'))
	{
		cout << "invalid input, enter y or n to choose" << endl;
		cin >> response;
		response = tolower(response);
	}
	if (response=='y')
	{
		return true;
	}
	return false;
}



void changeSize(int a, int b)
{
	SMALL_RECT windowSize = { 0,0,a, b};
	SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
}

void gotoxy(int x, int y)
{
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(hConsole,pos);
}

void gotoxy(COORD p)
{
	gotoxy(pos.X, pos.Y);
}


class Node
{
private:
	COORD pos;
public: 
	Node* next;
	Node* prev;
	Node(int x,int y)
	{
		pos.X = x;
		pos.Y = y;
	}
	COORD getPos()
	{
		return pos;
	}
};

class Snake
{
	int MAX_W = 79, MAX_H = 24;
	int times = 0;
	int point = 0;
	char direction;
	boolean dead=false;
	Node *head;
	Node *tail;
	COORD food;
public:
	Snake()
	{
		gotoxy(6, 5);
		cout << "■";
		gotoxy(6, 4);
		cout << "■";
		head = new Node(6, 5);
		tail = new Node(6, 4);
		head->next = tail;
		head->prev = NULL;
		tail->next = NULL;
		tail->prev = head;
	}
	boolean isDead()
	{
		return dead;
	}
	void setWH(int a, int b)
	{
		MAX_W = a;
		MAX_H = b;
	}
	boolean checkFood(int x, int y)
	{
		Node *current = head;
		while (current != NULL)
		{
			if ((x == current->getPos().X) && (y == current->getPos().Y))
			{
				return true;
			}
			current = current->next;
		}
		return false;
	}
	boolean paintItSelf(int x, int y)
	{
		Node *current = head;
		while (current->next != NULL)
		{
			if ((x == current->getPos().X) && (y == current->getPos().Y))
			{
				return true;
			}
			current = current->next;
		}
		return false;
	}
	boolean eatItSelf(int x,int y)
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
	void makeFood()
	{
		times++;
		point = point + times;
		int x, y;
		while (true)
		{
			x = rand() % (MAX_W/2) * 2;
			y = rand() % MAX_H;
			if (!checkFood(x, y))
				break;
		}
		food.X = x;
		food.Y = y;
		gotoxy(x, y);
		cout << "★";
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
	void move(int x,int y)
	{
		gotoxy(0, 0);
		cout << "Score: " << point;
		Node *newHead = new Node(x, y);
		head->prev = newHead;
		newHead->next = head;
		head = newHead;
		gotoxy(head->getPos().X, head->getPos().Y);
		cout << "■";
		if ((food.X == x)&&(food.Y == y))
		{
			makeFood();
		}
		else
		{
			gotoxy(tail->getPos().X, tail->getPos().Y);
			if (!paintItSelf(tail->getPos().X, tail->getPos().Y))
			{
				cout << "  ";
			}
			tail = tail->prev;
			tail->next = NULL;
		}
	}
	void headTo(char d)
	{
		direction = d;
		COORD headPos;
		headPos = head->getPos();
		switch (direction)
		{
		case 'w':
			headPos.Y = headPos.Y - 1;
			break;
		case 's':
			headPos.Y = headPos.Y + 1;
			break;
		case 'a':
			headPos.X = headPos.X - 2;
			break;
		case 'd':
			headPos.X = headPos.X + 2;
			break;
		}
		if ((!eatItSelf(headPos.X, headPos.Y)) && (!hitWall(headPos.X, headPos.Y)))
		{
			move(headPos.X,headPos.Y);
		}
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	int w, h;
	srand((int)time(NULL));
	double speed=100;
	gotoxy(0, 0);
	if (ask("Do you want to customize settings?"))
	{
		cout << "input width(max 39) and height(max 24)" << endl;
		cin >> w >> h;
		w = w * 2 + 1;
		changeSize(w, h);
		cout << "j and k can change the speed of snake" << endl;
		Sleep(1000);
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
			else if (newDirection=='k')
			{
				if (speed>=3)
					speed = speed/1.1;
				newDirection = 'x';
			}
			if (newDirection == 'w' || newDirection == 'a' || newDirection == 's' || newDirection == 'd')
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
