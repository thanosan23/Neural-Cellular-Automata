#pragma once

#include "util/std.hpp"

struct Cell {
  Cell(const int &x, const int &y) {
    this->x = x;
    this->y = y;
    this->opacity = 0;
    this->nextOpacity = 0;
  }

  // getters and setters
  float getOpacity() {
    return this->opacity;
  }

  void setOpacity(const float &_opacity) {
    this->opacity = _opacity;
  }

  float getNextOpacity() {
    return this->nextOpacity;
  }

  void setNextOpacity(const float &_opacity) {
    this->nextOpacity = _opacity;
  }

  // changing opacity of cell
  void darken() {
    this->opacity += 0.1;
    if(this->opacity > 1.0f) this->opacity = 1.0f;
  }

  void lighten() {
    this->opacity -= 0.1;
    if(this->opacity < 0.0f) this->opacity = 0.0f;
  }

  void darkenNext() {
    this->nextOpacity += 0.1;
    if(this->nextOpacity > 1.0f) this->nextOpacity = 1.0f;
  }

  void lightenNext() {
    this->nextOpacity -= 0.1;
    if(this->nextOpacity < 0.0f) this->nextOpacity = 0.0f;
  }
  // stepping through cellular automata simulation
  void step() {
    this->opacity = this->nextOpacity;
  }

private:
  int x, y;
  float opacity = 0, nextOpacity = 0;
};


struct CellManager {
  CellManager(const int &dim, const int &pixelSize) {
    this->dim = dim;
    this->pixelSize = pixelSize;
    assert(dim % pixelSize == 0);
    this->gridSize = dim / pixelSize;
    // Generate cells
    for(int y = 0; y < this->gridSize; ++y) {
      for(int x = 0; x < this->gridSize; ++x) {
        this->cells.emplace_back(std::move(Cell(x, y)));
      }
    }
  }

  Cell *getCell(const int &x, const int &y) {
    return &this->cells[y * this->gridSize + x];
  }

  void darkenCell(const int &x, const int &y) {
    Cell *cell = this->getCell(x / this->pixelSize, y / this->pixelSize);
    cell->darkenNext();
  }

  void lightenCell(const int &x, const int &y) {
    Cell *cell = this->getCell(x / this->pixelSize, y / this->pixelSize);
    cell->lightenNext();
  }

  void draw() {
    for(int y = 0; y < this->gridSize; ++y) {
      for(int x = 0; x < this->gridSize; ++x) {
        Cell *cell = this->getCell(x, y);
        DrawRectangle(x * pixelSize, y * pixelSize, pixelSize, pixelSize,
            Color {0, 0, 0, static_cast<unsigned char>(
                255 * cell->getOpacity()
            )});
      }
    }
  }

  void step() {
    for(int y = 0; y < this->gridSize; ++y) {
      for(int x = 0; x < this->gridSize; ++x) {
        Cell *cell = this->getCell(x, y);
        cell->step();
      }
    }
  }

  void applyKernelConvolution(const kernelType &kernel,
      const std::function<float(float)> &activation) {

    std::vector<int> dx = {-1, 0, 1, -1, 0, 1, -1, 0, 1};
    std::vector<int> dy = {-1, -1, -1, 0, 0, 0, 1, 1, 1};

    for(int y = 0; y < this->gridSize; ++y) {
      for(int x = 0; x < this->gridSize; ++x) {
        Cell *cell = this->getCell(x, y);
        float sum = 0;
        for(int i = 0; i < 9; ++i) {
          int nx = x + dx[i];
          int ny = y + dy[i];

          // wrap around the screen
          if(nx < 0) nx = this->gridSize + nx;
          if(ny < 0) ny = this->gridSize + ny;
          if(nx >= this->gridSize) nx = nx - this->gridSize;
          if(ny >= this->gridSize) ny = ny - this->gridSize;

          Cell *ncell = this->getCell(nx, ny);
          sum += ncell->getNextOpacity() * kernel[1+dy[i]][1+dx[i]];
        }
        cell->setNextOpacity(activation(cell->getNextOpacity() + sum));
      }
    }
  }

private:
  std::vector<Cell> cells;
  int dim, pixelSize, gridSize;
};
