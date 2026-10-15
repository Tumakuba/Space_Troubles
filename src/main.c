#include "raylib.h"
#include <math.h>
#define screenw 800
#define screenh 600
#define rottrust 1.7f
#define fortrust 

Vector2 RotatePoint(Vector2 point, Vector2 center,float rawangle){
  float angle = rawangle * DEG2RAD;
  float s = sinf(angle);
  float c = cosf(angle);
  point.x -= center.x;
  point.y -= center.y;
  float xangle = point.x * c - point.y * s;
  float yangle = point.x * s + point.y * c;
  return (Vector2){xangle + center.x, yangle + center.y};
}

void main() {
  float playerang = 0.0f;
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(screenw, screenh, "Space Troubles");
  SetTargetFPS(120);
  Vector2 playerPos  = {screenw / 2.0f, screenh / 2.0f};
  Vector2 playercen = playerPos;
  while (!WindowShouldClose()) {
      if (IsKeyDown(KEY_D)) {
        playerang  += rottrust;
      }
      if (IsKeyDown(KEY_A)) {
        playerang  -= rottrust;
      }
      Vector2 p1 = RotatePoint((Vector2){playerPos.x,playerPos.y - 20}, playercen, playerang);
      Vector2 p2 = RotatePoint((Vector2){playerPos.x - 15,playerPos.y + 10}, playercen, playerang);
      Vector2 p3 = RotatePoint((Vector2){playerPos.x + 15,playerPos.y + 10}, playercen, playerang);
      BeginDrawing();
          ClearBackground(BLACK);
            DrawTriangle(p1, p2, p3, RED);
          //DrawText("")
      EndDrawing();
  }
  CloseWindow();
}