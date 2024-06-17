#ifndef MOTO_H
#define MOTO_H

#include "imagen.h"
#include <math.h>

//TDA que representa a la moto
typedef struct moto moto_t;

//Creacion el TDA moto_t
moto_t *moto_crear(imagen_t *im, const pixel_t paleta[], size_t ancho, size_t alto);

//Destruye la creación de moto_t
void destruccion_moto(moto_t *moto);

// Setter para el cambio de imagen
void moto_set_imagen(moto_t *moto, imagen_t *im);

// Setter para el cambio de paleta
void moto_set_paleta(moto_t *moto, const pixel_t paleta[]);

// Setter para la distancia recorrida en y
void moto_set_distancia_y(moto_t *moto, double distancia_y);

//Setter para la velocidad de la moto
void moto_set_velocidad(moto_t *moto, bool acelerado, bool frenado, double tiempo);

//Setter para el tiempo jugado de la moto
void moto_set_tiempo_jugado(moto_t *moto, double tiempo);

//Setter de la distancia en x
void moto_set_distancia_x(moto_t *moto, double tiempo);

//Setter del puntaje obtenido en el juego
void moto_set_score(moto_t *moto, double dist_x);

//Getter para obtener la moto
imagen_t *moto_get_imagen(moto_t *moto);

//Getter para obtener la paleta
pixel_t *moto_get_paleta(moto_t *moto);

//Getter para la velocidad de la moto
double moto_get_velocidad(moto_t *moto);

//Getter para el tiempo
double moto_get_tiempo(moto_t *moto);

//Getter para el puntaje de la moto
int moto_get_score(moto_t *moto);

//Getter para la posicion x
double moto_get_posicion_x(moto_t *moto);

//Getter para la distancia con respecto al tiempo
double moto_get_distancia_tiempo(moto_t *moto, double tiempo);

//Getter para la posicion y
double moto_get_posicion_y(moto_t *moto);

/* Recibe un moto_t.
Moto_giro que puede ser 1 (giro derecho), 0(no giro), -1 (giro izquierdo)
Moto_i corresponde a la intensidad de giro, la cual ira incrementando hasta el valor 3 
Moto_girada_hacia sirve para saber desde donde esta volviendo
Recibe las diferentes motos correspondientes a los diferentes movimientos
Puntero de la nueva posicion correspondiente en x.
Internamente cambia la moto con respecto a su giro y posicion_y*/
void moto_girar(moto_t *moto, int moto_giro, int *moto_i, int *moto_girada_hacia, imagen_t *motos[], int *pos_x);

/*Recibe un moto_t.
Moto_i corresponde a la intensidad de giro, la cual ira disminuyendo hasta el valor 0
Moto_girada_hacia sirve para saber desde donde esta volviendo
Recibe las diferentes motos correspondientes a los diferentes movimientos
Puntero de la nueva posicion correspondiente en x.
Puntero de la nueva posicion correspondiente en y
Internamente devuelve la moto a su posicion inicial*/
void moto_centrar(moto_t *moto, int *moto_i, int *moto_girada_hacia, imagen_t *motos[], int *pos_x, int *pos_y);

/*
Desplaza a la moto modificando su distancia en y 
dado su recorrido en x y el radio de la ruta*/
void moto_giro_ruta(moto_t *moto, double dist_x, float radio);

/*Setea la velocidad y la distancia_y de la moto en 0*/
void moto_choque(moto_t *moto);

#endif