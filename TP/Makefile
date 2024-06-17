CC = gcc
CFLAGS = -Wall -std=c99 -pedantic -g -c
LFLAGS = -lm -lSDL2

all: hangon

main.o: main.c imagen.h config.h pixel.h fondo.h paleta.h rom.h figura.h ruta.h
	$(CC) $(CFLAGS) main.c 

paleta.o: paleta.c paleta.h 
	$(CC) $(CFLAGS) paleta.c

pixel.o: pixel.c pixel.h
	$(CC) $(CFLAGS) pixel.c

rom.o : rom.c rom.h imagen.h config.h fondo.h pixel.h paleta.h
	$(CC) $(CFLAGS) rom.c

imagen.o: imagen.c imagen.h 
	$(CC) $(CFLAGS) imagen.c 

moto.o: moto.c moto.h
	$(CC) $(CFLAGS) moto.c -lm

figura.o: figura.c figura.h
	$(CC) $(CFLAGS) figura.c -lm

fondo.o: fondo.c fondo.h
	$(CC) $(CFLAGS) fondo.c

ruta.o: ruta.c ruta.h
	$(CC) $(CFLAGS) ruta.c 

	
hangon: main.o imagen.o paleta.o fondo.o pixel.o rom.o figura.o moto.o ruta.o 
	$(CC) main.o imagen.o paleta.o fondo.o pixel.o rom.o figura.o moto.o ruta.o -o hangon $(LFLAGS)

clean:
	rm *.o
	rm hangon		