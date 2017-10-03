all:
	@g++ -o main src/*.cpp -I include -g -Wall
run:
	@./main
fullrun:
	@valgrind -v --leak-check=full --show-leak-kinds=all --track-origins=yes ./main