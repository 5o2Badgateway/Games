#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<math.h>
#include<Windows.h>
#include<conio.h>

const int MX =25;				//游戏界面的长度
const int MY= 16;				//游戏界面的高度
const int REFRESH_RATE = 300;	//屏幕刷新周期(")
const int WALL_CYCLE = 16;		//刷出墙的频率(多少个刷新周期)

int GameMap[MX][MY];		//游戏地图(0=Space,1=Wall,2=Player)
const int px = 4;
int py ;					//玩家的横纵坐标，其中横坐标不变(地图不断移动)
int WallCenter;				//墙的中心位置
int WallT ;					//定期生成墙的变量
char PlayerChar;			//代表玩家的字母
int score;					//得分
int key;

void MovementControl();
void Check();
void Initialize();
void MapGeneration();
void Show();
void GenerateWall(int y);

void main() {
	Initialize();
	Show();
	system("pause");
}

void Initialize() {
	system("cls");
	system("color 1F");
	system("title FlappyCHAR 1.0.0 (Cridit:ZZW)");
	printf("Inter your CHAR:\t");
	scanf_s("%c", &PlayerChar);
	score = -1;
	py = MY/2;
	WallCenter = MY/2;							//重置玩家和墙的位置
	for (int i = 0; i <= MX - 1; i++) {			//清空地图
		for (int j = 0; j <= MY - 1; j++) {
			GameMap[i][j] = 0;
		}
	}
	GenerateWall(WallCenter);					//生成第一堵墙
}

void Show() {
	while (1) {
		Sleep(REFRESH_RATE);
		system("cls");
		if (GameMap[px][py] == 0)
			GameMap[px][py] = 2;
		for (int i = 0; i <= MY - 1; i++) {		//显示地图
			for (int j = 0; j <= MX - 1; j++) {
				switch (GameMap[j][i]) {
				case 0:printf("  "); break;
				case 1:printf("XX"); break;
				case 2:printf("%c%c", PlayerChar, PlayerChar); break;
				}
			}
			printf("|\n");
		}
		for (int i = 0; i <= MX; i++)
			printf("--");
		Check();
		MovementControl();
		MapGeneration();
		WallT++;
	}
}


void MovementControl() {
	py++;										//玩家位置每周期下降一格(注意，y越大，越靠下)
	while(_kbhit()!=0)
		key = _getch();
	if (key != 0)
		py -= 3;								//按键就升高2格
	key = 0;
	if (py < 0)
		py = 0;									//防止玩家跑到画面上面去
}

void Check() {									//游戏结束和得分判定
	if (GameMap[px][py] == 1||py==MY) {
		printf("\n游戏结束，得分：%d\n", score);
		printf("你得到了：");
		if (score <= 10)
			printf("我们的同情\n");
		else if (score <= 20)
			printf("铜牌");
		else if (score <= 50)
			printf("银牌");
		else
			printf("最无聊奖和我们的爱");
		printf("\n按任意键重新开始")
		system("pause");
		Initialize();							//我不知道为什么，反正就是要重新开始两次
		Initialize();
	}
}

void MapGeneration() {
	for (int i = 0; i <= MX - 2; i++) {
		for (int j = 0; j <= MY - 1; j++) {
			switch (GameMap[i + 1][j]) {		//地图整体左移一格
			case 0:GameMap[i][j] = 0; break;
			case 1:GameMap[i][j] = 1; break;
			case 2:GameMap[i][j] = 0; break;
			}
		}
	}
	WallT++;
	if (WallT >= WALL_CYCLE) {
		WallCenter = rand() % (MY - 2) + 1;
		GenerateWall(WallCenter);
	}
		
}

void GenerateWall(int y) {
	for (int i = MX*3/4, j = 0; j <= MY - 1; j++) {//在地图末尾以y为孔洞中心生成一堵墙(1=Wall)，不要改i，有魔法
		if (abs(j - y) > 2) {
			GameMap[i][j] = 1;
		}
	}
	WallT = 0;
	score++;										//每过（造出）一堵墙得一分
}
