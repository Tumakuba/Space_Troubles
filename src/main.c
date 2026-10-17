#include "raylib.h"
#include <math.h>
#define screenw 800
#define screenh 600
#define rottrust 1.7f
#define fortrust 3.0f

static bool pause = false;

int main() {

  static Vector2 playerPos  = {400, 550};
  
  InitWindow(screenw, screenh, "Space Troubles");
  SetTargetFPS(120);
  while (!WindowShouldClose()) {
    bool laserfire = false;
    float fuel = 80.0f;
    float charge = 1.0f;

    if(IsKeyPressed('P'))pause = !pause;
    if(!pause){
      if (IsKeyDown(KEY_D)) {

      }
      if (IsKeyDown(KEY_A)) {

      }
      if (IsKeyDown(KEY_SPACE)) {
        if(charge <= 0.002)laserfire = false;
        else laserfire=true, charge -= 0.002;
      }
      else laserfire = false;
      if(charge<1&&!laserfire&&!IsKeyDown(KEY_SPACE))charge += 0.001;
    }

Color Invis = {0, 0, 0, 122};
    BeginDrawing();
      ClearBackground(BLACK);
      DrawTriangle((Vector2){playerPos.x,playerPos.y - 20}, (Vector2){playerPos.x - 15, playerPos.y + 10}, (Vector2){playerPos.x + 15, playerPos.y + 10}, RED);
      if(laserfire)DrawLineEx((Vector2){playerPos.x,playerPos.y - 20}, (Vector2){playerPos.x,playerPos.y - 750}, 1.0f, BLUE);
      DrawText(TextFormat("Fuel: %.2f%%", fuel), 10, 10, 26, GRAY);
      DrawText(TextFormat("Charge: %.02f%%",(float)charge), 10, 52, 26, GRAY);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}