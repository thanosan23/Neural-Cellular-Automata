#include "../include/raylib.h"

#include "util/std.hpp"
#include "cell.hpp"

constexpr int DIM = 800, FPS = 60;
constexpr int CELL_SIZE = 4;

int bound(const int &num, const int &min, const int &max) {
  return std::max(std::min(num, max), min);
}

std::pair<int, int> getMousePosBounded() {
  std::pair<int, int> res;
  Vector2 mousePos = GetMousePosition();
  res.first = bound(static_cast<int>(mousePos.x), 0, DIM);
  res.second = bound(static_cast<int>(mousePos.y), 0, DIM);
  return res;
}

int main() {
  InitWindow(DIM, DIM, "Neural Cellular Automotan");
  SetTargetFPS(FPS);

  CellManager cells(DIM, CELL_SIZE);
  bool updating = false;

  int mouseX, mouseY;

  while(!WindowShouldClose()) {
    // input
    if(!updating) {
      if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        std::tie(mouseX, mouseY) = getMousePosBounded();
        cells.darkenCell(mouseX, mouseY);
      } else if(IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
        std::tie(mouseX, mouseY) = getMousePosBounded();
        cells.lightenCell(mouseX, mouseY);
      }

      if(IsKeyPressed('R')) updating = true;

    }

    // updating
    if(updating) {
      kernelType kernel = {{+0.1, -0.1, +0.3},
                           {+0.0, +0.1, -0.3},
                           {-0.5, -0.1, +0.1}};

      cells.applyKernelConvolution(kernel,
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
