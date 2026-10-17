#include "raylib.h"
#include "raymath.h"
#include <math.h>
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

  }
  CloseWindow();
  return 0;
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
}