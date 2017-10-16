#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cassert>
#include "board.hpp"

using namespace std;

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

/* Busca cega. */
bool blind_search(Board &b, int &paint_count, bool solution){
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
		b.print();
		return solution;
	}

	// Tentando pintar a casa de preto.
	b.mat[x][y] = BLACK;
	b.simple_update(x, y);
	paint_count++;

	if (b.valid(x, y) and blind_search(b, paint_count, solution)){
		return true;
	}

	// Tentando pintar a casa de branco.
	b.mat[x][y] = WHITE;
	b.simple_update(x, y);
	paint_count++;

	if (b.valid(x, y) and blind_search(b, paint_count, solution)){
		return true;
	}

	// Não achou a solução, backtracking.
	b.mat[x][y] = NONE;

	return false;
}

void smart_switch(long double p, int x, int y, int color, long double &best_p, int &best_x, int &best_y, int &best_color){
	// Trocando a casa caso a probabilidade seja maior.
	if (p > best_p){
		best_p = p;
		best_x = x;
		best_y = y;
		best_color = color;
	}
}

/* Busca informada. */
bool smart_search(Board &b, int &paint_count, bool solution){
	int x, y, color, best_x, best_y, best_color;
	long double best_p;

	best_p = -1.0;
	best_x = best_y = -1;
	best_color = NONE;

	// Buscando a casa vazia com a maior possibilidade de ser preta ou branca.
	for (x = 1; x <= b.n; x++){
		for (y = 1; y <= b.m; y++){
			if (b.mat[x][y] == NONE){
				// Forward checking. Se houver alguma casa que não possui nenhuma possibilidade de pintura.
				if ((b.row_black[x][y] == 0 or b.col_black[x][y] == 0) and (b.row_white[x][y] == 0 or b.col_white[x][y] == 0)){
					return false;
				}

				// Buscando uma casa com maior probabilidade.
				smart_switch((long double)b.row_black[x][y] / (long double)b.row_combinations[x], x, y, BLACK, best_p, best_x, best_y, best_color);
				smart_switch((long double)b.col_black[x][y] / (long double)b.col_combinations[y], x, y, BLACK, best_p, best_x, best_y, best_color);
				smart_switch((long double)b.row_white[x][y] / (long double)b.row_combinations[x], x, y, WHITE, best_p, best_x, best_y, best_color);
				smart_switch((long double)b.col_white[x][y] / (long double)b.col_combinations[y], x, y, WHITE, best_p, best_x, best_y, best_color);
			}
		}
	}

	// Se todas as casas já estiverem preenchidas.
	if (best_color == NONE){
		b.print();
		return solution;
	}

	// Coordenada da melhor casa a ser preenchida.
	x = best_x;
	y = best_y;
	color = best_color;

	// Preenchendo com a melhor cor.
	b.mat[x][y] = color;
	b.full_update(x, y);
	paint_count++;

	if (b.valid(x, y) and smart_search(b, paint_count, solution)){
		return true;
	}

	// Melhor cor não funcionou, tentando a outra.
	b.mat[x][y] = b.toggle(color);
	b.full_update(x, y);
	paint_count++;

	if (b.valid(x, y) and smart_search(b, paint_count, solution)){
		return true;
	}

	// Não achou a solução, backtracking.
	b.mat[x][y] = NONE;
	b.full_update(x, y);

	return false;
}

int main(int argc, char *argv[]){
	int paint_count;
	Board b;

	if (argc == 1){
		printf("Please use make blind to run the Blind Search or make smart to run the Smart Search.\n");
		return 0;
	}

	paint_count = 0;

	// Lendo o tabuleiro.
	b = read_board();
	
	if (!strcmp(argv[1], "blind")){
		// Busca cega.
		blind_search(b, paint_count, argc != 3 or strcmp(argv[2], "all"));
	}
	else if (!strcmp(argv[1], "smart")){
		// Busca heurística.
		b.solve();
		smart_search(b, paint_count, argc != 3 or strcmp(argv[2], "all"));
	}
	else{
		printf("There's no such thing as %s search\n", argv[1]);
	}

	// Imprimindo resultado.
	printf("Paint count = %d\n", paint_count);

	return 0;
}