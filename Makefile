all:
	@g++ -o main src/*.cpp -I include -O2 -Wall
run:
	@./main
blind:
	@./main blind
full_blind:
	@./main blind all
smart:
	@./main smart
full_smart:
	@./main smart all
test_blind:
	./main blind < cases/$(f).in
test_smart:
	./main smart < cases/$(f).in