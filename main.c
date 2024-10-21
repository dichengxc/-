#include "snake.h"
int main() {
	srand((unsigned int)time(NULL));
	system("mode con cols=120 lines=35");
	system("title snake");
	setlocale(LC_ALL, "");
	char ch;
	do {
		Snake ps = { 0 };
		system("cls");
		GameStart(&ps);
		GameRun(&ps);
		GameEnd(&ps);
		SetPos(20, 15);
		printf("再来一局？(Y/N)");
		ch = getchar();
		getchar();
		SetPos(0, 27);
	} while (ch == 'Y' || ch == 'y');
	return 0;
}