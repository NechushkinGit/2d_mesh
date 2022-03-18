#include "mesh.h"

bool out_of_bounds(int x, int y, int scr_w, int scr_h){
  if (x < 0 || x >= scr_w || y < 0 || y >= scr_h) return true;
  else return false;
}

float distance(float x1, float y1, float x2, float y2){
  return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

Point normalize(Point vec){
  float length = distance(0.0f, 0.0f, vec.x, vec.y);
  if (length < 0.000001f) {
    return Point(0.0f, 0.0f);
  }
  return Point(vec.x / length, vec.y / length);
}

Point keep_on_screen(Point p){
  Point newp = Point(p.x, p.y);
  if (p.x > 1.0f){
    newp.x -= 2.0f;
  }
  else if (p.x < -1.0f){
    newp.x += 2.0f;
  }
  if (p.y > 1.0f){
    newp.y -= 2.0f;
  }
  else if (p.y < -1.0f){
    newp.y += 2.0f;
  }
  return newp;
}

bool get_line_intersection(Edge e1, Edge e2)
{
  float eps = 0.00001f;
  float s1_x, s1_y, s2_x, s2_y;
  s1_x = e1.p2.x - e1.p1.x;     s1_y = e1.p2.y - e1.p1.y;
  s2_x = e2.p2.x - e2.p1.x;     s2_y = e2.p2.y - e2.p1.y;

  float e1p1x = e1.p1.x + s1_x * eps;
  float e1p1y = e1.p1.y + s1_y * eps;
  float e2p1x = e2.p1.x + s2_x * eps;
  float e2p1y = e2.p1.y + s2_y * eps;
  s1_x *= (1.0f - eps * 2); s1_y *= (1.0f - eps * 2); s2_x *= (1.0f - eps * 2); s2_y *= (1.0f - eps * 2);

  float s_up = -s1_y * (e1p1x - e2p1x) + s1_x * (e1p1y - e2p1y);
  float s_down = -s2_x * s1_y + s1_x * s2_y;
  float t_up = s2_x * (e1p1y - e2p1y) - s2_y * (e1p1x - e2p1x);
  float t_down = -s2_x * s1_y + s1_x * s2_y;

  if ((abs(s_down) < eps) || (abs(t_down) < eps)){
    return false;
  }

  float s = s_up / s_down;
  float t = t_up / t_down;

  if (s >= 0.0f && s <= 1.0f && t >= 0.0f && t <= 1.0f)
  {
      // Collision detected
      return true;
  }

  return false; // No collision
}

bool compare_points(DynamicPoint p1, DynamicPoint p2){
  if (p1.pos.x < p2.pos.x) return true;
  if ((abs(p1.pos.x - p2.pos.x) < 0.00001f) && (p1.pos.y < p2.pos.y)) return true;
  return false;
}

vector<Edge> triangulate(vector<DynamicPoint> points)
{
  vector<Edge> edges = vector<Edge>();
  if (points.size() < 3) return edges;
  edges.push_back(Edge(points[0].pos, points[1].pos));
  edges.push_back(Edge(points[1].pos, points[2].pos));
  edges.push_back(Edge(points[2].pos, points[0].pos));
  for (size_t i = 3; i < points.size(); i++)
  {
    int esize = edges.size();
    for (size_t j = 0; j < i; j++)
    {
      Edge e = Edge(points[i].pos, points[j].pos);
      bool intersects = false;
      for (size_t k = 0; k < esize; k++)
      {
        if (get_line_intersection(e, edges[k]))
        {
          intersects = true;
        }
      }
      if (!intersects)
      {
        edges.push_back(e);
      }
    }    
  }
  return edges;
}

double GetCurrentTime()
{
	using Duration = std::chrono::duration<double>;
	return std::chrono::duration_cast<Duration>( 
		std::chrono::high_resolution_clock::now().time_since_epoch() 
	).count();
}

int clamp(int v, int min, int max){
  if (v < min) return min;
  if (v < max) return v;
  return max;
}

void paint_square(int x, int y, int rad, int mat){
  for (size_t i = x - rad + 1; i < x + rad; i++)
  {
    for (size_t j = y - rad + 1; j < y + rad; j++)
    {
      if (out_of_bounds(i, j, scr_w, scr_h)) continue;
      grid[i][j] = mat;
    }    
  }
}

void bresenham(int mat, int x0, int y0, int x1, int y1, int width){
    float deltax = x1 - x0;
    float deltay = y1 - y0;
    float deltaerr = abs(deltay / deltax);
    float error = 0.0;
    if (deltaerr < 1.0){
      int y = y0;
      int offY = (y1 > y0) ? 1 : -1;
      if (x0 > x1){
        swap(x0, x1);
        offY *= -1;
      }
      for (size_t i = x0; i <= x1; i++){
        paint_square(i, y, width, mat);
        error += deltaerr;
        if (error >= 0.5){
          y += offY; // sign(deltaerr)
        error -= 1.0;
        }
      } 
    }
    else{
      int x = x0;
      int offX = (x1 > x0) ? 1 : -1;
      if (y0 > y1){
        swap(y0, y1);
        offX *= -1;
      }
      deltaerr = 1.0 / deltaerr;
      for (size_t i = y0; i <= y1; i++){
        paint_square(x, i, width, mat);
        error += deltaerr;
        if (error >= 0.5){
          x += offX; // sign(deltaerr)
        error -= 1.0;
        }
      } 
    }
}