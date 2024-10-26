#include <iostream>
#include <Windows.h>


bool gameOver;
unsigned width, height, length;

enum class DIR {
	STOP, UP, DOWN, RIGHT, LEFT
}direction;

struct Vec2 { int x; int y; };

Vec2 headPos, prevHeadPos;
Vec2 tail[100];
Vec2 applePos;

bool TouchingTail(int x, int y) {
	for (int i = 0; i < length; i++) {
		if (x == tail[i].x && y == tail[i].y)
			return true;
	}
	return false;
}

void ResetCursor()
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0,0 });
}

void SetColor(short colorCode)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),colorCode);
}

void FindApplePos() {
	while (true) {
		applePos.x = rand() % (width - 2)+1;
		applePos.y = rand() % (height -3)+1;
		if (applePos.x == headPos.x && applePos.y == headPos.y)
			continue;
		else {
			if (TouchingTail(applePos.x, applePos.y))
				continue;
			break;
		}
	}
}




void SetUp(unsigned width, unsigned height)
{
	system("cls");
	gameOver = false;
	::width = width+1;
	::height = height-1;
	direction = DIR::STOP;
	length = 0;
	headPos.x = width / 2;
	headPos.y = height / 2;
	FindApplePos();
}

void Input()
{
	if (GetAsyncKeyState(VK_UP)&&direction!=DIR::DOWN) {
		direction = DIR::UP;
	}
	else if (GetAsyncKeyState(VK_DOWN) && direction != DIR::UP) {
		direction = DIR::DOWN;
	}
	else if (GetAsyncKeyState(VK_RIGHT) && direction != DIR::LEFT) {
		direction = DIR::RIGHT;
	}
	else if (GetAsyncKeyState(VK_LEFT) && direction != DIR::RIGHT) {
		direction = DIR::LEFT;
	}
}


void Draw()
{
	ResetCursor();
	std::string canvas = "";

	for (unsigned i = 0; i < width; i++) {
		canvas += "[]";
	}
	canvas += '\n';

	for (unsigned j = 0; j < height; j++) {
		for (unsigned i = 0; i < width; i++) {
			if(i==0||i==width-1)
				canvas += "[]";
			else if (i==headPos.x&&j==headPos.y) {
				canvas += "()";
			}
			else if (i == applePos.x && j == applePos.y) {
				canvas += "`O";
			}
			else if (TouchingTail(i, j)) {
				canvas += "::";
			}
			else  {
				canvas += "  ";
			}
		}
		canvas += '\n';
	}

	for (unsigned i = 0; i < width; i++) {
		canvas += "[]";
	}

	std::cout << canvas << std::endl;
}

void Logic()
{

	prevHeadPos = headPos;

	switch (direction) //Movement
	{
	case DIR::UP:
		headPos.y--;
		break;
	case DIR::DOWN:
		headPos.y++;
		break;
	case DIR::LEFT:
		headPos.x--;
		break;
	case DIR::RIGHT:
		headPos.x++;
		break;
	}

	
	if (TouchingTail(headPos.x, headPos.y)) //Crushing on Tail
		gameOver = true;

	for (int i = length; i >= 1; i--) { //Updating Tale
		tail[i] = tail[i - 1];
	}
	tail[0] = prevHeadPos;

	if (headPos.x == applePos.x && headPos.y == applePos.y) { //Collecting the Apple
		length++;
		FindApplePos();
	}
	
	if (headPos.x < 1)
		headPos.x = width-2; 
	else if (headPos.x > width - 2)
		headPos.x = 1;
	else if (headPos.y < 0)
		headPos.y = height - 1;
	else if (headPos.y > height - 1)
		headPos.y = 0;

}

int main()
{
	SetUp(30,30);


	while (!gameOver)
	{
		Input();
		Logic();
		Draw();

		Sleep(100);
	}
	
	std::cout << "GAME OVER!" << std::endl;
	system("pause");

	main();


}


