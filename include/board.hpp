#pragma once

#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

#define NONE 0
#define WHITE 1
#define BLACK 2

class Board{
public:
	vector<vector<int> > row; // (Constant) Restrições das linhas.
	vector<vector<int> > row_sum; // (Constant) Soma acumulada das restrições das linhas.
	vector<vector<int> > mat_row_sum[3]; // (Constant) Soma acumulada das linhas da matriz.
	vector<vector<int> > col; // (Constant) Restrições das colunas.
	vector<vector<int> > col_sum; // (Constant) Soma acumulada das restrições das colunas.
	vector<vector<int> > mat_col_sum[3]; // (Constant) Soma acumulada das colunas da matriz.
	vector<vector<int> > mat; // Matriz N x M (1-based).
	vector<vector<long long> > row_black; // Número de combinações da linha i nas quais a celula (i, j) é pintada de preto.
	vector<vector<long long> > row_white; // Número de combinações da linha i nas quais a celula (i, j) é pintada de branco.
	vector<vector<long long> > col_black; // Número de combinações da coluna j nas quais a celula (i, j) é pintada de preto.
	vector<vector<long long> > col_white; // Número de combinações da coluna j nas quais a celula (i, j) é pintada de branco.
	vector<vector<long long> > dp; // Tabela de memoização para Programação Dinâmica.
	vector<long long> row_combinations; // Número de combinações da linha i.
	vector<long long> col_combinations; // Número de combinações da coluna j.
	int n, m; // (Constant) Dimensões do tabuleiro.

	// O(1) Construtor vazio.
	Board(){
		this->n = this->m = 0;
	}

	// O(N * M) Construtor.
	Board(int n, int m){
		int i, k;

		// Dimensões.
		this->n = n;
		this->m = m;

		// Restrições.
		this->row.resize(n + 1);
		this->row_sum.resize(n + 1);
		this->col.resize(m + 1);
		this->col_sum.resize(m + 1);

		// Número de combinações de cada linha/coluna.
		this->row_combinations.assign(n + 1, 0);
		this->col_combinations.assign(m + 1, 0);

		// Matriz principal.
		this->mat.resize(n + 1);

		// Matrizes de soma acumulada
		for (k = 0; k < 3; k++){
			this->mat_row_sum[k].resize(n + 1);
			this->mat_col_sum[k].resize(n + 1);
		}

		// Número de combinações.
		this->row_black.resize(n + 1);
		this->row_white.resize(n + 1);
		this->col_black.resize(n + 1);
		this->col_white.resize(n + 1);

		// Alocando as colunas.
		for (i = 0; i <= n; i++){			
			// Tabuleiro vazio.
			this->mat[i].assign(m + 1, NONE);

			// Matrizes de soma acumulada vazias.
			for (k = 0; k < 3; k++){
				this->mat_row_sum[k][i].assign(m + 1, 0);
				this->mat_col_sum[k][i].assign(m + 1, 0);
			}

			// Número de combinações vazio.
			this->row_black[i].assign(m + 1, 0);
			this->row_white[i].assign(m + 1, 0);
			this->col_black[i].assign(m + 1, 0);
			this->col_white[i].assign(m + 1, 0);
		}

		// Tabela da DP.
		this->dp.resize(max(n, m) + 1);
	}

	/* O(1) Retorna o símbolo de impressão de cada cor. */
	static char symbol(int type){
		if (type == WHITE){
			return '.';
		}

		if (type == BLACK){
			return '#';
		}

		return '_';
	}

	/* O(1) Retorna a cor oposta. */
	static int toggle(int color){
		return color == WHITE ? BLACK : WHITE;
	}

	/* O(N * M) Função que imprime o tabuleiro. */
	void print(){
		int krow_max, kcol_max, i, j;

		// Obtendo o número máximo de restrições-linha.
		for (i = 1, krow_max = 0; i <= n; i++){
			krow_max = max(krow_max, (int)row[i].size());

			if (row[i].empty()){
				row[i].push_back(0);
			}
		}

		// Obtendo o número máximo de restrições-coluna.
		for (i = 1, kcol_max = 0; i <= m; i++){
			kcol_max = max(kcol_max, (int)col[i].size());

			if (col[i].empty()){
				col[i].push_back(0);
			}
		}

		// Imprimindo dimensões do tabuleiro.
		printf("%d %d\n", n, m);

		// Imprimindo as restrições-coluna.
		for (i = 0; i < kcol_max; i++){
			for (j = 0; j < krow_max; j++){
				printf("   ");
			}

			for (j = 1; j <= m; j++){
				if (kcol_max - i <= (int)col[j].size()){
					printf("%3d", col[j][(int)col[j].size() - (kcol_max - i)]);
				}
				else{
					printf("   ");
				}
			}

			printf("\n");
		}

		for (i = 1; i <= n; i++){
			// Imprimindo as restrições-linha.
			for (j = 0; j < krow_max; j++){
				if (krow_max - j <= (int)row[i].size()){
					printf("%3d", row[i][(int)row[i].size() - (krow_max - j)]);
				}
				else{
					printf("   ");
				}
			}

			// Imprimindo o tabuleiro.
			for (j = 1; j <= m; j++){
				printf("%3c", symbol(mat[i][j]));
			}

			printf("\n");
		}

		for (i = 1; i <= n; i++){
			if (row[i][0] == 0){
				row[i].clear();
			}
		}

		for (i = 1; i <= m; i++){
			if (col[i][0] == 0){
				col[i].clear();
			}
		}
	}

	/* O(N * M) Imprime as possibilidades por linha e coluna. */
	void print_possibilities(){
		int i, j, k;

		// Para cada cor
		for (k = WHITE; k <= BLACK; k++){
			printf("\n%s possibilities:\n", k == WHITE ? "White" : "Black");
			
			// Imprimindo as combinações por linha.
			printf("\tRow possibilities:\n");

			for (i = 1; i <= n; i++){
				printf("%5lld: ", row_combinations[i]);

				for (j = 1; j <= m; j++){
					printf("%6lld ", k == WHITE ? row_white[i][j] : row_black[i][j]);
				}

				printf("\n");
			}

			// Imprimindo as combinações por coluna.
			printf("\n\tColumn possibilities:\n");
			printf("     ");

			for (j = 1; j <= m; j++){
				printf("%5lld: ", col_combinations[j]);
			}

			printf("\n");

			for (i = 1; i <= n; i++){
				printf("     ");

				for (j = 1; j <= m; j++){
					printf("%6lld ", k == WHITE ? col_white[i][j] : col_black[i][j]);
				}

				printf("\n");
			}
		}
	}

	/* O(1) Retorna o respectivo caractere para impressão em PGM. */
	char pgm_char(int color){
		return color == BLACK ? '1' : '0';
	}

	/* O(N * M) Imprime o tabuleiro em formato PGM. */
	void print_pgm(){
		int scale, i, j;
		FILE *fp;

		// Fazendo com que a coordenada mínima seja pelo menos 1000.
		scale = (1000 + min(n, m) - 1) / min(n, m);

		fp = fopen("solution.pgm", "w");

		fprintf(fp, "P1\n");
		fprintf(fp, "%d %d\n", scale * m, scale * n);
		fprintf(fp, "1\n");

		for (i = 0; i < scale * n; i++){
			for (j = 0; j < scale * m - 1; j++){
				fprintf(fp, "%c ", pgm_char(mat[(i / scale) + 1][(j / scale) + 1]));
			}

			fprintf(fp, "%c\n", pgm_char(mat[(i / scale) + 1][(j / scale) + 1]));
		}

		fclose(fp);
	}

	/* O(Dimensão * Restrições * Restrição_Máxima). Inicializa a dp com -1. */
	void init_dp(int a, int b){
		int i;

		for (i = 0; i < a; i++){
			fill(dp[i].begin(), dp[i].begin() + b, -1);
		}
	}

	long long solve_row_aux(int x, int y, int p){
		int k;

		// Caso base.
		if (p == (int)row[x].size()){
			return count_query_row(x, y, m, BLACK) == 0;
		}

		// Quantidade mínima de casas para conseguir preencher tudo.
		k = query_row(x, p, row[x].size() - 1) + row[x].size() - p - 1;

		// Corte.
		if (m - y < k or mat[x][y] == BLACK){
			return 0;
		}

		if (dp[y][p] != -1){
			return dp[y][p];
		}

		// Se não tiver branco nas próximas casas posso escolher entre preencher a restrição ou não.
		if (count_query_row(x, y + 1, y + row[x][p], WHITE) == 0){
			return dp[y][p] = solve_row_aux(x, y + row[x][p] + 1, p + 1) + solve_row_aux(x, y + 1, p);
		}

		// Não preenche a restrição.
		return dp[y][p] = solve_row_aux(x, y + 1, p);
	}

	void special_row(int x){
		int y;

		if (count_query_row(x, 1, m, BLACK) == 0){
			row_combinations[x] = 1;

			for (y = 1; y <= m; y++){
				row_black[x][y] = 0;
				row_white[x][y] = 1;
			}
		}
		else{
			row_combinations[x] = 0;

			for (y = 1; y <= m; y++){
				row_black[x][y] = 0;
				row_white[x][y] = 0;
			}
		}
	}

	void full_solve_row(int x){
		int y;

		if (row[x].empty()){
			special_row(x);
			return;
		}

		// Calculando o número de combinações da linha.
		init_dp(m + 1, (int)row[x].size() + 1);
		row_combinations[x] = solve_row_aux(x, 1, 0) + (count_query_row(x, 1, row[x][0], WHITE) == 0 ? solve_row_aux(x, row[x][0] + 1, 1) : 0);

		// Se não houver nenhuma combinação da linha.
		if (row_combinations[x] == 0){
			return;
		}

		for (y = 1; y <= m; y++){
			if (mat[x][y] == BLACK){
				// Se já estiver pintado de preto.
				row_black[x][y] = row_combinations[x];
				row_white[x][y] = 0;
			}
			else if (mat[x][y] == WHITE){
				// Se já estiver pintado de branco.
				row_black[x][y] = 0;
				row_white[x][y] = row_combinations[x];
			}
			else{
				// Se ainda não estiver pintado devo calcular as possibilidades.
				mat[x][y] = BLACK;
				update_row(x);

				init_dp(m + 1, (int)row[x].size() + 1);
				row_black[x][y] = solve_row_aux(x, 1, 0) + (count_query_row(x, 1, row[x][0], WHITE) == 0 ? solve_row_aux(x, row[x][0] + 1, 1) : 0);
				row_white[x][y] = row_combinations[x] - row_black[x][y];

				mat[x][y] = NONE;
				update_row(x);
			}
		}
	}

	/* Função que calcula as possibilidades da linha x. */
	void simple_solve_row(int x){
		// Calculando o número de combinações da linha.
		init_dp(m + 1, (int)row[x].size() + 1);

		if (!row[x].empty() and count_query_row(x, 1, row[x][0], WHITE) == 0){
			row_combinations[x] = solve_row_aux(x, 1, 0) + solve_row_aux(x, row[x][0] + 1, 1);
		}
		else{
			row_combinations[x] = solve_row_aux(x, 1, 0);
		}
	}

	long long solve_col_aux(int y, int x, int p){
		int k;

		// Caso base.
		if (p == (int)col[y].size()){
			return count_query_col(y, x, n, BLACK) == 0;
		}

		// Quantidade mínima de casas para conseguir preencher tudo.
		k = query_col(y, p, col[y].size() - 1) + col[y].size() - p - 1;

		// Corte.
		if (n - x < k or mat[x][y] == BLACK){
			return 0;
		}

		if (dp[x][p] != -1){
			return dp[x][p];
		}

		// Se não tiver branco nas próximas casas posso escolher entre preencher a restrição ou não.
		if (count_query_col(y, x + 1, x + col[y][p], WHITE) == 0){
			return dp[x][p] = solve_col_aux(y, x + col[y][p] + 1, p + 1) + solve_col_aux(y, x + 1, p);
		}

		// Não preenche a restrição.
		return dp[x][p] = solve_col_aux(y, x + 1, p);
	}

	void special_col(int y){
		int x;

		if (count_query_col(y, 1, n, BLACK) == 0){
			col_combinations[y] = 1;

			for (x = 1; x <= n; x++){
				col_black[x][y] = 0;
				col_white[x][y] = 1;
			}
		}
		else{
			col_combinations[y] = 0;

			for (x = 1; x <= n; x++){
				col_black[x][y] = 0;
				col_white[x][y] = 0;
			}
		}
	}

	void full_solve_col(int y){
		int x;

		if (col[y].empty()){
			special_col(y);
			return;
		}

		// Calculando o número de combinações da linha.
		init_dp(n + 1, (int)col[y].size() + 1);
		col_combinations[y] = solve_col_aux(y, 1, 0) + (count_query_col(y, 1, col[y][0], WHITE) == 0 ? solve_col_aux(y, col[y][0] + 1, 1) : 0);

		// Se a coluna não possuir combinações
		if (col_combinations[y] == 0){
			return;
		}

		for (x = 1; x <= n; x++){
			if (mat[x][y] == BLACK){
				// Se já estiver pintado de preto.
				col_black[x][y] = col_combinations[y];
				col_white[x][y] = 0;
			}
			else if (mat[x][y] == WHITE){
				// Se já estiver pintado de branco.
				col_black[x][y] = 0;
				col_white[x][y] = col_combinations[y];
			}
			else{
				// Se ainda não estiver pintado devo calcular as possibilidades.
				mat[x][y] = BLACK;
				update_col(y);

				init_dp(n + 1, (int)col[y].size() + 1);
				col_black[x][y] = solve_col_aux(y, 1, 0) + (count_query_col(y, 1, col[y][0], WHITE) == 0 ? solve_col_aux(y, col[y][0] + 1, 1) : 0);
				col_white[x][y] = col_combinations[y] - col_black[x][y];

				mat[x][y] = NONE;
				update_col(y);
			}
		}
	}

	/* Função que calcula as possibilidades da linha x. */
	void simple_solve_col(int y){
		// Calculando o número de combinações da linha.
		init_dp(n + 1, (int)col[y].size() + 1);

		if (!col[y].empty() and count_query_col(y, 1, col[y][0], WHITE) == 0){
			col_combinations[y] = solve_col_aux(y, 1, 0) + solve_col_aux(y, col[y][0] + 1, 1);	
		}
		else{
			col_combinations[y] = solve_col_aux(y, 1, 0);	
		}
	}

	/* Função que retorna o número de casas iguais a k na linha x para y = [yi..yf]. */
	int count_query_row(int x, int yi, int yf, int k){
		return mat_row_sum[k][x][yf] - mat_row_sum[k][x][yi - 1];
	}

	/* Função que retorna o número de casas iguais a k na coluna y para x = [xi..xf]. */
	int count_query_col(int y, int xi, int xf, int k){
		return mat_col_sum[k][xf][y] - mat_col_sum[k][xi - 1][y];
	}

	/* O(1) Função que retorna a soma das restrições da linha x de l a r. */
	int query_row(int x, int l, int r){
		return row_sum[x][r] - row_sum[x][l] + row[x][l];
	}

	/* O(1) Função que retorna a soma das restrições da linha x de l a r. */
	int query_col(int y, int l, int r){
		return col_sum[y][r] - col_sum[y][l] + col[y][l];
	}

	/* O(M) Função que atualiza a soma acumulada da linha x. */
	void update_row(int x){
		int y, k;

		for (k = 0; k < 3; k++){
			for (y = 1; y <= m; y++){
				mat_row_sum[k][x][y] = mat_row_sum[k][x][y - 1] + (mat[x][y] == k);
			}
		}
	}

	/* O(N) Função que atualiza a soma acumulada da coluna y. */
	void update_col(int y){
		int x, k;

		for (k = 0; k < 3; k++){
			for (x = 1; x <= n; x++){
				mat_col_sum[k][x][y] = mat_col_sum[k][x - 1][y] + (mat[x][y] == k);
			}
		}
	}

	/* Função que calcula todas as possibilidades do tabuleiro inteiro. */
	void solve(){
		int x, y;

		// Calculando as possibilidades de todas as linhas.
		for (x = 1; x <= n; x++){
			update_row(x);
			full_solve_row(x);
		}

		// Calculando as possibilidades de todas as colunas.
		for (y = 1; y <= m; y++){
			update_col(y);
			full_solve_col(y);
		}
	}

	/* Função que verifica se a linha x está válida (possui alguma combinação possível). */
	bool valid_row(int x){
		return row_combinations[x] > 0;
	}

	/* Função que verifica se a coluna y está válida (possui alguma combinação possível). */
	bool valid_col(int y){
		return col_combinations[y] > 0;
	}

	/* Função que verifica se a linha x e a coluna y estão válidas. */
	bool valid(int x, int y){
		return valid_row(x) and valid_col(y);
	}

	/* Função que atualiza as combinações da linha x e da coluna y. */
	void simple_update(int x, int y){
		update_row(x);
		update_col(y);
		simple_solve_row(x);
		simple_solve_col(y);
	}

	/* Função que atualiza as combinações da linha x e da coluna y e de seus pixels. */
	void full_update(int x, int y){
		update_row(x);
		update_col(y);
		full_solve_row(x);
		full_solve_col(y);
	}
};