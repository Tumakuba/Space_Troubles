#include "raylib.h"
#include "raymath.h"
#include <math.h>
<<<<<<< HEAD
#define rottrust 1.7f
#define fortrust 0.025f
#define maxSpeed1 3.5f
#define maxSpeed2 5.5f
#define maxVel 7.5f
#define brakeForce 0.07f
#define starslimit 200
#define MAX_METEORS 1000
#define asteroidRadius 20 

typedef struct Asteroid {
    Vector2 position;
    bool loaded;
} Asteroid;


static Vector2 RotatePoint(Vector2 point, Vector2 center,float rawAngle);
static Asteroid smallasteroid[MAX_METEORS] = { 0 };
static int screenw = 800;
static int screenh = 600;
static Vector2 playerPos = {400, 300};
static Vector2 shipWorldPos = {0,0};
static Vector2 planetWorldPos = {-100000,2000};


//fom the world center (shippos) ring around planet
//first meteor ring x = <-1700;-11300>                  [sheeld 2.stage]
//second mateor ring x = <-10900;-21200>  drill 2.stage [sheeld 3.stage]
//first gravity ring x = <-16000;-27999>  trusters 2.stage
//third meteor ring x = <-20000;-32500>
//second gravity ring x = <-28000;-40000>
//forth meteor ring x = <-29800;-40120>
//third gravity ring x = <-40001;75000> fatal for ship (R.I.P)

int main() {
  bool laserfire = false;
  float fuel = 80.0f;
  float playerAng = -90.0f;
  float charge = 1.0f;
  bool pause = false;
  float playerToPlanetAng;
  float playerToPlanetDis;
  Vector2 playerVel = {0,0};
  Vector2 playerWorldPos = {-300,-20};

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(screenw, screenh, "Space Troubles");
  SetTargetFPS(120);

  while (!WindowShouldClose()) {

    screenw = GetScreenWidth();
    screenh = GetScreenHeight();
    playerPos = (Vector2){screenw / 2.0f, screenh / 2.0f};
    int fov;
    if(screenw / 2 > screenh / 2) fov = screenw / 2;
    else fov = screenh / 2;

    if(IsKeyPressed('P'))pause = !pause;

    if(!pause){

      if (IsKeyDown(KEY_W)){
        playerVel.x += sinf(playerAng * DEG2RAD) * fortrust;
        playerVel.y += -cosf(playerAng * DEG2RAD) * fortrust;
        fuel -= 0.001;
      }
      if (Vector2Length(playerVel)> maxSpeed1){
        playerVel = Vector2Normalize(playerVel);
        playerVel.x *= maxSpeed1;
        playerVel.y *= maxSpeed1;
      }

      if (IsKeyDown(KEY_S)){
        Vector2 playerVelFlip;
        playerVelFlip = Vector2Negate(Vector2Normalize(playerVel));
        playerVel.x += playerVelFlip.x * brakeForce;
        playerVel.y += playerVelFlip.y * brakeForce;
        if (Vector2Length(playerVel) > 0.0001)fuel -= 0.0027;
      }
      if (Vector2Length(playerVel) < 0.05 && !IsKeyDown(KEY_W)){
        playerVel.x = 0;
        playerVel.y = 0;
      }

      if (IsKeyDown(KEY_D)) {
        playerAng  += rottrust;
        fuel -= 0.0001;
      }

      if (IsKeyDown(KEY_A)) {
        playerAng  -= rottrust;
        fuel -= 0.0001;
      }

      if (IsKeyDown(KEY_SPACE)) {
        if(charge <= 0.002)laserfire = false;
        else laserfire=true, charge -= 0.002, fuel -= 0.0015;
      }
      else laserfire = false;
      if(charge < 1 && !laserfire && !IsKeyDown(KEY_SPACE))charge += 0.001, fuel -= 0.0035;

      playerWorldPos.x += playerVel.x;
      playerWorldPos.y += playerVel.y;
    }

    if(Vector2Distance(planetWorldPos, playerWorldPos) > 96700 && Vector2Distance(planetWorldPos, playerWorldPos) < 101300){
      float posx, posy, distance;
      int spawnsNo = 0;
      for (int i = 0; i < MAX_METEORS; i++){
        if(!smallasteroid[i].loaded && spawnsNo < 2){
          bool corectPos = true;

          playerToPlanetAng = atan2f(playerWorldPos.y - planetWorldPos.y, playerWorldPos.x - planetWorldPos.x);
          playerToPlanetDis = Vector2Distance(playerWorldPos, planetWorldPos);

          float spredAngle = atan2f(3000.0f,playerToPlanetDis);
          float angle = playerToPlanetAng + GetRandomValue(-3000, 3000) * (spredAngle / 3000.0f);
          float distance = GetRandomValue(95100, 98300);

          posx = planetWorldPos.x + cosf(angle) * distance;
          posy = planetWorldPos.y + sinf(angle) * distance;

          smallasteroid[i].position = (Vector2){posx, posy};

/*

          angle = GetRandomValue(0,36000) * (DEG2RAD/100.0f);
          distance = GetRandomValue(fov + asteroidRadius, 3000);

          posx = playerWorldPos.x + cosf(angle) * distance;
          posy = playerWorldPos.y + sinf(angle) * distance;

          float playerToAsteroidDis = Vector2Distance((Vector2){posx,posy}, playerWorldPos);
          float planetToAsteroidDis = Vector2Distance((Vector2){posx,posy}, planetWorldPos);

          smallasteroid[i].position = (Vector2){posx, posy};
*/
          for(int n = 0; n < MAX_METEORS && corectPos; n++){
            if(i != n && smallasteroid[i].loaded && Vector2Distance(smallasteroid[i].position, smallasteroid[n].position)<45)corectPos=false;
          }

          if(corectPos){
            smallasteroid[i].loaded = true;
            spawnsNo++;
          }
          else smallasteroid[i].loaded = false;
/*
          if(playerToAsteroidDis > fov + asteroidRadius && planetToAsteroidDis >= 88700 && planetToAsteroidDis <= 98300 && corectPos){
            //smallasteroid[i].position = (Vector2){posx, posy};
            smallasteroid[i].loaded = true;
            spawnsNo++;
          }*/
        }
      }
    }

    for(int i = 0; i < MAX_METEORS; i++){
      if(smallasteroid[i].loaded && (Vector2Distance(planetWorldPos, playerWorldPos) > 101300 || Vector2Distance(planetWorldPos, playerWorldPos) < 96700))smallasteroid[i].loaded=false;
    }


    Vector2 p1 = RotatePoint((Vector2){playerPos.x,playerPos.y - 20}, playerPos, playerAng);
    Vector2 p2 = RotatePoint((Vector2){playerPos.x - 15,playerPos.y + 10}, playerPos, playerAng);
    Vector2 p3 = RotatePoint((Vector2){playerPos.x + 15,playerPos.y + 10}, playerPos, playerAng);
    Vector2 l1 = RotatePoint((Vector2){playerPos.x, playerPos.y - 220}, playerPos, playerAng);
    Vector2 planetPos = {planetWorldPos.x - playerWorldPos.x + playerPos.x, planetWorldPos.y - playerWorldPos.y + playerPos.y};
    Vector2 shipPos = {shipWorldPos.x - playerWorldPos.x + playerPos.x, shipWorldPos.y - playerWorldPos.y + playerPos.y};

    Color DarkRed = {90, 10, 10, 255};
    Color Invis = {0, 0, 0, 0};

    BeginDrawing();
      ClearBackground(BLACK);
      DrawCircle(planetPos.x, planetPos.y, 25000, YELLOW);
      DrawCircleLines(planetPos.x, planetPos.y, 88700, BLUE);
      DrawCircleLines(planetPos.x, planetPos.y, 98300, BLUE);
      DrawCircle(shipPos.x, shipPos.y, 250, Invis);
      DrawRectangle(shipPos.x-75, shipPos.y-200, 140, 400, DarkRed);
      DrawTriangle(p1, p2, p3, RED);
      if(laserfire)DrawLineEx(p1, l1, 1.0f, BLUE);
      
      for(int i = 0; i < MAX_METEORS; i++){

        Vector2 meteorPos = {smallasteroid[i].position.x - playerWorldPos.x + playerPos.x, smallasteroid[i].position.y - playerWorldPos.y + playerPos.y};

        bool visible = !(((meteorPos.x - asteroidRadius) > screenw || (meteorPos.x + asteroidRadius) < 0)||((meteorPos.y + asteroidRadius) < 0 || (meteorPos.y - asteroidRadius) > screenh));

        if(visible && smallasteroid[i].loaded) DrawCircle(meteorPos.x, meteorPos.y, asteroidRadius, GRAY);
      }
    
      DrawFPS(10, 88);

      DrawText(TextFormat("Fuel: %.2f%%", fuel), 10, 10, 26, GREEN);
      DrawText(TextFormat("Charge: %.02f%%",(float)charge), 10, 36, 26, GREEN);
      DrawText(TextFormat("Cordinates: %.02f %.02f",-playerWorldPos.x,-playerWorldPos.y), 10, 62, 26, GREEN);
      DrawText(TextFormat("AsteroidDist: %.0f", Vector2Distance(smallasteroid[0].position, playerWorldPos)), 10, 218, 26, RED);
    EndDrawing();

=======
#include <stdlib.h>
#define screenw 800
#define screenh 600
#define trust 3.0f
#define drag 0.5f
#define hp 5
#define maxAsteroids 500
#define starsNo 100

typedef struct asteroid {
  Vector2 position;
  int radius;
  int cloor;
  float durability;
  bool visible;
  float velocity;
} Object;

typedef struct{
  Vector2 pos;
  int color;
  float vel;
  bool visible;
} Object2;

static bool pause = false;
static float playerVel = 0.0f;
static float charge = 100;
static float score = 0.0f;
static short asteroidNo = 30;
static float genTime = 3.0f;
static float diff = 0.0f;
static Object asteroid[maxAsteroids] = {0};
static int lvl = 1;
static bool gameOver = false;
static bool dev = false;
static Vector2 p1 = {0, 0};
static Vector2 p2 = {0, 0};
static Vector2 p3 = {0, 0};
static Vector2 p4 = {0, 0};
static short cap = 0;
static int hitId = 0;
static float hitDistance = 750;
static Object2 star[starsNo] = {0};
static bool gameStart = false;
static Vector2 playerPos  = {400, 550};
static Texture2D playerTex;

void gameRastart(){
  playerPos = (Vector2){400, 550};
  playerVel = 0.0f;
  charge = 100;
  score = 0.0f;
  asteroidNo = 30;
  genTime = 3.0f;
  diff = 0.0f;
  lvl = 1;
  gameOver = false;
  hitId = -1;
  hitDistance = 750;
  for(short i = 0; i < maxAsteroids; i++)asteroid[i].visible = false;
}

bool CheckCollisionCircleTriangle(Vector2 circlePos, float radius, Vector2 p1, Vector2 p2, Vector2 p3){
  if (CheckCollisionCircleLine(circlePos, radius, p1, p2)) return true;
  if (CheckCollisionCircleLine(circlePos, radius, p2, p3)) return true;
  if (CheckCollisionCircleLine(circlePos, radius, p3, p1)) return true;
  return false;
}


int main() {

  for(short i = 0; i < maxAsteroids; i++)asteroid[i].visible = false;

  for(short i = 0; i < starsNo; i++){
    star[i].pos.x = GetRandomValue(0, screenw);
    star[i].pos.y = GetRandomValue(0, screenh);
    star[i].color = GetRandomValue(0,2);
    star[i].vel = GetRandomValue(10, 100) / 100.0f; 
    star[i].visible = true;
  }

  p1 = (Vector2){playerPos.x,playerPos.y - 20};
  p2 = (Vector2){playerPos.x - 15, playerPos.y + 10};
  p3 = (Vector2){playerPos.x + 15, playerPos.y + 10};
  
  InitWindow(screenw, screenh, "Space Troubles");
  playerTex = LoadTexture("./Rocket.png");
  SetTargetFPS(60);
  while (!WindowShouldClose()) {

    BeginDrawing();

    bool laserfire = false;

   if(IsKeyPressed(KEY_P) && gameStart && !gameOver) pause = !pause;
   if(IsKeyPressed(KEY_F3)) dev = !dev;

    if(IsKeyPressed(KEY_ENTER) && !gameStart){
      gameStart = true;
      gameRastart();
    }


    if(IsKeyPressed(KEY_ENTER) && gameOver){
      gameOver = false;
      gameRastart();
    }

    if(IsKeyPressed(KEY_DELETE) && dev) gameOver = !gameOver;
    
    if(!pause && !gameOver && gameStart){
      if(score / (100 * lvl) > 1) asteroidNo++, lvl++;
      diff = 1.0f + ((score / 10) * 0.02f);
      genTime -= GetFrameTime();
      if(genTime <= 0){
        genTime = 2.0f / (double)diff;
        cap = 0;
        for(short i = 0; i < asteroidNo && cap < (diff / 10); i++){
          if(!asteroid[i].visible){
            asteroid[i].radius = GetRandomValue(3, 25);
            asteroid[i].position.x = GetRandomValue(0, 800);
            asteroid[i].position.y = 0 - asteroid[i].radius;
            asteroid[i].durability = hp * asteroid[i].radius * diff;
            asteroid[i].visible = true;
            asteroid[i].velocity = GetRandomValue(10, 40);
            cap++;
          }
        }
      }

      if (IsKeyDown(KEY_D)) {
        playerVel += trust;
      }
    
      if(!IsKeyDown(KEY_D) && playerVel > 0) playerVel -= drag;

      if (IsKeyDown(KEY_A)) {
        playerVel -= trust;
      }
    
      if(!IsKeyDown(KEY_A) && playerVel < 0) playerVel += drag;

      if (IsKeyDown(KEY_SPACE)) {
        if(charge <= 0)laserfire = false, charge = 0;
        else laserfire=true, charge -= 0.5;
      }
      else laserfire = false;
      if(charge<100 && !IsKeyDown(KEY_SPACE))charge += 0.35;

      playerPos.x += playerVel * GetFrameTime();

      if(playerPos.x < -15) playerPos.x = 815;
      if(playerPos.x > 815) playerPos.x = -15;

      p1 = (Vector2){playerPos.x,playerPos.y - 20};
      p2 = (Vector2){playerPos.x - 15, playerPos.y + 10};
      p3 = (Vector2){playerPos.x + 15, playerPos.y + 10};
      p4 = (Vector2){playerPos.x,playerPos.y - 750};

      hitId = -1;
      if(laserfire){
        hitDistance = 750;
        for(short i = 0; i < asteroidNo; i++){
          if(CheckCollisionCircleLine(asteroid[i].position, asteroid[i].radius, p1, p4) && asteroid[i].visible){
            float x = asteroid[i].position.x - p1.x;
            float y = asteroid[i].position.y - p1.y;
            float distance = sqrtf(x*x+y*y);
            if(distance < hitDistance)hitDistance = distance, hitId = i;
          }
        }
        if(hitId >= 0){
          float d = playerPos.x - asteroid[hitId].position.x;
          float r = asteroid[hitId].position.y + sqrtf(asteroid[hitId].radius * asteroid[hitId].radius - d * d);
          p4 = (Vector2){playerPos.x, r};
        }
      }

      for(short i = 0; i < asteroidNo; i++){

        asteroid[i].position.y += GetFrameTime() * asteroid[i].velocity * diff;

        if(laserfire && i == hitId)asteroid[i].durability -= 2.7 * (diff / 2);
        if(asteroid[i].durability < 0 && asteroid[i].visible){
          asteroid[i].visible = false;
          score += 1.5 * asteroid[i].radius;
        }
        if(asteroid[i].visible && CheckCollisionCircleTriangle(asteroid[i].position, asteroid[i].radius, p1, p2, p3))gameOver = true; 
        if(asteroid[i].position.y > 600 + asteroid[i].radius && asteroid[i].visible) asteroid[i].visible = false, score += 10;

      }  

    }

    ClearBackground(BLACK);

    for(short i = 0; i < starsNo; i++){

      if(star[i].pos.y > 601)star[i].visible = false;

      if(star[i].visible){

        star[i].pos.y += GetFrameTime() * star[i].vel * diff;

        switch(star[i].color){
          case 0:
            DrawCircleV(star[i].pos, 1, WHITE);
            break;
          case 1:
            DrawCircleV(star[i].pos, 1, GOLD);
            break;
          case 2:
            DrawCircleV(star[i].pos, 1, DARKPURPLE);
            break;
        }
      }
      else{
        star[i].pos.x = GetRandomValue(0, screenw);
        star[i].pos.y = 0;
        star[i].color = GetRandomValue(0,2);
        star[i].vel = GetRandomValue(10, 100) / 100.0f; 
        star[i].visible = true;
      }
    }

    if(dev){
    DrawText(TextFormat("Tex ID: %d  W:%d H:%d", playerTex.id, playerTex.width, playerTex.height), 10, 98, 20, WHITE);
    }

    DrawTextureV(playerTex, (Vector2){playerPos.x-15, playerPos.y-20}, WHITE);
    if(dev){
      DrawRectangleLines(playerPos.x-15,playerPos.y-20,30,40,GREEN);
      DrawTriangleLines(p1, p2, p3, PURPLE);
    }

    if(dev){
      DrawLineEx(p1, (Vector2){playerPos.x,playerPos.y - 750}, 1.5f, BLUE);
      if(laserfire)DrawLineEx(p1, p4, 1.5f, RED);
    }
    else if(laserfire)DrawLineEx(p1, p4, 1.5f, GREEN);

    for(short i = 0; i < asteroidNo; i++)if(asteroid[i].visible){
      DrawCircle(asteroid[i].position.x, asteroid[i].position.y, asteroid[i].radius, GRAY);
      if(dev)DrawText(TextFormat("%.02f",asteroid[i].durability), asteroid[i].position.x, asteroid[i].position.y, 20, BLUE);
    }

    DrawText(TextFormat("Charge: %.0f%%",(float)charge), 10, 10, 20, SKYBLUE);
    DrawText(TextFormat("Score: %.0f",score), 10, 32, 20, SKYBLUE);

    if(dev){
      Vector2 predPos = {playerPos.x + playerVel * GetFrameTime() * 60, playerPos.y};
      DrawLineEx((Vector2){playerPos.x, playerPos.y}, predPos, 2.0f, YELLOW);
      DrawText(TextFormat("Gen Time: %.02f",(float)genTime), 620, 10, 20, WHITE);
      DrawText(TextFormat("Dificulty: %f",diff), 620, 32, 20, WHITE);
      DrawText(TextFormat("AstNo: %d",asteroidNo), 620, 54, 20, WHITE);
      if(gameOver)DrawText(TextFormat("colision: Yes"), 620, 76, 20, WHITE);
      else DrawText(TextFormat("colision: No"), 620, 76, 20, WHITE);
      DrawFPS(10,54);
      DrawText(TextFormat("Cordinates: %.02f %.02f", playerPos.x, playerPos.y), 10, 76, 20, WHITE);
      DrawLine(screenw/2, 0, screenw/2, 600, GREEN);
      DrawLine(0, screenh/2, 800, screenh/2, GREEN);
    }


    if(!gameStart){

      const char *t1 = "SPACE TROUBLES";
      const char *t2 = "A / D to move";
      const char *t3 = "SPACE to fire laser";
      const char *t4 = "P to pause";
      const char *t5 = "Press ENTER to start";

      DrawRectangle(0, 0, screenw, screenh, (Color){0, 0, 0, 180});
      DrawText(t1, screenw/2 - MeasureText(t1, 50)/2, screenh/2 - 200, 50, SKYBLUE);
      DrawText(t2, screenw/2 - MeasureText(t2, 20)/2, screenh/2 + -20, 20, WHITE);
      DrawText(t3, screenw/2 - MeasureText(t3, 20)/2, screenh/2 + 5, 20, WHITE);
      DrawText(t4, screenw/2 - MeasureText(t4, 20)/2, screenh/2 + 30, 20, WHITE);
      DrawText(t5, screenw/2 - MeasureText(t5, 20)/2, screenh/2 + 75, 20, GRAY);
    }

    if(gameOver){

      const char *t1 = "GAME OVER";
      const char *t2 = TextFormat("Final Score: %.0f", score);
      const char *t3 = "Press ENTER to start";

      DrawRectangle(0, 0, screenw, screenh, (Color){0, 0, 0, 180});
      DrawText(t1, screenw/2 - MeasureText(t1, 40)/2, screenh/2 - 40, 40, RED);
      DrawText(t2, screenw/2 - MeasureText(t2, 25)/2, screenh/2 + 10, 25, WHITE);
      DrawText(t3, screenw/2 - MeasureText(t3, 20)/2, screenh/2 + 50, 20, GRAY);
    }

    if(pause){
      DrawRectangle(0, 0, screenw, screenh, (Color){0, 0, 0, 180});
      DrawText("GAME PAUSED", screenw/2 - 140, screenh/2 - 40, 40, LIME);
      DrawText("Press P to continue", screenw/2 - 104, screenh/2 + 50, 20, GRAY);
    }

    EndDrawing();
>>>>>>> closed-world
  }
  UnloadTexture(playerTex);
  CloseWindow();
  return 0;
<<<<<<< HEAD
}

Vector2 RotatePoint(Vector2 point, Vector2 center,float rawAngle){
  float angle = rawAngle * DEG2RAD;
  float s = sinf(angle);
  float c = cosf(angle);
  point.x -= center.x;
  point.y -= center.y;
  float angleX = point.x * c - point.y * s;
  float angleY = point.x * s + point.y * c;
  return (Vector2){angleX + center.x, angleY + center.y};
=======
>>>>>>> closed-world
}