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
    Texture2D texture;
} Player;

//-------------


//=================
// Variaveis gloais

static const int screenWidth = 1300;
static const int screenHeight = 600;

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




int main(){
    srand(time(NULL));
    
    InitWindow(screenWidth, screenHeight, "The Valkyrie's Quest");
    SetTargetFPS(60);
    
    
    InitPhysics();
    initGame();
    SetPhysicsGravity(0, 0);
    
    
    while(!WindowShouldClose() && victory==false){
        
        
        RunPhysicsStep();
        
        BeginDrawing();
        ClearBackground(BLACK);

        DrawRectangleRec((Rectangle){player.body->position.x-player.rec.width/2, player.body->position.y-player.rec.height/2, player.rec.width, player.rec.height}, player.color);
        
        movement();
        
        
        
        drawPhysicsEdge();
        
        
        EndDrawing();
        
    }
    
    CloseWindow(); 
}

void initGame(){
    // inicia os paramentros do jogo
    
    // jogador
    //player.texture = LoadTexture("arquivo da imagem");
    
    
    
    
    player.rec.width = 25;
    player.rec.height = 25;
    player.speed = 0.2;
    player.color = YELLOW;
    
    player.body = CreatePhysicsBodyRectangle((Vector2){screenWidth/2, screenHeight/2}, player.rec.width, player.rec.height, 10);
    
    player.rec.x = player.body->position.x;
    player.rec.y = player.body->position.y;
    player.body->freezeOrient=true;
    
}

void movement(){
    // movimento do personagem
    
    if(IsKeyDown(KEY_RIGHT) && player.rec.x<screenWidth-40) {
        player.body->velocity.x = player.speed;
    }
    if(IsKeyDown(KEY_LEFT) && player.rec.x>30){
        player.body->velocity.x = -player.speed;
    }
    if(IsKeyDown(KEY_DOWN) && player.rec.y<screenHeight-25){
        player.body->velocity.y = player.speed;
    }
    if(IsKeyDown(KEY_UP) && player.rec.y>20){
        player.body->velocity.y = -player.speed;
    }
    
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
