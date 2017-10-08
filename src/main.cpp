#include "board.h"
#include "rowheuristic.h"
#include "pixelheuristic.h"
#include <vector>
#include <chrono>


using namespace std;

/*
Sobre puzzles:
O puzzle 5.in nao eh resolvido em tempo razoavel por nenhuma das heuristicas
O puzzle 7.in, excepcionalmente, nao eh resolvido em tempo razoavel pela heuristica de linha/coluna (e eh resolvido pela outra heuristica ou pelo backtracking)
(tempo = 467.95s)
*/


int main(int argc, char *argv[]){
	Board b;
	int count;

	b = read_board();

	if(argc!=2 || (argc==2 && (argv[1][0]<'1' || argv[1][0]>'3' ))){
		printf("Erro na entrada!\nUso do programa: ./main op <arquivoDeEntrada\nop: heuristica usada\n\t1 - busca nao informada\n\t2 - heuristica de linha/coluna\n\t3 - heuristica de pixel\n");
		return 0;
	}

	
	auto start = std::chrono::system_clock::now();
	

	if(argc>1){
		if(argv[1][0]=='1'){
			solve(b, count); //solucao sem heuristica
		}
		else
			if(argv[1][0]=='2'){ //solucao heuristica de linha/coluna
				solveByRow(b, count);
			}
			else
				if(argv[1][0]=='3'){ //solucao heuristica pixel a pixel
					solveByPixel(b, count);
				}
	}

    auto end = std::chrono::system_clock::now();
 
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
 
    std::cout << "finished computation at " << std::ctime(&end_time)
              << "elapsed time: " << elapsed_seconds.count() << "s\n";

    printf("Count %d\n", count);
	print_board(b);	

	return 0;
}