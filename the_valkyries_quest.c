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

typedef struct Valkyrie {
    Rectangle rec;
    Vector2 speed;
    Color color;
    Vector2 prevPos;
} Valkyrie;

//-------------


//=================
// Variaveis gloais

static const int screenWidth = 1000;
static const int screenHeight = 650;

//-----------------


//========
// Funções

static void initGame(void);
static void movement(void);

//--------



int main(){
    srand(time(NULL));
    
    InitWindow(screenWidth, screenHeight, "The Valkyrie's Quest");
    SetTargetFPS(60);
    
    
    
    while(!WindowShouldClose()){
        
        //alteração 2
        
        BeginDrawing();
        ClearBackground(BLACK);
        
        
        EndDrawing();
        
    }
    
    CloseWindow();
    
}

void initGame(){
    // inicia os paramentros do jogo
}

void movement(){
    // movimento do personagem
}
