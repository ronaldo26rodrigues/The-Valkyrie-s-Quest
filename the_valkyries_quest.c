/*=========================================================================*
 *  The Valkyrie's Quest
 *  
 *  Desenvolvido por: Esdras, Gabriel, Gustavo, Marissol, Ronaldo e Vicente
 *=========================================================================*/

#include "C:\raylib\raylib\src\raylib.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#define PHYSAC_IMPLEMENTATION
//#define PHYSAC_NO_THREADS
#include "C:\raylib\raylib\src\physac.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif


//========
// defines


//----------


//===========
// estruturas

typedef struct Player {
    Rectangle rec;
    float speed;
    Color color;
    Vector2 prevPos;
    float vida;
    PhysicsBody body;
    int orientation;
    int mode;
    int max_frames;
    bool caiu;
} Player;

typedef struct Esqueleto {
    Rectangle rec;
    float speed;
    Color color;
    float vida;
    PhysicsBody body;
    int orientation;
    int max_frames;
} Esqueleto;
//-------------


//=================
// Variaveis gloais

int screenWidth = 1400;
int screenHeight = 600;

static bool gameOver = false;
static bool pause =  false;
static bool victory = false;


static Player player;

static int level=0;

int posParallax=0;

Camera2D camera;

bool criouCorpos = 0;

int currentFrame = 0;
clock_t timer_dash;


//-----------------


//========
// Funções

static void initGame(void);
static void movement(void);
void delay(float seconds);
void drawPhysicsEdge(void);
void drawHearts(void);

//--------


Texture2D hilda[6];
Texture2D heart;

int main(){
    srand(time(NULL));
    
    

    InitWindow(screenWidth, screenHeight, "The Valkyrie's Quest");
    screenWidth = GetMonitorWidth(0);
    screenHeight = GetMonitorHeight(0);
    CloseWindow();
    InitWindow(screenWidth, screenHeight, "The Valkyrie's Quest");
    
    SetTargetFPS(60);
    
    ToggleFullscreen();
    
    InitPhysics();
    
    //SetPhysicsGravity(0, 26);

    InitAudioDevice();
    
    int transparencia = 0;
    int sobe = true;
    
    int framesCounter =0;
    
    
    Vector2 iniciodoLvl = {0,screenHeight*80/100};
    
    Texture2D menuBG = LoadTexture("imagens/arvore_da_vida.png");
    Font vikingFont = LoadFont("VIKING-N.TTF");
    Font superMario = LoadFont("Super-Mario-World.ttf");
    Music zeldaMus = LoadMusicStream("som/musica_do_game1.ogg");
    
    hilda[0] = LoadTexture("imagens/hilda/idle/Warrior_Idle_1.png");
    hilda[1] = LoadTexture("imagens/hilda/idle/Warrior_Idle_2.png");
    hilda[2] = LoadTexture("imagens/hilda/idle/Warrior_Idle_3.png");
    hilda[3] = LoadTexture("imagens/hilda/idle/Warrior_Idle_4.png");
    hilda[4] = LoadTexture("imagens/hilda/idle/Warrior_Idle_5.png");
    hilda[5] = LoadTexture("imagens/hilda/idle/Warrior_Idle_6.png");
    
    Texture2D hildaDeath[11] = {
        LoadTexture("imagens/DeathEffect/Warrior_Death_1.png"),
        LoadTexture("imagens/DeathEffect/Warrior_Death_2.png"),
        LoadTexture("imagens/DeathEffect/Warrior_Death_3.png"),
        LoadTexture("imagens/DeathEffect/Warrior_Death_4.png"),
        LoadTexture("imagens/DeathEffect/Warrior_Death_5.png"),
        LoadTexture("imagens/DeathEffect/Warrior_Death_6.png"),
        LoadTexture("imagens/DeathEffect/Warrior_Death_7.png"),
        LoadTexture("imagens/DeathEffect/Warrior_Death_8.png"),
        LoadTexture("imagens/DeathEffect/Warrior_Death_9.png"),
        LoadTexture("imagens/DeathEffect/Warrior_Death_10.png"),
        LoadTexture("imagens/DeathEffect/Warrior_Death_11.png")
    };
    
    Texture2D hildaRun[8] = {
        LoadTexture("imagens/hilda/Run/Warrior_Run_1.png"),
        LoadTexture("imagens/hilda/Run/Warrior_Run_2.png"),
        LoadTexture("imagens/hilda/Run/Warrior_Run_3.png"),
        LoadTexture("imagens/hilda/Run/Warrior_Run_4.png"),
        LoadTexture("imagens/hilda/Run/Warrior_Run_5.png"),
        LoadTexture("imagens/hilda/Run/Warrior_Run_6.png"),
        LoadTexture("imagens/hilda/Run/Warrior_Run_7.png"),
        LoadTexture("imagens/hilda/Run/Warrior_Run_8.png")
    };
    
    Texture2D hildaUp[8] = {
        LoadTexture("imagens/hilda/Run/up/Warrior-Ladder-Grab_0.png"),
        LoadTexture("imagens/hilda/Run/up/Warrior-Ladder-Grab_1.png"),
        LoadTexture("imagens/hilda/Run/up/Warrior-Ladder-Grab_2.png"),
        LoadTexture("imagens/hilda/Run/up/Warrior-Ladder-Grab_3.png"),
        LoadTexture("imagens/hilda/Run/up/Warrior-Ladder-Grab_4.png"),
        LoadTexture("imagens/hilda/Run/up/Warrior-Ladder-Grab_5.png"),
        LoadTexture("imagens/hilda/Run/up/Warrior-Ladder-Grab_6.png"),
        LoadTexture("imagens/hilda/Run/up/Warrior-Ladder-Grab_7.png")
    };
    
     Texture2D hildaAttack[12] = {
         LoadTexture("imagens/Attack/Warrior_Attack_1.png"),
         LoadTexture("imagens/Attack/Warrior_Attack_2.png"),
         LoadTexture("imagens/Attack/Warrior_Attack_3.png"),
         LoadTexture("imagens/Attack/Warrior_Attack_4.png"),
         LoadTexture("imagens/Attack/Warrior_Attack_5.png"),
         LoadTexture("imagens/Attack/Warrior_Attack_6.png"),
         LoadTexture("imagens/Attack/Warrior_Attack_7.png"),
         LoadTexture("imagens/Attack/Warrior_Attack_8.png"),
         LoadTexture("imagens/Attack/Warrior_Attack_9.png"),
         LoadTexture("imagens/Attack/Warrior_Attack_10.png"),
         LoadTexture("imagens/Attack/Warrior_Attack_11.png"),
         LoadTexture("imagens/Attack/Warrior_Attack_12.png")
     };
     
     Texture2D hildaDash[7] = {
        LoadTexture("imagens/Dash/Warrior_Dash_1.png"),
        LoadTexture("imagens/Dash/Warrior_Dash_2.png"),
        LoadTexture("imagens/Dash/Warrior_Dash_3.png"),
        LoadTexture("imagens/Dash/Warrior_Dash_4.png"),
        LoadTexture("imagens/Dash/Warrior_Dash_5.png"),
        LoadTexture("imagens/Dash/Warrior_Dash_6.png"),
        LoadTexture("imagens/Dash/Warrior_Dash_7.png"), 
        };
        
    Texture2D hildaDashAttack[10] = {
        LoadTexture("imagens/DashAttack/Warrior_DashAttack_1.png"),
        LoadTexture("imagens/DashAttack/Warrior_DashAttack_2.png"),
        LoadTexture("imagens/DashAttack/Warrior_DashAttack_3.png"),
        LoadTexture("imagens/DashAttack/Warrior_DashAttack_4.png"),
        LoadTexture("imagens/DashAttack/Warrior_DashAttack_5.png"),
        LoadTexture("imagens/DashAttack/Warrior_DashAttack_6.png"),
        LoadTexture("imagens/DashAttack/Warrior_DashAttack_7.png"),
        LoadTexture("imagens/DashAttack/Warrior_DashAttack_8.png"),
        LoadTexture("imagens/DashAttack/Warrior_DashAttack_9.png"),
        LoadTexture("imagens/DashAttack/Warrior_DashAttack_10.png")
    };
    
    Texture2D hildaJump[3] = {
        LoadTexture("imagens/hilda/Jump/Warrior_Jump_1.png"),
        LoadTexture("imagens/hilda/Jump/Warrior_Jump_2.png"),
        LoadTexture("imagens/hilda/Jump/Warrior_Jump_3.png"),
    };
    
    Texture2D bglvl1 = LoadTexture("imagens/cenario/bglvl1.png");

    Texture2D plataformas[2] = {
        LoadTexture("imagens/cenario/plataforma1.png"),
        LoadTexture("imagens/cenario/plataforma2.png"),
    };
    
    Texture2D espinhos[2] = {
        LoadTexture("imagens/cenario/espinhos.png"),
        LoadTexture("imagens/cenario/espinhos.png"),

    };
    
    Texture2D pilar[2] = {
        LoadTexture("imagens/cenario/pilar.png"),
        LoadTexture("imagens/cenario/pilar.png"),

    };
     Texture2D terra[2] = {
        LoadTexture("imagens/cenario/terra.png"),
        LoadTexture("imagens/cenario/terra.png"),

    };

    


    heart = LoadTexture("imagens/heart_animated_2.png");

    Texture2D chao1 = LoadTexture("imagens/cenario/chao1.png");

    Texture2D skeletonIdle = LoadTexture("imagens/esqueleto/Skeleton Idle.png");
    Texture2D skeletonAtk = LoadTexture("imagens/esqueleto/Skeleton Attack.png");
    Texture2D skeletonWalk = LoadTexture("imagens/esqueleto/Skeleton Walk.png");
    
    CreatePhysicsBodyRectangle((Vector2){0+chao1.width/2,(screenHeight*80/100)+chao1.height/2}, chao1.width, chao1.height, 1)->enabled=false;

    CreatePhysicsBodyRectangle((Vector2){iniciodoLvl.x+1900+plataformas[1].width/2, iniciodoLvl.y-180+plataformas[1].height/2}, plataformas[1].width, plataformas[1].height,1)->enabled=false;
    
    CreatePhysicsBodyRectangle((Vector2){iniciodoLvl.x+2300+plataformas[1].width/2, iniciodoLvl.y-260+plataformas[1].height/2}, plataformas[1].width, plataformas[1].height,1)->enabled=false;
    
    CreatePhysicsBodyRectangle((Vector2){iniciodoLvl.x+2700+plataformas[1].width/2, iniciodoLvl.y-320+plataformas[1].height/2}, plataformas[1].width, plataformas[1].height,1)->enabled=false;
    
    CreatePhysicsBodyRectangle((Vector2){iniciodoLvl.x+3200+plataformas[1].width/2, iniciodoLvl.y-140+plataformas[1].height/2}, plataformas[1].width, plataformas[1].height,1)->enabled=false;
     
    CreatePhysicsBodyRectangle((Vector2){iniciodoLvl.x+5000+plataformas[1].width/2, iniciodoLvl.y-340+plataformas[1].height/2}, plataformas[1].width, plataformas[1].height,1)->enabled=false;
     
    CreatePhysicsBodyRectangle((Vector2){iniciodoLvl.x+5000+plataformas[1].width/2, iniciodoLvl.y-180+plataformas[1].height/2}, plataformas[1].width, plataformas[1].height,1)->enabled=false;
    
    CreatePhysicsBodyRectangle((Vector2){iniciodoLvl.x+5400+plataformas[1].width/2, iniciodoLvl.y-340+plataformas[1].height/2}, plataformas[1].width, plataformas[1].height,1)->enabled=false;
   
    CreatePhysicsBodyRectangle((Vector2){iniciodoLvl.x+5400+plataformas[1].width/2, iniciodoLvl.y-180+plataformas[1].height/2}, plataformas[1].width, plataformas[1].height,1)->enabled=false;
    
    CreatePhysicsBodyRectangle((Vector2){iniciodoLvl.x+7000+pilar[1].width/2, iniciodoLvl.y-400+pilar[1].height/2}, pilar[1].width, pilar[1].height,1)->enabled=false;
    
    CreatePhysicsBodyRectangle((Vector2){iniciodoLvl.x+7000+terra[1].width/2, iniciodoLvl.y-200+terra[1].height/10}, terra[1].width, pilar[1].height,1)->enabled=false;

    CreatePhysicsBodyRectangle((Vector2){iniciodoLvl.x+2750+espinhos[1].width/2, iniciodoLvl.y-60+espinhos[1].height/2}, espinhos[1].width*15, espinhos[1].height,1)->enabled=false;
     
     
     
     
     
    
    
    
    
    
    
    

    

    initGame();
    
    PlayMusicStream(zeldaMus);
    
    player.max_frames = 5;
    
    while(!WindowShouldClose() && victory==false){
        
        
        
        
        //RunPhysicsStep();
        
        BeginDrawing();
        
        ClearBackground(BLACK);
        
        framesCounter++;

        if(framesCounter>=(60/8)){
            framesCounter=0;
            
            currentFrame++;
            
            if(currentFrame>=player.max_frames) {
                currentFrame = 0;
                if(player.mode == 3 || player.mode == 4 || player.mode == 5) player.mode = 0;
                }
        }

        
        switch(level){
            
            case 0:
            
            UpdateMusicStream(zeldaMus);


            
            if(transparencia <=0) sobe = true;
            if(transparencia>=255) sobe=false;
            
            if(sobe==true) transparencia+=3; else transparencia-=3;
            
            DrawTexturePro(menuBG, (Rectangle){0,0, menuBG.width, menuBG.height}, (Rectangle){0,0,screenWidth, screenHeight}, (Vector2){0,0}, 0, LIGHTGRAY);
            
            DrawTextEx(vikingFont, "The Valkyrie's Quest", (Vector2){screenWidth/3.4, screenHeight/4}, 50,0,WHITE);
            DrawTextEx(vikingFont, "Presione enter para iniciar", (Vector2){screenWidth/2.5, screenHeight*80/100}, 20,0, (Color){255, 255, 255, transparencia});
            
            player.max_frames = 8;

            DrawTexture(hildaRun[currentFrame],100,100,WHITE);

            if(IsKeyPressed(KEY_ENTER)) {
                level++;
                UnloadTexture(menuBG);
                }

            break;


            case 1: ;
            
            char texto[2][800] = {"No início do mundo, não havia nada além do fogo e do gelo. No encontro destes dois elementos, surgiu a névoa primordial,\no Ginnungagap, e dela surgiu Ymir, o primeiro gigante, que daria origem para as duas raças: A raça dos Gigantes e os\nprimeiros grandes Deuses.Por estarem em constante conflito, os Deuses acabam assassinando Ymir, dando origem aos mundos.\nA árvore da vida era a responsável por portar os nove mundos, os Deuses se dividiram em dois clãs, os responsáveis pela guerra\n,Aesir, habitantes de Asgard, liderados por Bor, que passou este cargo para seu filho Odin, e infelizmente não passará para Thor,\npois sua morte já está escrita, os responsáveis pela Natureza, Vanir, habitantes de Vanaheim liderados por Frey e Freya,\nDeuses do verão e da primavera.\nA vida então se espalhou por todos os nove mundos, e como toda grande história, escrita em sangue, morte e heroismo.\nMidgard ate os dias atuais se perde em guerra, Alfheim, mundo dos elfos, perdeu-se em sua própria soberba, Nidavellir,\no mundo dos anoes, e assolado pelos elfos negros, Jotunheim, o mundo dos gigantes, e uma prisão de constante conflito e situações\nextremas e Muspelheim, o mundo dos gigantes de fogo e um literal inferno, alem dos reinos inalcançaveis.\nToda vida tem seu peso, e esse peso e medido, aqueles que morrem de maneira desonrosa caem nos poços de Helheim, cumprindo\nsua sentença para Hela, a Deusa da morte, e aqueles que morrem de maneira honrada, ganham a Bencao de ir para Valhalla,\no salao dos Deuses, onde herois comem, bebem, festejam e digladiam ate o fim.\n", "E existem aquelas responsaveis por dar a cada pessoa um destino apos a morte digno, as Valquirias, guerreiras escolhidas\npor Odin,que levam as almas dos mortos, e assim tudo funcionou por milênios…\nAte os dias de hoje... misteriosamente, almas de guerreiros e lendas passaram a cair de volta aos mundos dos vivos, causando\ncaos e atraindo monstros.\nEm tempos de necessidade, como sempre, herois se levantam, e Odin selecionou Brunhilda, a mais forte das novas recrutas\n\npara Valquirias, responsavel por resgatar as almas, e assim conseguir sua honra em ganhar suas asas e se tornar uma\nverdadeira heroína…\n\n\nOdin: Levante-se, pequena. . ."};

            static int framesCounterText;
            static int paragrafo = 0;

            //DrawText(texto, 10,10,20,WHITE);

            if(IsKeyDown(KEY_ENTER)) framesCounterText+=20; else framesCounterText++;

            //DrawTextEx(superMario, TextSubtext(texto[paragrafo], 0, framesCounterText/4), (Vector2){screenWidth/14, screenHeight/10}, screenWidth/75, 0,WHITE);
            DrawText(TextSubtext(texto[paragrafo], 0, framesCounterText/4), screenWidth/14, screenHeight/10, screenWidth/70, WHITE);

            if(framesCounterText/4 >= strlen(texto[paragrafo])) {
                
                if(IsKeyPressed(KEY_ENTER)) {
                    paragrafo++;
                    framesCounterText=0;
                    if(paragrafo>=1) {
                        free(&texto);
                        free(&framesCounterText);
                        free(&paragrafo);
                        level++;
                        }
                }
            }

            break;

            case 2:
            
            if(IsKeyPressed(KEY_C)){
                Esqueleto esqueleto[4];
                for(int i =0;i<4;i++){
                esqueleto[i].rec.x = rand() % bglvl1.width;
                esqueleto[i].rec.y = player.rec.y;
                esqueleto[i].rec.height=skeletonIdle.height;
                esqueleto[i].rec.width=skeletonIdle.width/11;
                
                esqueleto[i].body = CreatePhysicsBodyRectangle((Vector2){esqueleto[i].rec.x, esqueleto[i].rec.y}, esqueleto[i].rec.width, esqueleto[i].rec.height, 1);
                esqueleto[i].body->freezeOrient=true;
                }
                criouCorpos = true;
            }

            BeginMode2D(camera);
            
            

            
            //DrawTexture(bglvl1,0,(screenHeight*80/100)-bglvl1.height,WHITE);
            DrawTexturePro(bglvl1, (Rectangle){0,0, bglvl1.width, bglvl1.height}, (Rectangle){0,(screenHeight*80/100)-bglvl1.height,bglvl1.width*2, bglvl1.height+(10/100*screenHeight)},(Vector2){0,0},0,WHITE);
            
            drawPhysicsEdge();
              
            DrawTexture(plataformas[1], iniciodoLvl.x+1900, iniciodoLvl.y-180, WHITE);
            
            DrawTexture(plataformas[1], iniciodoLvl.x+2300, iniciodoLvl.y-260, WHITE);
            
            DrawTexture(plataformas[1], iniciodoLvl.x+2700, iniciodoLvl.y-320, WHITE);
            
            DrawTexture(plataformas[1], iniciodoLvl.x+3200, iniciodoLvl.y-140, WHITE);
            
            DrawTexture(plataformas[1], iniciodoLvl.x+5000, iniciodoLvl.y-180, WHITE);
            
            DrawTexture(plataformas[1], iniciodoLvl.x+5000, iniciodoLvl.y-340, WHITE);
            
            DrawTexture(plataformas[1], iniciodoLvl.x+5400, iniciodoLvl.y-180, WHITE);
            
            DrawTexture(plataformas[1], iniciodoLvl.x+5400, iniciodoLvl.y-340, WHITE);
            
            DrawTexture(pilar[1], iniciodoLvl.x+7000, iniciodoLvl.y-400, WHITE);
            
            DrawTexture(terra[1], iniciodoLvl.x+7000, iniciodoLvl.y-200, WHITE);
            
            DrawTexture(espinhos[1], iniciodoLvl.x+2300, iniciodoLvl.y-60, WHITE);
            DrawTexture(espinhos[1], iniciodoLvl.x+2360, iniciodoLvl.y-60, WHITE);
            DrawTexture(espinhos[1], iniciodoLvl.x+2420, iniciodoLvl.y-60, WHITE);
            DrawTexture(espinhos[1], iniciodoLvl.x+2480, iniciodoLvl.y-60, WHITE);
            DrawTexture(espinhos[1], iniciodoLvl.x+2440, iniciodoLvl.y-60, WHITE);
            DrawTexture(espinhos[1], iniciodoLvl.x+2500, iniciodoLvl.y-60, WHITE);
            DrawTexture(espinhos[1], iniciodoLvl.x+2560, iniciodoLvl.y-60, WHITE);
            DrawTexture(espinhos[1], iniciodoLvl.x+2620, iniciodoLvl.y-60, WHITE);
            DrawTexture(espinhos[1], iniciodoLvl.x+2680, iniciodoLvl.y-60, WHITE);
            DrawTexture(espinhos[1], iniciodoLvl.x+2740, iniciodoLvl.y-60, WHITE);
            DrawTexture(espinhos[1], iniciodoLvl.x+2800, iniciodoLvl.y-60, WHITE);
            DrawTexture(espinhos[1], iniciodoLvl.x+2860, iniciodoLvl.y-60, WHITE);
            DrawTexture(espinhos[1], iniciodoLvl.x+2920, iniciodoLvl.y-60, WHITE);
            DrawTexture(espinhos[1], iniciodoLvl.x+2980, iniciodoLvl.y-60, WHITE);
            DrawTexture(espinhos[1], iniciodoLvl.x+3040, iniciodoLvl.y-60, WHITE);
            DrawTexture(espinhos[1], iniciodoLvl.x+3100, iniciodoLvl.y-60, WHITE);
            DrawTexture(espinhos[1], iniciodoLvl.x+3160, iniciodoLvl.y-60, WHITE);
            
            
            
            EndMode2D();

            drawHearts();
            DrawText(FormatText("%f", player.body->velocity.x), 100,300,20,WHITE);

            BeginMode2D(camera);

            //0 = parada, 1 = andando, 2 = pulando, 3 = attack, 4 = dash, 5 = dash attack
            if(player.mode == 0){
                DrawTextureRec(hilda[currentFrame], (Rectangle){-hilda[currentFrame].width/1.3, -hilda[currentFrame].height/1.25, player.rec.width*player.orientation, player.rec.height}, (Vector2){player.body->position.x-player.rec.width/2, player.body->position.y-player.rec.height/2}, WHITE);
                player.max_frames = 5;
            }
            else if(player.mode == 1) {
                if(player.orientation == 1){
                DrawTextureRec(hildaRun[currentFrame], (Rectangle){hildaRun[currentFrame].width/4.6f, -hildaRun[currentFrame].height/1.25, (hildaRun[currentFrame].width/1.6f)*player.orientation, player.rec.height}, (Vector2){player.body->position.x-player.rec.width/2, player.body->position.y-player.rec.height/2}, WHITE);
                }
                if(player.orientation == -1){
                DrawTextureRec(hildaRun[currentFrame], (Rectangle){0, -hildaRun[currentFrame].height/1.25, (hildaRun[currentFrame].width/1.4f)*player.orientation, player.rec.height}, (Vector2){player.body->position.x-player.rec.width/2, player.body->position.y-player.rec.height/2}, WHITE);
                }
                player.max_frames = 8;
            }
            else if(player.mode == 2){
                if(player.orientation == 1){
                DrawTextureRec(hildaJump[currentFrame], (Rectangle){hildaJump[currentFrame].width/4.6f, -hildaJump[currentFrame].height/1.25, (hildaJump[currentFrame].width/1.6f)*player.orientation, player.rec.height}, (Vector2){player.body->position.x-player.rec.width/2, player.body->position.y-player.rec.height/2}, WHITE);
                }
                if(player.orientation == -1){
                DrawTextureRec(hildaJump[currentFrame], (Rectangle){0, -hildaJump[currentFrame].height/1.25, (hildaJump[currentFrame].width/1.4f)*player.orientation, player.rec.height}, (Vector2){player.body->position.x-player.rec.width/2, player.body->position.y-player.rec.height/2}, WHITE);
                }
            }
            
            //teste
            //attack
            //tava width/4.6f, height/1.25, width/1.6f, width/2, height/2
            else if (player.mode == 3) {
                if(player.orientation == 1){
                DrawTextureRec(hildaAttack[currentFrame], (Rectangle){hildaAttack[currentFrame].width/4.4f, -hildaAttack[currentFrame].height/1.15, (hildaAttack[currentFrame].width/1.25f)*player.orientation, player.rec.height}, (Vector2){player.body->position.x-player.rec.width/2, player.body->position.y-player.rec.height/2}, WHITE);
                }
                if(player.orientation == -1){
                DrawTextureRec(hildaAttack[currentFrame], (Rectangle){0, -hildaAttack[currentFrame].height/1.10, (hildaAttack[currentFrame].width/1.0f)*player.orientation, player.rec.height}, (Vector2){player.body->position.x-player.rec.width/1.2, player.body->position.y-player.rec.height/2}, WHITE);
                }
                player.max_frames = 12;
            }
              
            else if (player.mode == 4) {
                if(player.orientation == 1) {
                DrawTextureRec(hildaDash[currentFrame], (Rectangle){hildaDash[currentFrame].width/4.4f, -hildaDash[currentFrame].height/1.25, (hildaDash[currentFrame].width/1.4f)*player.orientation, player.rec.height}, (Vector2){player.body->position.x-player.rec.width/2, player.body->position.y-player.rec.height/2}, WHITE);
                }
                if(player.orientation == -1) {
                DrawTextureRec(hildaDash[currentFrame], (Rectangle){0, -hildaDash[currentFrame].height/1.25, (hildaDash[currentFrame].width/1.4f)*player.orientation, player.rec.height}, (Vector2){player.body->position.x-player.rec.width/2, player.body->position.y-player.rec.height/2}, WHITE);
                }
                player.max_frames = 7;  
            }

            else if (player.mode == 5) {
                if(player.orientation == 1) {
                DrawTextureRec(hildaDashAttack[currentFrame], (Rectangle){hildaDashAttack[currentFrame].width/4.4f, -hildaDashAttack[currentFrame].height/1.25, (hildaDashAttack[currentFrame].width/1.4f)*player.orientation, player.rec.height}, (Vector2){player.body->position.x-player.rec.width/2, player.body->position.y-player.rec.height/2}, WHITE);
                }
                if(player.orientation == -1) {
                DrawTextureRec(hildaDashAttack[currentFrame], (Rectangle){0, -hildaDashAttack[currentFrame].height/1.25, (hildaDashAttack[currentFrame].width/1.4f)*player.orientation, player.rec.height}, (Vector2){player.body->position.x-player.rec.width/2, player.body->position.y-player.rec.height/2}, WHITE);
                }
                //player.max_frames = 10;
            }

            DrawTexture(chao1,0,screenHeight*80/100,WHITE);

            if(IsKeyPressed(KEY_MINUS)) player.vida--;
            if(IsKeyPressed(KEY_EQUAL)) player.vida++;
            
            EndMode2D();
            
            break;
            
        }
        
        if(IsKeyPressed(KEY_PAGE_UP)) level++;
        if(IsKeyPressed(KEY_PAGE_DOWN)) level--;

        //DrawRectangleRec((Rectangle){player.body->position.x-player.rec.width/2, player.body->position.y-player.rec.height/2, player.rec.width, player.rec.height}, player.color);

        movement();


        //drawPhysicsEdge();

        EndDrawing();
        
        //som();
        
        
        
        
    }
    CloseAudioDevice();
    ClosePhysics();
    CloseWindow(); 
}

void initGame(){
    // inicia os paramentros do jogo
    
    // jogador
    

    player.rec.width = hilda[0].width/3;
    player.rec.height = hilda[0].height*80/100;
    player.speed = 0.45;
    player.color = YELLOW;
    player.max_frames = 5;
    player.mode = 0;
    player.orientation = 1;
    player.caiu = false;

    player.vida = 16;
    
    player.body = CreatePhysicsBodyRectangle((Vector2){screenWidth/2, screenHeight/2}, player.rec.width, player.rec.height, 10);
    
    player.rec.x = player.body->position.x;
    player.rec.y = player.body->position.y;
    player.body->freezeOrient=true;


    //camera
    camera.target = (Vector2){player.body->position.x, player.body->position.y};
    camera.rotation = 0;
    camera.zoom = 1;
    camera.offset = (Vector2){player.body->position.x, player.body->position.y};
    
    timer_dash = clock();
    
}

void movement(){
    // movimento do personagem
    
    if(IsKeyDown(KEY_RIGHT) && !(IsKeyDown(KEY_X))) {
        player.body->velocity.x = player.speed;
        if(player.mode != 3 && player.mode != 4 && player.mode != 5) player.mode = 1;
        player.max_frames = 8;
        player.orientation = 1;
    }
    if(IsKeyDown(KEY_LEFT) && !(IsKeyDown(KEY_X))){
        player.body->velocity.x = -player.speed;
        if(player.mode != 3 && player.mode != 4 && player.mode != 5) player.mode = 1;
        player.max_frames = 8;
        player.orientation = -1;
    }
    
    
    if(player.caiu==false && player.body->isGrounded==true) {
        player.caiu = true;
        player.mode = 0;
    }
    if(IsKeyPressed(KEY_UP) && player.body->isGrounded==true){
        player.body->velocity.y = -2.25f;
        player.mode = 2;
        player.caiu = false;
        player.max_frames = 3;
    }

    //attack
    if(IsKeyPressed(KEY_Z) && !(IsKeyPressed(KEY_X))) {
        //attack
        //currentFrame = 0;
        player.max_frames = 12;
        player.mode = 3;
    }
    

    //dash
    if(IsKeyPressed(KEY_X) && !(IsKeyPressed(KEY_Z)) && (clock()-timer_dash)/1000>2) {
        player.body->velocity.x = (player.speed * 2) * player.orientation;
        player.mode = 4;
        currentFrame = 0;
        player.max_frames = 7; 
        timer_dash=clock();
    }
    
    //dash attack
    if (IsKeyPressed(KEY_X) && IsKeyPressed(KEY_Z)) {
        player.body->velocity.x = (player.speed * 2) * player.orientation;
        currentFrame = 0;
        player.max_frames = 10;
        player.mode = 5;
    }

    
    
    camera.target = (Vector2){player.body->position.x, screenHeight/1.8f};   

    if((IsKeyReleased(KEY_RIGHT) || IsKeyReleased(KEY_LEFT))) player.mode = 0;
    
    //if (!IsKeyDown(KEY_UP) && !IsKeyDown(KEY_DOWN)) player.body->velocity.y = 0;
    //if (!IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT)) player.body->velocity.x = 0;
    
    player.rec.x = player.body->position.x-player.rec.width/2;
    player.rec.y = player.body->position.y-player.rec.height/2;
            
    
}

void delay(float seconds){
    float milliseconds = seconds*1000;
    clock_t start = clock();

    while((clock() - start) * 1000 / CLOCKS_PER_SEC < milliseconds);
}


void drawHearts(){
    static float vidaParte;
    for(int i=0;i<=player.vida/4;i++){
        vidaParte = player.vida/4 - floor(player.vida/4);
        if(i>=floor(player.vida/4)){
            if(vidaParte==0.25) DrawTextureRec(heart, (Rectangle){3*heart.width/5,0,heart.width/5, heart.height}, (Vector2){100+(heart.width/5)*i,100}, WHITE);
            if(vidaParte==0.5) DrawTextureRec(heart, (Rectangle){2*heart.width/5,0,heart.width/5, heart.height}, (Vector2){100+(heart.width/5)*i,100}, WHITE);
            if(vidaParte==0.75) DrawTextureRec(heart, (Rectangle){1*heart.width/5,0,heart.width/5, heart.height}, (Vector2){100+(heart.width/5)*i,100}, WHITE);
            //if(vidaParte==0) DrawTextureRec(heart, (Rectangle){4*heart.width/5,0,heart.width/5, heart.height}, (Vector2){100+(heart.width/5)*i,100}, WHITE);
        } else {
            DrawTextureRec(heart, (Rectangle){0,0,heart.width/5, heart.height}, (Vector2){100+(heart.width/5)*i,100}, WHITE);
        }
 
    }
}


void drawPhysicsEdge(){
    int bodiesCount = GetPhysicsBodiesCount();
    for (int i = 0; i < bodiesCount; i++)
    {
        PhysicsBody body = GetPhysicsBody(i);
        if (body != NULL)
        {
            int vertexCount = GetPhysicsShapeVerticesCount(i);
            for (int j = 0; j < vertexCount; j++)
            {
                // Get physics bodies shape vertices to draw lines
                // Note: GetPhysicsShapeVertex() already calculates rotation transformations
                Vector2 vertexA = GetPhysicsShapeVertex(body, j);
                int jj = (((j + 1) < vertexCount) ? (j + 1) : 0);   // Get next vertex or first to close the shape
                Vector2 vertexB = GetPhysicsShapeVertex(body, jj);

                DrawLineV(vertexA, vertexB, GREEN);     // Draw a line between two vertex positions
            }
        }
    }
}
