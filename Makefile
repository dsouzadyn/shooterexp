game: main.o
	g++ -o game main.o -lraylib
game.o:
	g++ -c main.cc
clean:
	rm -f main.o game
