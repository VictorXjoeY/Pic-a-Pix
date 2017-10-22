all:
	@g++ -o main src/*.cpp -I include -O2 -g -Wall
run:
	@./main
blind:
	@./main blind
blind_all:
	@./main blind all
smart:
	@./main smart
smart_all:
	@./main smart all
fullrun:
	valgrind -v --leak-check=full --track-origins=yes --show-leak-kinds=all ./main smart