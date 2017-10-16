all:
	@g++ -o main src/*.cpp -I include -O2 -Wall
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