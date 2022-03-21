#ifndef HEADERFILE_H
#define HEADERFILE_H 1

#include <vector>
#include <GL/glew.h> 
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <chrono>
#include <cmath>
#include <queue>
#include <ctime>
#include <bits/stdc++.h>

using namespace std;

struct Point{
    float x;
    float y;
    Point(float _x, float _y){
        x = _x;
        y = _y;
    }
    Point(){
        x = 0;
        y = 0;
    }
};

struct Edge{
    Point p1;
    Point p2;
    Edge(Point _p1, Point _p2){
        p1 = _p1;
        p2 = _p2;
    }
    Edge(float x1, float y1, float x2, float y2){
        p1 = Point(x1, y1);
        p2 = Point(x2, y2);
    }
};

struct DynamicPoint{
    Point pos;
    Point cur_dir;
    Point new_dir;
    float speed = 0.0003f * 0.5;
    float rotate_speed = 0.003f; // 0 - 1
    DynamicPoint(float _x, float _y){
        pos = Point(_x, _y);
        cur_dir = Point(0.0f, 0.0f); 
        new_dir = Point(0.0f, 0.0f);
    }
    DynamicPoint(float _x, float _y, float _dx, float _dy){
        pos = Point(_x, _y);
        cur_dir = Point(_dx, _dy);
        new_dir = Point(_dx, _dy);
    }
};

extern vector<vector<int>> grid;
extern int scr_w;
extern int scr_h;
extern int cursor_width;

bool out_of_bounds(int x, int y, int scr_w, int scr_h);
float distance(float x1, float y1, float x2, float y2);
float distance(Point p1, Point p2);
float distance(Edge e);
Point normalize(Point vec);
Point keep_on_screen(Point p);
bool get_line_intersection(Edge e1, Edge e2);
bool compare_points(DynamicPoint p1, DynamicPoint p2);
vector<Edge> triangulate(vector<DynamicPoint> points);
double GetCurrentTime();
int clamp(int v, int min, int max);

void draw_background();
void draw_grid(int w, int h, int full_time);
void draw_fractal();

void paint_square(int x, int y, int rad, int mat);
void bresenham(int mat, int x0, int y0, int x1, int y1, int width);
#endif