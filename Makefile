all:
	@g++ -o main src/*.cpp -I include -g -Wall -std=c++11
run:
	@./main
fullrun:
	@valgrind -v --leak-check=full --show-leak-kinds=all --track-origins=yes ./main