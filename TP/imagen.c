#include "imagen.h"

struct imagen{
    pixel_t **pixeles;
    size_t ancho, alto;
};


static imagen_t *_imagen_crear(size_t ancho, size_t alto){

    imagen_t *imagen_creada = malloc(sizeof(imagen_t));

    if(imagen_creada == NULL){
        return NULL;
    }

    imagen_creada -> ancho = ancho;
    imagen_creada -> alto = alto;

    imagen_creada -> pixeles = malloc(alto * (sizeof(pixel_t*)));
    if(imagen_creada -> pixeles == NULL){
        free(imagen_creada);
        return NULL;
    }

    for (size_t i = 0; i < alto; i++){
        imagen_creada -> pixeles[i] = malloc(ancho * sizeof(pixel_t));
        if(imagen_creada -> pixeles[i] == NULL){
            imagen_creada -> alto = i;
            imagen_destruir(imagen_creada);
            return NULL;
        }
    }

    return imagen_creada;
}

void imagen_destruir(imagen_t *imagen){

    for(size_t i = 0; i < (imagen -> alto); i++){
        free(imagen -> pixeles[i]);
    }
    free(imagen -> pixeles);
    free(imagen);

}

void destruccion_imagenes( imagen_t *imagenes[], size_t n){
        for(size_t i = 0; i < n; i++){
            imagen_destruir(imagenes[i]);
        }
       
}

imagen_t *imagen_generar(size_t ancho, size_t alto, pixel_t valor){

    imagen_t *imagen = _imagen_crear(ancho, alto);
    if(imagen == NULL){
        return NULL;
    }

    for (size_t i = 0; i < alto; i++){
        for (size_t j = 0; j < ancho; j++){
            imagen -> pixeles[i][j] = valor;
        }
    }
    
    return imagen;
}

imagen_t *imagen_escalar(const imagen_t *origen, size_t ancho_destino, size_t alto_destino){

    imagen_t *destino = _imagen_crear(ancho_destino, alto_destino);

    if(destino == NULL){
        return NULL;
    }

    size_t alto_origen = origen -> alto;
    size_t ancho_origen = origen -> ancho;

    if(alto_destino == 0 || ancho_destino == 0){
        imagen_destruir(destino);
        return NULL;
    }

    double escala_alto = alto_origen / ((double)alto_destino) ;
    double escala_ancho = ancho_origen / ((double)ancho_destino);

    for(size_t j = 0; j < alto_destino; j++){
        for(size_t i = 0; i < ancho_destino; i++){
            destino -> pixeles[j][i] = origen -> pixeles[(size_t)(escala_alto * j)][(size_t)(escala_ancho * i)];
        }
    }

    return destino;
}


size_t imagen_get_ancho(const imagen_t *im){
    return im->ancho;
}

size_t imagen_get_alto(const imagen_t *im){
    return im->alto;
}

pixel_t imagen_get_pixel(const imagen_t *im, size_t x, size_t y){
    return im->pixeles[y][x];
}

bool imagen_set_pixel(const imagen_t *im, size_t x, size_t y, pixel_t p){
    if(im->alto < y || im->ancho < x) return false;

    im->pixeles[y][x]= p;

    return true;
}

void imagen_pegar(imagen_t *destino, const imagen_t *origen, int x, int y){

    for(int f = y >= 0 ? 0 : -y; f < origen->alto && f + y < destino->alto; f++){
        for(int c = x >= 0 ? 0 : -x; c < origen->ancho && c + x < destino->ancho; c++){
            size_t fila = (y + f);
            size_t columna = (x + c);
            int p = origen -> pixeles[f][c];
            if(p == 0)
                continue;
            destino -> pixeles[fila][columna] = origen -> pixeles[f][c];
        }
    }
}

void imagen_pegar_con_paleta(imagen_t *destino, const imagen_t *origen, int x, int y, const pixel_t paleta[]){
    
    for(int f = y >= 0 ? 0 : -y; f < origen->alto && f + y < destino->alto; f++){ 
        for(int c = x >= 0 ? 0 : -x; c < origen->ancho && c + x < destino->ancho; c++){
            size_t fila = (y + f);
            size_t columna = (x + c);
            int p = origen -> pixeles[f][c];
            if(p == 0)
                continue;
            destino -> pixeles[fila][columna] = paleta[p];
        }
    }

}


void imagen_a_textura(const imagen_t *im, uint16_t *v){
    for(size_t f = 0; f < im -> alto; f++){
        for(size_t c = 0; c < im -> ancho; c++){
            v[(f * (im -> ancho)) + c] = im -> pixeles[f][c];
        }
    }
}

imagen_t *imagen_espejar(const imagen_t *original){ // Mismo Alto y Ancho
    size_t alto = original -> alto;
    size_t ancho = original -> ancho;
    
    imagen_t *nueva = _imagen_crear(ancho, alto);
    if( nueva == NULL)
        return NULL;

    for(size_t f = 0; f < alto; f++){
        for(size_t c = 0; c < ancho; c++){
            nueva -> pixeles[f][(ancho - c) - 1] = original -> pixeles[f][c];
        }
    }

    return nueva;
}
