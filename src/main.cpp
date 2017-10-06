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

	b = read_board();
	//solveByRow(b);

	//solve(b); //solucao sem heuristica
	 auto start = std::chrono::system_clock::now();
 // solve(b);
   solveByPixel(b); //solucao heuristica pixel a pixel
	//solveByRow(b); //solucao heuristica de linha/coluna
    auto end = std::chrono::system_clock::now();
 
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
 
    std::cout << "finished computation at " << std::ctime(&end_time)
              << "elapsed time: " << elapsed_seconds.count() << "s\n";

	//solveByPixel(b);

	print_board(b);	

	return 0;
}