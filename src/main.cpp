#include <cstdio>
#include <vector>
#include <iostream>
#include <cassert>

using namespace std;

#define NONE 0
#define WHITE 1
#define BLACK 2

/* Preenche uma submatriz retangular com o valor k. */
void fill_matrix(vector<vector<long long> > &mat, int xi, int yi, int xf, int yf, long long k){
	int i, j;

	for (i = xi; i <= xf; i++){
		for (j = yi; j <= yf; j++){
			mat[i][j] = k;
		}
	}
}

/* Soma um valor k a todos os elementos de uma submatriz retangular. */
void update_matrix(vector<vector<long long> > &mat, int xi, int yi, int xf, int yf, long long k){
	int i, j;

	for (i = xi; i <= xf; i++){
		for (j = yi; j <= yf; j++){
			mat[i][j] += k;
		}
	}
}

class Board{
public:
	vector<vector<int> > row; // (Constant) Restrições das linhas.
	vector<vector<int> > row_sum; // (Constant) Soma acumulada das restrições das linhas.
	vector<vector<int> > col; // (Constant) Restrições das colunas.
	vector<vector<int> > col_sum; // (Constant) Soma acumulada das restrições das colunas.
	vector<vector<int> > mat; // Matriz N x M (1-based).
	vector<vector<int> > sum[3]; // Soma acumulada da matriz N x M (1-based).
	vector<vector<long long> > row_black; // Número de combinações da linha i nas quais a celula (i, j) é pintada de preto.
	vector<vector<long long> > row_white; // Número de combinações da linha i nas quais a celula (i, j) é pintada de branco.
	vector<vector<long long> > col_black; // Número de combinações da coluna j nas quais a celula (i, j) é pintada de preto.
	vector<vector<long long> > col_white; // Número de combinações da coluna j nas quais a celula (i, j) é pintada de branco.
	vector<long long> row_combinations; // Número de combinações da linha i.
	vector<long long> col_combinations; // Número de combinações da coluna j.
	int n, m; // (Constant) Dimensões do tabuleiro.

	Board(){
		this->n = this->m = 0;
	}

	Board(int n, int m){
		int i;

		// Dimensões.
		this->n = n;
		this->m = m;

		// Restrições.
		this->row.resize(n + 1);
		this->row_sum.resize(n + 1);
		this->col.resize(m + 1);
		this->col_sum.resize(m + 1);

		this->row_combinations.assign(n + 1, 0);
		this->col_combinations.assign(m + 1, 0);

		// Matriz principal.
		this->mat.resize(n + 1);

		// Matriz de soma acumulada.
		this->sum[NONE].resize(n + 1);
		this->sum[WHITE].resize(n + 1);
		this->sum[BLACK].resize(n + 1);

		// Número de combinações.
		this->row_black.resize(n + 1);
		this->row_white.resize(n + 1);
		this->col_black.resize(n + 1);
		this->col_white.resize(n + 1);

		for (i = 0; i <= n; i++){
			this->mat[i].assign(m + 1, NONE);

			this->sum[NONE][i].assign(m + 1, 0);
			this->sum[WHITE][i].assign(m + 1, 0);
			this->sum[BLACK][i].assign(m + 1, 0);

			this->row_black[i].assign(m + 1, 0);
			this->row_white[i].assign(m + 1, 0);
			this->col_black[i].assign(m + 1, 0);
			this->col_white[i].assign(m + 1, 0);
		}
	}

	static char symbol(int type){
		if (type == WHITE){
			return '_';
		}

		if (type == BLACK){
			return '#';
		}

		return '.';
	}

	static int toggle(int color){
		return color == WHITE ? BLACK : WHITE;
	}

	/* Função que imprime o tabuleiro. */
	void print(){
		int krow_max, kcol_max, i, j;

		// Obtendo o número máximo de restrições-linha.
		for (i = 1, krow_max = 0; i <= this->n; i++){
			krow_max = max(krow_max, (int)this->row[i].size());
		}

		// Obtendo o número máximo de restrições-coluna.
		for (i = 1, kcol_max = 0; i <= this->m; i++){
			kcol_max = max(kcol_max, (int)this->col[i].size());
		}

		// Imprimindo dimensões do tabuleiro.
		printf("%d %d\n", this->n, this->m);

		// Imprimindo as restrições-coluna.
		for (i = 0; i < kcol_max; i++){
			for (j = 0; j < krow_max; j++){
				printf("   ");
			}

			for (j = 1; j <= this->m; j++){
				if (kcol_max - i <= (int)this->col[j].size()){
					printf("%3d", this->col[j][(int)this->col[j].size() - (kcol_max - i)]);
				}
				else{
					printf("   ");
				}
			}

			printf("\n");
		}

		for (i = 1; i <= this->n; i++){
			// Imprimindo as restrições-linha.
			for (j = 0; j < krow_max; j++){
				if (krow_max - j <= (int)this->row[i].size()){
					printf("%3d", this->row[i][(int)this->row[i].size() - (krow_max - j)]);
				}
				else{
					printf("   ");
				}
			}

			// Imprimindo o tabuleiro.
			for (j = 1; j <= this->m; j++){
				printf("%3c", symbol(this->mat[i][j]));
			}

			printf("\n");
		}
	}

	/* Função que atualiza a matriz de soma acumulada. */
	void update_sum(){
		int i, j, k;

		for (k = NONE; k <= BLACK; k++){
			for (i = 1; i <= this->n; i++){
				for (j = 1; j <= this->m; j++){
					this->sum[k][i][j] = this->sum[k][i - 1][j] + this->sum[k][i][j - 1] - this->sum[k][i - 1][j - 1] + (this->mat[i][j] == k);
				}
			}
		}
	}

	/* Função que retorna quantas celulas estão preenchidas com k no intervalo [xi, xf] x [yi, yf]. */
	int query(int xi, int yi, int xf, int yf, int k){
		if (xi > xf or yi > yf){
			swap(xi, xf);
			swap(yi, yf);
		}

		return this->sum[k][xf][yf] - this->sum[k][xi - 1][yf] - this->sum[k][xf][yi - 1] + this->sum[k][xi - 1][yi - 1];
	}

	/* Função que retorna a soma dos elementos de row[x][pi..pf]. */
	int query_row(int x, int pi, int pf){
		return this->row_sum[x][pf] - this->row_sum[x][pi] + this->row[x][pi];
	}

	/* Função que retorna a soma dos elementos de col[y][pi..pf]. */
	int query_col(int y, int pi, int pf){
		return this->col_sum[y][pf] - this->col_sum[y][pi] + this->col[y][pi];
	}

	long long solve_row_aux(int x, int y, int p){
		long long ans, ret;
		int j, k;

		// Todas as restrições foram preenchidas.
		if (p == (int)row[x].size()){
			// Checando se os espaços restantes são não-pretos.
			if (y > m or query(x, y, x, m, BLACK) == 0){
				if (y <= m){
					update_matrix(row_white, x, y, x, m, 1);
				}

				return 1;
			}

			return 0;
		}

		// Preciso de pelo menos k casas para as restrições row[x][p..].
		k = query_row(x, p, (int)row[x].size() - 1) + (int)row[x].size() - p - 1;
		ans = 0;

		// Para cada posição inicial possível para essa restrição.
		for (j = y; j <= m - k + 1; j++){
			// Posso colocar entre [x][j .. j + row[x][p] - 1] se não houver peças brancas nesse intervalo e [x][j + row[x][p]] não for preta.
			if ((j - 1 < y or query(x, y, x, j - 1, BLACK) == 0) and query(x, j, x, j + row[x][p] - 1, WHITE) == 0 and (j + row[x][p] > m or query(x, j + row[x][p], x, j + row[x][p], BLACK) == 0)){
				// Chamada atual é responsável por [y .. j + row[x][p]].
				ret = solve_row_aux(x, j + row[x][p] + 1, p + 1);

				// Atualizando número de combinações.
				update_matrix(row_white, x, y, x, j - 1, ret); // [y .. j)
				update_matrix(row_black, x, j, x, j + row[x][p] - 1, ret); // [j .. j + row[x][p])

				if (j + row[x][p] <= m){ // j + row[x][p]
					update_matrix(row_white, x, j + row[x][p], x, j + row[x][p], ret);
				}

				// Número total de combinações.
				ans += ret;
			}
		}

		return ans;
	}

	long long solve_row(int x){
		// Inicializando número de combinações
		fill_matrix(row_black, x, 0, x, m, 0);
		fill_matrix(row_white, x, 0, x, m, 0);

		return row_combinations[x] = solve_row_aux(x, 1, 0);
	}

	long long solve_col_aux(int y, int x, int p){
		long long ans, ret;
		int i, k;

		// Todas as restrições foram preenchidas.
		if (p == (int)col[y].size()){
			// Checando se os espaços restantes são não-pretos.
			if (x > n or query(x, y, n, y, BLACK) == 0){
				if (x <= n){
					update_matrix(col_white, x, y, n, y, 1);
				}

				return 1;
			}

			return 0;
		}

		// Preciso de pelo menos k casas para as restrições col[y][p..].
		k = query_col(y, p, (int)col[y].size() - 1) + (int)col[y].size() - p - 1;
		ans = 0;

		// Para cada posição inicial possível para essa restrição.
		for (i = x; i <= n - k + 1; i++){
			// Posso colocar entre [i .. i + col[y][p] - 1][y] se não houver peças brancas nesse intervalo e [i + col[y][p]][y] não for preta.
			if ((i - 1 < x or query(x, y, i - 1, y, BLACK) == 0) and query(i, y, i + col[y][p] - 1, y, WHITE) == 0 and (i + col[y][p] > n or query(i + col[y][p], y, i + col[y][p], y, BLACK) == 0)){
				// Chamada atual é responsável por [x .. i + col[y][p]]
				ret = solve_col_aux(y, i + col[y][p] + 1, p + 1);

				// Atualizando número de combinações.
				update_matrix(col_white, x, y, i - 1, y, ret); // [x .. i)
				update_matrix(col_black, i, y, i + col[y][p] - 1, y, ret); // [i .. i + col[x][p])

				if (i + col[y][p] <= n){
					update_matrix(col_white, i + col[y][p], y, i + col[y][p], y, ret); // i + col[x][p]
				}

				// Número total de combinações.
				ans += ret;
			}
		}

		return ans;
	}

	long long solve_col(int y){
		// Inicializando número de combinações
		fill_matrix(col_black, 0, y, n, y, 0);
		fill_matrix(col_white, 0, y, n, y, 0);

		return col_combinations[y] = solve_col_aux(y, 1, 0);
	}

	void solve(){
		int x, y;

		update_sum();

		for (x = 1; x <= n; x++){
			solve_row(x);
		}

		for (y = 1; y <= m; y++){
			solve_col(y);
		}
	}

	bool valid_row(int x){
		return solve_row(x) > 0;
	}

	bool valid_col(int y){
		return solve_col(y) > 0;
	}

	bool valid(int x, int y){
		update_sum();
		return valid_row(x) and valid_col(y);
	}
};

struct Cell{
	long long num, den;
	int x, y, color;

	Cell(){

	}

	Cell(int x, int y, int color, long long num, long long den){
		this->x = x;
		this->y = y;
		this->color = color;
		this->num = num;
		this->den = den;
	}

	bool operator < (const Cell &b) const{
		return num * b.den < den * b.num;
	}

	bool operator > (const Cell &b) const{
		return b < *this;
	}
};

/* Função que lê um tabuleiro da stdin. */
Board read_board(){
	int n, m, k, i, j;
	Board b;

	assert(scanf("%d%d", &n, &m) == 2);

	// Criando um tabuleiro vazio N x M.
	b = Board(n, m);

	// Lendo as restrições-linha.
	for (i = 1; i <= n; i++){
		assert(scanf("%d", &k) == 1);

		if (k){
			b.row[i].resize(k);
			b.row_sum[i].resize(k);

			for (j = 0; j < k; j++){
				assert(scanf("%d", &b.row[i][j]) == 1);
			}

			// Soma acumulada das restrições-linha.
			b.row_sum[i][0] = b.row[i][0];

			for (j = 1; j < k; j++){
				b.row_sum[i][j] = b.row_sum[i][j - 1] + b.row[i][j];
			}
		}
	}

	// Lendo as restrições-coluna.
	for (i = 1; i <= m; i++){
		assert(scanf("%d", &k) == 1);

		if (k){
			b.col[i].resize(k);
			b.col_sum[i].resize(k);

			for (j = 0; j < k; j++){
				assert(scanf("%d", &b.col[i][j]) == 1);
			}

			// Soma acumulada das restrições-coluna.
			b.col_sum[i][0] = b.col[i][0];

			for (j = 1; j < k; j++){
				b.col_sum[i][j] = b.col_sum[i][j - 1] + b.col[i][j];
			}
		}
	}

	return b;
}

int count;

bool blind_search(Board &b){
	int x, y;

	for (x = 1; x <= b.n; x++){
		for (y = 1; y <= b.m; y++){
			if (b.mat[x][y] == NONE){
				break;
			}
		}

		if (y <= b.m){
			break;
		}
	}

	if (x > b.n){
		return true;
	}

	b.mat[x][y] = BLACK;
	count++;

	if (b.valid(x, y) and blind_search(b)){
		return true;
	}

	b.mat[x][y] = WHITE;
	count++;

	if (b.valid(x, y) and blind_search(b)){
		return true;
	}

	b.mat[x][y] = NONE;

	return false;
}

bool smart_search(Board &b){
	Cell best, cur_black_row, cur_black_col, cur_white_row, cur_white_col;
	int x, y;

	// b.solve();
	best = Cell(0, 0, NONE, 0, 1);

	for (x = 1; x <= b.n; x++){
		for (y = 1; y <= b.m; y++){
			if (b.mat[x][y] == NONE){
				// Forward checking.
				if (b.row_black[x][y] * b.col_black[x][y] + b.row_white[x][y] * b.col_white[x][y] == 0){
					return false;
				}

				cur_black_row = Cell(x, y, BLACK, b.row_black[x][y], b.row_combinations[x]);
				cur_black_col = Cell(x, y, BLACK, b.col_black[x][y], b.col_combinations[y]);
				cur_white_row = Cell(x, y, WHITE, b.row_white[x][y], b.row_combinations[x]);
				cur_white_col = Cell(x, y, WHITE, b.col_white[x][y], b.col_combinations[y]);

				best = max(best, max(max(cur_black_row, cur_black_col), max(cur_white_row, cur_white_col)));
			}
		}
	}

	if (best.color == NONE){
		return true;
	}

	x = best.x;
	y = best.y;

	b.mat[x][y] = best.color;
	count++;

	if (b.valid(x, y) and smart_search(b)){
		return true;
	}

	b.mat[x][y] = b.toggle(best.color);
	count++;

	if (b.valid(x, y) and smart_search(b)){
		return true;
	}

	b.mat[x][y] = NONE;

	return false;
}

/* Função auxiliar. */
void print_possibilities(Board &b){
	int i, j;

	b.solve();

	for (i = 1; i <= b.n; i++){
		printf("%3lld: ", b.row_combinations[i]);

		for (j = 1; j <= b.m; j++){
			printf("%4lld ", b.row_black[i][j]);
		}

		printf("\n");
	}

	printf("\n");
	printf("     ");

	for (j = 1; j <= b.m; j++){
		printf("%3lld: ", b.col_combinations[j]);
	}

	printf("\n");

	for (i = 1; i <= b.n; i++){
		printf("     ");

		for (j = 1; j <= b.m; j++){
			printf("%4lld ", b.col_black[i][j]);
		}

		printf("\n");
	}
}

int main(int argc, char *argv[]){
	Board b;

	// Lendo o tabuleiro.
	b = read_board();

	b.solve();

	print_possibilities(b);

	// Busca cega.
	blind_search(b);

	// Busca heurística.
	// smart_search(b);

	// Imprimindo resultado.
	b.print();
	printf("Count = %d\n", count);

	return 0;
}