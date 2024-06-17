#ifndef PIXEL_H
#define PIXEL_H

#include <stdint.h>
#include <stdbool.h>


typedef uint16_t pixel_t;

// crea un pixel de 3 bits
pixel_t pixel3_crear(bool r, bool g, bool b);

// devuelve las componentes de un pixel de 3 bits
void pixel3_a_rgb(pixel_t pixel3, uint8_t *r, uint8_t *g, uint8_t *b);

//crea un pixel de 12 bits
pixel_t pixel12_crear(uint8_t r, uint8_t g, uint8_t b);

//devuelve las componentes de un pixel de 12 bits
void pixel12_a_rgb(pixel_t pixel12, uint8_t *r, uint8_t *g, uint8_t *b);



#endif