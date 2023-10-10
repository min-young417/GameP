#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

void start_display(void);
void gotoxy(int x, int y);
void display_map(int*** matrix, int* level);
void basic_map(int* level);
void move_arrow_key(char key, int *x1, int *y1, int x_b, int y_b);
void game_control(int*** matrix, int* level);
void draw_square(int w, int h);
int select_display(void);
int* level_setting(int level);
int*** make_map(int* level);
void game_clear(double pst);
void game_over(void);
void show_mine(int*** matrix, int* level);
void setColor(unsigned short text);
int calc_frequency(int octave, int inx);

int main(void)
{
	int difficulty;
	int* level;
	int*** matrix;
    clock_t start, end;
    double pst;
    srand(time(NULL));
	start_display();
	difficulty = select_display();
	level = level_setting(difficulty);	
	matrix = make_map(level);	
    start=clock();
	system("cls");
    game_control(matrix, level);
    end=clock();
	pst=(double)(end-start)/CLK_TCK;
	game_clear(pst);
	return 0;
}

void draw_square(int w, int h)
{
	unsigned char a=0xa6;
	unsigned char b[7];
	for(int i=1;i<7;i++) b[i]=0xa0+i;
	
	printf("%c%c", a, b[3]);
	for(int i=0;i<w*2;i++) printf("%c%c", a, b[1]);
	printf("%c%c", a, b[4]);
	printf("\n");
	for(int i=0;i<h;i++){
		printf("%c%c", a, b[2]);
		for(int j=0;j<w;j++) printf("  ");
		printf("%c%c", a, b[2]);
		printf("\n");
	}
	printf("%c%c", a, b[6]);
	for(int i=0;i<w*2;i++) printf("%c%c", a, b[1]);
	printf("%c%c", a, b[5]);
}

void start_display(void)
{
	system("cls");
	gotoxy(1, 1);
	draw_square(22, 15);
	gotoxy(17, 6);
	setColor(10);
	printf("--------------");
	gotoxy(20, 7);
	printf("지뢰찾기");
	gotoxy(17, 8);
	printf("--------------");
	setColor(15);
	gotoxy(10,13);
	printf("-> press any key to start <-\n");
	getch();
}

int select_display(void)
{	
	system("cls");
	gotoxy(1, 1);
	draw_square(22, 15);
	gotoxy(19, 5);
	printf("난이도 선택");
	gotoxy(17, 6);
	printf("--------------");
	gotoxy(20, 9);
	printf("1. 초급");
	gotoxy(20, 10);
	printf("2. 중급");
	gotoxy(20, 11);
	printf("3. 고급");
	
	int select;
	select=getch()-48;
	if(select!=1 && select!=2 && select!=3) 
		select = select_display();
	return select;
}

int* level_setting(int level)
{
	static int array[3];
	
	switch(level)
	{
		case 1 :
			array[0] = 9;
			array[1] = 9;
			array[2] = 10;
			break;
		case 2 :
			array[0] = 16;
			array[1] = 16;
			array[2] = 40;
			break;
		case 3 :
			array[0] = 30;
			array[1] = 16;
			array[2] = 99;
			break;
		default :
			break;	
	}	
	
	return array;
}

int*** make_map(int* level)
{
	int*** matrix = (int***)malloc((level[0]+1) * sizeof(int**));
    for (int i = 0; i <(level[0]+1); i++) {
        matrix[i] = (int**)malloc((level[1]+1) * sizeof(int*));
        for (int j = 0; j < (level[1]+1); j++) {
            matrix[i][j] = (int*)malloc(3 * sizeof(int));
        }
    }
    
    for (int i = 0; i < (level[0]+1); i++) {
        for (int j = 0; j < (level[1]+1); j++) {
            for (int k = 0; k < 3; k++) {
                matrix[i][j][k] = 0;
            }
        }
    }

	int x, y;
	for(int i=0;i<level[2];i++){
		x=rand()%(level[0]+1);
		y=rand()%(level[1]+1);
		if(x==0||y==0||matrix[x][y][0]==9)
			i-=1;
		else
			matrix[x][y][0]=9;
	}
	
	int count;    
    for(int x=1;x<level[0];x++){
    	for(int y=1;y<level[1];y++){
    		count = 0;
    		if(matrix[x-1][y-1][0] == 9) count++;
    		if(matrix[x-1][y][0] == 9) count++;
    		if(matrix[x-1][y+1][0] == 9) count++;
    		if(matrix[x][y-1][0] == 9) count++;
    		if(matrix[x][y+1][0] == 9) count++;
    		if(matrix[x+1][y-1][0] == 9) count++;
    		if(matrix[x+1][y][0] == 9) count++;
    		if(matrix[x+1][y+1][0] == 9) count++;
    		if(matrix[x][y][0] != 9) matrix[x][y][0] = count;		
		}
	}
	
	for(int x=1;x<level[0];x++){
    	count = 0;
    	if(matrix[x-1][level[1]-1][0] == 9) count++;
    	if(matrix[x-1][level[1]][0] == 9) count++;
    	if(matrix[x][level[1]-1][0] == 9) count++;
    	if(matrix[x+1][level[1]-1][0] == 9) count++;
    	if(matrix[x+1][level[1]][0] == 9) count++;
    	if(matrix[x][level[1]][0] != 9) matrix[x][level[1]][0] = count;		
	}
	
	for(int y=1;y<level[1];y++){
    	count = 0;
    	if(matrix[level[0]-1][y-1][0] == 9) count++;
    	if(matrix[level[0]-1][y][0] == 9) count++;
    	if(matrix[level[0]-1][y+1][0] == 9) count++;
    	if(matrix[level[0]][y-1][0] == 9) count++;
    	if(matrix[level[0]][y+1][0] == 9) count++;
    	if(matrix[level[0]][y][0] != 9) matrix[level[0]][y][0] = count;		
	}
	
	count=0;
	if(matrix[level[0]-1][level[1]][0] == 9) count++;
    if(matrix[level[0]-1][level[1]-1][0] == 9) count++;
    if(matrix[level[0]][level[1]-1][0] == 9) count++;
    if(matrix[level[0]][level[1]][0] != 9) matrix[level[0]][level[1]][0] = count;	
	
	return matrix;
}

void basic_map(int* level)
{
	int i, j;
	for(i=0;i<level[1];i++)
	{
		gotoxy(7, i+5);
		for(j=0;j<level[0];j++)
			printf("■ ");
			
	}
}

void gotoxy(int x, int y)
{
   COORD Pos = {x - 1, y - 1};
   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void display_map(int*** matrix, int* level)
{
	int i, j;
	basic_map(level);
	for(i=1;i<=level[0];i++)
		for(j=1;j<=level[1];j++)
			if (matrix[i][j][0]==0 && matrix[i][j][1]==1)
			{
				gotoxy(2*i+5, j+4);
				printf(". ");
			}
			else if (matrix[i][j][1]==1)
			{
				gotoxy(2*i+5, j+4);
				printf("%d ", matrix[i][j][0]);
			}
			else if (matrix[i][j][1]==2)
			{
				gotoxy(2*i+5, j+4);
				printf("▷ ");
			}
}

void move_arrow_key(char key, int *x1, int *y1, int x_b, int y_b)
{
	switch(key)
	{
	case 72:  
		*y1=*y1-1;
		if (*y1<1)	*y1=1; 
		break;
	case 75:  
		*x1=*x1-2;            
		if (*x1<1)	*x1=1; 
		break;
	case 77:  
		*x1=*x1+2; 
		if (*x1>x_b)  *x1=x_b; 
		break;
	case 80: 
		*y1=*y1+1;
		if (*y1>y_b)  *y1=y_b; 
		break;
	default:
		return;
	}
}

void game_control(int*** matrix, int* level)
{
	char key;
	int i, j, count, open;
	int x=1, y=1;
	int*** map = matrix;
	
	do
	{
		gotoxy(7, 5);
		display_map(map, level);
		
		gotoxy(x+6, y+4);
		printf("○");

		count=0;
		open=0;
		for(i=1;i<=level[0];i++)
			for(j=1;j<=level[1];j++)
			{
				if (map[i][j][1]==2) count=count+1;
				if (map[i][j][1]==1) open=open+1;	
			}
		gotoxy(7, 3);
   		printf("남은 지뢰의 개수 : %d", level[2]-count);
		
		gotoxy(level[0]*2+10, 5);
   		printf("칸 열어보기 : z");
   		gotoxy(level[0]*2+10, 6);
   		printf("깃발 설치하기 : x");
   		gotoxy(level[0]*2+10, 7);
   		printf("설치된 깃발 : ▷");
		
		key=getch();
		if(key == 'z'){
			if (map[(x+1)/2][y][0] == 9) {
				show_mine(map, level);
				sleep(3);
				game_over();	
			}
			map[(x+1)/2][y][1] = 1;	
		}
		else if(key == 'x')
			map[(x+1)/2][y][1] = 2;
		else{
			move_arrow_key(key, &x, &y, level[0]*2-1, level[1]);
			int freq = calc_frequency(5, 5);
			Beep(freq, 250);	
		}
		system("cls");	
	}while(level[0]*level[1]-level[2] > open);
}

void show_mine(int*** matrix, int* level)
{
	display_map(matrix, level);	
	for(int i=1;i<=level[0];i++)
		for(int j=1;j<=level[1];j++)
			if (matrix[i][j][0]==9)
			{
				gotoxy(2*i+5, j+4);
				printf("★");
			}
} 

void game_over(void)
{
	system("cls");
	system("color 04");
	gotoxy(1, 1);
	draw_square(22, 15);
	gotoxy(17, 6);
	printf("--------------");
	gotoxy(18, 7);
	printf("Game Over!!!");
	gotoxy(17, 8);
	printf("--------------");
	gotoxy(1,20);
	getchar();
	exit(0);
}

void game_clear(double pst)
{
	system("cls");
	system("color 01");
	gotoxy(1, 1);
	draw_square(22, 15);
	gotoxy(17, 6);
	printf("--------------");
	gotoxy(18, 7);
	printf("Game Clear!!!");
	gotoxy(17, 8);
	printf("--------------");
	gotoxy(15,13);
	printf("경과 시간 : %.1f 초", pst);
	gotoxy(1,20);
	getch();
	exit(0);
}

void setColor(unsigned short text) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text);
}

int calc_frequency(int octave, int inx)
{
	double do_scale=32.7032;
	double ratio=pow(2., 1/12.), temp;
	int i;
	temp=do_scale*pow(2, octave-1);
	for(i=0;i<inx;i++)
	{
		temp=(int)(temp+0.5);
		temp*=ratio;
	}
	return (int) temp;
}
