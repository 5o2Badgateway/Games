#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<time.h>
#include<Windows.h>
#include<conio.h>

const int MY = 25;				//迷宫大小、墙和玩家的显示、etc
const int MX = 35;
char MazeMap[MX][MY];
const char WALL = 'Z';
const char PATH = ' ';
const char PLAYER = '@';
const char ANSWER = 'X';
const char ANSWERED = '.';

int WallX[MX*MY] = { 0,1 };		//邻墙的横纵坐标
int WallY[MY*MX] = { 1,0 };
int n = 1;

int px = 0;
int py = 0;						//玩家位置坐标
int ax = 0;
int ay = 0;						//自动寻路临时坐标
int t = 1;						//一个让玩家图标闪动的变量

const int REFRESH_RATE = 500;	//屏幕刷新间隔(毫秒)

void Initialize();				//初始化并生成地图
int count(int x, int y);		//计算某区块四周墙的数目
void AddWall(int x, int y);		//生成新的通路、etc.

void Control();		//接受玩家控制
int key;			//接受玩家输入

void Show();		//显示游戏画面
void ShowMap();		//显示地图

void Check();		//玩家胜利判断

void Answer();		//计算正确路线并显示



int main() {
	Initialize();
	Show();
	system("pause");
}


void ReInitialize() {
	srand(time(NULL));
	WallX[0] = 0, WallX[1] = 1;
	WallY[0] = 1, WallY[1] = 0;
	n = 1;
	Initialize();
}

void Initialize() {
	px = 0, py = 0;
	system("color 1F");
	system("title Maze 1.1.1 (Credit:ZZW)");		//哪个人贡献编程，哪个人就把名字写上去
	for (int i = 0 ; i <= MY - 1; i++) {			//重置地图
		for (int j = 0 ; j <= MX - 1; j++) {
			MazeMap[j][i] = WALL;						
		}
	}
	srand(time(NULL));
	MazeMap[0][0] = PATH;							//入口是通路
	int m;											//m是个随机数
	for (; n > 0; ) {
		m = rand() % (n+1);
		int cont = count(WallX[m], WallY[m]);
		if (cont >= 2) {							//如果邻墙隔开两个通路，那么从列表中去掉它
			WallX[m] = WallX[n];
			WallY[m] = WallY[n];
			n--;
		}
		else {
			AddWall(WallX[m], WallY[m]);			//生成通路块
			WallX[m] = WallX[n];
			WallY[m] = WallY[n];
			n--;
		}
	}
}

int count(int x, int y) {							//计算相邻墙壁数量
	int cont = 0;
	if (x - 1 >= 0 && MazeMap[x - 1][y] == PATH)
		cont++;
	if (x + 1 <= MX - 1 && MazeMap[x + 1][y] == PATH)
		cont++;
	if (y - 1 >= 0 && MazeMap[x][y - 1] == PATH)
		cont++;
	if (y + 1 <= MY - 1 && MazeMap[x][y + 1] == PATH)
		cont++;
	return cont;
}

void AddWall(int x, int y) {
	MazeMap[x][y] = '0';
	int x2 = x, y2 = y;
	if (x - 1 >= 0 && MazeMap[x - 1][y] == PATH)		//找到墙堵上的通路
		x2 = x + 1 ;
	if (x + 1 <= MX - 1 && MazeMap[x + 1][y] == PATH)
		x2 = x - 1;
	if (y - 1 >= 0 && MazeMap[x][y - 1] == PATH)
		y2 = y + 1;
	if (y + 1 <= MY - 1 && MazeMap[x][y + 1] == PATH)
		y2 = y - 1;
	if (x2 - 1 >= 0 && MazeMap[x2 - 1][y2] == WALL){
		n++;
		WallX[n] = x2 - 1;
		WallY[n] = y2;
	}
	if (x2 + 1 <= MX - 1 && MazeMap[x2 + 1][y2] == WALL) {
		n++;
		WallX[n] = x2 + 1;
		WallY[n] = y2;
	}
	if (y2 - 1 >= 0 && MazeMap[x2][y2 - 1] == WALL) {
		n++;
		WallX[n] = x2;
		WallY[n] = y2 - 1;
	}
	if (y2 + 1 <= MY - 1 && MazeMap[x2][y2 + 1] == WALL) {
		n++;
		WallX[n] = x2;
		WallY[n] = y2 + 1;
	}
	MazeMap[x][y] = PATH;
	MazeMap[x2][y2] = PATH;
}



void Control() {
	Check();					//检查玩家是否达到终点
	if (_kbhit() != 0) 
	{
		MazeMap[px][py] = PATH;
		t = -1;
		while (_kbhit() != 0) 
			key = _getch(); 
		switch (key)
		{   
		case 75:  if (px - 1 >= 0 && MazeMap[px - 1][py] == PATH)		//左
			px--;
			break;
		case 77:  if (px + 1 <= MX - 1 && MazeMap[px + 1][py] == PATH)	//右
			px++;
			break;
		case 80:  if (py + 1 <= MY - 1 && MazeMap[px][py + 1] == PATH)	//上
			py++;
			break;
		case 72:  if (py - 1 >= 0 && MazeMap[px][py - 1] == PATH)		//下
			py--;
			break;
		case 32: ReInitialize();
			break;
		default:Answer();
		}
	}
}



void Show() {											//显示地图和玩家		
	while (1) {
		ShowMap();
		Sleep(REFRESH_RATE);
		t *= -1;
		Control();										//接收控制，改变玩家位置	
		if (t == -1)
			MazeMap[px][py] = PLAYER;
		else
			MazeMap[px][py] = PATH;
	}
}

void ShowMap() {
	system("cls");									//清屏
	for (int i = 0; i <= MY-1; i++) {				//输出地图
		for (int j = -0; j <= MX-1 ; j++) {
			switch (MazeMap[j][i]) {
			case PATH:printf("  "); break;
			case WALL:printf("■"); break;
			case PLAYER:printf("□"); break;
			case ANSWER:printf("╳ "); break;
			case ANSWERED:printf("  "); break;
			}
		}
		printf("\n");
	}
	printf("\n按方向键移动，按除了方向键以外的任意键查看答案路径\n");
}
/*if ((i = -1 && j == -1) || (i == -1 && j == MX) || (i == MY && j == -1) || (i == MY) && (j == MX))
				printf(" ");
			else if (i == -1 || i == MY)
				printf("|");
			else if (j == -1 || j == MX)
				printf("-");
			else*/



void Check() {//玩家胜利判定
	if (px == MX - 1 && py == MY - 1) {
		ShowMap();
		printf("\n恭喜你走出迷宫！\n按任意键再来一轮\n");
		system("pause");
		ReInitialize();
	}
}



void Answer() {
	for (ax = 0, ay = 0; ax != MX - 1 || ay != MY - 1;) {//从左上角一直找到右下角
		MazeMap[ax][ay] = ANSWER;
		if (count(ax, ay) == 0) {
			MazeMap[ax][ay] = ANSWERED;//标明死路
			if (ax + 1 <= MX - 1 && MazeMap[ax + 1][ay] == ANSWER)
				ax++;
			else if (ay + 1 <= MY - 1 && MazeMap[ax][ay + 1] == ANSWER)
				ay++;
			else if (ax - 1 >= 0 && MazeMap[ax - 1][ay] == ANSWER)
				ax--;
			else if (ay - 1 >= 0 && MazeMap[ax][ay - 1] == ANSWER)
				ay--;
		
		}
		else {//检测块周围还有通路时
			if (ax + 1 <= MX - 1 && MazeMap[ax + 1][ay] == PATH)
				ax++;
			else if (ay + 1 <= MY - 1 && MazeMap[ax][ay + 1] == PATH)
				ay++;
			else if (ax - 1 >= 0 && MazeMap[ax - 1][ay] == PATH)
				ax--;
			else if (ay - 1 >= 0 && MazeMap[ax][ay - 1] == PATH)
				ay--;
		}
	}
	MazeMap[ax][ay] = ANSWER;
	ShowMap();
	printf("正确路径如上面%c号所示\n",ANSWER);
	system("pause");
	for (int i = 0; i <= MY - 1; i++) {				//还原地图
		for (int j = 0; j <= MX - 1; j++) {
			if (MazeMap[j][i] == ANSWER || MazeMap[j][i] == ANSWERED)
				MazeMap[j][i] = PATH;
		}
	}
}