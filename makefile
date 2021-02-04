all:
	g++ figure_libre.cpp fonctions.cpp -o main `pkg-config --cflags --libs opencv` -lsfml-audio
clean:
	rm -rf main *.o