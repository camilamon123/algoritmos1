#ifndef FIGURA_H
#define FIGURA_H

#include "imagen.h"
#include "ruta.h"
#include "paleta.h"
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// Devuelve una imagen generada por los pixeles de la rom desde inicio, con las dimensiones de ancho y alto
imagen_t *figura_pegar(const uint16_t rom[], size_t inicio, size_t ancho, size_t alto);

//Pega en imagen cuadro lo que se recibe como texto 
void texto(char *texto,imagen_t *cuadro, imagen_t *teselas[], size_t ancho, size_t alto, const pixel_t paleta[]);

// Genera imagenes a partir de los pixeles ingresados y los pega en la imagen cuadro
void pixel_textos(size_t n , const pixel_t pixeles[][n], imagen_t *cuadro, imagen_t *teselas[], size_t ancho, size_t alto_1,size_t alto_2,size_t alto_3, const pixel_t paleta[]);

/*Recibe los parametros correspondientes a d = x_x - x_m, siendo esta la distancia entre un objeto y la moto
y devuelve la ubicación v del objeto en ese momento.*/
float v(float x_x, float x_m);

/*Distancia en la que se encuentra un objeto en determinada posición.
El parametro v debe estar entre 0 y 96*/
float d(float v);

/*Altura de un objeto escalado a una determinada distancia v, con una altura inicial h_0.
El parametro v debe estar entre 0 y 96.*/
float h(float v, float h_0);

/*Posición u de un objeto en la pantalla teniendo en cuenta la pos v, ur el desplazamiento total del centro de la ruta
y su distancia al centro de la ruta y*/
float u(float v, int ur, int y);

// Desplazamiento lateral de la franja de la ruta dado la v, y la posicion de la moto con respecto al centro de la ruta ym
float u_l(int v, double ym);

/*Desplazamiento curvo dada la posicion v, el radio de curvatura actual y desplazamiento curvo de la franja anterior.
Si v = 0, devuelve 0*/
float u_c(int v, float radio, float uc_anterior);

/*Dada la posicion x de la figura_actual y la posición x_m de la moto, se devuelve escalada la imagen correspondiente en figuras[]
y también asigna posicion u y v en aux_u y aux_v los cuales son punteros*/
imagen_t *figura_a_pegar(size_t x, float x_m, struct figura_en_ruta figura_en_ruta, imagen_t *figuras[], float *aux_u, int *aux_v, float ur);

#endif