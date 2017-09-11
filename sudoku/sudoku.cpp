// Sudoku.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;


int Sudoku[9][9] = { 0 };													//用二维数组存数独
int Sudoku_f[9][9];
int flag = 0;																//作为回溯的标志
int S[9];															

int check_row(int x, int y) {												//行检查
	int j = 0;
	while (j < 9) {
		if (Sudoku[x][y] == Sudoku[x][j] && j != y) return 1;
		j++;
	}
	return 0;
}

int check_line(int x, int y) {												//列检查
	int i = 0;
	while (i < 9) {
		if (Sudoku[x][y] == Sudoku[i][y] && i != x) return 1;
		i++;
	}
	return 0;
}

void Sdk() {																//当只剩下数字9没填时，直接把剩下的0变成9
	int i, j;
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			if (Sudoku[i][j] == 0) {
				Sudoku[i][j] = 9;
			}
		}
	}
}

void creat_Sudoku(int num, int number) {									//建立数独、这里的num和number分别是要填的数和所填的块的编号
	int x, y, l_flag, r_flag;											//x和y存随机的位置,l_flag和r_flag是判断当前的位置是否符合数独规则
	int	t = 0;																//记录重新随机的次数
	int last1 = -1, last2 = -1;												//上次的x和y的值用last1和last2来保存
	if (num == 9) {															//只剩9没填时
		Sdk();
	}
	else {
		while (true) {
			x = rand() % 3 + 3 * (number / 3);								//跟据块的编号，随机生成位置(x,y)
			y = rand() % 3 + 3 * (number % 3);
			if (Sudoku[x][y] != 0) {										//当生成的位置有数时,重新随机,重新随机次数过多时,回溯
				t++;
				if (t == 8) {
					flag = 2;
					t = 0;
					return;
				}
				continue;
			}
			if (Sudoku[x][y] == 0) {
				if (last1 == x&&last2 == y) {								//当生成的位置没数,但一直是这个位置,回溯
					t++;
					if (t == 8) {
						flag = 2;
						t = 0;
						return;
					}
					continue;
				}
				last1 = x;
				last2 = y;
				Sudoku[x][y] = num;
				r_flag = check_row(x, y);
				l_flag = check_line(x, y);
				if (r_flag == 1 || l_flag == 1) {
					Sudoku[x][y] = 0;
					t++;
					if (t < 8)		continue;
					else {
						t = 0;
						flag = 2;
						return;
					}
				}
				if (r_flag == 0 && l_flag == 0) {
					if (number != 8) {
						creat_Sudoku(num, number + 1);											//填入的这个数满足数独条件，到下一个数
						if (flag == 2) {
							Sudoku[x][y] = 0;
							flag = 0;
							continue;
						}
						else break;
					}
					else {
						creat_Sudoku(num + 1, 0);											//填入的这个数满足数独条件，到下一个数
						if (flag == 2) {
							Sudoku[x][y] = 0;
							flag = 0;
							continue;
						}
						else break;
					}
				}
			}
		}
	}
}

void creat_random() {														//产生一组随机数
	int i, j;
	S[5] = Sudoku[0][0];
	for (i = 1; i < 10; i++) {
		if (i == 5)continue;
		S[i] = rand() % 9 + 1;
		if (S[i] == Sudoku[0][0]) {
			i--; continue;
		}
		for (j = 1; j < i; j++) {
			if (S[i] == S[j]) {
				i--;  break;
			}
		}
	}
}

void change_Sudoku() {														//通过一组随机数去转换数独保证满足条件
	int i, j, k;
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			for (k = 1; k < 10; k++) {
				if (S[k] == Sudoku[i][j]) {
					Sudoku_f[i][j] = k;
					break;
				}
			}
		}
	}
}

int main(int agrc, char* agrv[])
{
	ofstream outfile;
	int i, j, k;												
	int num;																	//记录所需要产生数独的数目
	if (agrc != 3) {
		printf("Usage: sudoku.exe -c [N:a number]\n");
		return 0;
	}
	if (agrc == 3){
		num=atoi(agrv[2]);
		if (num == 0) {
			printf("Please enter a number greater than 1!\n");
			return 0;
		}
		else {
			srand((int)time(0));
			outfile.open("../BIN/sudoku.txt");
			for (i = 0; i < num; i++) {
				creat_Sudoku(1, 0);
				creat_random();
				change_Sudoku();
				for (j = 0; j < 9; j++) {										//输出数独
					for (k = 0; k < 9; k++) {
						outfile << Sudoku_f[j][k] << " ";
					}
					outfile << endl;
				}
				outfile << endl;
			}
		}
	}
	outfile.close();
	return 0;
}