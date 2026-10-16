#include "raylib.h"
#include "raymath.h"
#include <math.h>
#define screenw 800
#define screenh 600
#define rottrust 1.7f
#define fortrust 0.025f
#define maxVel 2.5f
#define brakeForce 0.05f
#define starslimit 200

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

int main() {
  bool laserfire = false;
  float fuel = 80.0f;
  float playerAng = 0.0f;
  float charge = 1.0f;
  bool pause = false;
  bool corectPos = false;
  int posX, posY;
  Vector2 playerVel = {0,0};
  Vector2 shipWorldPos = {0,0};
  Vector2 playerWorldPos = {-300,0};
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(screenw, screenh, "Space Troubles");
  SetTargetFPS(120);
  while (!WindowShouldClose()) {
    int scw = GetScreenWidth();
    int sch = GetScreenHeight();
    Vector2 playerPos = {scw / 2.0f, sch / 2.0f};
    if(IsKeyPressed('P'))pause = !pause;
    if(!pause){
      if (IsKeyDown(KEY_W)){
        playerVel.x += sinf(playerAng * DEG2RAD) * fortrust;
        playerVel.y += -cosf(playerAng * DEG2RAD) * fortrust;
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
      if(charge<1&&!laserfire&&!IsKeyDown(KEY_SPACE))charge += 0.001, fuel -= 0.0035;
      playerWorldPos.x += playerVel.x;
      playerWorldPos.y += playerVel.y;
    }
    Vector2 p1 = RotatePoint((Vector2){playerPos.x,playerPos.y - 20}, playerPos, playerAng);
    Vector2 p2 = RotatePoint((Vector2){playerPos.x - 15,playerPos.y + 10}, playerPos, playerAng);
    Vector2 p3 = RotatePoint((Vector2){playerPos.x + 15,playerPos.y + 10}, playerPos, playerAng);
    Vector2 l1 = RotatePoint((Vector2){playerPos.x, playerPos.y - 220}, playerPos, playerAng);
    Vector2 shipPos = {shipWorldPos.x - playerWorldPos.x + playerPos.x, shipWorldPos.y - playerWorldPos.y + playerPos.y};
    BeginDrawing();
      ClearBackground(BLACK);
      DrawCircle(shipPos.x,shipPos.y,20.0,GRAY);
      DrawTriangle(p1, p2, p3, RED);
      if(laserfire)DrawLineEx(p1, l1, 1.0f, BLUE);
      DrawText(TextFormat("Fuel: %.2f%%", fuel), 10, 10, 26, GRAY);
      DrawText(TextFormat("Charge: %.02f%%",(float)charge), 10, 36, 26, GRAY);
      DrawText(TextFormat("Cordinates: %.02f %.02f",-playerWorldPos.x,-playerWorldPos.y), 10, 62, 26, GRAY);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}