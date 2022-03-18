#include "mesh.h"


void draw_background(){
  glBegin(GL_QUADS);
  glColor4f(0.1, 0.1, 0.1, 1.0f); glVertex2f(-1, -1);
  glColor4f(0.1, 0.1, 0.1, 1.0f); glVertex2f(1, -1);
  glColor4f(0.2, 0.2, 0.2, 1.0f); glVertex2f(1, 1);
  glColor4f(0.2, 0.2, 0.2, 1.0f); glVertex2f(-1, 1);
  glEnd();
}

void draw_grid(int w, int h, int full_time){
  glPointSize(2.0f);
  glBegin(GL_POINTS);
  for (size_t i = 0; i < w; i++)
  {
    for (size_t j = 0; j < h; j++)
    {
      if (grid[i][j] > 0){
        float x = -1 + 2.0 * i / ((float)w);
        float y = -1 + 2.0 * j / ((float)h);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glVertex2f(x,y);
      }

      // float period = (full_time % 60) / 60.0f;

      // C c = C(x * 10.0f,y * 10.0f);
      // C a = c - (c*c + C(0.36f, 0.1f));
      // if (a.length() < 0.1f){
      //   glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
      //   glVertex2f(x,y);
      // }

      
    }
  }
  glEnd();
}

void draw_fractal(){
  glLineWidth(5.0f);
  glBegin(GL_LINES);

  glEnd();
}