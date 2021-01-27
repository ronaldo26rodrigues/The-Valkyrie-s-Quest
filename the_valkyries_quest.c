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
    Vector2 speed;
    Color color;
    Vector2 prevPos;
    int vida;
} Player;

//-------------


//=================
// Variaveis gloais

static const int screenWidth = 1000;
static const int screenHeight = 650;

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

//--------

//====================
// carregando texturas



//--------------------


int main(){
    srand(time(NULL));
    
    InitWindow(screenWidth, screenHeight, "The Valkyrie's Quest");
    SetTargetFPS(60);
    
    initGame();
    
    
    while(!WindowShouldClose()){
        
        //alteração 2
        
        BeginDrawing();
        ClearBackground(BLACK);
        
        DrawRectangleRec(player.rec, player.color);
        
        movement();
        
        
        EndDrawing();
        
    }
    
    CloseWindow(); 
}

void initGame(){
    // inicia os paramentros do jogo
    
    // jogador
    player.rec.x =  screenWidth/2;
    player.rec.y = screenHeight/2;
    player.rec.width = 25;
    player.rec.height = 25;
    player.speed.x = 5;
    player.speed.y = 5;
    player.color = YELLOW;
    
}

void movement(){
    // movimento do personagem
    
    if(IsKeyDown(KEY_RIGHT) && player.rec.x<screenWidth-40) {
        player.rec.x += player.speed.x;
    }
    if(IsKeyDown(KEY_LEFT) && player.rec.x>30){
        player.rec.x -= player.speed.x;
    }
    if(IsKeyDown(KEY_DOWN) && player.rec.y<screenHeight-25){
        player.rec.y += player.speed.y;
    }
    if(IsKeyDown(KEY_UP) && player.rec.y>20){
        player.rec.y -= player.speed.y;
    }
}

void delay(float seconds){
    float milliseconds = seconds*1000;
    clock_t start = clock();

    while((clock() - start) * 1000 / CLOCKS_PER_SEC < milliseconds);
}