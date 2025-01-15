all: debug run

debug:
	clang --debug game.c -o game.out

compile:
	clang game.c -o game.out

run:
	./game.out

clean:
	rm -f ./game.out
