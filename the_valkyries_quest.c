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
#define PHYSAC_DEBUG
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

typedef struct GameObject {
    Rectangle rec;
    float speed;
    Color color;
    float vida;
    PhysicsBody body;
    int orientation;
    int max_frames;
    int frames;
    int mode;
    bool enabled;
    clock_t cooldown;
} GameObject;
//-------------


//=================
// Variaveis gloais

int screenWidth = 1400;
int screenHeight = 600;

static bool gameOver = false;
static bool pause =  false;
static bool victory = false;


static Player player;
GameObject beowulf;

static int level = 0;

int posParallax = 0;

Camera2D camera;

bool criouCorpos = 0;

int currentFrame = 0;
clock_t timer_dash;

int transparencia = 0;
int sobe = true;
    
int framesCounter = 0;
static int pegou_pocao = 0;
static int pegou_pocao_atk = 0;

static int aparecefase2 = 0;
  
static int aparece_pilar = 0;

static int some_pilar = 0;

static int trava_camera = 0;




int morreu = 0;
//-----------------


//========
// Funções

static void initGame(void);
static void movement(void);
void delay(float seconds);
void drawPhysicsEdge(void);
void drawHearts(void);
void criaresqueleto(int bglvl_width, int sklt_height, int sklt_width, GameObject* esqueleto, int posicaox);
void destroyAllBodies(void);
void esqueletosIA(GameObject* esqueleto, Texture2D bglvl1, int framesCounter);
void level_0(Texture2D menuBG, Font vikingFont, Texture2D* hildaRun);
void level_1(void);
void reinicializar(Font vikingFont, int screenWidth, int screenHeight, int transparencia);
void criarcogumelo(int bglvl1_width, int mushroom_height, int mushroom_width, GameObject* cogumelo);
void CogumelosIA(GameObject* cogumelo, Texture2D bglvl1, int framesCounter);
void criarBeowulf(int beowulf_height, int beowulf_width, Vector2 iniciodoLvl);
void BeowulfIA();
void criarzubat(int bglvl_width, int sklt_height, int sklt_width, GameObject* OIAO);
void ZubatsIA(GameObject* OIAO, Texture2D bglvl1, int framesCounter);
//--------


Texture2D hilda[6];
Texture2D hildaRun[8];

Texture2D heart;

Texture2D skeletonIdle;
Texture2D skeletonAtk;
Texture2D skeletonWalk;
Texture2D skeletonDead;

Texture2D mushroomIdle;
Texture2D mushroomWalk;
Texture2D mushroomDead;
Texture2D mushroomAtk;
Texture2D mushroomHit;

Texture2D beowulfIdle;
Texture2D beowulfWalk;
Texture2D beowulfAttack;
Texture2D beowulfDashAttack;
Texture2D beowulfSlash;
Texture2D beowulfStomp;
Texture2D beowulfDeath;

Texture2D OIAOIdle;
Texture2D OIAOWalk;
Texture2D OIAODead;
Texture2D OIAOAtk;
Texture2D OIAOHit;


Font vikingFont;
Music zeldaMus;
Sound sound_hilda_atk;
Sound som_pocao;
Music BeowulfMus;


Vector2 iniciodoLvl;

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

    
    Vector2 iniciodoLvl = {0,screenHeight*80/100};
    
    
    
    Texture2D menuBG = LoadTexture("imagens/arvore_da_vida.png");
    Font vikingFont = LoadFont("VIKING-N.TTF");
    Font superMario = LoadFont("Super-Mario-World.ttf");
    zeldaMus = LoadMusicStream("som/musica_do_game1.ogg");
    sound_hilda_atk = LoadSound("som/ha.ogg");
    SetSoundVolume(sound_hilda_atk, 0.2f);
    som_pocao = LoadSound("som/som_pocao.wav");
    BeowulfMus = LoadMusicStream("som/musica_beowulf.ogg");
    
    
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
    
     Texture2D bglvl2 = LoadTexture("imagens/cenario2/bglvl1.png");
     
     Texture2D bglvl3 = LoadTexture("imagens/cenario3/bglvl3.png");

    Texture2D plataformas[2] = {
        LoadTexture("imagens/cenario/plataforma1.png"),
    LoadTexture("imagens/cenario/plataforma2.png"),
    };
        
        Texture2D plataformas2[2] = {
        LoadTexture("imagens/cenario2/plataformas.png"),
        LoadTexture("imagens/cenario2/plataformas2.png"),
    };
    
     Texture2D plataforminha[2] = {
        LoadTexture("imagens/cenario2/plataforminha.png"),
        LoadTexture("imagens/cenario2/plataforminha.png"),
    };
    
    Texture2D espinhos[2] = {
        LoadTexture("imagens/cenario/espinhos.png"),
        LoadTexture("imagens/cenario/espinhos.png"),

    };
    
    Texture2D pilar[2] = {
        LoadTexture("imagens/cenario/pilar.png"),
        LoadTexture("imagens/cenario/pilar.png"),
    };
        
    Texture2D pilar2[2] = {
        LoadTexture("imagens/cenario2/pilar.png"),
        LoadTexture("imagens/cenario2/pilar.png"),

    };
     Texture2D terra[2] = {
        LoadTexture("imagens/cenario/terra.png"),
        LoadTexture("imagens/cenario/terra.png"),

    };
    Texture2D pocao[2] = {
        LoadTexture("imagens/itens/pocao.png"),
        LoadTexture("imagens/itens/pocao.png"),
        
    };
    Texture2D pocao_atk[2] = {
        LoadTexture("imagens/itens/pocao_atk.png"),
        LoadTexture("imagens/itens/pocao_atk.png"),
        
    };
    
    Texture2D portal[2] = {
        LoadTexture("imagens/cenario/portal.png"),
        LoadTexture("imagens/cenario/portal.png"),
    };
    
    Texture2D bloco[2] = {
        LoadTexture("imagens/cenario2/bloco.png"),
        LoadTexture("imagens/cenario2/bloco.png"),
    };
    
    
    beowulfIdle = LoadTexture("imagens/beowulf/beowulf-idle.png"); 
    beowulfWalk = LoadTexture("imagens/beowulf/beowulf-walk.png");
    beowulfAttack = LoadTexture("imagens/beowulf/beowulf-attack.png");
    beowulfDashAttack = LoadTexture("imagens/beowulf/beowulf-dash-attack.png");
    beowulfSlash = LoadTexture("imagens/beowulf/beowulf-slash.png");
    beowulfStomp = LoadTexture("imagens/beowulf/beowulf-ground-stomp.png");
    beowulfDeath = LoadTexture("imagens/beowulf/beowulf-deathteste1.png"); //teste
    
    skeletonIdle = LoadTexture("imagens/esqueleto/Skeleton Idle.png");
    skeletonAtk = LoadTexture("imagens/esqueleto/Skeleton Attack.png");
    skeletonWalk = LoadTexture("imagens/esqueleto/Skeleton Walk.png");
    skeletonDead = LoadTexture("imagens/esqueleto/Skeleton Dead.png");
    
    mushroomIdle = LoadTexture("imagens/cogumelo/Idle.png");
    mushroomAtk = LoadTexture("imagens/cogumelo/Attack.png");
    mushroomWalk = LoadTexture("imagens/cogumelo/Run.png");
    mushroomDead = LoadTexture("imagens/cogumelo/Morto.png");
    mushroomHit = LoadTexture("imagens/cogumelo/Take Hit.png");
    
    OIAOIdle = LoadTexture("imagens/zubat/Flying.png");
    OIAOAtk = LoadTexture("imagens/zubat/Attack.png");
    OIAOWalk = LoadTexture("imagens/zubat/Flying.png");
    OIAODead = LoadTexture("imagens/zubat/Deadth.png");
    OIAOHit = LoadTexture("imagens/zubat/Take Hit.png");
    
    heart = LoadTexture("imagens/heart_animated_2.png");
    Texture2D health_bar = LoadTexture("imagens/health_bar_decoration.png");

    Texture2D chao1 = LoadTexture("imagens/cenario/chao1.png");
    
    Texture2D chao2 = LoadTexture("imagens/cenario2/chao2.png");
    
    Texture2D chao3 = LoadTexture("imagens/cenario3/chao3.png");
          
    GameObject esqueleto[4];
    GameObject cogumelo[5];
    GameObject zubat[4];
    
    initGame();
    
    PlayMusicStream(zeldaMus);
    //PlayMusicStream(BeowulfMus);
    player.max_frames = 5;
    
    while(!WindowShouldClose() && victory==false){
        
        
        
        
        //RunPhysicsStep();
        
        BeginDrawing();
        
        ClearBackground(BLACK);
        
        framesCounter++;

        if(framesCounter>=(60/8)){
            //framesCounter=0;
            
            currentFrame++;
            
            if(currentFrame>=player.max_frames) {
                if(player.mode!=6) currentFrame = 0;
                if(player.mode == 3 || player.mode == 4 || player.mode == 5) player.mode = 0;
                }
            
        }

        
        switch(level){
            
            case 0:
            
            UpdateMusicStream(zeldaMus);

            level_0(menuBG, vikingFont, hildaRun);

            break;


            case 1: ;
            
            level_1();

            break;

            case 2:
            
            
            if(criouCorpos==false){
                CreatePhysicsBodyRectangle((Vector2){0+chao1.width/2,(screenHeight*80/100)+chao1.height/2}, chao1.width, chao1.height, 1)->enabled=false;

                CreatePhysicsBodyRectangle((Vector2){iniciodoLvl.x+2750+plataformas[1].width/2, iniciodoLvl.y-260+plataformas[1].height/2}, plataformas[1].width, plataformas[1].height,1)->enabled=false;
                
                CreatePhysicsBodyRectangle((Vector2){iniciodoLvl.x+2300+plataformas[1].width/2, iniciodoLvl.y-260+plataformas[1].height/2}, plataformas[1].width, plataformas[1].height,1)->enabled=false;
                
                CreatePhysicsBodyRectangle((Vector2){iniciodoLvl.x+1900+plataformas[1].width/2, iniciodoLvl.y-180+plataformas[1].height/2}, plataformas[1].width, plataformas[1].height,1)->enabled=false;  
    
                CreatePhysicsBodyRectangle((Vector2){iniciodoLvl.x+4000+plataformas[1].width/2, iniciodoLvl.y-180+plataformas[1].height/2}, plataformas[1].width, plataformas[1].height,1)->enabled=false;
                
                CreatePhysicsBodyRectangle((Vector2){iniciodoLvl.x+4200+plataformas[1].width/2, iniciodoLvl.y-340+plataformas[1].height/2}, plataformas[1].width, plataformas[1].height,1)->enabled=false;
                
                CreatePhysicsBodyRectangle((Vector2){iniciodoLvl.x+5000+terra[1].width/2, iniciodoLvl.y-180+terra[1].height/2}, terra[1].width, terra[1].height,1)->enabled=false;
                
                initGame();
                criaresqueleto(bglvl1.width, skeletonIdle.width, skeletonIdle.height, esqueleto, rand() % bglvl1.width);
                
            }
            BeginMode2D(camera);
            
            //DrawTexture(bglvl1,0,(screenHeight*80/100)-bglvl1.height,WHITE);
            DrawTexturePro(bglvl1, (Rectangle){0,0, bglvl1.width, bglvl1.height}, (Rectangle){0,(screenHeight*80/100)-bglvl1.height,bglvl1.width*2, bglvl1.height+(10/100*screenHeight)},(Vector2){0,0},0,WHITE);
            
            //drawPhysicsEdge();
              
            DrawTexture(plataformas[1], iniciodoLvl.x+1900, iniciodoLvl.y-180, WHITE);    
            DrawTexture(plataformas[1], iniciodoLvl.x+2300, iniciodoLvl.y-260, WHITE);
            DrawTexture(plataformas[1], iniciodoLvl.x+2750, iniciodoLvl.y-260, WHITE);
            DrawTexture(plataformas[1], iniciodoLvl.x+4000, iniciodoLvl.y-180, WHITE);
            DrawTexture(plataformas[1], iniciodoLvl.x+4200, iniciodoLvl.y-340, WHITE);

            DrawTexture(terra[1], iniciodoLvl.x+5000, iniciodoLvl.y-180, WHITE);
            
            DrawTexture(portal[1], iniciodoLvl.x+6000, iniciodoLvl.y-180, WHITE);
            
            

            //DrawRectangleRec((Rectangle){iniciodoLvl.x+2300, iniciodoLvl.y-35, espinhos[1].width*14.5f, espinhos[1].height*60/100}, (Color){255,0,0,100});
            if( CheckCollisionRecs(player.rec, (Rectangle){iniciodoLvl.x+2300, iniciodoLvl.y-35, espinhos[1].width*14.0f, espinhos[1].height*60/100}))  player.vida-=1;
            
            if( CheckCollisionRecs(player.rec, (Rectangle){iniciodoLvl.x+5000, iniciodoLvl.y-180, espinhos[1].width*12.0f, espinhos[1].height*60/100}))  player.vida-=1;
            
            
            if( CheckCollisionRecs(player.rec, (Rectangle){iniciodoLvl.x+6100, iniciodoLvl.y-100, portal[1].width*1.0f, portal[1].height*60/100}) ) {
                
                destroyAllBodies();
            level++;
            criouCorpos = false;
            
               
                
            }
            

            
            
            if( CheckCollisionRecs(player.rec, (Rectangle){iniciodoLvl.x+4320, iniciodoLvl.y-375, pocao[1].width*1.0f, pocao[1].height*60/100}) && pegou_pocao==0) {
                if (player.vida < 40) {
                    player.vida+=8;
                    if (player.vida > 40) player.vida = 40;
                }
                PlaySound(som_pocao);
                pegou_pocao = 1; 
                
            }
            
            if (pegou_pocao == 0) {
                DrawTexture(pocao[1], iniciodoLvl.x+4320, iniciodoLvl.y-375, WHITE);
            }
            
            
             
            
            
            
            if( CheckCollisionRecs(player.rec, (Rectangle){iniciodoLvl.x+5050, iniciodoLvl.y-170, pilar[1].width*1.0f, pilar[1].height*60/100}) && aparece_pilar==0) {
               
                DestroyPhysicsBody(GetPhysicsBody(2));
                DestroyPhysicsBody(GetPhysicsBody(3));
                
                CreatePhysicsBodyRectangle((Vector2){iniciodoLvl.x+5900+pilar[1].width/2, iniciodoLvl.y-170+pilar[1].height/2}, pilar[1].width, pilar[1].height,1)->enabled=false;
                 CreatePhysicsBodyRectangle((Vector2){iniciodoLvl.x+5000+pilar[1].width/2, iniciodoLvl.y-170+pilar[1].height/2}, pilar[1].width, pilar[1].height,1)->enabled=false;
               
                
                for(int i=0;i<2;i++){
                        
                            if(esqueleto[i].body->enabled==true) DestroyPhysicsBody(esqueleto[i].body);
                        
                        
                    }
                criaresqueleto(bglvl1.width, skeletonIdle.width, skeletonIdle.height, esqueleto, rand() % 700+5200);
                aparece_pilar = 1;
            }
            
            if (aparece_pilar == 1) {
                
                DrawTexture(pilar[1], iniciodoLvl.x+5000, iniciodoLvl.y-170, WHITE);
                DrawTexture(pilar[1], iniciodoLvl.x+5900, iniciodoLvl.y-170, WHITE);
            }
            if( CheckCollisionRecs(player.rec,(Rectangle){iniciodoLvl.x+5800, iniciodoLvl.y-170, chao1.width*1.0f, chao1.height*60/100}) ) {


                DestroyPhysicsBody(GetPhysicsBody(6));
  
                CreatePhysicsBodyRectangle((Vector2){iniciodoLvl.x+3000+pilar[1].width/2, iniciodoLvl.y-170+pilar[1].height/2}, pilar[1].width, pilar[1].height,1)->enabled=false;
                
                
                
      
                aparece_pilar = 0;
                some_pilar = 1;

            }
            
            if( CheckCollisionRecs(player.rec,(Rectangle){iniciodoLvl.x+5500, iniciodoLvl.y-170, chao1.width*1.0f, chao1.height*60/100}) ) {


                trava_camera = 1;

            }
            
            if( CheckCollisionRecs(player.rec,(Rectangle){iniciodoLvl.x+6100, iniciodoLvl.y-170, chao1.width*1.0f, chao1.height*60/100}) ) {


                trava_camera = 0;

            }
            
            if(some_pilar == 1){
                if( CheckCollisionRecs(player.rec, (Rectangle){iniciodoLvl.x+5000, iniciodoLvl.y-35, espinhos[1].width*7.0f, espinhos[1].height*60/100}))  player.vida-=1;
                 for(int i=0;i<8;i++){
                
                
                DrawTexture(espinhos[1], iniciodoLvl.x+5000+(60*i), iniciodoLvl.y-60, WHITE);
            }
                
            }

        
            if (player.vida < 0.25) {
                player.mode = 6;
                morreu = 1;
            }

            for(int i=0;i<15;i++){
                DrawTexture(espinhos[1], iniciodoLvl.x+2300+(60*i), iniciodoLvl.y-60, WHITE);
                DrawTexture(espinhos[1], iniciodoLvl.x+5000+(60*i), iniciodoLvl.y-240, WHITE);
            }

             
         
            
          
            
            //DrawRectangleRec((Rectangle){iniciodoLvl.x+2750, iniciodoLvl.y-60, espinhos[1].width*16, espinhos[1].height}, GREEN);
            
            EndMode2D();

            drawHearts();
            // int esqueleto[i].frames;
            DrawText(FormatText("%i", GetPhysicsBodiesCount()), 100,300,20,WHITE);

            BeginMode2D(camera);

            

            esqueletosIA(esqueleto, bglvl1, framesCounter);
            

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
            else if (player.mode == 3){
                if(player.orientation == 1){
                DrawTextureRec(hildaAttack[currentFrame], (Rectangle){hildaAttack[currentFrame].width/4.4f, -hildaAttack[currentFrame].height/1.15, (hildaAttack[currentFrame].width/1.25f)*player.orientation, player.rec.height},(Vector2){player.body->position.x-player.rec.width/2, player.body->position.y-player.rec.height/2}, WHITE);
                
                }
                if(player.orientation == -1){
                DrawTextureRec(hildaAttack[currentFrame], (Rectangle){0, -hildaAttack[currentFrame].height/1.18f, (hildaAttack[currentFrame].width)*player.orientation, player.rec.height}, (Vector2){player.body->position.x-player.rec.width-36, player.body->position.y-player.rec.height/2}, WHITE);
                
                }
                if(currentFrame>=6){
                    for(int i=0;i<4;i++){
                        if(CheckCollisionRecs(esqueleto[i].rec, (Rectangle){player.rec.x+(hildaAttack[currentFrame].width/2.8f*player.orientation), player.rec.y, player.rec.width, player.rec.height}) && esqueleto[i].mode!=1){
                            //esqueleto[i].body->enabled=false;
                            esqueleto[i].mode = 1;
                            esqueleto[i].frames = 0;
                            if(esqueleto[i].body->enabled==true) DestroyPhysicsBody(esqueleto[i].body);
                        }
                        
                    }
                }
             
             
             player.max_frames = 12;
                
            }
  //drawPhysicsEdge            
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
            
            else if (player.mode == 6) {
                if(currentFrame>=10) {
                    currentFrame=10;
                    }
                if(player.orientation == 1) {
                DrawTextureRec(hildaDeath[currentFrame], (Rectangle){0, -hildaDeath[currentFrame].height/1.25, (hildaDeath[currentFrame].width/1.4f)*player.orientation, player.rec.height}, (Vector2){player.body->position.x-player.rec.width/2, player.body->position.y-player.rec.height/2}, WHITE);
                

                }
                if(player.orientation == -1) {
                DrawTextureRec(hildaDeath[currentFrame], (Rectangle){0, -hildaDeath[currentFrame].height/1.25, (hildaDeath[currentFrame].width/1.4f)*player.orientation, player.rec.height}, (Vector2){player.body->position.x-player.rec.width/2, player.body->position.y-player.rec.height/2}, WHITE);
                
               
                }
                
               player.max_frames = 12;  
               
               
               
            }
            
            DrawTexture(chao1,0,screenHeight*80/100,WHITE);

            

            if(IsKeyPressed(KEY_MINUS)) player.vida--;
            if(IsKeyPressed(KEY_EQUAL)) player.vida++;
            //DrawRectangle(player.rec.x+(hildaAttack[currentFrame].width/2.8f*player.orientation), player.rec.y, player.rec.width, player.rec.height, (Color){255,0,0,100});
            EndMode2D();
            

            
            break;
            
            case 3:
            
            if(criouCorpos==false){
                destroyAllBodies();
                CreatePhysicsBodyRectangle((Vector2){0+chao2.width/2,(screenHeight*80/100)+chao2.height/2}, chao2.width, chao2.height, 1)->enabled=false;


                CreatePhysicsBodyRectangle((Vector2){iniciodoLvl.x+1900+plataformas2[1].width/2, iniciodoLvl.y-180+plataformas2[1].height/2}, plataformas[1].width, plataformas2[1].height,1)->enabled=false;
                
                CreatePhysicsBodyRectangle((Vector2){iniciodoLvl.x+2750+pilar[1].width/2, iniciodoLvl.y-320+pilar[1].height/2}, pilar[1].width, pilar[1].height,1)->enabled=false;
                
                CreatePhysicsBodyRectangle((Vector2){iniciodoLvl.x+2500+plataforminha[1].width/2, iniciodoLvl.y-320+plataforminha[1].height/2}, plataforminha[1].width, plataforminha[1].height,1)->enabled=false;

                CreatePhysicsBodyRectangle((Vector2){iniciodoLvl.x+2200+plataforminha[1].width/2, iniciodoLvl.y-420+plataforminha[1].height/2}, plataforminha[1].width, plataforminha[1].height,1)->enabled=false;
                
                
                
                
               
                
                
                initGame();
                //criaresqueleto(bglvl1.width, skeletonIdle.width, skeletonIdle.height, esqueleto);
                criarcogumelo(bglvl1.width, mushroomIdle.width, mushroomIdle.height, cogumelo);
                //criarBeowulf(beowulfIdle.height, beowulfIdle.width, iniciodoLvl);
                //criarzubat(bglvl1.width, OIAOIdle.width, OIAOIdle.height, zubat);
                
                
                criouCorpos = true;
            }
            
            
            BeginMode2D(camera);
            
            
            
            
            //DrawTexture(bglvl1,0,(screenHeight*80/100)-bglvl1.height,WHITE);
            DrawTexturePro(bglvl2, (Rectangle){0,0, bglvl2.width, bglvl1.height}, (Rectangle){0,(screenHeight*80/100)-bglvl2.height,bglvl1.width*2, bglvl2.height+(10/100*screenHeight)},(Vector2){0,0},0,WHITE);
            //esqueletosIA(esqueleto, bglvl2, framesCounter);
            CogumelosIA(cogumelo, bglvl2, framesCounter);
            BeowulfIA();
            //drawPhysicsEdge();
              
            DrawTexture(plataformas2[1], iniciodoLvl.x+1900, iniciodoLvl.y-180, WHITE);
            DrawTexture(plataforminha[1], iniciodoLvl.x+2200, iniciodoLvl.y-420, WHITE);
            DrawTexture(plataforminha[1], iniciodoLvl.x+2500, iniciodoLvl.y-320, WHITE);
            DrawTexture(pilar2[1], iniciodoLvl.x+2750, iniciodoLvl.y-320, WHITE);
            
            DrawTexture(plataformas2[1], iniciodoLvl.x+4250, iniciodoLvl.y-350, WHITE);
            DrawTexture(plataformas2[1], iniciodoLvl.x+4750, iniciodoLvl.y-350, WHITE);
            
            DrawTexture(plataforminha[1], iniciodoLvl.x+4000, iniciodoLvl.y-240, WHITE);
            DrawTexture(plataforminha[1], iniciodoLvl.x+5235, iniciodoLvl.y-240, WHITE);
            
            DrawTexture(portal[1], iniciodoLvl.x+6000, iniciodoLvl.y-180, WHITE);
            
            
            for(int i=0;i<12;i++){
                DrawTexture(espinhos[1], iniciodoLvl.x+4250+(60*i), iniciodoLvl.y-60, WHITE);
                
            }

            
            
            if( CheckCollisionRecs(player.rec, (Rectangle){iniciodoLvl.x+6100, iniciodoLvl.y-100, portal[1].width*1.0f, portal[1].height*60/100}) ) {
                
                destroyAllBodies();
            level++;
            criouCorpos = false;
            
               
                
            }
            if( CheckCollisionRecs(player.rec, (Rectangle){iniciodoLvl.x+4250, iniciodoLvl.y-35, espinhos[1].width*12.0f, espinhos[1].height*60/100}))  player.vida-=1;
             if( CheckCollisionRecs(player.rec, (Rectangle){iniciodoLvl.x+3600, iniciodoLvl.y-100, chao2.width*1.0f, chao2.height*60/100}) && aparecefase2==0) {
                
                DestroyPhysicsBody(GetPhysicsBody(1));
                DestroyPhysicsBody(GetPhysicsBody(2));
                DestroyPhysicsBody(GetPhysicsBody(8));
               
                
                
                
                
                CreatePhysicsBodyRectangle((Vector2){iniciodoLvl.x+4250+plataformas2[1].width/2, iniciodoLvl.y-350+plataformas2[1].height/2}, plataformas2[1].width, plataformas2[1].height,1)->enabled=false;
                 CreatePhysicsBodyRectangle((Vector2){iniciodoLvl.x+5235+plataforminha[1].width/2, iniciodoLvl.y-240+plataforminha[1].height/2}, plataforminha[1].width, plataforminha[1].height,1)->enabled=false;
                  CreatePhysicsBodyRectangle((Vector2){iniciodoLvl.x+4000+plataforminha[1].width/2, iniciodoLvl.y-240+plataforminha[1].height/2}, plataforminha[1].width, plataforminha[1].height,1)->enabled=false;
                CreatePhysicsBodyRectangle((Vector2){iniciodoLvl.x+4750+plataformas2[1].width/2, iniciodoLvl.y-350+plataformas2[1].height/2}, plataformas2[1].width, plataformas2[1].height,1)->enabled=false;
               
                DestroyPhysicsBody(player.body);
                player.body = CreatePhysicsBodyRectangle((Vector2){player.rec.x+player.rec.width/2, player.rec.y+player.rec.height/2}, player.rec.width, player.rec.height, 1);
                player.body->freezeOrient=true;
                aparecefase2 = 1;
            }
            
            DrawTexture(pilar2[1], iniciodoLvl.x+2750, iniciodoLvl.y-320, WHITE);
             
            
            

            
          //DrawRectangleRec((Rectangle){iniciodoLvl.x+2300, iniciodoLvl.y-35, espinhos[1].width*14.5f, espinhos[1].height*60/100}, (Color){255,0,0,100});
          // if( CheckCollisionRecs(player.rec, (Rectangle){iniciodoLvl.x+2300, iniciodoLvl.y-35, espinhos[1].width*12.0f, espinhos[1].height*60/100}))  player.vida-=1;
          //  if( CheckCollisionRecs(player.rec, (Rectangle){iniciodoLvl.x+7000, iniciodoLvl.y-35, espinhos[1].width*13.5f, espinhos[1].height*60/100}))  player.vida-=1;
            if (player.vida < 0.25) {
                player.mode = 6;
                morreu = 1;
            }

  
            
            //DrawRectangleRec((Rectangle){iniciodoLvl.x+2750, iniciodoLvl.y-60, espinhos[1].width*16, espinhos[1].height}, GREEN);
            
            EndMode2D();

            drawHearts();
            // int esqueleto[i].frames;
            //DrawText(FormatText("%i", GetPhysicsBodiesCount()), 100,300,20,WHITE);

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
            else if (player.mode == 3) {
                if(player.orientation == 1){
                DrawTextureRec(hildaAttack[currentFrame], (Rectangle){hildaAttack[currentFrame].width/4.4f, -hildaAttack[currentFrame].height/1.15, (hildaAttack[currentFrame].width/1.25f)*player.orientation, player.rec.height}, (Vector2){player.body->position.x-player.rec.width/2, player.body->position.y-player.rec.height/2}, WHITE);
                }
                if(player.orientation == -1){
                DrawTextureRec(hildaAttack[currentFrame], (Rectangle){0, -hildaAttack[currentFrame].height/1.18f, (hildaAttack[currentFrame].width)*player.orientation, player.rec.height}, (Vector2){player.body->position.x-player.rec.width-36, player.body->position.y-player.rec.height/2}, WHITE);
                }
                if(currentFrame>=6){
                    for(int i=0;i<4;i++){
                        if(CheckCollisionRecs(cogumelo[i].rec, (Rectangle){player.rec.x+(hildaAttack[currentFrame].width/2.8f*player.orientation), player.rec.y, player.rec.width, player.rec.height}) && cogumelo[i].mode!=1){
                            //esqueleto[i].body->enabled=false;
                            cogumelo[i].mode = 1;
                            cogumelo[i].frames = 0;
                            if(cogumelo[i].body->enabled==true) DestroyPhysicsBody(cogumelo[i].body);
                        }
                    }
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
            else if (player.mode == 6) {
                if(currentFrame>=10) {
                    currentFrame=10;
                    }
                if(player.orientation == 1) {
                DrawTextureRec(hildaDeath[currentFrame], (Rectangle){0, -hildaDeath[currentFrame].height/1.25, (hildaDeath[currentFrame].width/1.4f)*player.orientation, player.rec.height}, (Vector2){player.body->position.x-player.rec.width/2, player.body->position.y-player.rec.height/2}, WHITE);
                }
                if(player.orientation == -1) {
                DrawTextureRec(hildaDeath[currentFrame], (Rectangle){0, -hildaDeath[currentFrame].height/1.25, (hildaDeath[currentFrame].width/1.4f)*player.orientation, player.rec.height}, (Vector2){player.body->position.x-player.rec.width/2, player.body->position.y-player.rec.height/2}, WHITE);
                }
               player.max_frames = 12;  
            }
            //drawPhysicsEdge();
            DrawTexture(chao2,0,screenHeight*80/100,WHITE);

            if(IsKeyPressed(KEY_MINUS)) player.vida--;
            if(IsKeyPressed(KEY_EQUAL)) player.vida++;
            //DrawRectangle(player.rec.x+(hildaAttack[currentFrame].width/2.8f*player.orientation), player.rec.y, player.rec.width, player.rec.height, (Color){255,0,0,100});
            EndMode2D();
            
            break;
            
            case 4:
            
            
            
            if(criouCorpos==false){
                PlayMusicStream(BeowulfMus);
                destroyAllBodies();
                CreatePhysicsBodyRectangle((Vector2){0+chao3.width/2,(screenHeight*80/100)+chao3.height/2}, chao3.width, chao3.height, 1)->enabled=false;
                
                
                
                CreatePhysicsBodyRectangle((Vector2){0+chao1.width/2,(screenHeight*80/100)+chao1.height/2}, chao1.width, chao1.height, 1)->enabled=false;
                
                CreatePhysicsBodyRectangle((Vector2){iniciodoLvl.x+1900+plataformas2[1].width/2, iniciodoLvl.y-180+plataformas2[1].height/2}, plataformas[1].width, plataformas2[1].height,1)->enabled=false;
                
                CreatePhysicsBodyRectangle((Vector2){iniciodoLvl.x+2750+pilar[1].width/2, iniciodoLvl.y-320+pilar[1].height/2}, pilar[1].width, pilar[1].height,1)->enabled=false;


                //CreatePhysicsBodyRectangle((Vector2){iniciodoLvl.x+1900+plataformas2[1].width/2, iniciodoLvl.y-180+plataformas2[1].height/2}, plataformas[1].width, plataformas2[1].height,1)->enabled=false;

               // CreatePhysicsBodyRectangle((Vector2){iniciodoLvl.x+1900+plataformas2[1].width/2, iniciodoLvl.y-180+plataformas2[1].height/2}, plataformas[1].width, plataformas2[1].height,1)->enabled=false;

               // CreatePhysicsBodyRectangle((Vector2){iniciodoLvl.x+3600+plataformas2[1].width/2, iniciodoLvl.y-180+plataformas2[1].height/2}, plataformas[1].width, plataformas2[1].height,1)->enabled=false;


                //CreatePhysicsBodyRectangle((Vector2){iniciodoLvl.x+2200+plataforminha[1].width/2, iniciodoLvl.y-420+plataforminha[1].height/2}, plataforminha[1].width, plataforminha[1].height,1)->enabled=false;
                
                 //CreatePhysicsBodyRectangle((Vector2){iniciodoLvl.x+3700+plataforminha[1].width/2, iniciodoLvl.y-420+plataforminha[1].height/2}, plataforminha[1].width, plataforminha[1].height,1)->enabled=false;
                
                //CreatePhysicsBodyRectangle((Vector2){iniciodoLvl.x+4000+plataforminha[1].width/2, iniciodoLvl.y-420+plataforminha[1].height/2}, plataforminha[1].width, plataforminha[1].height,1)->enabled=false;
 
                //CreatePhysicsBodyRectangle((Vector2){iniciodoLvl.x+2500+plataforminha[1].width/2, iniciodoLvl.y-320+plataforminha[1].height/2}, plataforminha[1].width, plataforminha[1].height,1)->enabled=false;
                                                              
                //CreatePhysicsBodyRectangle((Vector2){iniciodoLvl.x+2750+pilar[1].width/2, iniciodoLvl.y-320+pilar[1].height/2}, pilar[1].width, pilar[1].height,1)->enabled=false;
                
                initGame();
               // criaresqueleto(bglvl1.width, skeletonIdle.width, skeletonIdle.height, esqueleto);
              //  criarcogumelo(bglvl1.width, mushroomIdle.width, mushroomIdle.height, cogumelo);
                criarBeowulf(beowulfIdle.height, beowulfIdle.width, iniciodoLvl);
                criarzubat(bglvl1.width, OIAOIdle.width, OIAOIdle.height, zubat);
                
                criouCorpos = true;
            }
            
            UpdateMusicStream(BeowulfMus);
            
            BeginMode2D(camera);
            
            
            
            
            //DrawTexture(bglvl1,0,(screenHeight*80/100)-bglvl1.height,WHITE);
            DrawTexturePro(bglvl3, (Rectangle){0,0, bglvl3.width, bglvl3.height}, (Rectangle){0,(screenHeight*80/100)-bglvl3.height,bglvl1.width*2, bglvl3.height+(10/100*screenHeight)},(Vector2){0,0},0,WHITE);
            //esqueletosIA(esqueleto, bglvl2, framesCounter);
            //CogumelosIA(cogumelo, bglvl2, framesCounter);
            ZubatsIA(zubat, bglvl3, framesCounter);
            BeowulfIA();
            //drawPhysicsEdge();
            DrawTexture(chao3,0,screenHeight*80/100,WHITE);
            DrawTexture(plataformas2[1], iniciodoLvl.x+1900, iniciodoLvl.y-180, WHITE);
            DrawTexture(plataformas2[1], iniciodoLvl.x+3600, iniciodoLvl.y-180, WHITE);
            //DrawTexture(plataforminha[1], iniciodoLvl.x+2200, iniciodoLvl.y-420, WHITE);
            //DrawTexture(plataforminha[1], iniciodoLvl.x+2500, iniciodoLvl.y-320, WHITE);
            
            
             
            DrawTexture(pilar2[1], iniciodoLvl.x+2750, iniciodoLvl.y-320, WHITE);
            
          //DrawRectangleRec((Rectangle){iniciodoLvl.x+2300, iniciodoLvl.y-35, espinhos[1].width*14.5f, espinhos[1].height*60/100}, (Color){255,0,0,100});
          // if( CheckCollisionRecs(player.rec, (Rectangle){iniciodoLvl.x+2300, iniciodoLvl.y-35, espinhos[1].width*12.0f, espinhos[1].height*60/100}))  player.vida-=1;
          //  if( CheckCollisionRecs(player.rec, (Rectangle){iniciodoLvl.x+7000, iniciodoLvl.y-35, espinhos[1].width*13.5f, espinhos[1].height*60/100}))  player.vida-=1;
            if (player.vida < 0.25) {
                player.mode = 6;
                morreu = 1;
            }
            
            if( CheckCollisionRecs(player.rec, (Rectangle){iniciodoLvl.x+1920, iniciodoLvl.y-100, pocao_atk[1].width*1.0f, pocao_atk[1].height*60/100}) && pegou_pocao_atk==0) {
             
                PlaySound(som_pocao);
                pegou_pocao_atk = 1; 
                
            }
            
            if (pegou_pocao_atk == 0) {
                DrawTexture(pocao_atk[1], iniciodoLvl.x+1920, iniciodoLvl.y-32, WHITE);
            }

  
            
            //DrawRectangleRec((Rectangle){iniciodoLvl.x+2750, iniciodoLvl.y-60, espinhos[1].width*16, espinhos[1].height}, GREEN);
            
            EndMode2D();

            drawHearts();
            
            DrawRectangle(screenWidth*38/100+26,120, beowulf.vida, 18, (Color){122,0,0,255});
            DrawTexture(health_bar, screenWidth*38/100,110, WHITE);
            DrawTextEx(vikingFont, "BEOWULF", (Vector2){screenWidth*44/100, 150}, 20,0,WHITE);

            DrawText(FormatText("%i", GetPhysicsBodiesCount()), 100,300,20,WHITE);

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
            else if (player.mode == 3) {
                if(player.orientation == 1){
                DrawTextureRec(hildaAttack[currentFrame], (Rectangle){hildaAttack[currentFrame].width/4.4f, -hildaAttack[currentFrame].height/1.15, (hildaAttack[currentFrame].width/1.25f)*player.orientation, player.rec.height}, (Vector2){player.body->position.x-player.rec.width/2, player.body->position.y-player.rec.height/2}, WHITE);
                }
                if(player.orientation == -1){
                DrawTextureRec(hildaAttack[currentFrame], (Rectangle){0, -hildaAttack[currentFrame].height/1.18f, (hildaAttack[currentFrame].width)*player.orientation, player.rec.height}, (Vector2){player.body->position.x-player.rec.width-36, player.body->position.y-player.rec.height/2}, WHITE);
                }
                if(currentFrame>=6){
                    for(int i=0;i<4;i++){
                        if(CheckCollisionRecs(cogumelo[i].rec, (Rectangle){player.rec.x+(hildaAttack[currentFrame].width/2.8f*player.orientation), player.rec.y, player.rec.width, player.rec.height}) && cogumelo[i].mode!=1){
                            //esqueleto[i].body->enabled=false;
                            cogumelo[i].mode = 1;
                            cogumelo[i].frames = 0;
                            if(cogumelo[i].body->enabled==true) DestroyPhysicsBody(cogumelo[i].body);
                        }
                    }
                }
                //hildaAttack
                if(CheckCollisionRecs(beowulf.rec, (Rectangle){player.rec.x+(hildaAttack[currentFrame].width/2.8f*player.orientation), player.rec.y, player.rec.width, player.rec.height}) && (currentFrame==3 || currentFrame==6)){
                    beowulf.vida -= 100; //pra teste de morte
                    if(pegou_pocao_atk == 1){
                        beowulf.vida -= 200; //pra teste
                    }
                    beowulf.color = (Color){255,0,0,170};
                } else {
                    beowulf.color = WHITE;
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
            else if (player.mode == 6) {
                if(currentFrame>=10) {
                    currentFrame=10;
                    }
                if(player.orientation == 1) {
                DrawTextureRec(hildaDeath[currentFrame], (Rectangle){0, -hildaDeath[currentFrame].height/1.25, (hildaDeath[currentFrame].width/1.4f)*player.orientation, player.rec.height}, (Vector2){player.body->position.x-player.rec.width/2, player.body->position.y-player.rec.height/2}, WHITE);
                }
                if(player.orientation == -1) {
                DrawTextureRec(hildaDeath[currentFrame], (Rectangle){0, -hildaDeath[currentFrame].height/1.25, (hildaDeath[currentFrame].width/1.4f)*player.orientation, player.rec.height}, (Vector2){player.body->position.x-player.rec.width/2, player.body->position.y-player.rec.height/2}, WHITE);
                }
               player.max_frames = 12;  
            }
            //drawPhysicsEdge();
            DrawTexture(chao3,0,screenHeight*80/100,WHITE);

            if(IsKeyPressed(KEY_MINUS)) player.vida--;
            if(IsKeyPressed(KEY_EQUAL)) player.vida++;
            //DrawRectangle(player.rec.x+(hildaAttack[currentFrame].width/2.8f*player.orientation), player.rec.y, player.rec.width, player.rec.height, (Color){255,0,0,100});
            EndMode2D();
            
            break;
            
        }
        
        if(IsKeyPressed(KEY_PAGE_UP)) {
            destroyAllBodies();
            level++;
            criouCorpos = false;
            }
        if(IsKeyPressed(KEY_PAGE_DOWN)) level--;

        //DrawRectangleRec((Rectangle){player.body->position.x-player.rec.width/2, player.body->position.y-player.rec.height/2, player.rec.width, player.rec.height}, player.color);

        if(morreu==0) movement();

        if(framesCounter>=(60/8)){
            framesCounter=0;
        }

        //drawPhysicsEdge();
        if (morreu == 1) {
            //função de morte aqui
            reinicializar(vikingFont, screenWidth, screenHeight, transparencia);
        }
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

    player.vida = 40;
    
    player.body = CreatePhysicsBodyRectangle((Vector2){screenWidth/2, screenHeight/2}, player.rec.width, player.rec.height, 10);
    
    player.rec.x = player.body->position.x;
    player.rec.y = player.body->position.y;
    player.body->freezeOrient=true;
    player.prevPos.x = player.rec.x;
    player.prevPos.y = player.rec.y;


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
    if(IsKeyDown(KEY_LEFT) && !(IsKeyDown(KEY_X)) && player.body->position.x>screenWidth/4){
        player.body->velocity.x = -player.speed;
        if(player.mode != 3 && player.mode != 4 && player.mode != 5) player.mode = 1;
        player.max_frames = 8;
        player.orientation = -1;
    }
    
    
    if(player.caiu==false && player.body->isGrounded==true) {
        player.caiu = true;
        player.mode = 0;
    }
    if(IsKeyPressed(KEY_UP) && (player.body->isGrounded==true)){
        player.body->velocity.y = -2.25f;
        player.mode = 2;
        player.caiu = false;
        player.max_frames = 3;
    }

    //attack
    if(IsKeyPressed(KEY_Z) && !(IsKeyPressed(KEY_X)) && player.mode!=3) {
        //attack
        currentFrame = 0;
        player.max_frames = 12;
        player.mode = 3;
        if((rand() % 101) > 60){
            (currentFrame==0);
            PlaySound(sound_hilda_atk);
        }
    }
    

    //dash
    if(IsKeyPressed(KEY_X) && !(IsKeyPressed(KEY_Z)) && (clock()-timer_dash)/500>2) {
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


    
    if(player.body->position.x>screenWidth/2) camera.target = (Vector2){player.body->position.x, screenHeight/1.8f};  
    if(trava_camera == 1)  camera.target = (Vector2){iniciodoLvl.x+5450, screenHeight/1.8f};

    if(((IsKeyReleased(KEY_RIGHT) || IsKeyReleased(KEY_LEFT))) && player.mode != 3) player.mode = 0;
    
    //if (!IsKeyDown(KEY_UP) && !IsKeyDown(KEY_DOWN)) player.body->velocity.y = 0;
    //if (!IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT)) player.body->velocity.x = 0;
    
    player.rec.x = player.body->position.x-player.rec.width/2;
    player.rec.y = player.body->position.y-player.rec.height/2;

    //if(player.rec.y<=510.0f) player.body->isGrounded=true;        
    
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


//void drawPhysicsEdge(){
    //int bodiesCount = GetPhysicsBodiesCount();
    //for (int i = 0; i < bodiesCount; i++)
    //{
        //PhysicsBody body = GetPhysicsBody(i);
        //if (body != NULL)
        //{
            //int vertexCount = GetPhysicsShapeVerticesCount(i);
            //for (int j = 0; j < vertexCount; j++)
            //{
                // Get physics bodies shape vertices to draw lines
                // Note: GetPhysicsShapeVertex() already calculates rotation transformations
                //Vector2 vertexA = GetPhysicsShapeVertex(body, j);
                //int jj = (((j + 1) < vertexCount) ? (j + 1) : 0);   // Get next vertex or first to close the shape
                //Vector2 vertexB = GetPhysicsShapeVertex(body, jj);

                //DrawLineV(vertexA, vertexB, GREEN);     // Draw a line between two vertex positions
            //}
        //}
    //}
//}

void destroyAllBodies(){
    ResetPhysics();
}

void criaresqueleto(int bglvl_width, int sklt_height, int sklt_width, GameObject* esqueleto, int posicaox) {
    
    for(int i =0;i<3;i++){
        esqueleto[i].rec.x = posicaox;
        esqueleto[i].rec.y = player.rec.y;
        esqueleto[i].rec.height=sklt_width;
        esqueleto[i].rec.width=sklt_height/11;
        esqueleto[i].max_frames = 11;
        esqueleto[i].mode = 0;
        esqueleto[i].enabled = true;
        esqueleto[i].frames = 0;
                
        esqueleto[i].body = CreatePhysicsBodyRectangle((Vector2){esqueleto[i].rec.x, esqueleto[i].rec.y}, esqueleto[i].rec.width, esqueleto[i].rec.height, 1);
        esqueleto[i].body->freezeOrient=true;
    }
    criouCorpos = true;
}

void esqueletosIA(GameObject* esqueleto, Texture2D bglvl1, int framesCounter){
    for(int i=0;i<3;i++){
        if(esqueleto[i].enabled==true){
            if(esqueleto[i].mode == 0){
                if(esqueleto[i].body->velocity.x>(float){0.03f} || esqueleto[i].body->velocity.x<(float){-0.03f}){
                    DrawTextureRec(skeletonWalk, (Rectangle){(skeletonWalk.width/13)*esqueleto[i].frames, 0, (skeletonWalk.width/13)*esqueleto[i].orientation,skeletonWalk.height},(Vector2){esqueleto[i].body->position.x-esqueleto[i].rec.width/2, esqueleto[i].body->position.y-esqueleto[i].rec.height/2}, WHITE);
                    esqueleto[i].max_frames = 13;
                }
                else {
                    DrawTextureRec(skeletonIdle, (Rectangle){(skeletonIdle.width/11)*esqueleto[i].frames, 0, (skeletonIdle.width/11)*esqueleto[i].orientation,skeletonIdle.height},(Vector2){esqueleto[i].body->position.x-esqueleto[i].rec.width/2, esqueleto[i].body->position.y-esqueleto[i].rec.height/2}, WHITE);
                    esqueleto[i].max_frames = 11;
                }
                if(abs(esqueleto[i].body->position.x-player.body->position.x)<5){
                        esqueleto[i].body->velocity.x=0.0f;
                        esqueleto[i].orientation = 1;
                    }
                else if(esqueleto[i].body->position.x<player.body->position.x){
                        esqueleto[i].body->velocity.x = 0.1f;
                        esqueleto[i].orientation = 1;
                }
                else if(esqueleto[i].body->position.x>player.body->position.x) {
                        esqueleto[i].body->velocity.x = -0.1f;
                        esqueleto[i].orientation = -1;
                }
            }
                    
        //mode 1 = morto
        if(esqueleto[i].mode==1){
            esqueleto[i].max_frames = 15;
            DrawTextureRec(skeletonDead, (Rectangle){(skeletonDead.width/15)*esqueleto[i].frames, 0, (skeletonDead.width/15)*esqueleto[i].orientation,skeletonDead.height},(Vector2){esqueleto[i].body->position.x-esqueleto[i].rec.width/2, esqueleto[i].body->position.y-esqueleto[i].rec.height/2}, WHITE);
            if(esqueleto[i].frames>=14) {
                //esqueleto[i].enabled = false;
                esqueleto[i].rec.x = rand()%bglvl1.width;
                esqueleto[i].body = CreatePhysicsBodyRectangle((Vector2){esqueleto[i].rec.x, esqueleto[i].rec.y}, esqueleto[i].rec.width, esqueleto[i].rec.height, 1);               esqueleto[i].body->freezeOrient=true;
                esqueleto[i].mode = 0;
            }
        }
                
        if(abs(esqueleto[i].body->position.x-player.body->position.x)<esqueleto[i].rec.width && esqueleto[i].mode!=2 && esqueleto[i].mode!=1){
            esqueleto[i].mode = 2;
            esqueleto[i].max_frames = 18;
            esqueleto[i].frames = 0;
        }

        if(esqueleto[i].mode==2){
            DrawTextureRec(skeletonAtk, (Rectangle){(skeletonAtk.width/18)*esqueleto[i].frames, 0, (skeletonAtk.width/18)*esqueleto[i].orientation,skeletonAtk.height},(Vector2){esqueleto[i].body->position.x-esqueleto[i].rec.width, esqueleto[i].body->position.y-esqueleto[i].rec.height/1.47f}, WHITE);
            if(esqueleto[i].frames==8){
                if(CheckCollisionRecs(player.rec, (Rectangle){esqueleto[i].rec.x+10+(skeletonAtk.width/36*esqueleto[i].orientation), esqueleto[i].rec.y, 44, esqueleto[i].rec.height})){
                    player.vida-=1;
                }
            }
                    
            if(esqueleto[i].frames>=17) esqueleto[i].mode = 0;

        }
        //DrawRectangle(esqueleto[i].rec.x+10+(skeletonAtk.width/36*esqueleto[i].orientation), esqueleto[i].rec.y, 44, esqueleto[i].rec.height, (Color){255,0,0,100});
        esqueleto[i].rec.x = esqueleto[i].body->position.x-esqueleto[i].rec.width/2;
        esqueleto[i].rec.y = esqueleto[i].body->position.y-esqueleto[i].rec.height/2;
        if(framesCounter>=(60/8)){      
            esqueleto[i].frames++;
            if(esqueleto[i].frames>=esqueleto[i].max_frames) esqueleto[i].frames=0;
        }
        DrawText(FormatText("%i", esqueleto[i].frames), esqueleto[i].body->position.x, esqueleto[i].body->position.y-100, 20, WHITE);
        }
    }
}


void criarcogumelo(int bglvl_width, int mushroom_height, int mushroom_width, GameObject* cogumelo){
    for(int i =0;i<3;i++){
        cogumelo[i].rec.x = rand() % bglvl_width;
        cogumelo[i].rec.y = player.rec.y;
        cogumelo[i].rec.height=mushroom_width;
        cogumelo[i].rec.width=mushroom_height/4;
        cogumelo[i].max_frames = 4;
        cogumelo[i].mode = 0;
        cogumelo[i].enabled = true;
        cogumelo[i].frames = 0;
                
        cogumelo[i].body = CreatePhysicsBodyRectangle((Vector2){cogumelo[i].rec.x, cogumelo[i].rec.y}, cogumelo[i].rec.width, cogumelo[i].rec.height, 1);
        cogumelo[i].body->freezeOrient=true;
    }
    criouCorpos = true;
}

void CogumelosIA(GameObject* cogumelo, Texture2D bglvl1, int framesCounter){
    for(int i=0; i<3; i++){
        if(cogumelo[i].enabled==true) {
            if(cogumelo[i].mode == 0){
                    if(cogumelo[i].body->velocity.x>(float){0.03f} || cogumelo[i].body->velocity.x<(float){-0.03f}){
                    DrawTextureRec(mushroomWalk, (Rectangle){(mushroomWalk.width/8)*cogumelo[i].frames, 0, (mushroomWalk.width/8)*cogumelo[i].orientation,mushroomWalk.height},(Vector2){cogumelo[i].body->position.x-cogumelo[i].rec.width/2, cogumelo[i].body->position.y-cogumelo[i].rec.height/2}, WHITE);
                    cogumelo[i].max_frames = 8;
                } else {
                    DrawTextureRec(mushroomIdle, (Rectangle){(mushroomIdle.width/4)*cogumelo[i].frames, 0, (mushroomIdle.width/4)*cogumelo[i].orientation,mushroomIdle.height},(Vector2){cogumelo[i].body->position.x-cogumelo[i].rec.width/2, cogumelo[i].body->position.y-cogumelo[i].rec.height/2}, WHITE);
                    cogumelo[i].max_frames = 4;

                }
                if(abs(cogumelo[i].body->position.x-player.body->position.x)<5){
                        cogumelo[i].body->velocity.x=0.0f;
                        cogumelo[i].orientation = 1;
                    } else if(cogumelo[i].body->position.x<player.body->position.x){
                        cogumelo[i].body->velocity.x = 0.1f;
                        cogumelo[i].orientation = 1;
                    } else if(cogumelo[i].body->position.x>player.body->position.x) {
                        cogumelo[i].body->velocity.x = -0.1f;
                        cogumelo[i].orientation = -1;
                    }
                }
                    
                //mode 1 = morto
                if(cogumelo[i].mode==1){
                    cogumelo[i].max_frames = 4;
                    DrawTextureRec(mushroomDead, (Rectangle){(mushroomDead.width/5)*cogumelo[i].frames, 0, (mushroomDead.width/5)*cogumelo[i].orientation,mushroomDead.height},(Vector2){cogumelo[i].body->position.x-cogumelo[i].rec.width/2, cogumelo[i].body->position.y-cogumelo[i].rec.height/2}, WHITE);
                    if(cogumelo[i].frames>=4) {
                        //esqueleto[i].enabled = false;
                        cogumelo[i].rec.x = rand()%bglvl1.width;
                        cogumelo[i].body = CreatePhysicsBodyRectangle((Vector2){cogumelo[i].rec.x, cogumelo[i].rec.y}, cogumelo[i].rec.width, cogumelo[i].rec.height, 1);               cogumelo[i].body->freezeOrient=true;
                        cogumelo[i].mode = 0;
                    }
                }
                
                if(abs(cogumelo[i].body->position.x-player.body->position.x)<cogumelo[i].rec.width && cogumelo[i].mode!=2 && cogumelo[i].mode!=1){
                    cogumelo[i].mode = 2;
                    cogumelo[i].max_frames = 8;
                    cogumelo[i].frames = 0;
                }

                if(cogumelo[i].mode==2){
                    DrawTextureRec(mushroomAtk, (Rectangle){(mushroomAtk.width/8)*cogumelo[i].frames, 0, (mushroomAtk.width/8)*cogumelo[i].orientation,mushroomAtk.height},(Vector2){cogumelo[i].body->position.x-(mushroomAtk.width/8)/2, cogumelo[i].body->position.y-cogumelo[i].rec.height/1.47f}, WHITE);
                    if(cogumelo[i].frames==6){
                        if(CheckCollisionRecs(player.rec, (Rectangle){cogumelo[i].rec.x+10+(mushroomAtk.width/36*cogumelo[i].orientation), cogumelo[i].rec.y, 44, cogumelo[i].rec.height})){
                            player.vida-=3;
                        }
                    }
                    
                    if(cogumelo[i].frames>=7) cogumelo[i].mode = 0;

                }
                //DrawRectangle(esqueleto[i].rec.x+10+(skeletonAtk.width/36*esqueleto[i].orientation), esqueleto[i].rec.y, 44, esqueleto[i].rec.height, (Color){255,0,0,100});
                cogumelo[i].rec.x = cogumelo[i].body->position.x-cogumelo[i].rec.width/2;
                cogumelo[i].rec.y = cogumelo[i].body->position.y-cogumelo[i].rec.height/2;
                if(framesCounter>=(60/8)){
                    
                    cogumelo[i].frames++;
                    if(cogumelo[i].frames>=cogumelo[i].max_frames) cogumelo[i].frames=0;
                }
                DrawText(FormatText("%i", cogumelo[i].frames), cogumelo[i].body->position.x, cogumelo[i].body->position.y-100, 20, WHITE);
            }
        }
    }
 void criarzubat(int bglvl_width, int OIAO_height, int OIAO_width, GameObject* zubat) {
    
    for(int i =0;i<3;i++){
       zubat[i].rec.x = rand() % bglvl_width;
       zubat[i].rec.y = player.rec.y;
       zubat[i].rec.height=OIAO_width;
       zubat[i].rec.width=OIAO_height/4;
       zubat[i].max_frames = 8;
       zubat[i].mode = 0;
       zubat[i].enabled = true;
       zubat[i].frames = 0;
                
      zubat[i].body = CreatePhysicsBodyRectangle((Vector2){zubat[i].rec.x, zubat[i].rec.y}, zubat[i].rec.width, zubat[i].rec.height, 1);
      zubat[i].body->freezeOrient=true;
    }
    criouCorpos = true;
}
void ZubatsIA(GameObject* zubat, Texture2D bglvl1, int framesCounter){
    for(int i=0; i<3; i++){
        if(zubat[i].enabled==true) {
            if(zubat[i].mode == 0){
                    if(zubat[i].body->velocity.x>(float){0.03f} || zubat[i].body->velocity.x<(float){-0.03f}){
                    DrawTextureRec(OIAOWalk, (Rectangle){(OIAOWalk.width/8)*zubat[i].frames, 0, (OIAOWalk.width/8)*zubat[i].orientation,OIAOWalk.height},(Vector2){zubat[i].body->position.x-zubat[i].rec.width/2, zubat[i].body->position.y-zubat[i].rec.height/2}, WHITE);
                    zubat[i].max_frames = 8;
                } else {
                    DrawTextureRec(OIAOIdle, (Rectangle){(OIAOIdle.width/4)*zubat[i].frames, 0, (OIAOIdle.width/4)*zubat[i].orientation,OIAOIdle.height},(Vector2){zubat[i].body->position.x-zubat[i].rec.width/2, zubat[i].body->position.y-zubat[i].rec.height/2}, WHITE);
                    zubat[i].max_frames = 4;

                }
                if(abs(zubat[i].body->position.x-player.body->position.x)<5){
                        zubat[i].body->velocity.x=0.0f;
                        zubat[i].orientation = 1;
                    } else if(zubat[i].body->position.x<player.body->position.x){
                        zubat[i].body->velocity.x = 0.1f;
                        zubat[i].orientation = 1;
                    } else if(zubat[i].body->position.x>player.body->position.x) {
                        zubat[i].body->velocity.x = -0.1f;
                        zubat[i].orientation = -1;
                    }
                }
                    
                //mode 1 = morto
                if(zubat[i].mode==1){
                    zubat[i].max_frames = 4;
                    DrawTextureRec(OIAODead, (Rectangle){(OIAODead.width/5)*zubat[i].frames, 0, (OIAODead.width/5)*zubat[i].orientation,OIAODead.height},(Vector2){zubat[i].body->position.x-zubat[i].rec.width/2, zubat[i].body->position.y-zubat[i].rec.height/2}, WHITE);
                    if(zubat[i].frames>=4) {
                        //esqueleto[i].enabled = false;
                        zubat[i].rec.x = rand()%bglvl1.width;
                        zubat[i].body = CreatePhysicsBodyRectangle((Vector2){zubat[i].rec.x, zubat[i].rec.y}, zubat[i].rec.width, zubat[i].rec.height, 1);               zubat[i].body->freezeOrient=true;
                        zubat[i].mode = 0;
                    }
                }
                
                if(abs(zubat[i].body->position.x-player.body->position.x)<zubat[i].rec.width && zubat[i].mode!=2 && zubat[i].mode!=1){
                    zubat[i].mode = 2;
                    zubat[i].max_frames = 8;
                    zubat[i].frames = 0;
                }

                if(zubat[i].mode==2){
                    DrawTextureRec(OIAOAtk, (Rectangle){(OIAOAtk.width/8)*zubat[i].frames, 0, (OIAOAtk.width/8)*zubat[i].orientation,OIAOAtk.height},(Vector2){zubat[i].body->position.x-(OIAOAtk.width/8)/2, zubat[i].body->position.y-zubat[i].rec.height/1.47f}, WHITE);
                    if(zubat[i].frames==6){
                        if(CheckCollisionRecs(player.rec, (Rectangle){zubat[i].rec.x+10+(OIAOAtk.width/36*zubat[i].orientation), zubat[i].rec.y, 44, zubat[i].rec.height})){
                            player.vida-=3;
                        }
                    }
                    
                    if(zubat[i].frames>=7) zubat[i].mode = 0;

                }
                //DrawRectangle(esqueleto[i].rec.x+10+(skeletonAtk.width/36*esqueleto[i].orientation), esqueleto[i].rec.y, 44, esqueleto[i].rec.height, (Color){255,0,0,100});
                zubat[i].rec.x = zubat[i].body->position.x-zubat[i].rec.width/2;
                zubat[i].rec.y = zubat[i].body->position.y-zubat[i].rec.height/2;
                if(framesCounter>=(60/8)){
                    
                    zubat[i].frames++;
                    if(zubat[i].frames>=zubat[i].max_frames) zubat[i].frames=0;
                }
                DrawText(FormatText("%i", zubat[i].frames), zubat[i].body->position.x, zubat[i].body->position.y-100, 20, WHITE);
            }
        }
    }
void level_0(Texture2D menuBG, Font vikingFont, Texture2D* hildaRun){
        

        UpdateMusicStream(zeldaMus);
 

        if(transparencia <= 0) sobe = true;
        if(transparencia >= 255) sobe=false;
            
        if(sobe==true) transparencia+=3; else transparencia-=3;
            
        DrawTexturePro(menuBG, (Rectangle){0,0, menuBG.width, menuBG.height}, (Rectangle){0,0,screenWidth, screenHeight}, (Vector2){0,0}, 0, LIGHTGRAY);
            
        DrawTextEx(vikingFont, "The Valkyrie's Quest", (Vector2){screenWidth/3.4, screenHeight/4}, 50,0,WHITE);
        DrawTextEx(vikingFont, "Pressione enter para iniciar", (Vector2){screenWidth/2.5, screenHeight*80/100}, 20,0, (Color){255, 255, 255, transparencia});
            
        player.max_frames = 8;

        DrawTexture(hildaRun[currentFrame],100,100,WHITE);
        if(framesCounter>=(60/8)) currentFrame++;
        
        //DrawTextureRec(hildaRun[currentFrame], (Rectangle){hildaRun[currentFrame].width/4.6f, -hildaRun[currentFrame].height/1.25, (hildaRun[currentFrame].width/1.6f)*player.orientation, player.rec.height}, (Vector2){player.body->position.x-player.rec.width/2, player.body->position.y-player.rec.height/2}, WHITE);
        
        if(IsKeyPressed(KEY_ENTER)) {
            level++;
            UnloadTexture(menuBG);
        }
}

void level_1() {

     
     static int framesCounterText;
     static int paragrafo = 0;
     
     char texto[2][1000] = {"No início do mundo, não havia nada além do fogo e do gelo. No encontro destes dois elementos, surgiu a névoa primordial,\no Ginnungagap, e dela surgiu Ymir, o primeiro gigante, que daria origem para as duas raças: A raça dos Gigantes e os\nprimeiros grandes Deuses.Por estarem em constante conflito, os Deuses acabam assassinando Ymir, dando origem aos mundos.\nA árvore da vida era a responsável por portar os nove mundos, os Deuses se dividiram em dois clãs, os responsáveis pela guerra\n,Aesir, habitantes de Asgard, liderados por Bor, que passou este cargo para seu filho Odin, e infelizmente não passará para Thor,\npois sua morte já está escrita, os responsáveis pela Natureza, Vanir, habitantes de Vanaheim liderados por Frey e Freya,\nDeuses do verão e da primavera.\nA vida então se espalhou por todos os nove mundos, e como toda grande história, escrita em sangue, morte e heroismo.\nMidgard ate os dias atuais se perde em guerra, Alfheim, mundo dos elfos, perdeu-se em sua própria soberba, Nidavellir,\no mundo dos anoes, e assolado pelos elfos negros, Jotunheim, o mundo dos gigantes, e uma prisão de constante conflito e situações\nextremas e Muspelheim, o mundo dos gigantes de fogo e um literal inferno, alem dos reinos inalcançaveis.\nToda vida tem seu peso, e esse peso e medido, aqueles que morrem de maneira desonrosa caem nos poços de Helheim, cumprindo\nsua sentença para Hela, a Deusa da morte, e aqueles que morrem de maneira honrada, ganham a Bencao de ir para Valhalla,\no salao dos Deuses, onde herois comem, bebem, festejam e digladiam ate o fim.\n", "E existem aquelas responsaveis\npor dar a cada pessoa um destino apos a morte digno, as Valquirias, guerreiras escolhidas\npor Odin,que levam as almas dos mortos, e assim tudo funcionou por milênios...\nAte os dias de hoje... misteriosamente, a alma do lendário guerreiro Beowulf de alguma maneira misteriosa se corrompeu e caiu \nem Muspelheim, tendo reverberações também em Midgard e Nidavellir, atraindo monstros e caos.\nEm tempos de necessidade, como sempre, herois se levantam, e Odin selecionou Brunhilda, a mais forte das novas recrutas\n\npara Valquirias, responsavel por resgatar as almas, e assim conseguir sua honra em ganhar suas asas e se tornar uma\nverdadeira heroína…\n\n\nOdin: Levante-se, pequena. . ." };

 //por

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
}

void reinicializar(Font vikingFont, int screenWidth, int screenHeight, int transparencia) {
    
    
    DrawTextEx(vikingFont, "VOCE MORREU", (Vector2){screenWidth/3.5, screenHeight/4}, 80,0,WHITE);
    
    DrawTextEx(vikingFont, "Pressione enter para reiniciar", (Vector2){screenWidth/3.35, screenHeight*60/100}, 30,0, (Color){255, 255, 255, transparencia});
    
    if(IsKeyDown(KEY_ENTER)) {
        destroyAllBodies();
        criouCorpos = 0;
        morreu = 0;
        aparece_pilar = 0;
        pegou_pocao = 0;
        aparecefase2 = 0;
        
    }
    

}

void criarBeowulf(int beowulf_height, int beowulf_width, Vector2 iniciodoLvl) {
    beowulf.rec.x = iniciodoLvl.x + 100; //pra teste 
    beowulf.rec.y = player.rec.y-100;
    beowulf.rec.height = beowulf_height; //beowulfIdle.height
    beowulf.rec.width = beowulf_width / 3; //beowulfIdle.width
    beowulf.max_frames = 3;
    beowulf.mode = 0;
    beowulf.enabled = true;
    beowulf.frames = 0;
    beowulf.orientation = 1;
    beowulf.color = WHITE;
    beowulf.vida = 274;

    beowulf.cooldown = clock();

    beowulf.body = CreatePhysicsBodyRectangle((Vector2){beowulf.rec.x, beowulf.rec.y}, beowulf.rec.width, beowulf.rec.height, 1);
    beowulf.body->freezeOrient=true;
}

void BeowulfIA() {
    if(beowulf.enabled == true) {
        if(beowulf.mode == 0) {
            //beowulfWalk
            if(beowulf.body->velocity.x>(float){0.03f} || beowulf.body->velocity.x<(float){-0.03f}) {
                DrawTextureRec(beowulfWalk, (Rectangle){(beowulfWalk.width/4)*beowulf.frames, 0, (beowulfWalk.width/4)*beowulf.orientation,beowulfWalk.height},(Vector2){beowulf.rec.x-(beowulfWalk.width/4)/3.4f, beowulf.body->position.y-beowulf.rec.height/1.35f}, beowulf.color);
                beowulf.max_frames = 4;
            }
            //beowulfIdle    
            else {
                DrawTextureRec(beowulfIdle, (Rectangle){(beowulfIdle.width/3)*beowulf.frames, 0, (beowulfIdle.width/3)*beowulf.orientation,beowulfIdle.height},(Vector2){beowulf.body->position.x-beowulf.rec.width/2, beowulf.body->position.y-beowulf.rec.height/2}, beowulf.color);
                beowulf.max_frames = 3;
            }
    
            if(abs(beowulf.body->position.x-player.body->position.x)<5){
                beowulf.body->velocity.x=0.0f;
                beowulf.orientation = 1;
            }
            else if(beowulf.body->position.x<player.body->position.x){
                beowulf.body->velocity.x = 0.1f;
                beowulf.orientation = 1;
            }
            else if(beowulf.body->position.x>player.body->position.x) {
                beowulf.body->velocity.x = -0.1f;
                beowulf.orientation = -1;
            }
        }

        //beowulfAttack
        if(beowulf.mode == 1) {
            beowulf.max_frames = 6;
            
            DrawTextureRec(beowulfAttack, (Rectangle){(beowulfAttack.width/6)*beowulf.frames, 0, (beowulfAttack.width/6)*beowulf.orientation,beowulfAttack.height},(Vector2){beowulf.body->position.x-(beowulfAttack.width/6)/2, beowulf.body->position.y-beowulf.rec.height}, beowulf.color);
            
            if(beowulf.frames == 4) {
                if(CheckCollisionRecs(player.rec, (Rectangle) {beowulf.rec.x+10+(beowulfAttack.width/36*beowulf.orientation), beowulf.rec.y, 44, beowulf.rec.height})) {
                    player.vida -= 2;
                }
            }
            if(beowulf.frames>=5) beowulf.mode = 0;
        }
        //DrawRectangle(beowulf.rec.x+10+(beowulfAttack.width/36*beowulf.orientation), beowulf.rec.y, 60, beowulf.rec.height, WHITE);
        //beowulfDashAttack
        if(beowulf.mode == 2) {
            beowulf.max_frames = 1;
            static int counterDash = 0;
            counterDash++;
            beowulf.body->velocity.x=1.2f*beowulf.orientation;
            DrawTextureRec(beowulfDashAttack, (Rectangle){(beowulfDashAttack.width)*beowulf.frames, 0, (beowulfDashAttack.width)*beowulf.orientation,beowulfDashAttack.height},(Vector2){beowulf.body->position.x-(beowulfDashAttack.width/8)/2, beowulf.body->position.y-beowulf.rec.height/1.47f}, beowulf.color);
            
            if(CheckCollisionRecs(player.rec, (Rectangle) {beowulf.rec.x+10+(beowulfAttack.width/36*beowulf.orientation), beowulf.rec.y, 44, beowulf.rec.height})) {
                player.vida -=5;
            }
            if(counterDash>=25) {
                beowulf.mode = 0;
                counterDash=0;
                }
        }
        static bool projetil = false;
        //beowulfSlash
        if(beowulf.mode == 3) {
            beowulf.max_frames = 8;
            
            DrawTextureRec(beowulfSlash, (Rectangle){(beowulfSlash.width/8)*beowulf.frames, 0, (beowulfSlash.width/8)*beowulf.orientation,beowulfSlash.height},(Vector2){beowulf.body->position.x-(beowulfSlash.width/8)/2, beowulf.body->position.y-beowulf.rec.height/1.1f}, beowulf.color);
        //rand    
            if(CheckCollisionRecs(player.rec, (Rectangle) {beowulf.rec.x+10+(beowulfAttack.width/36*beowulf.orientation), beowulf.rec.y, 44, beowulf.rec.height})) {
                player.vida -=9;
            }
            if(beowulf.frames>=7) {
                beowulf.mode = 0;
                projetil = true;
            }
        }  
        
        if(projetil == true){
            static int projPosx = 0;
            DrawTextureRec(beowulfSlash, (Rectangle){(beowulfSlash.width/8)*7, 0, (beowulfSlash.width/8)*beowulf.orientation,beowulfSlash.height}, (Vector2){beowulf.body->position.x+projPosx*beowulf.orientation, beowulf.body->position.y-80}, WHITE);
            if(CheckCollisionRecs(player.rec, (Rectangle){beowulf.body->position.x+projPosx*beowulf.orientation, beowulf.body->position.y-80, (beowulfSlash.width/8),beowulfSlash.height})) player.vida -= 0.5f;
            projPosx+=14;
            if(projPosx>=500){
                projPosx=0;
                projetil = false;
            }
        }

        //beowulfStomp
        if(beowulf.mode == 4) {
            beowulf.max_frames = 10;
            
           DrawTextureRec(beowulfStomp, (Rectangle){(beowulfStomp.width/9)*beowulf.frames, 0, (beowulfStomp.width/9)*beowulf.orientation,beowulfStomp.height},(Vector2){beowulf.body->position.x-(beowulfStomp.width/8)/2, beowulf.body->position.y-beowulf.rec.height/1.47f}, beowulf.color);
            
            if(CheckCollisionRecs(player.rec, (Rectangle) {beowulf.rec.x+10+(beowulfAttack.width/36*beowulf.orientation), beowulf.rec.y, 44, beowulf.rec.height})) {
                player.vida -= 18;
            }
            if(beowulf.frames>=9) beowulf.mode = 0;
        }
        
        //beowulfDeath
        if(beowulf.mode == 5) {
            beowulf.max_frames = 6;
            
            DrawTextureRec(beowulfDeath, (Rectangle){(beowulfDeath.width/6)*beowulf.frames, 0, (beowulfDeath.width/6)*beowulf.orientation,beowulfDeath.height},(Vector2){beowulf.body->position.x-(beowulfDeath.width/8)/2, beowulf.body->position.y-beowulf.rec.height/1.85f}, beowulf.color);
            
             if(beowulf.frames > 5) {
                beowulf.frames = 6;
            }
            beowulf.mode = 5;
            beowulf.enabled = false;
        }
        
        //atacar
        if(abs(beowulf.body->position.x-player.body->position.x)<beowulf.rec.width && beowulf.mode ==0) {
            beowulf.mode = 1;
            beowulf.max_frames = 6;
            beowulf.frames = 0;
        }
        /* if(((beowulf.body->position.x-500 < player.body->position.x && beowulf.body->position.x-498 > player.body->position.x) ||
            (beowulf.body->position.x+500 > player.body->position.x && beowulf.body->position.x+498 < player.body->position.x))){
                if((rand() % 101) > 60) beowulf.mode = 2; else if((rand() % 101)>50) beowulf.mode = 3;
            } */

        if((clock()-beowulf.cooldown)/500==5){
            beowulf.mode = 3;
        }
        if((clock()-beowulf.cooldown)/500==8){
            if(abs(player.body->position.x-beowulf.body->position.x)>520) beowulf.mode = 2;
            beowulf.cooldown = clock();
        }
        
        //morrer
        if(beowulf.vida <= 0 && beowulf.mode != 5) {
            beowulf.mode = 5;
            beowulf.frames = 0;
        }
        
        if(framesCounter>=(60/8)){
            beowulf.frames++;
            if(beowulf.frames>=beowulf.max_frames) beowulf.frames = 0;
        }
        
        
        beowulf.rec.x = beowulf.body->position.x-beowulf.rec.width/2;
        beowulf.rec.y = beowulf.body->position.y-beowulf.rec.height/2;

    } else {
        //beowulfDeath
        if(beowulf.mode == 5) {
            beowulf.max_frames = 6;
            
            DrawTextureRec(beowulfDeath, (Rectangle){(beowulfDeath.width/6)*beowulf.frames, 0, (beowulfDeath.width/6)*beowulf.orientation,beowulfDeath.height},(Vector2){beowulf.body->position.x-(beowulfDeath.width/8)/2, beowulf.body->position.y-beowulf.rec.height/1.85f}, beowulf.color);
            
             if(beowulf.frames > 5) {
                beowulf.frames = 6;
            }
            beowulf.mode = 5;
        }
        if(framesCounter>=(60/8)){
            
            if(beowulf.frames <5){beowulf.frames++;
            if(beowulf.frames>=beowulf.max_frames) beowulf.frames = 0;}
        }
    }
}



