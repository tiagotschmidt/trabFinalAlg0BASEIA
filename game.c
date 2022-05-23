// Trabalho Final ERE - Gabriel Castelo Branco Gomes e Tiago Torres Schmidt.

#include <stdio.h> //Bibliotecas utilizadas no projeto. Projeto desenvolvido utilizando Allegro.
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "engine.h"

#define KEY_SEEN     1//Constantes utilizadas no projeto.
#define KEY_RELEASED 2
#define N_LINHAS 11
#define N_COLUNAS 27
#define TEMPO_ESPERA 20
#define ESCALA 50
#define SAVEFILE_ID "currentSave.bin"
#define MAP "mapa"
#define MAPENDFILE_ID ".txt"
#define MAX_LEVEL 99

int main()//Função main, executa o projeto.
{
    srand(time(NULL));//Variáveis principais do projeto.
    int randomItem;
    int i,j;
    int onMenu = 1;
    int scoreCounter = 00;
    int hp = 3;
    int leverCooldown;
    int imortalCooldown;
    int hasSword = 0;
    int currentLevel = 1;
    int isInverted = 0;
    int isLeverActivated = 0;
    bool done = false;
    bool redraw = true;
    float x, y;
    FILE *saveLocation;
    unsigned char tecla[ALLEGRO_KEY_MAX];
    ALLEGRO_EVENT event;
    char map[N_LINHAS][N_COLUNAS];
    char mapFile[10];
    obj objImage[N_LINHAS][N_COLUNAS];

    inicializa(al_init(), "Allegro.");//Inicialização da Allegro...
    inicializa(al_install_keyboard(), "Teclado.");
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 15.0);//Framerate e velocidade do jogo.
    inicializa(timer, "Temporizador.");
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    inicializa(queue, "Fila de eventos");

    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    ALLEGRO_DISPLAY* disp = al_create_display(ESCALA*N_COLUNAS , N_LINHAS*ESCALA);//Tamanho do display.
    inicializa(disp, "Tela.");
    ALLEGRO_FONT* font = al_create_builtin_font();
    inicializa(font, "Fonte.");
    inicializa(al_init_primitives_addon(), "Adicionais primitivos.");
    inicializa(al_init_image_addon(), "Adicional de imagem.");

    ALLEGRO_BITMAP* heroI = al_load_bitmap("images/heroF.png");//Inicialização de sprites gerais do jogo.
    ALLEGRO_BITMAP* heroGI = al_load_bitmap("images/heroGF.png");
    ALLEGRO_BITMAP* heroRI = al_load_bitmap("images/heroRF.png");

    ALLEGRO_BITMAP* heroII = al_load_bitmap("images/heroIF.png");
    ALLEGRO_BITMAP* heroGII = al_load_bitmap("images/heroIGF.png");
    ALLEGRO_BITMAP* heroRII = al_load_bitmap("images/heroIRF.png");

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    memset(tecla, 0, sizeof(tecla));

    al_start_timer(timer);
    while(1) //Loop do game.
    {
        if(onMenu)//If para uso de menu.
        {
            al_wait_for_event(queue, &event);

            switch(event.type)//Switch para recepção do evento acionado pelo usuário.
            {
                case ALLEGRO_EVENT_TIMER:
                    if(tecla[ALLEGRO_KEY_N])//Opção de novo jogo.
                    {
                        currentLevel = 1;
                        readMap(saveLocation,map,&currentLevel,mapFile);
                        loadMap(map,objImage,&x,&y);
                        scoreCounter = 00;
                        hp = 3;
                        leverCooldown = 00;
                        imortalCooldown = 00;
                        hasSword = 0;
                        onMenu = 0;
                    }

                    if(tecla[ALLEGRO_KEY_C])//Opção de carregar o jogo.
                    {
                        loadSave(saveLocation,objImage,map,&scoreCounter,&hp,&leverCooldown,&imortalCooldown,&hasSword,&x,&y,&currentLevel, &isLeverActivated, &isInverted);
                        onMenu = 0;
                    }

                    if(tecla[ALLEGRO_KEY_S])//Opção de salvar jogo.
                    {
                        saveMap(saveLocation,objImage,map,scoreCounter,hp,leverCooldown,imortalCooldown,hasSword,x,y,currentLevel,isLeverActivated,isInverted);
                    }

                    if(tecla[ALLEGRO_KEY_V])//Opção de voltar para o jogo.
                    {
                        onMenu = 0;
                    }

                    if(tecla[ALLEGRO_KEY_Q])//Opção de sair do jogo.
                    {
                        done = true;
                    }

                    for(int i = 0; i < ALLEGRO_KEY_MAX; i++)//Sistema de leitura para duas teclas.
                    {
                        tecla[i] &= KEY_SEEN;
                    }


                    redraw = true;
                    break;

                case ALLEGRO_EVENT_KEY_DOWN:
                    tecla[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
                    break;

                case ALLEGRO_EVENT_KEY_UP:
                    tecla[event.keyboard.keycode] &= KEY_RELEASED;
                    break;

                case ALLEGRO_EVENT_DISPLAY_CLOSE:
                    done = true;
                    break;
            }

            if(done){
                break;
            }

            if(redraw && al_is_event_queue_empty(queue))
            {
                al_clear_to_color(al_map_rgb(0, 0, 0));//Visualização do menu.
                al_draw_textf(font, al_map_rgb(255, 255, 255), 8*ESCALA, 5*ESCALA, 0, "Novo Jogo(N)|");
                al_draw_textf(font, al_map_rgb(255, 255, 255), 10.25*ESCALA, 5*ESCALA, 0, "Carregar Jogo(C)|");
                al_draw_textf(font, al_map_rgb(255, 255, 255), 13.25*ESCALA, 5*ESCALA, 0, "Salvar Jogo(S)|");
                al_draw_textf(font, al_map_rgb(255, 255, 255), 16*ESCALA, 5*ESCALA, 0, "Sair do Jogo(Q)|");
                al_draw_textf(font, al_map_rgb(255, 255, 255), 19*ESCALA, 5*ESCALA, 0, "Voltar(V)");
                al_draw_textf(font, al_map_rgb(255, 255, 255), 24*ESCALA, 0, 0, "Pontuação: %d",scoreCounter);
                al_draw_textf(font, al_map_rgb(255, 255, 255), 5*ESCALA, 0, 0, "%s",mapFile);
                al_flip_display();

                redraw = false;
            }

        }else
        {//Loop do próprio jogo.
            randomItem =  rand() % 4;//Item randômico para os ogros.
            al_wait_for_event(queue, &event);

            if(checkLevel(objImage, &currentLevel))//Primeira atividade: checkar o nivel do jogo, se sim, avança o mapa.
            {
                if(currentLevel > MAX_LEVEL)
                {
                    currentLevel = 1;//Mecanismo para 99 níveis.
                }
                readMap(saveLocation,map,&currentLevel, mapFile);
                loadMap(map,objImage,&x,&y);
                hp = 3;
                leverCooldown = 00;
                imortalCooldown = 00;
                hasSword = 0;
            }

            updateOgre(objImage, &x, &y);//Realiza a inteligênia dos ogros.
            updateOgre(objImage, &x, &y);//Realiza a inteligênia dos ogros.
            /*updateOgre(objImage, &x, &y);//Realiza a inteligênia dos ogros.
            updateOgre(objImage, &x, &y);//Realiza a inteligênia dos ogros.
            updateOgre(objImage, &x, &y);//Realiza a inteligênia dos ogros.
            updateOgre(objImage, &x, &y);//Realiza a inteligênia dos ogros.*/

            ogreHit(objImage,&x,&y,map,&hp,&scoreCounter,&onMenu,&imortalCooldown,&currentLevel,&leverCooldown,&hasSword);//Verifica o sistema de hits.

            giveSword(objImage,&hasSword);//Verifica o sistema de espadas.

            if((objImage[(int)y/ESCALA][(int)x/ESCALA].identity == 'M') || (objImage[(int)y/ESCALA][(int)x/ESCALA].identity == 'C'))
            {
                scoreCounter = collectItem(scoreCounter,objImage, x, y, &imortalCooldown);//Sistema para coletar itens.
            }

            if(leverCooldown > 0)
            {
                leverCooldown--;//Reseta o tempo de recarga das alavancas.
            }

            if(imortalCooldown > 0)
            {
                imortalCooldown--;//Reseta o tempo de imortalidade.
            }

            switch(event.type)//Sistema de leitura de movimentação de ativação de alavancas.
            {
                case ALLEGRO_EVENT_TIMER:
                    if(tecla[ALLEGRO_KEY_UP])
                    {
                        if(y == 0 || (objImage[(int)(y-ESCALA)/ESCALA][(int)x/ESCALA].body == 1)){}else
                        {
                            y-=ESCALA;
                        }
                    }

                    if(tecla[ALLEGRO_KEY_DOWN])
                    {
                        if(y == 10*ESCALA || (objImage[(int)(y+ESCALA)/ESCALA][(int)x/ESCALA].body == 1)){}else
                        {
                            y+=ESCALA;
                        }
                    }

                    if(tecla[ALLEGRO_KEY_LEFT])
                    {
                        if(x == 0 || (objImage[(int)y/ESCALA][(int)(x-ESCALA)/ESCALA].body == 1)){}else
                        {
                            x-=ESCALA;
                            isInverted = 1;
                        }
                    }

                    if(tecla[ALLEGRO_KEY_RIGHT])
                    {
                        if(x == 26*ESCALA || (objImage[(int)y/ESCALA][(int)(x+ESCALA)/ESCALA].body == 1)){}else
                        {
                            x+=ESCALA;
                            isInverted = 0;
                        }
                    }

                    if(tecla[ALLEGRO_KEY_ESCAPE])
                    {
                        done = true;
                    }

                    if(tecla[ALLEGRO_KEY_TAB])
                    {
                        onMenu = 1;
                    }

                    if((tecla[ALLEGRO_KEY_B] && ((objImage[(int)y/ESCALA][(int)x/ESCALA].identity == 'B') || (objImage[(int)(y-ESCALA)/ESCALA][(int)x/ESCALA].identity == 'B') || (objImage[(int)(y+ESCALA)/ESCALA][(int)x/ESCALA].identity == 'B') || (objImage[(int)y/ESCALA][(int)(x-ESCALA)/ESCALA].identity == 'B')|| (objImage[(int)y/ESCALA][(int)(x+ESCALA)/ESCALA].identity == 'B'))) && (leverCooldown == 0))
                    {
                        leverActivate(&leverCooldown, objImage, &isLeverActivated);
                    }

                    for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
                    {
                        tecla[i] &= KEY_SEEN;
                    }


                    redraw = true;
                    break;

                case ALLEGRO_EVENT_KEY_DOWN:
                    tecla[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
                    break;

                case ALLEGRO_EVENT_KEY_UP:
                    tecla[event.keyboard.keycode] &= KEY_RELEASED;
                    break;

                case ALLEGRO_EVENT_DISPLAY_CLOSE:
                    done = true;
                    break;
            }

            if(done){
                break;
            }

            if(redraw && al_is_event_queue_empty(queue))//Sistema para projeção dos gráficos.
            {
                al_clear_to_color(al_map_rgb(0, 0, 0));
                drawMap(objImage, isLeverActivated);
                if(hasSword)
                {
                    al_draw_textf(font, al_map_rgb(255, 255, 255), 12*ESCALA, 0, 0, "Excalibur ativada.");
                }
                if(imortalCooldown > 0)
                {
                    if(imortalCooldown % 2 == 0)
                    {
                        if(isInverted)
                        {
                             al_draw_bitmap(heroGII, x,y, 0);
                        }else
                        {
                            al_draw_bitmap(heroGI, x,y, 0);
                        }
                        //al_draw_filled_rectangle(x, y, x + ESCALA-1, y + ESCALA-1, al_map_rgb(  43,255,0));

                    }else
                    {
                        if(isInverted)
                        {
                            al_draw_bitmap(heroRII, x,y, 0);
                        }else
                        {
                            al_draw_bitmap(heroRI, x,y, 0);
                        }
                        //al_draw_filled_rectangle(x, y, x + ESCALA-1, y + ESCALA-1, al_map_rgb(255,0,0));
                    }

                }else
                {
                    if(isInverted)
                    {
                        al_draw_bitmap(heroII, x,y, 0);
                    }else
                    {
                        al_draw_bitmap(heroI, x,y, 0);
                    }
                    //al_draw_filled_rectangle(x, y, x + ESCALA-1, y + ESCALA-1, al_map_rgb(153,102,255));
                }
                //al_draw_textf(font, al_map_rgb(255, 255, 255), 0, 0, 0, "X: %.1f Y: %.1f", x, y);
                al_draw_textf(font, al_map_rgb(255, 255, 255), 24*ESCALA, 0, 0, "Pontuação: %d",scoreCounter);
                al_draw_textf(font, al_map_rgb(255, 255, 255), 22*ESCALA, 0, 0, "Vida: %d",hp);
                al_flip_display();

                redraw = false;
            }
        }
    }

    al_destroy_font(font);//Encerra os addons quando o while é quebrado.
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}
