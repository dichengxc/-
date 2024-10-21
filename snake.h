#ifndef _SNAKE_H_
#define _SNAKE_H_

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <windows.h>
#include <time.h>

#define ture 1
#define false 0
#define POSX 10
#define POSY 5
#define KEY_PRESS(VK) ((GetAsyncKeyState(VK)&0x1)?1:0)

enum DIRECTION {
	UP=0,
	DOWN,
	LEFT,
	RIGHT
};
enum GAME_STATUS {
	END,
	OK,
	WALLKILL,
	SELFKILL
};

typedef struct SnakeNode {
	int x;
	int y;
	struct SnakeNode* next;
}SnakeNode,*pSnakeNode;

typedef struct Snake {
	pSnakeNode Body;
	pSnakeNode Food;
	enum DIRECTION Dir;
	enum GAME_STATUS Status;
	int Sleeptime;
	int Score;
	int FoodScore;

}Snake, * pSnake;

void SetPos(short x, short y);

void Welcome();

void CreatMap();

void Init_Snake(pSnake psnake);

void CreateFood(pSnake psnake);

void GameStart(pSnake psnake);

void PrintHelpInfo();

void GamePause();

int NextIsFood(pSnakeNode pNextNode, pSnake psnake);

void EatFood(pSnakeNode pNextNode, pSnake psnake);

void NoFood (pSnakeNode pNextNode, pSnake psnake);

void WALLKILLEND(pSnake psnake);

void SELFKILLEND(pSnake psnake);

void SnakeMove(pSnake psnake);

void GameRun(pSnake psnake);

void GameEnd(pSnake psnake);
#endif
