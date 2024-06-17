#ifndef IMAGEN_H
#define IMAGEN_H

#include "pixel.h"
#include "paleta.h"
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

//TDA que representa la imagen de hang on
typedef struct imagen imagen_t;

//Destruye la memoria asociada a una imagen_t
void imagen_destruir(imagen_t *imagen);

/*Destruye n cantidad de imagen_t dadas en imagenes
imagenes[] tiene n casilleros*/
void destruccion_imagenes( imagen_t *imagenes[], size_t n);

//Genera una imagen dado un ancho, alto y un color valor
imagen_t *imagen_generar(size_t ancho, size_t alto, pixel_t valor);

/*Genera una imagen a partir de una imagen origen ya hecha para 
ajustarla a un nuevo ancho ancho_destino y alto alto_destino*/
imagen_t *imagen_escalar(const imagen_t *origen, size_t ancho_destino, size_t alto_destino);

//Getter del ancho de la imagen
size_t imagen_get_ancho(const imagen_t *im);

//Getter del alto de la imagen
size_t imagen_get_alto(const imagen_t *im);

//Getter del pixel de la imagen en la posicion [y][x]
pixel_t imagen_get_pixel(const imagen_t *im, size_t x, size_t y);

//Setter de los pixeles de la imagen en la posicion [y][x] con el valor p
bool imagen_set_pixel(const imagen_t *im, size_t x, size_t y, pixel_t p);

//Recibe dos imagenes y pega origen en destino en la coordenada (x,y)
void imagen_pegar(imagen_t *destino, const imagen_t *origen, int x, int y);

//Recibe dos imagenes y pega origen en destino en la coordenada (x,y) con una determinada paleta
void imagen_pegar_con_paleta(imagen_t *destino, const imagen_t *origen, int x, int y, const pixel_t paleta[]);

//Vuelca cada pixel de im en el vector v
void imagen_a_textura(const imagen_t *im, uint16_t *v);

//Recibe una imagen y devuelve la misma imagen espejada
imagen_t *imagen_espejar(const imagen_t *original);

#endif