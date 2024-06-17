
#include "imagen.h"
#include "ruta.h"
#include <math.h>

#define MASK_A 0xf000
#define MASK_B 0x0f00
#define MASK_C 0x00f0
#define MASK_D 0x000f


imagen_t *figura_pegar(const uint16_t rom[], size_t inicio, size_t ancho, size_t alto){
    
    int masks[4] = {MASK_A, MASK_B, MASK_C, MASK_D};
    imagen_t *imagen = imagen_generar(ancho, alto, 0);
    if(imagen == NULL)
        return NULL;

    for(size_t f = 0; f < alto; f++){
        bool nueva_linea = false;
        while((rom[inicio] & masks[3]) == 0xf)
            inicio++;
        for(size_t c = 0; c < ancho;){
            for(size_t i = 0; i < 4; i++){
                if(c >= ancho) break;

                pixel_t pixel = (rom[inicio] & masks[i]) >> (4 * (3-i));
                if(i == 3 && pixel == 0xf){
                    nueva_linea = true;
                    break;
                }
                if(pixel == 0xf) pixel = 0;

                imagen_set_pixel(imagen, c++, f, pixel);
            }
            inicio++;
            if(nueva_linea) break;
        }
    }
    return imagen;
}

void texto(char *texto,imagen_t *cuadro, imagen_t *teselas[], size_t ancho, size_t alto, const pixel_t paleta[]){
     
        for(size_t i = 0; texto[i]; i++)
            imagen_pegar_con_paleta(cuadro, teselas[(size_t)texto[i]], ancho + i*8, alto, paleta);
}

void pixel_textos(size_t n , const pixel_t pixeles[][n], imagen_t *cuadro, imagen_t *teselas[], size_t ancho, size_t alto_1,size_t alto_2,size_t alto_3, const pixel_t paleta[]){
    
        for(size_t j = 0; j < n  ; j++){
            imagen_pegar_con_paleta(cuadro,teselas[(size_t)pixeles[0][j]], ancho + j*8, alto_1, paleta);
            imagen_pegar_con_paleta(cuadro,teselas[(size_t)pixeles[1][j]], ancho + j*8, alto_2, paleta);
            imagen_pegar_con_paleta(cuadro,teselas[(size_t)pixeles[2][j]], ancho + j*8, alto_3, paleta);
        }
    
}

float v(float x_x, float x_m){
    int d = x_x - x_m; 
     // x_x = pos del objeto. x_m = pos de la moto
    if(d >= 0 && d < 60)  // se restringe el campo de visión hasta 60 metros
        return (96 - 96*exp(-0.11*d));

    return -9999.9;
}

// // implementamos la inversa a v

float d(float v){
    if(v >= 0 && v < 96)
        return (-1/0.11)*log((96 - v)/96);

    return -9999.9;
}

// la pos posicion1_objetcs == 0, los objetos tienen una determinada altura pero a medida que avanzan tienen q ir escalando para
// // adecuarse al mov
// //h_0 altura inicial
float h(float v, float h_0){

    if(v >= 0 && v < 96)
        return (h_0*((96-v)/96) + ((5*v)/96));

    return -9999.9;
}

float u(float v, int ur, int y){
    return ((y *((96 - v)/96)) + ( y * (v/5000)) + ur);
}

float u_l(int v, double ym){
    return ((-1)* ym * ((96 - v)/96.0));
}

float u_c(int v, float radio, float uc_anterior){
    if(!v) return 0;

    return uc_anterior + (radio * exp((0.105*v)-8.6));
}


imagen_t *figura_a_pegar(size_t x, float x_m, struct figura_en_ruta figura_actual, imagen_t *figuras[], float *aux_u, int *aux_v, float ur){
    int v_fig = v((float)x, x_m);

    if(v_fig != -9999.9){

        imagen_t *fig = figuras[figura_actual.figura];
        if (fig == NULL) return NULL;

        float h_fig = h(v_fig, (int)(imagen_get_alto(fig)));

        float escala = h_fig/(float)(imagen_get_alto(fig));

        float ancho_fig = (size_t)(imagen_get_ancho(fig)*escala);

        if(ancho_fig < 3) ancho_fig = 3;

        imagen_t *fig_escalada = imagen_escalar(fig, (size_t)ancho_fig , (size_t)h_fig);
        if(fig_escalada == NULL){
            imagen_destruir(fig);
            return NULL;
        }
        imagen_t *imagen_reflejada;
        
        float u_fig = u(v_fig, ur, figura_actual.y);

        if(figura_actual.reflejar){

            imagen_reflejada = imagen_espejar(fig_escalada);
            
            if(imagen_reflejada == NULL){
                imagen_destruir(fig);
                imagen_destruir(fig_escalada);
                return NULL;
            }
            *aux_u = u_fig;
            *aux_v = (int)v_fig + (int)(imagen_get_alto(imagen_reflejada));
            imagen_destruir(fig_escalada);
            return imagen_reflejada;
        }

        *aux_u = u_fig;
        *aux_v = (int)v_fig + (int)(imagen_get_alto(fig_escalada));

        

        return fig_escalada;
        
    }

    return NULL;
}