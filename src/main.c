#include "raylib.h"
#include "raymath.h"
#include <math.h>
#define rottrust 1.7f
#define fortrust 0.025f
#define maxVel 30.5f
#define brakeForce 0.07f
#define starslimit 200
#define MAX_MEDIUM_METEORS 50
#define meteorRadius 20 

typedef struct Meteor {
    Vector2 position;
    bool loaded;
} Meteor;

static void InitGame(void);
static Vector2 RotatePoint(Vector2 point, Vector2 center,float rawangle);
static Meteor mediumMeteor[MAX_MEDIUM_METEORS] = { 0 };
static int screenw = 800;
static int screenh = 600;
static Vector2 playerPos = {400, 300};
static Vector2 shipWorldPos = {0,0};
static Vector2 planetWorldPos = {-100000,2000};

//first meteor strip x = <-1700;-11300>
//second mateor strip x = <-10900;-21200>
//first gravity sprip x = <-16000;-27999>
//third meteor strip x = <-20000;-32500>
//second gravity strip x = <-28000;-40000>
//forth meteor strip x = <-29800;-40120>
//third gravity strip x = <-40001;75000> fatal for ship (R.I.P)

int main() {
  bool laserfire = false;
  float fuel = 80.0f;
  float playerAng = -90.0f;
  float charge = 1.0f;
  bool pause = false;
  Vector2 playerVel = {0,0};
  Vector2 playerWorldPos = {-300,-20};

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(screenw, screenh, "Space Troubles");
  SetTargetFPS(120);
  InitGame();

  while (!WindowShouldClose()) {

    screenw = GetScreenWidth();
    screenh = GetScreenHeight();
    playerPos = (Vector2){screenw / 2.0f, screenh / 2.0f};

    if(IsKeyPressed('P'))pause = !pause;

    if(!pause){

      if (IsKeyDown(KEY_W)){
        playerVel.x += sinf(playerAng * DEG2RAD) * fortrust;
        playerVel.y += -cosf(playerAng * DEG2RAD) * fortrust;
        fuel -= 0.001;
      }
      if (Vector2Length(playerVel)> maxVel){
        playerVel = Vector2Normalize(playerVel);
        playerVel.x *= maxVel;
        playerVel.y *= maxVel;
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

    for(short i = 0; i < MAX_MEDIUM_METEORS; i++){
      if(mediumMeteor[i].loaded && Vector2Distance(mediumMeteor[i].position, playerWorldPos) > 3000)mediumMeteor[i].loaded=false;
    }


    for(short i = 0; i < MAX_MEDIUM_METEORS; i++){
      if(!mediumMeteor[i].loaded){
        float posx, posy, angle, distance;
        angle = GetRandomValue(0, 360) * DEG2RAD;
        distance = GetRandomValue(1000, 2900);
        posx = playerWorldPos.x + sinf(angle) * distance;
        posy = playerWorldPos.y -cos(angle) * distance;
        mediumMeteor[i].position = (Vector2){posx, posy};
        mediumMeteor[i].loaded = true;
      }
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
      DrawCircle(shipPos.x, shipPos.y, 250, Invis);
      DrawRectangle(shipPos.x-75, shipPos.y-200, 140, 400, DarkRed);
      DrawTriangle(p1, p2, p3, RED);
      if(laserfire)DrawLineEx(p1, l1, 1.0f, BLUE);
      for(short i = 0; i < MAX_MEDIUM_METEORS; i++){
        Vector2 meteorPos = {mediumMeteor[i].position.x - playerWorldPos.x + playerPos.x, mediumMeteor[i].position.y - playerWorldPos.y + playerPos.y};

        bool visible = !(((meteorPos.x - 520) > screenw || (meteorPos.x + 520) < 0)||((meteorPos.y + 520) < 0 || (meteorPos.y - 520) > screenh));

        if(visible && mediumMeteor[i].loaded) DrawCircle(meteorPos.x, meteorPos.y, meteorRadius, GRAY);
      }
      DrawText(TextFormat("Fuel: %.2f%%", fuel), 10, 10, 26, GREEN);
      DrawText(TextFormat("Charge: %.02f%%",(float)charge), 10, 36, 26, GREEN);
      DrawText(TextFormat("Cordinates: %.02f %.02f",-playerWorldPos.x,-playerWorldPos.y), 10, 62, 26, GREEN);
    EndDrawing();

  }
  CloseWindow();
  return 0;
}

Vector2 RotatePoint(Vector2 point, Vector2 center,float rawangle){
  float angle = rawangle * DEG2RAD;
  float s = sinf(angle);
  float c = cosf(angle);
  point.x -= center.x;
  point.y -= center.y;
  float angleX = point.x * c - point.y * s;
  float angleY = point.x * s + point.y * c;
  return (Vector2){angleX + center.x, angleY + center.y};
}

void InitGame(void){
  float posx, posy, angle, distance;

  for (int i = 0; i < MAX_MEDIUM_METEORS; i++){

    angle = GetRandomValue(0, 360) * DEG2RAD;
    distance = GetRandomValue(520, 2900);

    posx = shipWorldPos.x + sinf(angle) * distance;
    
    posy = shipWorldPos.y -cos(angle) * distance;

    mediumMeteor[i].position = (Vector2){posx, posy};
    mediumMeteor[i].loaded = true;
  }
}