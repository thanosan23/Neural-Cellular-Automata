#include "../include/raylib.h"

#include "util/std.hpp"
#include "cell.hpp"

constexpr int DIM = 800, FPS = 60;

int bound(const int &num, const int &min, const int &max) {
  return std::max(std::min(num, max), min);
}

int main() {
  InitWindow(DIM, DIM, "Neural Cellular Automotan");
  SetTargetFPS(FPS);


  CellManager cells(DIM, 5);
  bool updating = false;

  Vector2 mousePos;
  int x, y;

  while(!WindowShouldClose()) {
    // input
    if(!updating) {
      if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        mousePos = GetMousePosition();
        x = bound(static_cast<int>(mousePos.x), 0, DIM);
        y = bound(static_cast<int>(mousePos.y), 0, DIM);
        cells.darkenCell(x, y);
      } else if(IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
        mousePos = GetMousePosition();
        x = bound(static_cast<int>(mousePos.x), 0, DIM);
        y = bound(static_cast<int>(mousePos.y), 0, DIM);
        cells.lightenCell(x, y);
      }
      if(IsKeyPressed('R')) {
        updating = true;
      }
    }
    // updating
    if(updating) {
      cells.applyKernelConvolution({{0.1, -0.1,  0.3},
                                    {  0,    1, -0.3},
                                    {-0.5, -0.1, 0.2}},
                                    [&](const float &num) -> float {
                                      return std::abs(num);
                                    });
      WaitTime(0.1);
    }

    cells.step();

    // rendering
    BeginDrawing();
    ClearBackground(RAYWHITE);
    cells.draw();
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
