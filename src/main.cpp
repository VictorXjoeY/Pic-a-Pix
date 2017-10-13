#include <cstdio>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cassert>
#include "board.hpp"

using namespace std;

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

			for (j = 0; j < k; j++){
				assert(scanf("%d", &b.row[i][j]) == 1);
			}
		}
		else{
			b.row[i].resize(1);
			b.row[i][0] = 0;
		}
	}

	// Lendo as restrições-coluna.
	for (i = 1; i <= m; i++){
		assert(scanf("%d", &k) == 1);

		if (k){
			b.col[i].resize(k);

			for (j = 0; j < k; j++){
				assert(scanf("%d", &b.col[i][j]) == 1);
			}
		}
		else{
			b.col[i].resize(1);
			b.col[i][0] = 0;
		}
	}

	return b;
}

bool blind_search(Board &b, int &paint_count){
	int x, y;

	// Buscando a primeira casa não preenchida.
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

	// Se todas as casas já estiverem preenchidas.
	if (x > b.n){
		return true;
	}

	// Tentando pintar a casa de preto.
	b.mat[x][y] = BLACK;
	paint_count++;

	if (b.valid(x, y) and blind_search(b, paint_count)){
		return true;
	}

	// Tentando pintar a casa de branco.
	b.mat[x][y] = WHITE;
	paint_count++;

	if (b.valid(x, y) and blind_search(b, paint_count)){
		return true;
	}

	// Não achou a solução, backtracking.
	b.mat[x][y] = NONE;

	return false;
}

bool smart_search_new(Board &b, int &paint_count){
	Cell best, cur_black_row, cur_black_col, cur_white_row, cur_white_col;
	int x, y;

	best = Cell(0, 0, NONE, 0, 1);

	// Buscando a casa vazia com a maior possibilidade de ser preta ou branca.
	for (x = 1; x <= b.n; x++){
		for (y = 1; y <= b.m; y++){
			if (b.mat[x][y] == NONE){
				// Forward checking. Se houver alguma casa que não possui nenhuma possibilidade de pintura.
				if ((b.row_black[x][y] == 0 or b.col_black[x][y] == 0) and (b.row_white[x][y] == 0 or b.col_white[x][y] == 0)){
					return false;
				}

				cur_black_row = Cell(x, y, BLACK, b.row_black[x][y], b.row_combinations[x]);
				cur_black_col = Cell(x, y, BLACK, b.col_black[x][y], b.col_combinations[y]);
				cur_white_row = Cell(x, y, WHITE, b.row_white[x][y], b.row_combinations[x]);
				cur_white_col = Cell(x, y, WHITE, b.col_white[x][y], b.col_combinations[y]);

				if (best.color == NONE){
					best = max(max(cur_black_row, cur_black_col), max(cur_white_row, cur_white_col));
				}
				else{
					best = max(best, max(max(cur_black_row, cur_black_col), max(cur_white_row, cur_white_col)));
				}
			}
		}
	}

	// Se todas as casas já estiverem preenchidas.
	if (best.color == NONE){
		return true;
	}

	// Coordenada da melhor casa a ser preenchida.
	x = best.x;
	y = best.y;

	// Preenchendo com a melhor cor.
	b.mat[x][y] = best.color;
	paint_count++;

	if (b.valid(x, y) and smart_search_new(b, paint_count)){
		return true;
	}

	// Melhor cor não funcionou, tentando a outra.
	b.mat[x][y] = b.toggle(best.color);
	paint_count++;

	if (b.valid(x, y) and smart_search_new(b, paint_count)){
		return true;
	}

	// Não achou a solução, backtracking.
	b.mat[x][y] = NONE;
	b.valid(x, y);

	return false;
}

int main(int argc, char *argv[]){
	int paint_count;
	Board b;

	paint_count = 0;

	// Lendo o tabuleiro.
	b = read_board();

	printf("Bruteforcing\n");

	b.solve();

	printf("Bruteforced\n");

	// b.print_possibilities();

	// Busca cega.
	// blind_search(b, paint_count);

	// Busca heurística.
	smart_search_new(b, paint_count);

	// Imprimindo resultado.
	b.print();

	printf("Paint count = %d\n", paint_count);

	return 0;
}