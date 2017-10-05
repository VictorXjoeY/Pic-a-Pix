#ifndef _BOARD_H_
#define _BOARD_H_

#include <cstdio>
#include <vector>
#include <iostream>

using namespace std;

#define NONE '.'
#define WHITE '_'
#define BLACK '#'
#define ROW 0
#define COLUMN 1


/*
Arquivo com as definicoes iniciais e resolucao por busca nao informada
*/

struct Board{
	vector<vector<int> > row;
	vector<vector<int> > col;
	vector<vector<int> > mat;
	int n, m;

	Board(){

	}

	Board(int n, int m){
		int i;

		this->n = n;
		this->m = m;

		this->row.resize(n + 1);
		this->col.resize(m + 1);
		this->mat.resize(n + 1);

		for (i = 0; i <= n; i++){
			mat[i].assign(m + 1, NONE);
		}
	}
};



void print_board(Board &b){
	int krow_max, kcol_max, i, j;

	for (i = 1, krow_max = 0; i <= b.n; i++){
		krow_max = max(krow_max, (int)b.row[i].size());
	}

	for (i = 1, kcol_max = 0; i <= b.m; i++){
		kcol_max = max(kcol_max, (int)b.col[i].size());
	}

	printf("%d %d\n", b.n, b.m);

	for (i = 0; i < kcol_max; i++){
		for (j = 0; j < krow_max; j++){
			printf("   ");
		}

		for (j = 1; j <= b.m; j++){
			if (kcol_max - i <= (int)b.col[j].size()){
				printf("%3d", b.col[j][(int)b.col[j].size() - (kcol_max - i)]);
			}
			else{
				printf("   ");
			}
		}

		printf("\n");
	}

	for (i = 1; i <= b.n; i++){
		for (j = 0; j < krow_max; j++){
			if (krow_max - j <= (int)b.row[i].size()){
				printf("%3d", b.row[i][(int)b.row[i].size() - (krow_max - j)]);
			}
			else{
				printf("   ");
			}
		}

		for (j = 1; j <= b.m; j++){
			printf("%3c", b.mat[i][j]);
		}

		printf("\n");
	}
}

Board read_board(){
	int n, m, k, i, j;
	Board b;

	scanf("%d%d", &n, &m);

	b = Board(n, m);

	for (i = 1; i <= n; i++){
		scanf("%d", &k);

		b.row[i].resize(k);

		for (j = 0; j < k; j++){
			scanf("%d", &b.row[i][j]);
		}
	}

	for (i = 1; i <= m; i++){
		scanf("%d", &k);

		b.col[i].resize(k);

		for (j = 0; j < k; j++){
			scanf("%d", &b.col[i][j]);
		}
	}

	return b;
}

int solve_row_aux(vector<vector<int> > &dp, Board &b, int x, int y, int p){
	int i;

	if (y > b.m){
		return p == (int)b.row[x].size();
	}
	if(p>= (int)b.row[x].size()){
		for(i=y; i<=b.m; i++)
			if(b.mat[x][i]==BLACK)
				return false;
		return true;
	}


	if (dp[y][p] != -1){
		return dp[y][p];
	}

	if (b.mat[x][y] == BLACK){
		for(int i=y; i<y+b.row[x][p] && i<=b.m; i++)
			if(b.mat[x][i]==WHITE)
				return false;
		if (y + b.row[x][p] > b.m+1)
			return false;
		if (y + b.row[x][p] == b.m+1){
			return dp[y][p] = solve_row_aux(dp, b, x, y + b.row[x][p] + 1, p + 1);
		}

		if (b.mat[x][y + b.row[x][p]] != BLACK){
			return dp[y][p] = solve_row_aux(dp, b, x, y + b.row[x][p] + 1, p + 1);
		}else
			return false;

	}

	if (b.mat[x][y] == WHITE){
		return dp[y][p] = solve_row_aux(dp, b, x, y + 1, p);
	}

	if (p < (int)b.row[x].size() and y + b.row[x][p] - 1 <= b.m){
		for(int i=y; i<y+b.row[x][p] && i<=b.m; i++)
			if(b.mat[x][i]==WHITE) //pixel so pode ser branco
				return dp[y][p] = solve_row_aux(dp, b, x, y + 1, p);
	
		if (y + b.row[x][p] == b.m+1){ //branco ou preto
			return dp[y][p] = solve_row_aux(dp, b, x, y + b.row[x][p] + 1, p + 1) + solve_row_aux(dp, b, x, y + 1, p);
		}

		if (b.mat[x][y + b.row[x][p]] != BLACK){ //branco ou preto
			return dp[y][p] = solve_row_aux(dp, b, x, y + b.row[x][p] + 1, p + 1) + solve_row_aux(dp, b, x, y + 1, p);
		}else //branco
			return dp[y][p] = solve_row_aux(dp, b, x, y + 1, p);
	}

	return dp[y][p] = solve_row_aux(dp, b, x, y + 1, p);
}

int solve_row(Board &b, int x){
	vector<vector<int> > dp;
	int i;

	dp.resize(b.m + 1);

	for (i = 0; i <= b.m; i++){
		dp[i].assign(b.row[x].size() + 1, -1);
	}

	return solve_row_aux(dp, b, x, 1, 0);
}

int solve_col_aux(vector<vector<int> > &dp, Board &b, int y, int x, int p){
	int i;

	if (x > b.n){
		return p == (int)b.col[y].size();
	}

	//se condicoes ja foram preenchidas, verifica se nao ha mais pixels pretos a frente
	if(p>= (int)b.col[y].size()){
		for(i=x; i<=b.n; i++)
			if(b.mat[i][y]==BLACK)
				return false;
		
		return true;
	}
	
	if (dp[x][p] != -1){
		return dp[x][p];
	}


	if (b.mat[x][y] == BLACK){
		for(int i=x; i<x+b.col[y][p] && i<=b.n; i++)
			if(b.mat[i][y]==WHITE)
				return false;

		if(x + b.col[y][p] > b.n+1)
			return false;	

		if (x + b.col[y][p] == b.n+1){
			return dp[x][p] = solve_col_aux(dp, b, y, x + b.col[y][p] + 1, p + 1);
		}

		if (b.mat[x + b.col[y][p]][y] != BLACK){
			return dp[x][p] = solve_col_aux(dp, b, y, x + b.col[y][p] + 1, p + 1);
		}else
			return false;

	}

	if (b.mat[x][y] == WHITE){
		return dp[x][p] = solve_col_aux(dp, b, y, x + 1, p);
	}

	if (p < (int)b.col[y].size() and x + b.col[y][p] - 1 <= b.n){
		//se ha uma casa branca impedindo a sequencia de pixels pretos
		for(int i=x; i<x+b.col[y][p] && i<=b.n; i++)
			if(b.mat[i][y]==WHITE)
				return dp[x][p] = solve_col_aux(dp, b, y, x + 1, p); //casa pode ser apenas branca

		//sequencia preta acabaria na ultima casa
		if (x + b.col[y][p] == b.n+1){
			//preta ou branca
			return dp[x][p] = solve_col_aux(dp, b, y, x + b.col[y][p] + 1, p + 1) + solve_col_aux(dp, b, y, x + 1, p);
		}

		//se casa apos possivel sequencia preta nao eh preta
		if (b.mat[x + b.col[y][p]][y] != BLACK){
			//preta ou branca
			return dp[x][p] = solve_col_aux(dp, b, y, x + b.col[y][p] + 1, p + 1) + solve_col_aux(dp, b, y, x + 1, p);
		}else //branca
			return dp[x][p] = solve_col_aux(dp, b, y, x + 1, p);
	}

	return dp[x][p] = solve_col_aux(dp, b, y, x + 1, p);
}

int solve_col(Board &b, int y){
	vector<vector<int> > dp;
	int i;

	dp.resize(b.n + 1);

	for (i = 0; i <= b.n; i++){
		dp[i].assign(b.col[y].size() + 1, -1);
	}

	return solve_col_aux(dp, b, y, 1, 0);
}

bool valid_row(Board &b, int x){
	return solve_row(b, x) > 0;
}

bool valid_col(Board &b, int y){
	return solve_col(b, y) > 0;
}

bool solve(Board &b){
	int i, j;

	for (i = 1; i <= b.n; i++){
		for (j = 1; j <= b.m; j++){
			if (b.mat[i][j] == NONE){
				break;
			}
		}

		if (j <= b.m){
			break;
		}
	}

	if (i > b.n){
		return true;
	}

	b.mat[i][j] = BLACK;
	if (valid_row(b, i) and valid_col(b, j) and solve(b)){
		return true;
	}

	b.mat[i][j] = WHITE;
	

	if (valid_row(b, i) and valid_col(b, j) and solve(b)){
		return true;
	}

	b.mat[i][j] = NONE;
	

	return false;
}

#endif