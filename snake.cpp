// ConsoleApplication1.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD pos;

void gotoxy(int x, int y)
{
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(console,pos);
}

void gotoxy(COORD p)
{
	pos.X = p.X;
	pos.Y = p.Y;
	SetConsoleCursorPosition(console, pos);
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
	Node getHead()
	{
		return *head;
	}
	Node getTail()
	{
		return *tail;
	}
	boolean isDead()
	{
		return dead;
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
	void makeFood()
	{
		times++;
		point = point + times;
		gotoxy(0, 0);
		cout << "Score: " << point;
		int x, y;
		while (true)
		{
			x = rand();
			y = rand();
			x=x % 40*2;
			y=y % 23;
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
		if (x<0 || y<0 || x>79 || y>24)
		{
			dead = true;
			return true;
		}
		return false;
	}
	void move(int x,int y)
	{
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
	while (true)
	{
		srand((int)time(NULL));
		char direction = 'd';
		char newDirection = 'x';
		Snake s;
		s.makeFood();
		while (!s.isDead())
		{
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
			if (newDirection == 'w' || newDirection == 'a' || newDirection == 's' || newDirection == 'd')
			{
				if ((direction == 'w'&&newDirection != 's') || (direction == 's'&&newDirection != 'w') || (direction == 'a'&&newDirection != 'd') || (direction == 'd'&&newDirection != 'a'))
				{
					direction = newDirection;
				}
			}
			s.headTo(direction);
			Sleep(100);
		}
		gotoxy(14, 6);
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
