#include "snake.h"

void SetPos(short x, short y) {
	COORD pos = { x,y };
	HANDLE hOutput = NULL;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOutput, pos);
}

void Welcome() {
	SetPos(45, 10);
	printf("欢迎玩贪吃蛇");
	SetPos(45, 12);
	system("pause");
	system("cls");
	SetPos(45, 10);
	printf("按上下左右操纵");
	SetPos(45, 12);
	system("pause");
	system("cls");
}

void CreatMap() {
	int i;
	for (i = 0; i <= 56; i += 2) {
		SetPos(i, 0);
		printf("墙");
		SetPos(i, 26);
		printf("墙");
	}
	for (i = 0; i <= 26; i++) {
		SetPos(0, i);
		printf("墙");
		SetPos(56, i);
		printf("墙");
	}
}

void Init_Snake(pSnake psnake) {
	pSnakeNode cur = NULL;
	int i = 0;
	for (i = 0; i < 5; i++) {
		cur = (pSnakeNode)malloc(sizeof(SnakeNode));
		if (cur == NULL) {
			perror("Initsnake():malloc():");
			return;
		}
		cur->next = NULL;
		cur->x = POSX + i * 2;
		cur->y = POSY;
		if (psnake->Body == NULL) {
			psnake->Body = cur;
		}
		else {
			cur->next = psnake->Body;
			psnake->Body = cur;
		}
	}
	cur = psnake->Body;
	while (cur) {
		SetPos(cur->x, cur->y);
		printf("蛇");
		cur = cur->next;
	}
	psnake->Dir = RIGHT;
	psnake->Score = 0;
	psnake->FoodScore = 10;
	psnake->Status = OK;
	psnake->Sleeptime = 200;
}

void CreateFood(pSnake psnake) {
	int x = 0;
	int y = 0;
	int flag;
	do {
		x = rand() % 27 * 2 + 2;
		y = rand() % 25 + 1;
		flag = 0;
		pSnakeNode cur = psnake->Body;
		while(cur) {
			if (cur->x == x && y == cur->y)
				flag = 1;
			cur = cur->next;
		}
	} while (flag);
	pSnakeNode pFood = (pSnakeNode)malloc(sizeof(SnakeNode));
	if (pFood == NULL) {
		perror("Crearefood():malloc():");
		return;
	}
	else
	{
		pFood->x = x;
		pFood->y = y;
		SetPos(pFood->x, pFood->y);
		printf("果");
		psnake->Food = pFood;
	}

}

void GameStart (pSnake psnake){
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(hOutput, &CursorInfo);
	CursorInfo.bVisible = false;
	SetConsoleCursorInfo(hOutput, &CursorInfo);
	Welcome();
	CreatMap();
	Init_Snake(psnake);
	CreateFood(psnake);
}

void PrintHelpInfo() {
	SetPos(60, 15);
	printf("游戏说明");
	SetPos(60, 17);
	printf("按上下左右控制蛇的移动");
	SetPos(60, 19); 
	printf("正方向加速 反方向减速 空格暂停");
	SetPos(60, 21);
	printf("ESC,撞墙或咬到自己结束");
}
void GamePause() {
	while (1) {
		Sleep(100);
		if (KEY_PRESS(VK_SPACE))
			break;
	}
}
int NextIsFood(pSnakeNode pNextNode, pSnake psnake)
{
	return (pNextNode->x==psnake->Food->x)&& (pNextNode->y == psnake->Food->y);
}

void EatFood(pSnakeNode pNextNode, pSnake psnake)
{
	pNextNode->next = psnake->Body;
	psnake->Body = pNextNode;
	pSnakeNode cur = psnake->Body;
	while (cur) {
		SetPos(cur->x, cur->y);
		printf("蛇");
		cur = cur->next;
	}
	psnake->Score += psnake->FoodScore;
	free(psnake->Food);
	CreateFood(psnake);
}

void NoFood(pSnakeNode pNextNode, pSnake psnake)
{
	pNextNode->next = psnake->Body;
	psnake->Body = pNextNode;
	pSnakeNode cur = psnake->Body;
	while (cur->next->next) {
		SetPos(cur->x, cur->y);
		printf("蛇");
		cur = cur->next;
	}
	SetPos(cur->next->x, cur->next->y);
	printf("  ");
	free(cur->next);
	cur->next = NULL;
}

void WALLKILLEND(pSnake psnake)
{
	if (psnake->Body->x == 0 || psnake->Body->x == 56 || psnake->Body->y == 0 || psnake->Body->y == 26)
		psnake->Status = WALLKILL;
}

void SELFKILLEND(pSnake psnake)
{
	pSnakeNode cur = psnake->Body->next;
	while (cur&&psnake->Status==OK) {
		if (cur->x == psnake->Body->x && cur->y == psnake->Body->y)
			psnake->Status = SELFKILL;
		cur = cur->next;
	}
}

void SnakeMove(pSnake psnake){
	pSnakeNode pNextNode = (pSnakeNode)malloc(sizeof(SnakeNode));
	if (pNextNode == NULL) {
		perror("SnakeMove:malloc:");
		return;
	}
	switch (psnake->Dir) {
	case UP:pNextNode->x = psnake->Body->x; pNextNode->y = psnake->Body->y-1; break;
	case DOWN:pNextNode->x = psnake->Body->x; pNextNode->y = psnake->Body->y+1; break;
	case LEFT:pNextNode->x = psnake->Body->x-2; pNextNode->y = psnake->Body->y; break;
	case RIGHT:pNextNode->x = psnake->Body->x+2; pNextNode->y = psnake->Body->y; break;
	}
	if (NextIsFood(pNextNode,psnake)) {
		EatFood(pNextNode, psnake);
	}
	else {
		NoFood(pNextNode, psnake);
	}
	WALLKILLEND(psnake);
	SELFKILLEND(psnake);
}

void GameRun(pSnake psnake){
	PrintHelpInfo();
	do {
		SetPos(60, 10);
		printf("得分:%d",psnake->Score);
		SetPos(60, 11);
		printf("每个食物得分:%2d", psnake->FoodScore);
		SetPos(60, 13);
		if (KEY_PRESS(VK_UP)) {
			if (psnake->Dir == UP) {
				printf("加速");
				if (psnake->FoodScore < 22) {
					psnake->FoodScore += 2;
					psnake->Sleeptime -= 30;
				}
			}else if(psnake->Dir == DOWN) {
				printf("减速");
				if (psnake->FoodScore > 0) {
					psnake->FoodScore -= 2;
					psnake->Sleeptime += 30;
				}
			}else {
				printf("上  ");
				psnake->Dir = UP;
			}
		}else if (KEY_PRESS(VK_DOWN)) {
			if (psnake->Dir == DOWN) {
				printf("加速");
				if (psnake->FoodScore < 22) {
					psnake->FoodScore += 2;
					psnake->Sleeptime -= 30;
				}
			}
			else if (psnake->Dir == UP) {
				printf("减速");
				if (psnake->FoodScore > 0) {
					psnake->FoodScore -= 2;
					psnake->Sleeptime += 30;
				}
			}
			else {
				printf("下  ");
				psnake->Dir = DOWN;
			}
		}
		else if (KEY_PRESS(VK_LEFT)) {
			if (psnake->Dir == LEFT) {
				printf("加速");
				if (psnake->FoodScore < 22) {
					psnake->FoodScore += 2;
					psnake->Sleeptime -= 30;
				}
			}
			else if (psnake->Dir == RIGHT) {
				printf("减速");
				if (psnake->FoodScore > 0) {
					psnake->FoodScore -= 2;
					psnake->Sleeptime += 30;
				}
			}
			else {
				printf("左  ");
				psnake->Dir = LEFT;
			}
		}
		else if (KEY_PRESS(VK_RIGHT)) {
			if (psnake->Dir == RIGHT) {
				printf("加速");
				if (psnake->FoodScore < 22) {
					psnake->FoodScore += 2;
					psnake->Sleeptime -= 30;
				}
			}
			else if (psnake->Dir == LEFT) {
				printf("减速");
				if (psnake->FoodScore > 0) {
					psnake->FoodScore -= 2;
					psnake->Sleeptime += 30;
				}
			}
			else {
				printf("右  ");
				psnake->Dir = RIGHT;
			}
		}
		else if (KEY_PRESS(VK_SPACE)) {
			printf("暂停");
			GamePause();
		}
		else if (KEY_PRESS(VK_ESCAPE)) {
			psnake->Status = END;
		}
		Sleep(psnake->Sleeptime);
		SnakeMove(psnake);
	} while (psnake->Status == OK);
	SetPos(60, 13);
	printf("结束");
}

void GameEnd(pSnake psnake)
{
	SetPos(24, 12);
	switch (psnake->Status) {
	case END:printf("您主动退出，游戏结束"); break;
	case WALLKILL:printf("撞墙而死，游戏结束"); break;
	case SELFKILL:printf("自杀，游戏结束"); break;
	}
	pSnakeNode cur = psnake->Body;
	while (cur) {
		pSnakeNode del = cur;
		cur = cur->next;
		free(del);
	}
}
