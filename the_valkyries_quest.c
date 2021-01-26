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


//-------------


//=================
// Variaveis gloais

static const int screenWidth = 1000;
static const int screenHeight = 650;

//-----------------


//========
// Funções


//--------



int main(){
    srand(time(NULL));
    
    InitWindow(screenWidth, screenHeight, "The Valkyrie's Quest");
    SetTargetFPS(60);
    
    
    
    while(!WindowShouldClose()){
        
        //alteração
        
        BeginDrawing();
        ClearBackground(BLACK);
        
        
        EndDrawing();
        
    }
    
    CloseWindow();
    
}
