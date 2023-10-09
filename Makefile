build:
	touch turing.in
	gcc -g -o turing main.c turing_functions.c
run:
	./turing
clean:
	rm turing
valgrind:
	valgrind --leak-check=yes --track-origins=yes ./turing
