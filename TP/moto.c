#include "moto.h"

struct moto{
    imagen_t *imagen_moto;
    pixel_t paleta_moto[16];
    double velocidad;
    double tiempo_jugado;
    int score;
    double distancia_recorrida_x;
    double distancia_y;
};

void moto_set_imagen(moto_t *moto, imagen_t *im){
    moto -> imagen_moto = im;
}

void moto_set_paleta(moto_t *moto, const pixel_t paleta[]){
    for(size_t i = 0; i < 16; i++){
        moto -> paleta_moto[i] = paleta[i];
    }
}

void moto_set_distancia_y(moto_t *moto, double distancia_y){
    moto->distancia_y += distancia_y;

    if(moto -> distancia_y > 435)
        moto -> distancia_y = 435;
    else if(moto -> distancia_y < -435)
        moto -> distancia_y = -435;
}

moto_t *moto_crear(imagen_t *im, const pixel_t paleta[], size_t ancho, size_t alto){
    moto_t *moto_creada = malloc(sizeof(moto_t));
    if(moto_creada == NULL) return NULL;
    
   /* moto_creada ->imagen_moto = imagen_generar(ancho, alto, 0);
    if(moto_creada->imagen_moto== NULL){
        free(moto_creada);
        return NULL;
    }*/

    moto_set_imagen(moto_creada, im);
    moto_set_paleta(moto_creada, paleta);

    moto_creada -> tiempo_jugado = 0.0;
    moto_creada -> velocidad = 0.0;
    moto_creada -> score = 0;
    moto_creada -> distancia_recorrida_x = 0.0;
    moto_creada -> distancia_y = 0.0;

    return moto_creada;
}

void destruccion_moto(moto_t *moto){
    
   //imagen_destruir(moto->imagen_moto);
    free(moto);
 
}

static double conversion_kmh_a_ms(double velocidad){
    return (velocidad / 3.6);
}

void moto_set_velocidad(moto_t *moto, bool acelerado, bool frenado, double tiempo){
    
    if(acelerado ||( moto -> velocidad) <= 80.0)
        moto -> velocidad = (double)(279 - ( 279 - (moto -> velocidad))*exp(-0.224358*tiempo));
    
    else if (frenado)
        moto -> velocidad -= 10.0;

    else
        moto -> velocidad -= 3.0;


    if(abs((int)(moto -> distancia_y)) > 215 && moto -> velocidad > 92)
        moto -> velocidad -= 3.0;

    if((moto -> velocidad) < 0)
        moto -> velocidad = 0;
}

void moto_set_tiempo_jugado(moto_t *moto, double tiempo){
    moto -> tiempo_jugado = tiempo;
}

void moto_set_distancia_x(moto_t *moto, double tiempo){
    if(moto -> distancia_recorrida_x >= 4200){
        moto -> distancia_recorrida_x = 4200;
        return;
    }
    moto -> distancia_recorrida_x += (tiempo) * (conversion_kmh_a_ms(moto -> velocidad));
}

void moto_set_score(moto_t *moto, double dist_x){
    if(abs((int)(moto -> distancia_y)) <= 215){
        if(moto -> velocidad < 117)
            moto -> score += ((int)(125*dist_x));
        else
            moto -> score += ((int)(dist_x * ((3.13 * moto -> velocidad) - 240)));
    }
}

imagen_t *moto_get_imagen(moto_t *moto){
    return moto -> imagen_moto;
}

pixel_t *moto_get_paleta(moto_t *moto){
    return moto -> paleta_moto;
}

double moto_get_velocidad(moto_t *moto){
    return moto->velocidad;
}

double moto_get_tiempo(moto_t *moto){
    return moto->tiempo_jugado;
}

int moto_get_score(moto_t *moto){
    return moto->score;
}

double moto_get_posicion_x(moto_t *moto){
    return moto->distancia_recorrida_x;
}

double moto_get_distancia_tiempo(moto_t *moto, double tiempo){
    return ((conversion_kmh_a_ms(moto -> velocidad)) * tiempo);
}

double moto_get_posicion_y(moto_t *moto){
    return moto->distancia_y;
}

void moto_girar(moto_t *moto, int moto_giro, int *moto_i, int *moto_girada_hacia, imagen_t *motos[], int *pos_x){
    int aux_moto;
    int aux_x;
    if(*moto_i < 3)
        (*moto_i)++;

    if(moto_giro == 1){
        *moto_girada_hacia = 1;
        aux_moto = 0;
        aux_x = 144;
    }
    else {
        *moto_girada_hacia = -1;
        aux_moto = 3;
        aux_x = 136 - (*moto_i * 7);
    }

    moto_set_distancia_y(moto, (3*((*moto_i) - 1) +3*(*moto_i))*moto_giro );

    moto_set_imagen(moto, motos[(*moto_i) + aux_moto]);
    *pos_x = aux_x;
}

void moto_centrar(moto_t *moto, int *moto_i, int *moto_girada_hacia, imagen_t *motos[], int *pos_x, int *pos_y){
    int aux_x;
    int aux_y;
    int aux_moto;
    if(*moto_girada_hacia == -1){
        aux_x = 144 - (7 * (*moto_i));
        aux_y = 146 + (7 * (*moto_i));
        aux_moto = 3;
    }
    else{
        aux_x = 144;
        aux_moto = 0;
        if(*moto_i > 0)
            aux_y = 146 + (7*(*moto_i));
        else
            aux_y = 150;
    }

    moto_set_imagen(moto, motos[(*moto_i) + aux_moto]);

    if(*moto_i > 0)
        (*moto_i)--;
        
    if(*moto_i == 0)
        *moto_girada_hacia = 0;

    *pos_x = aux_x;
    *pos_y = aux_y;
    
}


void moto_giro_ruta(moto_t *moto, double dist_x, float radio){
    moto -> distancia_y -= (double)(2.5 * dist_x * radio);
}

void moto_choque(moto_t *moto){
    moto -> distancia_y = 0;
    moto -> velocidad = 0;
}