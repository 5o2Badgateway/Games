#include<stdlib.h>
#include<stdio.h>
#include<conio.h>
#include<time.h>
#include<math.h>
#include<Windows.h>

const int MX = 4;			//棋盘的长度和宽度
const int MY = 4;

int board[MX][MY];			//棋盘格
int key;					//玩家键盘输入值
int done;					//检查玩家的操作是否有效

void Initialize();			//初始化
void Show();				//显示游戏画面
void Control();				//输入操作
void Check();				//游戏结束判定
void U_D(int Dy);			//上下移动
void L_R(int Dx);			//左右移动
void SpawnBlock();

void gotoxy(int x, int y) 
{
	COORD pos;
	pos.X = x - 1;
	pos.Y = y - 1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

int GetDigits(int n) {
	int digit = 1;
	while (n / 10 != 0) {
		n /= 10;
		digit++;
	}
	return digit;
}

int main() {
	Initialize();
	Show();
	system("pause");
	return 0;
}

void Initialize() {
	srand((unsigned)time(NULL));
	system("cls");
	system("title 8196 1.1.2 (Cridit:ZZW)");
	system("color 1F");
	for (int i = 0; i <= MX - 1; i++) {										//清空棋盘
		for (int j = 0; j <= MY - 1; j++) {
			board[i][j] = 0;
		}
	}
	done = 0;
}

void Show() {
	while (1) {
		system("cls");
		if (done == 0)
			SpawnBlock();
		for (int i = 0; i <= MY - 1; i++) {									//表示棋盘
			for (int j = 0; j <= MX - 1; j++) {
				gotoxy(5 * j + 1, 4 * i + 1); printf("+----+");
				gotoxy(5 * j + 1, 4 * i + 2); printf("|    |");
				if (board[j][i] != 0) {
					gotoxy(5 * j + 1, 4 * i + 3); printf("|%d", board[j][i]);
				}
				else {
					gotoxy(5 * j + 1, 4 * i + 3); printf("| ");
				}
				switch (GetDigits(board[j][i])) {
				case 1:printf("   |"); break;
				case 2:printf("  |"); break;
				case 3:printf(" |"); break;
				case 4:printf("|"); break;
				}
				gotoxy(5 * j + 1, 4 * i + 4); printf("|    |");
			}
		}
		printf("\n+");
		for (int i = 1; i <= 5 * MX-1; i++)
			printf("-");
		printf("+");
		Sleep(1000);
		Control();
		Check();
	}
}

void Control() {
	if (_kbhit() != 0) {
		while (_kbhit() != 0)
			key = _getch();
		switch (key) {
		case 77:/*右*/L_R(1); break;
		case 75:/*左*/L_R(-1); break;
		case 80:/*下*/U_D(1); break;
		case 72:/*上*/U_D(-1); break;
		}
	}
}

void Check() {
	int check = 0;
	int score = 0;
	for (int i = 0; i <= MX - 1; i++) {			//先检查棋盘是不是已经满了
		for (int j = 0; j <= MY - 1; j++) {
			score += board[i][j];
			if (board[i][j] == 0)
				check = 1;
		}
	}
	if (check == 0) {							//如果满了，再检查能不能继续走下去
		for (int i = 0; i <= MX - 1; i++) {
			for (int j = 0; j <= MY - 1; j++) {
				if (board[i][j] < 8196 && ((i + 1 <= MX - 1 && board[i][j] == board[i + 1][j]) || (j + 1 <= MY - 1 && board[i][j] == board[i][j + 1]))) {
					check = 1;
				}

			}

		}
	}
	if (check == 0) {							//如果都满足，游戏结束
		printf("\n\nGame Over!\nYour Score is %d\n\nPress any key to retry\n",score);
		system("pause");
		Initialize();

	}
}

void U_D(int Dy) {								//上下移动
	done = 0;
	for (int i = 0; i <= MX-1; i++) {
		while (1) {
			int moved = 0;
			for (int j = 0; j <= MY - 1; j++) {
				if (board[i][j] != 0) {
					if ((j + Dy <= MY - 1 && j + Dy >= 0) && board[i][j + Dy] == 0) {
						board[i][j + Dy] = board[i][j];
						board[i][j] = 0;
						moved = 1;
					}
					if ((j + Dy <= MY - 1 && j + Dy >= 0) && board[i][j + Dy] == board[i][j]&&board[i][j]<8196) {
						board[i][j + Dy] *= 2;
						board[i][j] = 0;
						moved = 1;
						done = 1;
					}
				}
			}
			if (moved == 0)
				break;
		}
		
	}
}
void L_R(int Dx) {								//左右移动
	done = 0;
	for (int i = 0; i <= MY - 1; i++) {
		while (1) {
			int moved = 0;
			for (int j = 0; j <= MX - 1; j++) {
				if (board[j][i] != 0) {
					if ((j + Dx <= MX - 1 && j + Dx >= 0) && board[j + Dx][i] == 0) {
						board[j + Dx][i] = board[j][i];
						board[j][i] = 0;
						moved = 1;
					}
					if ((j + Dx <= MX - 1 && j + Dx >= 0) && board[j + Dx][i] == board[j][i] && board[j][i] < 8196) {
						board[j + Dx][i] *= 2;
						board[j][i] = 0;
						moved = 1;
						done = 1;
					}
				}
			}
			if (moved == 0)
				break;
		}

	}
}

void SpawnBlock() {								//随机找到一个空的方块，插入2
	int i, j;
	for ( i = rand() % MX, j = rand() % MY; board[i][j] != 0; i = rand() % MX, j = rand() % MY);
	board[i][j] = 2;
	done = 1;
}