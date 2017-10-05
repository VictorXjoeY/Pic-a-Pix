#include "board.h"
#include "rowheuristic.h"
#include "pixelheuristic.h"
#include <vector>
#include <chrono>


using namespace std;

int main(int argc, char *argv[]){
	Board b;

	b = read_board();
	//solveByRow(b);

	//solve(b);
	 auto start = std::chrono::system_clock::now();
   solve(b);
   //solveByPixel(b);
	//solveByRow(b);
    auto end = std::chrono::system_clock::now();
 
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
 
    std::cout << "finished computation at " << std::ctime(&end_time)
              << "elapsed time: " << elapsed_seconds.count() << "s\n";

	//solveByPixel(b);

	print_board(b);	

	return 0;
}