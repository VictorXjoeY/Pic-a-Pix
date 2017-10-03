#include <cstdio>
#include <vector>
#include <iostream>

using namespace std;

#define NONE '.'
#define WHITE '_'
#define BLACK '#'

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
	if (y > b.m){
		return p == (int)b.row[x].size();
	}

	if (dp[y][p] != -1){
		return dp[y][p];
	}

	if (b.mat[x][y] == BLACK){
		if (y + b.row[x][p] > b.m or b.mat[x][y + b.row[x][p]] != BLACK){
			return dp[y][p] = solve_row_aux(dp, b, x, y + b.row[x][p] + 1, p + 1);
		}

		return 0;
	}

	if (b.mat[x][y] == WHITE){
		return dp[y][p] = solve_row_aux(dp, b, x, y + 1, p);
	}

	if (p < (int)b.row[x].size() and y + b.row[x][p] - 1 <= b.m){
		return dp[y][p] = solve_row_aux(dp, b, x, y + b.row[x][p] + 1, p + 1) + solve_row_aux(dp, b, x, y + 1, p);
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
	if (x > b.n){
		return p == (int)b.col[y].size();
	}

	if (dp[x][p] != -1){
		return dp[x][p];
	}

	if (b.mat[x][y] == BLACK){
		if (x + b.col[y][p] > b.n or b.mat[x + b.col[y][p]][y] != BLACK){
			return dp[x][p] = solve_col_aux(dp, b, y, x + b.col[y][p] + 1, p + 1);
		}

		return 0;
	}

	if (b.mat[x][y] == WHITE){
		return dp[x][p] = solve_col_aux(dp, b, y, x + 1, p);
	}

	if (p < (int)b.col[y].size() and y + b.col[y][p] - 1 <= b.m){
		return dp[x][p] = solve_col_aux(dp, b, y, x + b.col[y][p] + 1, p + 1) + solve_col_aux(dp, b, y, x + 1, p);
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

	print_board(b);

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

	if (i == 1 and j == 9){
		printf("aqui\n");
		print_board(b);
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

int main(int argc, char *argv[]){
	Board b;

	b = read_board();

	b.mat[1][1] = WHITE;
	b.mat[1][2] = WHITE;
	b.mat[1][3] = WHITE;
	b.mat[1][4] = BLACK;
	b.mat[1][5] = WHITE;
	b.mat[1][6] = WHITE;
	b.mat[1][7] = WHITE;
	b.mat[1][8] = BLACK;
	
	b.mat[1][9] = BLACK;
	printf("%d %d\n", solve_row(b, 1), solve_col(b, 9));

	b.mat[1][9] = WHITE;
	printf("%d %d\n", solve_row(b, 1), solve_col(b, 9));

	solve(b);

	print_board(b);

	return 0;
}