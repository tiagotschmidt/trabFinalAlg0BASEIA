#include <stdio.h> //Bibliotecas utilizadas no projeto. Projeto desenvolvido utilizando Allegro.
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <math.h>

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

typedef struct//Estrutura obj, base do jogo. 'obj' serve como um molde para cada elemento do jogo. Ou seja, em determinado ponto da execução, a matriz de caracteres é convertida em uma matriz de obj.
{
        int body;//Propriedades dos objetos do jogo. Nem todos os elementos tem todas propriedades definidas.
        int shiftable;
        int state;
        int colectable;
        int player;
        int cd;
        char identity;
        char lastStep;
}obj;

typedef struct//Estrutura mapState, utilizada para salvar o jogo.
{
        obj objImage[N_LINHAS][N_COLUNAS];// Salva todas as propriedades interessantes para o jogo.
        char src[N_LINHAS][N_COLUNAS];
        int cScoreCounter;
        int cHp;
        int cLeverCooldown;
        int cImortalCooldown;
        int cHasSword;
        int cLevel;
        int cLeverS;
        int cPlayerS;
        float cX, cY;
}mapState;

void inicializa(bool teste, const char *descricao);

void loadMap(char src[N_LINHAS][N_COLUNAS], obj objImage[N_LINHAS][N_COLUNAS], float* x, float* y);

int collectItem(int counter,obj objImage[N_LINHAS][N_COLUNAS], int x, int y, int* imortalCooldown);

void leverActivate(int* cd,obj objImage[N_LINHAS][N_COLUNAS], int* isLever);

void updateOgre(obj objImage[N_LINHAS][N_COLUNAS], float* playerX,float* playerY);

void ogreHit(obj objImage[N_LINHAS][N_COLUNAS],float* pX,float* pY,char src[N_LINHAS][N_COLUNAS],int* playerHp,int* counter,int* menu, int* imortalCooldown, int* currentLevel, int* leverCooldown, int *hasSword);

void giveSword(obj objImage[N_LINHAS][N_COLUNAS], int* hasSword);

void drawMap(obj objImage[N_LINHAS][N_COLUNAS], int isLever);

void saveMap(FILE *arq, obj objImage[N_LINHAS][N_COLUNAS], char src[N_LINHAS][N_COLUNAS], int cScoreCounter, int cHp, int cLeverCooldown, int cImortalCooldown, int cHasSword, float cX, float cY, int cLevel, int isLever, int isInverted);

void loadSave(FILE *arq, obj objImage[N_LINHAS][N_COLUNAS], char src[N_LINHAS][N_COLUNAS], int* cScoreCounter, int* cHp, int* cLeverCooldown, int* cImortalCooldown, int* cHasSword, float* cX, float* cY, int* cLevel, int* cIsLever, int* cIsInverted);

void readMap(FILE *arq,  char src[N_LINHAS][N_COLUNAS], int* cLevel, char mapFile[10]);

int checkLevel(obj objImage[N_LINHAS][N_COLUNAS], int* levelNumber);

float checkDistance(float* playerX,float* playerY, float x, float y);

int pathFinder(obj objImage[N_LINHAS][N_COLUNAS], float* playerX,float* playerY, int* direction);

int pathFinderR(int distanceImage[N_LINHAS][N_COLUNAS], int firstMove, int cX, int cY, int* direction);
