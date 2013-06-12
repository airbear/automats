all:
	gcc range.c -o range -lpthread
run:
	./range
grind:
	gcc range.c -o range -l pthread
	valgrind ./range
optimized:
	gcc -O3 range.c -o range -lpthread
