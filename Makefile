all: debug run

debug:
	clang --debug game.c

compile:
	clang game.c

run:
	./a.out
