#include "imagen.h"
#include "config.h"
#include "rom.h"

//#include <stdbool.h>

bool leer_ruta(imagen_t *teselas){
    
    FILE *f = fopen(ROM_RUTA, "rb");
    if (f == NULL){
        return false;
    };
    uint8_t bit1 = 0;

    for(size_t imagen = 0; imagen < 4; imagen++){
        
        for(size_t alto = 0 ; alto < 128; alto++){  
            for(size_t ancho = 0; ancho < 64; ancho++){
                fread(&bit1, 1, 1,f);
                if(alto < 16 || alto >= 112) continue;
                for(size_t bits = 0; bits < 8; bits++){

                    int counter = 7 - bits; 
                    int g = (bit1&(0x1<<counter))>>counter;     
                    uint8_t bit = (imagen_get_pixel(teselas, (ancho*8)+bits,alto))<<1;
                    uint8_t pixel4 = g | bit;
                    imagen_set_pixel(teselas, (ancho*8) + bits , alto , pixel4);
                }
            }
        }
    }
    fclose(f);         
    return true;
}

bool leer_teselas(imagen_t *teselas[], size_t cantidad_rom, char *rutas[cantidad_rom]){
   
    for(size_t iman = 0 ; iman < cantidad_rom; iman++){
        FILE *rom = fopen(rutas[iman], "rb");
        if (rom == NULL){
            return false;
        }
        for(size_t tesela1 = 0 ; tesela1 < CANTIDAD_TESELAS ; tesela1++){

        uint8_t byte;
        size_t  y, x;

            for( y = 0 ; y < ALTO_TESELA ; y++){
                fread(&byte, 1, 1, rom);
                for( x = 0; x < ANCHO_TESELA ; x++){
                    size_t counter = 7 - x;
                    int g = (byte&(0x1<<counter))>>counter;
                    uint8_t r = (imagen_get_pixel(teselas[tesela1],x, y))<<1;
                    uint8_t pixel = (r|g) ;
                    imagen_set_pixel(teselas[tesela1], x ,y, pixel);
                }
            }
        }
        fclose(rom);
    }
    return true;
}


bool vector_teselas(uint16_t teselas[], size_t cantidad_rom, char *rutas[cantidad_rom]){
   
    size_t contador = 0;
    for(size_t ruta = 0 ; ruta < cantidad_rom;){

        FILE *rom1 = fopen(rutas[ruta++], "rb");
        if(rom1 == NULL) return false;
        FILE *rom2 = fopen(rutas[ruta++], "rb");
        if (rom2 == NULL){
            fclose(rom1);
            return false;
        }
        uint8_t byte1;
        uint8_t byte2;

        for(size_t bytes = (32768 * contador) ; bytes < (32768 * (contador + 1)) ; bytes++){
          
            fread(&byte1, sizeof(byte1), 1, rom1);
            fread(&byte2, sizeof(byte2), 1, rom2);

            teselas[bytes] = (byte1 | (byte2 << 8));
            
        }
        fclose(rom1);
        fclose(rom2);

        contador++;
    }
    return true;
}






imagen_t *generar_mosaico(imagen_t *teselas[], const pixel_t paleta[][8],
                          size_t filas, size_t columnas, const uint16_t mosaico_teselas[filas][columnas],
                          const uint8_t mosaico_paletas[filas][columnas])
{
    imagen_t *imagen_final= imagen_generar(ANCHO_TESELA*columnas, ALTO_TESELA*filas,0);  
    if(imagen_final == NULL) return NULL;
    
    for (size_t y = 0; y < filas; y++)
    {
        for (size_t x = 0; x < columnas; x++)
        {
            size_t tesela =  ((mosaico_teselas[y][x]));
            size_t colores =  ((mosaico_paletas[y][x]));

            imagen_pegar_con_paleta(imagen_final, teselas[tesela], ANCHO_TESELA*x, ALTO_TESELA*y, paleta[colores]);
        }
    }
    return imagen_final;
}


