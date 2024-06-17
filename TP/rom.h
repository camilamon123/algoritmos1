#ifndef ROM_H
#define ROM_H
#include <stdbool.h>
#include "imagen.h"
#include "pixel.h"


//Lee la rom de la ruta y las setea como corresponde en teselas
bool leer_ruta(imagen_t *teselas);

/*Que reciba un vector con 4096 imágenes de 8x8 ya inicializadas y cargue 
en ellas las teselas contenidas en los archivos ARCHIVO_ROM_R, ARCHIVO_ROM_G y ARCHIVO_ROM_B*/

bool leer_teselas(imagen_t *teselas[], size_t cantidad_rom, char *rutas[cantidad_rom]);

//Lee las rom correspondientes a pares de las figuras y la moto. 
bool vector_teselas(uint16_t teselas[], size_t cantidad_rom, char *rutas[cantidad_rom]);

/*Que dado el vector con las teselas y un vector la paleta reciba una tabla mosaico_teselas con los índices de cada tesela y 
otra tabla mosaico_paletas con los índices de la paleta a usar, y genere una imagen que tenga columnas x filas teselas.*/
imagen_t *generar_mosaico(imagen_t *teselas[], const pixel_t paleta[][8],
                          size_t filas, size_t columnas, const uint16_t mosaico_teselas[filas][columnas],
                          const uint8_t mosaico_paletas[filas][columnas]);


#endif 