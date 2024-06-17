#include <SDL2/SDL.h>
#include <stdbool.h>

#include "moto.h"
#include "config.h"
#include "imagen.h"
#include "paleta.h"
#include "fondo.h"
#include "rom.h"
#include "figura.h"
#include "ruta.h"


int main(void) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;

    SDL_CreateWindowAndRenderer(VENTANA_ANCHO, VENTANA_ALTO, 0, &window, &renderer);
    SDL_SetWindowTitle(window, "Hang-On");

    SDL_Texture * texture = SDL_CreateTexture(renderer,
    SDL_PIXELFORMAT_RGB444, SDL_TEXTUREACCESS_STATIC, VENTANA_ANCHO, VENTANA_ALTO);
    uint16_t canvas[VENTANA_ALTO * VENTANA_ANCHO];

    int dormir = 0;

    // BEGIN código del alumno

    double x_fondo1 = 320;
    double x_fondo2 = 320;
    bool mover_derecha = false;
    bool mover_izquierda = false;
    bool acelerado = false;
    bool frenado = false;
    int moto_giro = 0;
    int moto_i = 0;
    int moto_girada_hacia = 0;

    double tiempo_inicial = TIEMPO_NIVEL;
    double tiempo_restante = tiempo_inicial;
    double tick_tiempo = (1 / (double)JUEGO_FPS);

    //------------------------Creacion-----------------------
          
    imagen_t *teselas[CANTIDAD_TESELAS];

        for(size_t i = 0; i < CANTIDAD_TESELAS; i++)
            teselas[i] = imagen_generar(ANCHO_TESELA, ALTO_TESELA, 0);

        char *rutas[3] = {ARCHIVO_ROM_R, ARCHIVO_ROM_G, ARCHIVO_ROM_B};
        if(! leer_teselas(teselas, 3, rutas)) {
            fprintf(stderr, "No se pudieron leer las teselas\n");

                destruccion_imagenes(teselas, CANTIDAD_TESELAS);

            return 1;
        }

    char *numero; 
    char *stage = "STAGE";
    char *n_stage = NIVEL;
    char *numero_time; 
    char *speed = "SPEED";
    char *n_speed;  
    char *n_score; 
    char *km = "KM";

    imagen_t *imagenes[20];

    imagen_t *fondo1 = generar_mosaico(teselas, paleta_3, FONDO1_FILAS, FONDO1_COLUMNAS, fondo1_mosaico, fondo1_paleta);
    if(fondo1 == NULL){
        destruccion_imagenes(teselas, CANTIDAD_TESELAS);
        return 1;
    }

    imagenes[0] = fondo1;

    imagen_t *fondo2 = generar_mosaico(teselas, paleta_3, FONDO2_FILAS, FONDO2_COLUMNAS, fondo2_mosaico, fondo2_paleta);
    if(fondo2 == NULL){
        destruccion_imagenes(imagenes, 1);
        destruccion_imagenes(teselas, CANTIDAD_TESELAS);
        return 1;
    }

    imagenes[1] = fondo2;

    imagen_t *pasto = imagen_generar(1, 96, pixel12_crear(0, 13, 9));
    if(pasto == NULL){
        destruccion_imagenes(imagenes, 2);
        destruccion_imagenes(teselas, CANTIDAD_TESELAS);
        return 1;
    }

    imagenes[2] = pasto;
    
    for(size_t i = 0; i < 10; i++)
        imagen_set_pixel(pasto, 0, i, colores_pasto[i]);


    imagen_t *pasto_estirado = imagen_escalar(pasto, 320, 96);
    if( pasto_estirado == NULL){
        destruccion_imagenes(imagenes, 3);
        destruccion_imagenes(teselas, CANTIDAD_TESELAS);
        return 1;
    }

    imagenes[3] = pasto_estirado;

    imagen_t *ruta_completa = imagen_generar(1024,128,0);
    if(ruta_completa == NULL){
        destruccion_imagenes(imagenes, 4);
        destruccion_imagenes(teselas, CANTIDAD_TESELAS);
        return 1;
    }
    imagenes[4] = ruta_completa;

    imagen_t *ruta_izq = imagen_generar(512, 128, 0);
        if(!leer_ruta(ruta_izq)) {
            fprintf(stderr, "No se pudieron leer las teselas\n");
            destruccion_imagenes(imagenes, 5);
            destruccion_imagenes(teselas, CANTIDAD_TESELAS);
            return 1;
        }
        for(size_t f = 0; f < 128; f++){
            for(size_t c = 0; c < 512; c++){
                uint8_t pixel = imagen_get_pixel(ruta_izq, c, f);
                if(pixel == 0xf){
                    imagen_set_pixel(ruta_izq, c, f, 0);
                    continue;
                }
                break;
            }
        }
    imagenes[5] = ruta_izq;
    
    imagen_t *ruta_der = imagen_espejar(ruta_izq);
    if(ruta_der == NULL){
        destruccion_imagenes( imagenes, 6);
        destruccion_imagenes(teselas, CANTIDAD_TESELAS);
        return 1;
    }
    imagenes[6] = ruta_der;

    imagen_pegar(ruta_completa, ruta_izq, 0, 0);
    imagen_pegar(ruta_completa, ruta_der, 504, 0);

    imagen_t *ruta_aux = imagen_generar(1024,1,0);
    if(ruta_aux == NULL){
        destruccion_imagenes(imagenes, 7);
        destruccion_imagenes(teselas, CANTIDAD_TESELAS);
        return 1;
    }

    imagenes[7] = ruta_aux;

    size_t ruta_i = 0;
    
    uint16_t rom[229376];
        
        char *rutas_rom[14] = { ROM_6819, ROM_6820, ROM_6821, ROM_6822, ROM_6823, ROM_6824,
                                ROM_6825, ROM_6826, ROM_6827, ROM_6828, ROM_6829, ROM_6830,
                                ROM_6845, ROM_6846};

        

        if(!vector_teselas(rom, 14, rutas_rom)) {
            fprintf(stderr, "No se pudieron leer las teselas\n");
            return 1;
        }

    enum{ARBOL, CARTEL, ROCA, DELFIN, BELL, FORUM, VIGA, SEMAFORO};

    imagen_t *arbol = figura_pegar(rom, 50116, 46, 188);
    if(arbol == NULL){
        destruccion_imagenes(imagenes, 8);
        destruccion_imagenes(teselas, CANTIDAD_TESELAS);
        return 1;
    }
    imagenes[8] = arbol;

    imagen_t *cartel = figura_pegar(rom, 37390, 96, 112);
    if(cartel == NULL){
        destruccion_imagenes(imagenes, 9);
        destruccion_imagenes(teselas, CANTIDAD_TESELAS);
        return 1;
    }
    imagenes[9] = cartel;

    imagen_t *roca = figura_pegar(rom, 69464, 136, 76);
    if(roca == NULL){
       destruccion_imagenes(imagenes, 10);
       destruccion_imagenes(teselas, CANTIDAD_TESELAS);
        return 1;
    }
    imagenes[10] = roca;

    imagen_t *delfin = figura_pegar(rom, 119280, 144, 110);
    if( delfin == NULL){
        destruccion_imagenes(imagenes, 11);
        destruccion_imagenes(teselas, CANTIDAD_TESELAS);
        return 1;
    }
    imagenes[11] = delfin;

    imagen_t *bell = figura_pegar(rom, 82922, 63, 146);
    if( bell == NULL){
        destruccion_imagenes( imagenes, 12);
        destruccion_imagenes(teselas, CANTIDAD_TESELAS);
        return 1;
    }
    imagenes[12] = bell;

    imagen_t *forum = figura_pegar(rom, 89102, 118, 114); 
    if( forum == NULL){
        destruccion_imagenes(imagenes, 13);
        destruccion_imagenes(teselas, CANTIDAD_TESELAS);
        return 1;
    }
    imagenes[13] = forum;
    
    imagen_t *viga_completa = imagen_generar(470, 249,0); 
    if( viga_completa == NULL){
        destruccion_imagenes(imagenes, 14);
        destruccion_imagenes(teselas, CANTIDAD_TESELAS);
        return 1;
    }
    imagenes[14] = viga_completa;

    imagen_t *semaforos_com = imagen_generar(470, 249,0);  
    if( semaforos_com == NULL){
        destruccion_imagenes(imagenes, 15);
        destruccion_imagenes(teselas, CANTIDAD_TESELAS);
        return 1;
    }
    imagenes[15] = semaforos_com;

    imagen_t *viga_banner = figura_pegar(rom, 127098, 198, 48);
    if( viga_banner == NULL){
        destruccion_imagenes(imagenes, 16);
        destruccion_imagenes(teselas, CANTIDAD_TESELAS);
        return 1;
    }
    imagenes[16] = viga_banner;

    imagen_t *banner = figura_pegar(rom,114518, 244, 48 );
    if( banner == NULL){
        destruccion_imagenes(imagenes, 17);
        destruccion_imagenes(teselas, CANTIDAD_TESELAS);
        return 1;
    }
    imagenes[17] = banner;

    imagen_t *semaforos = figura_pegar(rom, 108724, 66, 201);
    if( semaforos == NULL){
        destruccion_imagenes(imagenes, 18);
        destruccion_imagenes(teselas, CANTIDAD_TESELAS);
        return 1;
    }
    imagenes[18] = semaforos;
    

    imagen_pegar(viga_completa, viga_banner,0,0);
    imagen_pegar(viga_completa, viga_banner, 272 ,0);
    imagen_pegar(viga_completa, banner, 113, 0);

    imagen_pegar(semaforos_com, semaforos, 0,48);
    imagen_pegar(semaforos_com, semaforos, 404,48);

    imagen_t *semaforo_escalado = imagen_escalar(semaforos_com, 222,118 );
    if(semaforo_escalado == NULL){
        destruccion_imagenes(imagenes, 19);
        destruccion_imagenes(teselas, CANTIDAD_TESELAS);
        return 1;
    }
    
    imagenes[19] = semaforo_escalado;

    imagen_t *figuras[8] = {
        [ARBOL] = arbol, 
        [CARTEL] = cartel,
        [ROCA] = roca,
        [DELFIN] = delfin,
        [BELL] = bell,
        [FORUM] = forum,
        [VIGA] = viga_completa,
        [SEMAFORO] = semaforos_com
    };

    imagen_t *moto_0 = figura_pegar(rom, 532, 36, 73) ;
    if( moto_0 == NULL){
        destruccion_imagenes(imagenes, 20);
        destruccion_imagenes(teselas, CANTIDAD_TESELAS);
        return 1;
    }
    imagen_t *moto_1 = figura_pegar(rom, 5670, 36, 70);
    if( moto_1 == NULL){
        destruccion_imagenes(imagenes, 20); 
        imagen_destruir(moto_0);
        destruccion_imagenes(teselas, CANTIDAD_TESELAS);
        return 1;
    }
    imagen_t *moto_2 = figura_pegar(rom, 11284, 46, 63) ;
    if( moto_2 == NULL){
        destruccion_imagenes( imagenes, 20);
        imagen_destruir(moto_0);
        imagen_destruir(moto_1);
        destruccion_imagenes(teselas, CANTIDAD_TESELAS);
        return 1;
    }
    imagen_t *moto_3 = figura_pegar(rom, 17215, 60, 54) ;
    if( moto_3== NULL){
        destruccion_imagenes(imagenes, 20);
        imagen_destruir(moto_0);
        imagen_destruir(moto_1);
        imagen_destruir(moto_2);
        destruccion_imagenes(teselas, CANTIDAD_TESELAS);
        return 1;
    }
    imagen_t *moto_4 = imagen_espejar(moto_1);  
    if( moto_4 == NULL){
        destruccion_imagenes(imagenes, 20);
        imagen_destruir(moto_0);
        imagen_destruir(moto_1);
        imagen_destruir(moto_2);
        imagen_destruir(moto_3);
        destruccion_imagenes(teselas, CANTIDAD_TESELAS);
        return 1;
    }
    imagen_t *moto_5 = imagen_espejar(moto_2);
    if( moto_5 == NULL){
        destruccion_imagenes(imagenes, 20);
        imagen_destruir(moto_0);
        imagen_destruir(moto_1);
        imagen_destruir(moto_2);
        imagen_destruir(moto_3);
        imagen_destruir(moto_4);
        destruccion_imagenes(teselas, CANTIDAD_TESELAS);
        return 1;
    }
    
    imagen_t *moto_6 = imagen_espejar(moto_3); 
    if( moto_6 == NULL){
        destruccion_imagenes(imagenes, 20);
        imagen_destruir(moto_0);
        imagen_destruir(moto_1);
        imagen_destruir(moto_2);
        imagen_destruir(moto_3);
        imagen_destruir(moto_4);
        imagen_destruir(moto_5);
        destruccion_imagenes(teselas, CANTIDAD_TESELAS);
        return 1;
    }

    imagen_t *motos[7] = {moto_0, moto_1, moto_2, moto_3, moto_4, moto_5, moto_6};

    size_t paleta_rueda = 4;

    moto_t *moto_creada = moto_crear(motos[0], paleta_4[paleta_rueda],36,73);
    if(moto_creada == NULL){
        destruccion_imagenes( imagenes, 20);  
        destruccion_imagenes(motos, 7);
        destruccion_imagenes(teselas, CANTIDAD_TESELAS);

        return 1;
    }
    imagen_t *cuadro = imagen_generar(320, 224, 0xfff);
        if( cuadro == NULL){
            destruccion_imagenes(imagenes,20);
            destruccion_imagenes(motos, 7);
            destruccion_imagenes(teselas, CANTIDAD_TESELAS);
            return 1;
    }
    imagen_t *cuadro_inicial = imagen_generar(320,224,0x00f);
    if(cuadro_inicial == NULL){
        destruccion_imagenes(imagenes,20);
        destruccion_imagenes(motos, 7);
        destruccion_imagenes(teselas, CANTIDAD_TESELAS);
        imagen_destruir(cuadro);
        return 1;
    }
    
   
    float contador = 0;
    float pausa = 8.0f;

    size_t paleta_rueda1 = 4;
    size_t paleta_rueda2 = 6;
    size_t pos_x_anterior = 0;

    // END código del alumno

    unsigned int ticks = SDL_GetTicks();

    while(moto_get_posicion_x(moto_creada) < 4200 && tiempo_restante > 0) {

        if(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                break;
            // BEGIN código del alumno
            if (event.type == SDL_KEYDOWN) {
                // Se apretó una tecla
                switch(event.key.keysym.sym) {
                    case SDLK_UP:
                        acelerado = true;
                        break;
                    case SDLK_DOWN:
                        frenado = true;
                        break;
                    case SDLK_RIGHT:
                        mover_derecha = true;
                        break;
                    case SDLK_LEFT:
                        mover_izquierda = true;
                        break;

                    case SDLK_w:
                        acelerado = true;
                        break;
                    case SDLK_s:
                        frenado = true;
                        break;
                    case SDLK_d:
                        mover_derecha = true;
                        break;
                    case SDLK_a:
                        mover_izquierda = true;
                        break;
                }
            }
            else if (event.type == SDL_KEYUP) {
                // Se soltó una tecla
                switch(event.key.keysym.sym) {
                    case SDLK_UP:
                        acelerado = false;                        
                        break;
                    case SDLK_DOWN:
                        frenado = false;
                        break;
                    case SDLK_RIGHT:
                        mover_derecha = false;
                        break;
                    case SDLK_LEFT:
                        mover_izquierda = false;
                        break;

                    case SDLK_w:
                        acelerado = false;
                        break;
                    case SDLK_s:
                        frenado = false;
                        break;
                    case SDLK_d:
                        mover_derecha = false;
                        break;
                    case SDLK_a:
                        mover_izquierda = false;
                        break;
                }
            }
            // END código del alumno
            continue;
        }

        // BEGIN código del alumno
        imagen_pegar(cuadro, cuadro_inicial, 0,0);
    //tiempo-----------------------------------------------

        if(pausa <= 0){
            tiempo_restante -= 0.0000000001f;
            moto_set_velocidad(moto_creada, acelerado, frenado, tick_tiempo);
             
            moto_set_tiempo_jugado(moto_creada, (tiempo_inicial - tiempo_restante));
            moto_set_distancia_x(moto_creada, tick_tiempo);
        
            moto_set_score(moto_creada, moto_get_distancia_tiempo(moto_creada, tick_tiempo));

            moto_giro = 0;

            if(mover_derecha){
                moto_giro = 1;
            }
            if(mover_izquierda){
                moto_giro = -1;
            }
        }
        
        if(tiempo_restante != tiempo_inicial && contador < 84)
            tiempo_restante -= tick_tiempo;

        contador += tick_tiempo;
        pausa -= tick_tiempo;

        if(contador > 82){
            pausa = 9999.9f;
            for(size_t j = 0 ; j < 16 ; j++){
                imagen_pegar_con_paleta(cuadro,teselas[(size_t)game_o[0][j]],100 + j*8, 93, paleta_3[8]);
                imagen_pegar_con_paleta(cuadro,teselas[(size_t)game_o[1][j]], 100 + j*8, 100, paleta_3[8]);
            }

        }

        

    //textos-------------------------------------------------------
        char aux[10];
        
        numero = "10000000"; 

        sprintf(aux, "%d", (int)tiempo_restante);
        numero_time = aux;
        texto(numero_time, cuadro, teselas, 160, 38, paleta_3[5]);

        sprintf(aux, "%d", (int)(moto_get_velocidad(moto_creada)));
        n_speed = aux; 
        texto(n_speed, cuadro, teselas, 260, 38, paleta_3[5]); 
        
        sprintf(aux, "%d", (int)(moto_get_score(moto_creada)));
        n_score = aux; 
        texto(n_score, cuadro, teselas, 260, 20, paleta_3[7]);
        
        texto(numero, cuadro, teselas, 64, 20, paleta_3[8]);
        texto(stage, cuadro, teselas, 25, 38, paleta_3[5]);
        texto(n_stage, cuadro, teselas, 70, 38, paleta_3[5]);
        
        texto(speed, cuadro, teselas, 210, 38, paleta_3[6]);
        texto(km,cuadro, teselas,284,38,paleta_3[5] );
    
        pixel_textos(5, tesela, cuadro, teselas, 20, 22, 20, 18, paleta_3[5]);

        pixel_textos(7, score, cuadro, teselas, 200, 12, 20, 28, paleta_3[5] );
    
        pixel_textos(6, time, cuadro, teselas, 145, 12, 20, 28, paleta_3[5]);

        //fondos---------------------------------------------------------

         if(x_fondo2 > 320) x_fondo2 = -1216;
        if(x_fondo2 < -1216) x_fondo2 = 320;
        if(x_fondo1 > 320) x_fondo1 = -1728;
        if(x_fondo1 < -1728) x_fondo1 = 320;

        imagen_pegar(cuadro, fondo2, x_fondo2, 64);
        imagen_pegar(cuadro, fondo1, x_fondo1, 112);

        imagen_pegar(cuadro, pasto_estirado, 0, 128);

        size_t pos_x_actual = (size_t)(moto_get_posicion_x(moto_creada));

        for(size_t i = pos_x_anterior; i < pos_x_actual; i++){
            x_fondo1 -= ruta[i].radio_curva;
            x_fondo2 -= (ruta[i].radio_curva)*0.75;
            moto_giro_ruta(moto_creada, moto_get_distancia_tiempo(moto_creada, tick_tiempo), ruta[ruta_i].radio_curva);
        }

        pos_x_anterior = pos_x_actual;
               
    //Ruta--------------------------------------------------------------------- 

        size_t figuras_encontradas[20] = {0};
        float desplazamientos_para_figuras[20] = {0};
        
        size_t i_encontrada = 0;
        float uc_anterior = 0;

        for(size_t f = 112; f > 16; f--){
            
            float aux_u;
            int aux_v;

            for(size_t c = 0; c < 1024; c++)
                imagen_set_pixel(ruta_aux, c,0, imagen_get_pixel(ruta_completa, c, f));

            
            int v_franja = 224 - (112 + f);

            aux_v = 224 - v_franja;

            ruta_i = (size_t)(d(v_franja) + moto_get_posicion_x(moto_creada));
            
            float ul = u_l(v_franja, moto_get_posicion_y(moto_creada));

            float uc = u_c(v_franja, ruta[ruta_i].radio_curva, uc_anterior);
                 
            uc_anterior = uc;

            float ur = ul + uc;

            aux_u = -346 + ur;

            imagen_pegar_con_paleta(cuadro, ruta_aux, aux_u, aux_v, colores_ruta[(size_t)(ruta_i % 4)]);
            
        }

    //Figuras en ruta-----------------------------------------------------

        uc_anterior = 0;

        for(size_t i = 0; i < 60; i++){
            size_t aux = i + (size_t)(moto_get_posicion_x(moto_creada));

            int v_fig = v((float)aux, moto_get_posicion_x(moto_creada));

            size_t figura_i = ruta[aux].indice_figura;

            float ul = u_l(v_fig, moto_get_posicion_y(moto_creada));

            float uc = u_c(v_fig, ruta[aux].radio_curva, uc_anterior);
                
            uc_anterior = uc;

            float ur = ul + uc;

            if(figura_i != 9999){

                bool ya_la_encontre_antes = false;
                for(size_t j = i_encontrada; j > 0; j--){
                    if(figuras_encontradas[j] == aux){
                        ya_la_encontre_antes = true;
                    }        
                }
                if(!ya_la_encontre_antes){
                    figuras_encontradas[i_encontrada] = aux;

                    desplazamientos_para_figuras[i_encontrada++] = ur;
                }
                
            }
        }

        uc_anterior = 0;

        for(size_t i = 0; i < i_encontrada; i++){

            float aux_u;
            int aux_v;
            int veces = 1;

            size_t figura_i = ruta[(figuras_encontradas[i])].indice_figura;
            
            if(figura_i == 13 || figura_i == 15){
                veces = 2;    
            }
   
            for(size_t j = 0; j < veces; j++){
                figura_i += j;

                struct figura_en_ruta figura_actual = figuras_en_ruta[figura_i];
                
                imagen_t *fig = figura_a_pegar(figuras_encontradas[i], moto_get_posicion_x(moto_creada), figura_actual, figuras, &aux_u, &aux_v, desplazamientos_para_figuras[i]);
                if(fig == NULL){
                    imagen_destruir(fig);
                    destruccion_imagenes(imagenes,20);
                    destruccion_imagenes(motos, 7);
                    destruccion_imagenes(teselas, CANTIDAD_TESELAS);
                    imagen_destruir(cuadro);
                    return 1;
                }

                aux_v = 224 - aux_v;

                int ancho_fig = (int)((imagen_get_ancho(fig))/2);

                imagen_pegar_con_paleta(cuadro, fig, (int)(aux_u + 162), aux_v, paleta_4[figura_actual.paleta]);
                imagen_destruir(fig);

                int ancho_moto = (int)(imagen_get_ancho(moto_get_imagen(moto_creada)) / 2) ;

                if(!(figura_i >= 13 && figura_i <= 16)){
                    if(((ancho_moto > (int)(aux_u))&&(((-1) * ancho_moto) < ((int)(aux_u) + ancho_fig))) && ((figuras_encontradas[i] - moto_get_posicion_x(moto_creada)) < ((int)(77.5 * tick_tiempo)))){
                        moto_choque(moto_creada);
                         moto_giro = 0;
                        pausa = 5.0f;
                        
                    }
                    
                    
                }
                
            }
           
        }
    //Semaforo-------------------------------------------------------

        if(contador < 2){
            imagen_pegar_con_paleta(cuadro, semaforo_escalado,46,54 , paleta_4[41]);
      
        }
        if(contador >2 && contador < 4){
            imagen_pegar_con_paleta(cuadro, semaforo_escalado,46,54 , paleta_4[42]);
        }
        if(contador > 4 && contador <6){
            imagen_pegar_con_paleta(cuadro, semaforo_escalado,46,54 , paleta_4[43]);
        }
      
    //Moto----------------------------------------------------------

        if((int)moto_get_distancia_tiempo(moto_creada, tick_tiempo) >= 1){

            if(paleta_rueda1 != 4 && paleta_rueda1 != 5)
                paleta_rueda1 = 4;

            if(paleta_rueda1 == 4)
                moto_set_paleta(moto_creada, paleta_4[paleta_rueda1++]);

            else if(paleta_rueda1 == 5)
                moto_set_paleta(moto_creada, paleta_4[paleta_rueda1--]);
        
        }
        else if(frenado){
            if(paleta_rueda2 == 6)
                moto_set_paleta(moto_creada, paleta_4[paleta_rueda2++]);

            else if(paleta_rueda2 == 7)
                moto_set_paleta(moto_creada, paleta_4[paleta_rueda2--]);
            
        }
        else
            moto_set_paleta(moto_creada, paleta_4[paleta_rueda1]);

        if(moto_giro == 0){
            int aux_x;
            int aux_y;
            moto_centrar(moto_creada, &moto_i, &moto_girada_hacia, motos, &aux_x, &aux_y);
            imagen_pegar_con_paleta(cuadro, moto_get_imagen(moto_creada), aux_x, aux_y, moto_get_paleta(moto_creada));
        }
        else{
            int aux_x;
            moto_girar(moto_creada, moto_giro, &moto_i, &moto_girada_hacia, motos, &aux_x);
            imagen_pegar_con_paleta(cuadro, moto_get_imagen(moto_creada), aux_x, 146 + (7*moto_i), moto_get_paleta(moto_creada));

        }
        moto_set_tiempo_jugado(moto_creada, (tiempo_inicial - tiempo_restante));
        moto_set_distancia_x(moto_creada, tick_tiempo);
        
            moto_set_score(moto_creada, moto_get_distancia_tiempo(moto_creada, tick_tiempo));


        // Procedemos a dibujar a pantalla completa:
        imagen_t *cuadro_escalado = imagen_escalar(cuadro, VENTANA_ANCHO, VENTANA_ALTO);
        if(cuadro_escalado == NULL){
            destruccion_imagenes(imagenes, 20);
            destruccion_moto(moto_creada);
            destruccion_imagenes(motos,7);
            destruccion_imagenes(teselas,CANTIDAD_TESELAS);
            imagen_destruir(cuadro);

        }
        
        imagen_a_textura(cuadro_escalado, canvas);

        imagen_destruir(cuadro_escalado);
       
        // END código del alumno

        SDL_UpdateTexture(texture, NULL, canvas, VENTANA_ANCHO * sizeof(uint16_t));
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        ticks = SDL_GetTicks() - ticks;
        if(dormir) {
            SDL_Delay(dormir);
            dormir = 0;
        }
        else if(ticks < 1000 / JUEGO_FPS)
            SDL_Delay(1000 / JUEGO_FPS - ticks);
        else
            printf("Perdiendo cuadros\n");
        ticks = SDL_GetTicks();
   
    }

    // BEGIN código del alumno

    
    destruccion_imagenes(motos, 7);
    destruccion_imagenes(imagenes, 20);
    destruccion_imagenes(teselas, CANTIDAD_TESELAS);
    destruccion_moto(moto_creada);
    imagen_destruir(cuadro_inicial);
    imagen_destruir(cuadro);
    
    // END código del alumno

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}

