#include "raylib.h"
#include <math.h>
#define screenw 800
#define screenh 600
#define rottrust 1.7f
#define fortrust 3.0f

Vector2 RotatePoint(Vector2 point, Vector2 center,float rawangle){
  float angle = rawangle * DEG2RAD;
  float s = sinf(angle);
  float c = cosf(angle);
  float o = -cosf(angle);
  point.x -= center.x;
  point.y -= center.y;
  float xangle = point.x * c - point.y * s;
  float yangle = point.x * s + point.y * c;
  return (Vector2){xangle + center.x, yangle + center.y};
}
void Game(){
  bool laserfire = false;
  float fuel = 80.0f;
  float playerang = 0.0f;
  float charge = 1.0f;
  bool pause = false;
  if(IsKeyPressed('P'))pause = !pause;
  if(!pause){
          Vector2 playerPos  = {scw / 2.0f, sch / 2.0f};
      Vector2 playercen = playerPos;
      if (IsKeyDown(KEY_D)) {
        playerang  += rottrust;
        fuel -= 0.0001;
      }
      if (IsKeyDown(KEY_A)) {
        playerang  -= rottrust;
        fuel -= 0.0001;
      }
      if (IsKeyDown(KEY_SPACE)) {
        if(charge <= 0.002)laserfire = false;
        else laserfire=true, charge -= 0.002;
      }
      else laserfire = false;
      if(charge<1&&!laserfire&&!IsKeyDown(KEY_SPACE))charge += 0.001, fuel -= 0.005;
      Vector2 p1 = RotatePoint((Vector2){playerPos.x,playerPos.y - 20}, playercen, playerang);
      Vector2 p2 = RotatePoint((Vector2){playerPos.x - 15,playerPos.y + 10}, playercen, playerang);
      Vector2 p3 = RotatePoint((Vector2){playerPos.x + 15,playerPos.y + 10}, playercen, playerang);
      Vector2 l1 = RotatePoint((Vector2){playerPos.x, playerPos.y - 220}, playercen, playerang);
  }
}

void main() {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(screenw, screenh, "Space Troubles");
  SetTargetFPS(120);
  while (!WindowShouldClose()) {
      int scw = GetScreenWidth();
      int sch = GetScreenHeight();
      Game();
      BeginDrawing();
          ClearBackground(BLACK);
          DrawTriangle(p1, p2, p3, RED);
          if(laserfire)DrawLineEx(p1, l1, 1.0f, BLUE);
          DrawText(TextFormat("Fuel: %.2f%%", fuel), 10, 10, 26, GRAY);
          DrawText(TextFormat("Charge: %.02f%%",(float)charge), 10, 52, 26, GRAY);
      EndDrawing();
  }
  CloseWindow();
}