/*=========================================================================*
 *  The Valkyrie's Quest
 *  
 *  Desenvolvido por: Esdras, Gabriel, Gustavo, Marissol, Ronaldo e Vicente
 *=========================================================================*/

#include <raylib.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#define PHYSAC_IMPLEMENTATION
#define PHYSAC_NO_THREADS
#include "./lib/physac.h"

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
    int vida;
    PhysicsBody body;
    int orientation;
    int walking;
    int max_frames;
} Player;

//-------------


//=================
// Variaveis gloais

int screenWidth = 1300;
int screenHeight = 600;

static bool gameOver = false;
static bool pause =  false;
static bool victory = false;

static Player player;

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
    
    InitWindow(screenWidth, screenHeight, "Primeiro teste");
    screenWidth = GetMonitorWidth(0);
    screenHeight = GetMonitorHeight(0);
    CloseWindow();
    InitWindow(screenWidth, screenHeight, "Primeiro teste");
    
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
    
    initGame();
    
    
    player.max_frames = 5;
    
    while(!WindowShouldClose() && victory==false){
        
        
        
        
        RunPhysicsStep();
        
        BeginDrawing();
        //ClearBackground(BLACK);
        
        framesCounter++;
        
        
        switch(0){
            
            case 0:
            
            if(transparencia <=0) sobe = true;
            if(transparencia>=255) sobe=false;
            
            if(sobe==true) transparencia+=3; else transparencia-=3;
            
            DrawTexturePro(menuBG, (Rectangle){0,0, menuBG.width, menuBG.height}, (Rectangle){0,0,menuBG.width*2, menuBG.height*2.2}, (Vector2){0,0}, 0, GRAY);
            
            DrawTextEx(vikingFont, "The Valkyrie's Quest", (Vector2){screenWidth/4, screenHeight/4}, 50,0,WHITE);
            DrawTextEx(vikingFont, "Presione enter para iniciar", (Vector2){screenWidth/2.8, screenHeight*80/100}, 20,0, (Color){255, 255, 255, transparencia});
            
            break;
            
        }
        

        //DrawRectangleRec((Rectangle){player.body->position.x-player.rec.width/2, player.body->position.y-player.rec.height/2, player.rec.width, player.rec.height}, player.color);
        
        if(framesCounter>=(60/8)){
            framesCounter=0;
            
            currentFrame++;
            
            if(currentFrame>=player.max_frames) currentFrame=0;
        }
        
        if(player.walking==0){
        DrawTextureRec(hilda[currentFrame], (Rectangle){-hilda[currentFrame].width/1.3, -hilda[currentFrame].height/1.25, player.rec.width*player.orientation, player.rec.height}, (Vector2){player.body->position.x-player.rec.width/2, player.body->position.y-player.rec.height/2}, WHITE);
        player.max_frames = 5;
        } else {
            DrawTextureRec(hildaRun[currentFrame], (Rectangle){-hildaRun[currentFrame].width/1.3, -hildaRun[currentFrame].height/1.25, player.rec.width*player.orientation, player.rec.height}, (Vector2){player.body->position.x-player.rec.width/2, player.body->position.y-player.rec.height/2}, WHITE);
            player.max_frames = 8;
        }
        DrawTexture(hildaRun[currentFrame],100,100,WHITE);
        
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
    player.speed = 0.2;
    player.color = YELLOW;
    player.max_frames=5;
    player.walking=0;
    player.orientation=1;
    
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
