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
#define PHYSAC_NO_THREADS
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
    int walking;
    int max_frames;
} Player;

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

//-----------------


//========
// Funções

static void initGame(void);
static void movement(void);
void delay(float seconds);
void drawPhysicsEdge(void);

//--------


Texture2D hilda[6];


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
    
    SetPhysicsGravity(0, 0);
    
    int transparencia = 0;
    int sobe = true;
    
    int framesCounter =0;
    int currentFrame = 0;
    
    Texture2D menuBG = LoadTexture("imagens/arvore_da_vida.png");
    Font vikingFont = LoadFont("VIKING-N.TTF");
    
    
    hilda[0] = LoadTexture("imagens/hilda/idle/Warrior_Idle_1.png");
    hilda[1] = LoadTexture("imagens/hilda/idle/Warrior_Idle_2.png");
    hilda[2] = LoadTexture("imagens/hilda/idle/Warrior_Idle_3.png");
    hilda[3] = LoadTexture("imagens/hilda/idle/Warrior_Idle_4.png");
    hilda[4] = LoadTexture("imagens/hilda/idle/Warrior_Idle_5.png");
    hilda[5] = LoadTexture("imagens/hilda/idle/Warrior_Idle_6.png");
    
    Texture2D hildaDeath[11] = {
        LoadTexture("imagens/Death-Effect/Warrior_Death_1.png"),
        LoadTexture("imagens/Death-Effect/Warrior_Death_2.png"),
        LoadTexture("imagens/Death-Effect/Warrior_Death_3.png"),
        LoadTexture("imagens/Death-Effect/Warrior_Death_4.png"),
        LoadTexture("imagens/Death-Effect/Warrior_Death_5.png"),
        LoadTexture("imagens/Death-Effect/Warrior_Death_6.png"),
        LoadTexture("imagens/Death-Effect/Warrior_Death_7.png"),
        LoadTexture("imagens/Death-Effect/Warrior_Death_8.png"),
        LoadTexture("imagens/Death-Effect/Warrior_Death_9.png"),
        LoadTexture("imagens/Death-Effect/Warrior_Death_10.png"),
        LoadTexture("imagens/Death-Effect/Warrior_Death_11.png")};
    
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

    Texture2D heart = LoadTexture("imagens/heart_animated_2.png");
    
    initGame();
    
    
    player.max_frames = 5;
    
    while(!WindowShouldClose() && victory==false){
        
        
        
        
        RunPhysicsStep();
        
        BeginDrawing();
        ClearBackground(BLACK);
        
        framesCounter++;

        if(framesCounter>=(60/8)){
            framesCounter=0;
            
            currentFrame++;
            
            if(currentFrame>=player.max_frames) currentFrame=0;
        }

        
        switch(level){
            
            case 0:
            
            if(transparencia <=0) sobe = true;
            if(transparencia>=255) sobe=false;
            
            if(sobe==true) transparencia+=3; else transparencia-=3;
            
            DrawTexturePro(menuBG, (Rectangle){0,0, menuBG.width, menuBG.height}, (Rectangle){0,0,screenWidth, screenHeight}, (Vector2){0,0}, 0, GRAY);
            
            DrawTextEx(vikingFont, "The Valkyrie's Quest", (Vector2){screenWidth/3.4, screenHeight/4}, 50,0,WHITE);
            DrawTextEx(vikingFont, "Presione enter para iniciar", (Vector2){screenWidth/2.5, screenHeight*80/100}, 20,0, (Color){255, 255, 255, transparencia});
            
            DrawTexture(hildaRun[currentFrame],100,100,WHITE);

            if(IsKeyPressed(KEY_ENTER)) level++;

            break;


            case 1: ;

            char texto[10][500] = {"A vida então se espalhou por todos os nove mundos, e como toda grande história,\nescrita em sangue, morte e heroísmo.\nMidgard até os dias atuais se perde em guerra,\nAlfheim, mundo dos elfos, perdeu-se em sua própria soberba,\nNidavellir, o mundo dos anões, é assolado pelos elfos negros,\nJotunheim, o mundo dos gigantes, é uma prisão de constante conflito e situações extremas\ne Muspelheim, o mundo dos gigantes de fogo é um literal inferno,\nalém dos reinos inalcançáveis.",
             "A árvore da vida era a responsável por portar os nove mundos, \nos Deuses se dividiram em dois clãs, os responsáveis pela guerra,\n Aesir, habitantes de Asgard, liderados por Bor,\nque passou este cargo para seu filho Odin,\ne infelizmente não passará para Thor, pois sua morte já está escrita, \nos responsáveis pela Natureza, Vanir,\nhabitantes de Vanaheim liderados por Frey e Freya, Deuses do verão e da primavera.",
             "A vida então se espalhou por todos os nove mundos, e como toda grande história, escrita em sangue, morte e heroismo. Midgard ate os dias atuais se perde em guerra, Alfheim, mundo dos elfos, perdeu-se em sua própria soberba, Nidavellir, o mundo dos anoes, e assolado pelos elfos negros, Jotunheim, o mundo dos gigantes, e uma prisão de constante conflito e situações extremas e Muspelheim, o mundo dos gigantes de fogo e um literal inferno, alem dos reinos inalcançaveis.",
             "Toda vida tem seu peso, e esse peso e medido, aqueles que morrem de maneira desonrosa caem nos poços de Helheim, cumprindo sua sentença para Hela, a Deusa da morte, e aqueles que morrem de maneira honrada, ganham a Bencao de ir para Valhalla, o salao dos Deuses, onde herois comem, bebem, festejam e digladiam ate o fim. E existem aquelas responsaveis por dar a cada pessoa um destino apos a morte digno, as Valquirias, guerreiras escolhidas por Odin, que levam as almas dos mortos, e assim tudo funcionou por milênios…",
              "Ate os dias de hoje... misteriosamente, almas de guerreiros e lendas passaram a cair de volta aos mundos dos vivos, causando caos e atraindo monstros.Em tempos de necessidade, como sempre, herois se levantam, e Odin selecionou Brunhilda, a mais forte das novas recrutas para Valquirias, responsavel por resgatar as almas, e assim conseguir sua honra em ganhar suas asas e se tornar uma verdadeira heroína…"};

            static int framesCounterText;
            static int paragrafo;

            //DrawText(texto, 10,10,20,WHITE);

            if(IsKeyDown(KEY_ENTER)) framesCounterText+=20; else framesCounterText++;

            DrawTextEx(vikingFont, TextSubtext(texto[paragrafo], 0, framesCounterText/4), (Vector2){screenWidth/14, screenHeight/10}, 18,0,WHITE);

            if(framesCounterText/4 >= strlen(texto[paragrafo])) {
                
                if(IsKeyPressed(KEY_ENTER)) {
                    paragrafo++;
                    framesCounterText=0;
                    if(paragrafo>=5) level++;
                }
            }

            break;

            case 2:

            if(player.walking==0){
                DrawTextureRec(hilda[currentFrame], (Rectangle){-hilda[currentFrame].width/1.3, -hilda[currentFrame].height/1.25, player.rec.width*player.orientation, player.rec.height}, (Vector2){player.body->position.x-player.rec.width/2, player.body->position.y-player.rec.height/2}, WHITE);
                player.max_frames = 5;
            } else {

            DrawTextureRec(hildaRun[currentFrame], (Rectangle){-hildaRun[currentFrame].width/1.3, -hildaRun[currentFrame].height/1.25, player.rec.width*player.orientation, player.rec.height}, (Vector2){player.body->position.x-player.rec.width/2, player.body->position.y-player.rec.height/2}, WHITE);
            
            player.max_frames = 8;
            }
            static float vidaParte;
            for(int i=0;i<=player.vida/4;i++){
                vidaParte = player.vida/4 - floor(player.vida/4);
                if(i>=floor(player.vida/4)){
                    if(vidaParte==0.25) DrawTextureRec(heart, (Rectangle){3*heart.width/5,0,heart.width/5, heart.height}, (Vector2){100+(heart.width/5)*i+1,100}, WHITE);
                    if(vidaParte==0.5) DrawTextureRec(heart, (Rectangle){2*heart.width/5,0,heart.width/5, heart.height}, (Vector2){100+(heart.width/5)*i+1,100}, WHITE);
                    if(vidaParte==0.75) DrawTextureRec(heart, (Rectangle){1*heart.width/5,0,heart.width/5, heart.height}, (Vector2){100+(heart.width/5)*i+1,100}, WHITE);
                    //if(vidaParte==0) DrawTextureRec(heart, (Rectangle){4*heart.width/5,0,heart.width/5, heart.height}, (Vector2){100+(heart.width/5)*i,100}, WHITE);
                } else {
                    DrawTextureRec(heart, (Rectangle){0,0,heart.width/5, heart.height}, (Vector2){100+(heart.width/5)*i,100}, WHITE);
                }
 
            }
 
            if(IsKeyPressed(KEY_MINUS)) player.vida--;
            if(IsKeyPressed(KEY_EQUAL)) player.vida++;
            break;
            
        }
        

        //DrawRectangleRec((Rectangle){player.body->position.x-player.rec.width/2, player.body->position.y-player.rec.height/2, player.rec.width, player.rec.height}, player.color);

        movement();
   
        drawPhysicsEdge();

        EndDrawing();
        
    }
    
    CloseWindow(); 
}

void initGame(){
    // inicia os paramentros do jogo
    
    // jogador
    

    player.rec.width = hilda[0].width/3;
    player.rec.height = hilda[0].height*80/100;
    player.speed = 0.45;
    player.color = YELLOW;
    player.max_frames=5;
    player.walking=0;
    player.orientation=1;

    player.vida = 16;
    
    player.body = CreatePhysicsBodyRectangle((Vector2){screenWidth/2, screenHeight/2}, player.rec.width, player.rec.height, 10);
    
    player.rec.x = player.body->position.x;
    player.rec.y = player.body->position.y;
    player.body->freezeOrient=true;
    
}

void movement(){
    // movimento do personagem
    
    if(IsKeyDown(KEY_RIGHT) && player.rec.x<screenWidth-40) {
        player.body->velocity.x = player.speed;
        player.max_frames = 8;
        player.orientation = 1;
    }
    if(IsKeyDown(KEY_LEFT) && player.rec.x>30){
        player.body->velocity.x = -player.speed;

        player.max_frames = 8;
        player.orientation = -1;
    }
    if(IsKeyDown(KEY_DOWN) && player.rec.y<screenHeight-25){
        player.body->velocity.y = player.speed;

        player.max_frames = 8;
    }
    if(IsKeyDown(KEY_UP) && player.rec.y>20){
        player.body->velocity.y = -player.speed;

        player.max_frames = 8;
    }
    
    
    
    
 
    
    
    
    
    
 
    
    if(IsKeyDown(KEY_UP) || IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_LEFT)) player.walking = 1; else player.walking = 0;
    
    if (!IsKeyDown(KEY_UP) && !IsKeyDown(KEY_DOWN)) player.body->velocity.y = 0;
    if (!IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT)) player.body->velocity.x = 0;
    
    player.rec.x = player.body->position.x-player.rec.width/2;
    player.rec.y = player.body->position.y-player.rec.height/2;
            
    
}

void delay(float seconds){
    float milliseconds = seconds*1000;
    clock_t start = clock();

    while((clock() - start) * 1000 / CLOCKS_PER_SEC < milliseconds);
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
