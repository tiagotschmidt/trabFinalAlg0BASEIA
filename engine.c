#include "engine.h"

void inicializa(bool teste, const char *descricao)//Função de inicialização geral, para os adicionais do Allegro.
{
    if(teste) return;

    printf("Não foi possivel inicializar %s\n", descricao);
    exit(1);
}

void loadMap(char src[N_LINHAS][N_COLUNAS], obj objImage[N_LINHAS][N_COLUNAS], float* x, float* y)//Função loadMap, converte uma matriz de caracteres em uma matriz de obj.
{
    int i,j;
    for(i=0;i<N_COLUNAS;i++)
    {
        for(j=0;j<N_LINHAS;j++)
        {
            switch(src[j][i])
            {
                case '#':
                    objImage[j][i].body = 1;
                    objImage[j][i].colectable = 0;
                    objImage[j][i].player = 0;
                    objImage[j][i].shiftable = 0;
                    objImage[j][i].identity = '#';
                    break;
                case 'C':
                    objImage[j][i].body = 0;
                    objImage[j][i].colectable = 1;
                    objImage[j][i].player = 0;
                    objImage[j][i].shiftable = 0;
                    objImage[j][i].identity = 'C';
                    objImage[j][i].lastStep = 'C';
                    break;
                case 'A':
                    objImage[j][i].body = 0;
                    objImage[j][i].colectable = 0;
                    objImage[j][i].player = 0;
                    objImage[j][i].shiftable = 1;
                    objImage[j][i].identity = 'A';
                    break;
                case 'D':
                    objImage[j][i].body = 1;
                    objImage[j][i].colectable = 0;
                    objImage[j][i].player = 0;
                    objImage[j][i].shiftable = 1;
                    objImage[j][i].identity = 'D';
                    break;
                case 'J':
                    objImage[j][i].body = 0;
                    objImage[j][i].colectable = 0;
                    objImage[j][i].player = 1;
                    objImage[j][i].shiftable = 0;
                    objImage[j][i].identity = 'J';
                    objImage[j][i].lastStep = 'J';
                    *y = j*ESCALA;
                    *x = i*ESCALA;
                    break;
                case 'M':
                    objImage[j][i].body = 0;
                    objImage[j][i].colectable = 1;
                    objImage[j][i].player = 0;
                    objImage[j][i].shiftable = 0;
                    objImage[j][i].identity = 'M';
                    objImage[j][i].lastStep = 'M';
                    break;
                case 'B':
                    objImage[j][i].body = 0;
                    objImage[j][i].colectable = 0;
                    objImage[j][i].player = 0;
                    objImage[j][i].shiftable = 1;
                    objImage[j][i].identity = 'B';
                    break;
                case 'O':
                    objImage[j][i].body = 0 ;
                    objImage[j][i].colectable = 0;
                    objImage[j][i].player = 0;
                    objImage[j][i].shiftable = 0;
                    objImage[j][i].identity = 'O';
                    objImage[j][i].cd = 0;
                    break;
            }
        }
    }
}

int collectItem(int counter,obj objImage[N_LINHAS][N_COLUNAS], int x, int y, int* imortalCooldown)//Função collectItem, ativada para coletar moedas ou chaves.
{
    if(objImage[(int)y/ESCALA][(int)x/ESCALA].identity == 'M')
    {
        counter += 10;//Aumenta a pontuação.
    }
    if(counter % 100 == 0)
    {
        *imortalCooldown = (15 * 5);//Sistema de imortalidade.
    }
    objImage[(int)y/ESCALA][(int)x/ESCALA].identity = ' ';//Transforma a posição da moeda em nada, "excluindo-a".
    objImage[(int)y/ESCALA][(int)x/ESCALA].body = 0;
    objImage[(int)y/ESCALA][(int)x/ESCALA].colectable = 0;
    objImage[(int)y/ESCALA][(int)x/ESCALA].player = 0;
    objImage[(int)y/ESCALA][(int)x/ESCALA].shiftable = 0;
    objImage[(int)y/ESCALA][(int)x/ESCALA].lastStep = ' ';

    return counter;
}

void leverActivate(int* cd,obj objImage[N_LINHAS][N_COLUNAS], int* isLever)//Função leverActivate, realiza a mecânica de portas.
{
    int i,j;
    for(i=0;i<N_COLUNAS;i++)//Procura todas as portas do jogo, e inverte o estado de cada uma.
    {
        for(j=0;j<N_LINHAS;j++)
        {
            if(objImage[j][i].identity == 'A')
            {
                objImage[j][i].identity = 'D';
                objImage[j][i].body = 1;
                *cd = TEMPO_ESPERA;
            }
            else if(objImage[j][i].identity == 'D')
            {
                objImage[j][i].identity = 'A';
                objImage[j][i].body = 0;
                *cd = TEMPO_ESPERA;
            }
        }
    }

    if(*isLever)//Controle de animação da alavanca.
    {
        *isLever = 0;
    }else
    {
        *isLever = 1;
    }

}

void updateOgre(obj objImage[N_LINHAS][N_COLUNAS],float* playerX,float* playerY)//Função updateOgre, em geral, é a "inteligência artifical" dos ogros.
{
    int i,j;
    int direction;
    for(i=0;i<N_COLUNAS;i++)
    {
        for(j=0;j<N_LINHAS;j++)
        {
            if((objImage[j][i].identity == 'O')&&(objImage[j][i].cd == 0))
            {
                pathFinder(objImage, playerX, playerY, &direction);
                switch(direction)
                {
                    case 0:
                        if((objImage[j][i-1].body == 0) && (objImage[j][i-1].identity != 'O') && (objImage[j][i-1].identity != 'D')  && ((i-1)*ESCALA > 00))
                        {
                            objImage[j][i-1].lastStep = objImage[j][i-1].identity;


                            objImage[j][i].body = objImage[j][i-1].body;
                            objImage[j][i].colectable = objImage[j][i-1].colectable;
                            objImage[j][i].player = objImage[j][i-1].player;
                            objImage[j][i].shiftable =  objImage[j][i-1].shiftable;
                            objImage[j][i].identity = objImage[j][i].lastStep;

                            objImage[j][i-1].body = 0;
                            objImage[j][i-1].colectable = 0;
                            objImage[j][i-1].player = 0;
                            objImage[j][i-1].shiftable = 0;
                            objImage[j][i-1].identity = 'O';
                            objImage[j][i-1].cd = TEMPO_ESPERA;
                        }
                        break;
                    case 1:
                        if((objImage[j-1][i].body == 0) && (objImage[j-1][i].identity != 'O') && (objImage[j-1][i].identity != 'D') && ((j-1)*ESCALA > 00))
                        {
                            objImage[j-1][i].lastStep = objImage[j-1][i].identity;

                            objImage[j][i].body = objImage[j-1][i].body;
                            objImage[j][i].colectable = objImage[j-1][i].colectable;
                            objImage[j][i].player = objImage[j-1][i].player;
                            objImage[j][i].shiftable =  objImage[j-1][i].shiftable;
                            objImage[j][i].identity = objImage[j][i].lastStep;

                            objImage[j-1][i].body = 0;
                            objImage[j-1][i].colectable = 0;
                            objImage[j-1][i].player = 0;
                            objImage[j-1][i].shiftable = 0;
                            objImage[j-1][i].identity = 'O';
                            objImage[j-1][i].cd = TEMPO_ESPERA;
                        }
                        break;
                    case 2:
                        if((objImage[j][i+1].body == 0 )  && (objImage[j][i+1].identity != 'D') && (objImage[j][i+1].identity != 'O')  && ((i+1)*ESCALA < N_COLUNAS*ESCALA))
                        {
                            objImage[j][i+1].lastStep = objImage[j][i+1].identity;

                            objImage[j][i].body = objImage[j][i+1].body;
                            objImage[j][i].colectable = objImage[j][i+1].colectable;
                            objImage[j][i].player = objImage[j][i+1].player;
                            objImage[j][i].shiftable =  objImage[j][i+1].shiftable;
                            objImage[j][i].identity = objImage[j][i].lastStep;


                            objImage[j][i+1].body = 0;
                            objImage[j][i+1].colectable = 0;
                            objImage[j][i+1].player = 0;
                            objImage[j][i+1].shiftable = 0;
                            objImage[j][i+1].identity = 'O';
                            objImage[j][i+1].cd = TEMPO_ESPERA;
                        }
                        break;
                    case 3:
                        if((objImage[j+1][i].body == 0) && (objImage[j+1][i].identity != 'D') && (objImage[j+1][i].identity != 'O')  && ((j+1)*ESCALA < N_LINHAS*ESCALA))
                        {
                            objImage[j+1][i].lastStep = objImage[j+1][i].identity;

                            objImage[j][i].body = objImage[j+1][i].body;
                            objImage[j][i].colectable = objImage[j+1][i].colectable;
                            objImage[j][i].player = objImage[j+1][i].player;
                            objImage[j][i].shiftable =  objImage[j+1][i].shiftable;
                            objImage[j][i].identity = objImage[j][i].lastStep;


                            objImage[j+1][i].body = 0;
                            objImage[j+1][i].colectable = 0;
                            objImage[j+1][i].player = 0;
                            objImage[j+1][i].shiftable = 0;
                            objImage[j+1][i].identity = 'O';
                            objImage[j+1][i].cd = TEMPO_ESPERA;
                        }
                    default:
                        break;
                }
            }else if(objImage[j][i].cd > 0)
            {
                objImage[j][i].cd--;
            }
        }
    }
}

void ogreHit(obj objImage[N_LINHAS][N_COLUNAS],float* pX,float* pY,char src[N_LINHAS][N_COLUNAS],int* playerHp,int* counter,int* menu, int* imortalCooldown, int* currentLevel, int* leverCooldown, int *hasSword)
{//Função ogreHit, executa o sistema de ataque e vida do jogo. É responsável pelo ataque do ogro E do herói.
    int i,j,k,l;
    for(i=0;i<N_COLUNAS;i++)
    {
        for(j=0;j<N_LINHAS;j++)
        {
            if((objImage[j][i].identity == 'O')&&(*pX/ESCALA == i)&&(*pY/ESCALA == j))//Se os dois estão no mesmo local,
            {
                if(*hasSword || *imortalCooldown > 0)//Se o player tem a espada, ou está imortal.
                {
                    objImage[(int)*pY/ESCALA][(int)*pX/ESCALA].identity = ' ';//Mata ogro.
                    objImage[(int)*pY/ESCALA][(int)*pX/ESCALA].body = 0;
                    objImage[(int)*pY/ESCALA][(int)*pX/ESCALA].colectable = 0;
                    objImage[(int)*pY/ESCALA][(int)*pX/ESCALA].player = 0;
                    objImage[(int)*pY/ESCALA][(int)*pX/ESCALA].shiftable = 0;
                    *counter += 100;
                }else
                {//Se não, mata o player.
                    if(*imortalCooldown == 0)
                    {
                        for(k=0;k<N_COLUNAS;k++)
                        {
                            for(l=0;l<N_LINHAS;l++)
                            {
                                if(src[l][k] == 'J')
                                {
                                    *pX = k * ESCALA;//Reseta a posição através da src.
                                    *pY = l * ESCALA;
                                }
                            }
                        }

                        *playerHp -= 1;//Reduz o hp.

                        if(*playerHp == 0)
                        {
                            *menu = 1;//Reseta o game.
                            loadMap(src,objImage,pX,pY);
                            *playerHp = 3;
                            *counter = 0;
                            *currentLevel = 1;
                            *leverCooldown = 00;
                            *imortalCooldown = 00;
                            *hasSword = 0;
                        }
                    }
                }
            }
        }
    }
}

void giveSword(obj objImage[N_LINHAS][N_COLUNAS], int* hasSword)//Função giveSword, libera a espada para o guerreiro.
{
    int i,j;
    int totalCoins = 0;
    for(i=0;i<N_COLUNAS;i++)
    {
        for(j=0;j<N_LINHAS;j++)
        {
            if(objImage[j][i].identity == 'M')
            {
                totalCoins++;
            }
        }
    }
    if(totalCoins == 0)
    {
        *hasSword = 1;
    }
}

void drawMap(obj objImage[N_LINHAS][N_COLUNAS], int isLever)//Função drawMap, na verdade, é um apanhado dos recursos visuais da Allegro. Em geral, estão aqui todos os possíveis. Ou seja, os outros desenhos da Allegro não foram possíveis de ser retirado da main.
{
    int i,j;

    ALLEGRO_BITMAP* floorI = al_load_bitmap("images/floorF.png");
    ALLEGRO_BITMAP* wallI = al_load_bitmap("images/wallF.png");
    ALLEGRO_BITMAP* coinI = al_load_bitmap("images/coinF.png");
    ALLEGRO_BITMAP* leverI = al_load_bitmap("images/leverF.png");
    ALLEGRO_BITMAP* leverII = al_load_bitmap("images/leverIF.png");
    ALLEGRO_BITMAP* doorI = al_load_bitmap("images/doorF.png");
    ALLEGRO_BITMAP* keyI = al_load_bitmap("images/keyF.png");
    ALLEGRO_BITMAP* ogreI = al_load_bitmap("images/ogreF.png");

    for(i=0;i<N_COLUNAS;i++)//Carrega as imagens e desenha cada elemento.
    {
        for(j=0;j<N_LINHAS;j++)
        {
            switch(objImage[j][i].identity)
            {//Esses códigos comentados são, na verdade, a versão sem sprites.
                case '#':
                    //al_draw_filled_rectangle(i*ESCALA, j*ESCALA, i*ESCALA + ESCALA-1 , j*ESCALA + ESCALA-1 , al_map_rgb(172, 172, 172));
                    al_draw_bitmap(wallI, i*ESCALA, j*ESCALA, 0);
                    break;
                case 'J':
                    //al_draw_filled_rectangle(i*ESCALA, j*ESCALA, i*ESCALA + ESCALA-1 , j*ESCALA + ESCALA-1 , al_map_rgb(0, 0, 0));
                    al_draw_bitmap(floorI, i*ESCALA, j*ESCALA, 0);
                    break;
                case 'C':
                    //al_draw_filled_rectangle(i*ESCALA, j*ESCALA, i*ESCALA + ESCALA-1 , j*ESCALA + ESCALA-1 , al_map_rgb(255,165,0));
                    al_draw_bitmap(keyI, i*ESCALA, j*ESCALA, 0);
                    break;
                case 'M':
                    //al_draw_filled_rectangle(i*ESCALA, j*ESCALA, i*ESCALA + ESCALA-1 , j*ESCALA + ESCALA-1 , al_map_rgb(255, 247, 0));
                    al_draw_bitmap(coinI, i*ESCALA, j*ESCALA, 0);
                    break;
                case 'B':
                    //al_draw_filled_rectangle(i*ESCALA, j*ESCALA, i*ESCALA + ESCALA-1 , j*ESCALA + ESCALA-1 , al_map_rgb(139,69,19));
                    if(isLever)
                    {
                        al_draw_bitmap(leverII, i*ESCALA, j*ESCALA, 0);
                    }else
                    {
                        al_draw_bitmap(leverI, i*ESCALA, j*ESCALA, 0);
                    }
                    break;
                case 'O':
                    //al_draw_filled_rectangle(i*ESCALA, j*ESCALA, i*ESCALA + ESCALA-1 , j*ESCALA + ESCALA-1 , al_map_rgb(0,179,30));
                    al_draw_bitmap(ogreI, i*ESCALA, j*ESCALA, 0);
                    break;
                case 'A':
                    //al_draw_filled_rectangle(i*ESCALA, j*ESCALA, i*ESCALA + ESCALA-1 , j*ESCALA + ESCALA-1 , al_map_rgb(56 , 176, 222));
                    al_draw_bitmap(floorI, i*ESCALA, j*ESCALA, 0);
                    break;
                case 'D':
                    //al_draw_filled_rectangle(i*ESCALA, j*ESCALA, i*ESCALA + ESCALA-1 , j*ESCALA + ESCALA-1 , al_map_rgb(255 , 0, 0));
                    al_draw_bitmap(doorI, i*ESCALA, j*ESCALA, 0);
                    break;
                case ' ':
                    //al_draw_filled_rectangle(i*ESCALA, j*ESCALA, i*ESCALA + ESCALA-1 , j*ESCALA + ESCALA-1 , al_map_rgb(0, 0, 0));
                    al_draw_bitmap(floorI, i*ESCALA, j*ESCALA, 0);
                    break;
                default:
                    //al_draw_filled_rectangle(i*ESCALA, j*ESCALA, i*ESCALA + ESCALA-1 , j*ESCALA + ESCALA-1 , al_map_rgb(0, 0, 0));
                    al_draw_bitmap(floorI, i*ESCALA, j*ESCALA, 0);
                    break;
            }
        }
    }
}

void saveMap(FILE *arq, obj objImage[N_LINHAS][N_COLUNAS], char src[N_LINHAS][N_COLUNAS], int cScoreCounter, int cHp, int cLeverCooldown, int cImortalCooldown, int cHasSword, float cX, float cY, int cLevel, int isLever, int isInverted)
{
    mapState cMapState;//Cria um objeto com estado do mapa, e o salva.

    cMapState.cHasSword = cHasSword;
    cMapState.cImortalCooldown = cImortalCooldown;
    cMapState.cLeverCooldown = cLeverCooldown;
    cMapState.cHp = cHp;
    cMapState.cScoreCounter = cScoreCounter;
    cMapState.cX = cX;
    cMapState.cY = cY;
    cMapState.cLevel = cLevel;
    cMapState.cLeverS = isLever;
    cMapState.cPlayerS = isInverted;

    memcpy(cMapState.src,src,N_LINHAS*N_COLUNAS*sizeof(char));
    memcpy(cMapState.objImage,objImage,N_LINHAS*N_COLUNAS*sizeof(obj));

    arq = fopen(SAVEFILE_ID,"wb");//Abertura de arquivo, gravação e sinalização.
    if(arq == NULL){
        printf("Erro na abertura do arquivo dos funcionários.");
    }else{
        if(fwrite(&cMapState,sizeof(mapState),1,arq) != 1){
            printf("Erro na escrita.");
        }
    }
    fclose(arq);
}

void loadSave(FILE *arq, obj objImage[N_LINHAS][N_COLUNAS], char src[N_LINHAS][N_COLUNAS], int* cScoreCounter, int* cHp, int* cLeverCooldown, int* cImortalCooldown, int* cHasSword, float* cX, float* cY, int* cLevel, int* cIsLever, int* cIsInverted)
{//Função loadSave, carrega o save binário do game, através de uma máscara mapState.
    mapState cMapState;

    arq = fopen(SAVEFILE_ID,"rb+");//Abre o stream do arquivo.

    if(fread(&cMapState,sizeof(mapState),1,arq)){
        *cHasSword = cMapState.cHasSword;
        *cImortalCooldown = cMapState.cImortalCooldown;
        *cLeverCooldown = cMapState.cLeverCooldown;
        *cHp = cMapState.cHp;
        *cScoreCounter = cMapState.cScoreCounter;
        *cX = cMapState.cX;
        *cY = cMapState.cY;
        *cLevel = cMapState.cLevel;
        *cIsLever = cMapState.cLeverS;
        *cIsInverted = cMapState.cPlayerS;

        memcpy(src,cMapState.src,N_LINHAS*N_COLUNAS*sizeof(char));
        memcpy(objImage,cMapState.objImage,N_LINHAS*N_COLUNAS*sizeof(obj));
    }

    fclose(arq);
}

void readMap(FILE *arq,  char src[N_LINHAS][N_COLUNAS], int* cLevel, char mapFile[10])
{//Função readMap, lê o arquivo .txt para converter em um src.
    int i,j;
    char mapNumber[3];


    strcpy(mapFile,MAP);
    snprintf(mapNumber,sizeof(mapNumber),"%02d", *cLevel);
    strcat(mapFile,mapNumber);
    strcat(mapFile,MAPENDFILE_ID);//Diversas concatenações para definir o nome do arquivo de leitura.

    if(!(arq = fopen(mapFile,"rb+"))) //Abre o stream do arquivo.
    {
        *cLevel = 1;
        memset(mapFile, 0, 10);
        memset(mapNumber, 0, sizeof(mapNumber));
        strcpy(mapFile,MAP);
        snprintf(mapNumber,sizeof(mapNumber),"%02d", *cLevel);
        strcat(mapFile,mapNumber);
        strcat(mapFile,MAPENDFILE_ID);
        arq = fopen(mapFile,"rb+");
    }

    for(i=0;i<N_LINHAS;i++)
    {
        for(j=0;j<N_COLUNAS;j++)
        {
            src[i][j] = fgetc(arq);//Define a src.
            if(j == 26){
                fseek(arq,2,SEEK_CUR);
            }
        }
    }
    fclose(arq);
}

int checkLevel(obj objImage[N_LINHAS][N_COLUNAS], int* levelNumber)
{//Função checkLevel, encerra a fase e sinaliza o fim da fase.
    int totalKeys = 0;
    int i,j;
    int intReturn = 0;
    for(i=0;i<N_LINHAS;i++)
    {
        for(j=0;j<N_COLUNAS;j++)
        {
            if(objImage[i][j].identity == 'C')
            {
                totalKeys++;
            }
        }
    }

    if(totalKeys == 0)
    {
        *levelNumber += 1;
        intReturn = 1;
    }

    return intReturn;
}

float checkDistance(float* playerX,float* playerY, float x, float y)
{
  int totalDistance;

  totalDistance = pow((*playerX - x),2) + pow((*playerY - y),2);

  totalDistance = sqrt(totalDistance);

  return totalDistance;
}

int pathFinder(obj objImage[N_LINHAS][N_COLUNAS], float* playerX,float* playerY, int* direction)
{
  int i,j;
  float distanceImage[N_LINHAS][N_COLUNAS];
  int ogreI;
  int ogreJ;

  for(i=0;i<N_LINHAS;i++)
  {
      for(j=0;j<N_COLUNAS;j++)
      {
          if((objImage[i][j].identity == 'M') || (objImage[i][j].identity == ' ') || (objImage[i][j].identity == 'A') || (objImage[i][j].identity == 'B') || (objImage[i][j].identity == 'J'))
          {
              distanceImage[i][j] = checkDistance(playerX,playerY, j*ESCALA, i*ESCALA);
          }else if(objImage[i][j].identity == 'O'){
            distanceImage[i][j] = checkDistance(playerX,playerY, j*ESCALA, i*ESCALA);
            ogreI = i;
            ogreJ = j;
          }
          else
          {
              distanceImage[i][j] = -1;
          }
      }
  }

  /*
  puts("Começo.");
  for(i=0;i<N_LINHAS;i++)
  {
      for(j=0;j<N_COLUNAS;j++)
      {
          printf("%.2f\t",distanceImage[i][j]);
      }
      printf("\n");
  }*/

  *direction = 5;

  if((distanceImage[ogreI][ogreJ] >= distanceImage[ogreI-1][ogreJ]) && (distanceImage[ogreI-1][ogreJ] >= 0)){
    *direction = 1;
    return 0;
  }

  if((distanceImage[ogreI][ogreJ] >= distanceImage[ogreI][ogreJ-1]) && (distanceImage[ogreI][ogreJ-1] >= 0)){
    *direction = 0;
    return 0;
  }

  if((distanceImage[ogreI][ogreJ] >= distanceImage[ogreI+1][ogreJ]) && (distanceImage[ogreI+1][ogreJ] >= 0)){
    *direction = 3;
    return 0;
  }

  if((distanceImage[ogreI][ogreJ] >= distanceImage[ogreI][ogreJ+1]) && (distanceImage[ogreI][ogreJ+1] >= 0)){
    *direction = 2;
    return 0;
  }

  return 0;
}
