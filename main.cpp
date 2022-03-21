#include "mesh.h"
 
vector<vector<int>> grid = vector<vector<int>>();

vector<DynamicPoint> points = vector<DynamicPoint>();

int scr_w = 0;
int scr_h = 0;

const double frame_delay = 1.0 / 600.0; // 60 FPS
double last_render = 0;

int cur_mat = 1;

bool mDown = false;
int mouseX = 0;
int mouseY = 0;
int cursor_width = 3;
float period_time = 0.0f;
const float period = 2.0f;
int idle_time = 0;
int full_time = 0;
bool time_active = true;

Point curP = Point(0.0f, 0.0f);

const float POINTS_CNT = 20;


void display() {
  // glClear(GL_COLOR_BUFFER_BIT);
  draw_background();

  // draw_grid(scr_w, scr_h, full_time);
  sort(points.begin(), points.end(), compare_points);
  
  glPointSize(6.0f);
  glBegin(GL_POINTS);
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
  for (size_t i = 0; i < points.size(); i++)
  {
    glVertex2f(points[i].pos.x, points[i].pos.y);
  }
  glEnd();

  glBegin(GL_LINES);
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
  vector<Edge> edges = triangulate(points);
  for (size_t i = 0; i < edges.size(); i++)
  {
    glVertex2f(edges[i].p1.x, edges[i].p1.y);
    glVertex2f(edges[i].p2.x, edges[i].p2.y);
  }
  glEnd();

  // glFlush();
  glutSwapBuffers();
}

void Idle() {
  const double current_time = GetCurrentTime();
  if( ( current_time - last_render ) > frame_delay ){
    last_render = current_time;

    idle_time++;
    full_time++;

    if (mDown){
      idle_time = 0;
    }
    if (time_active)
      period_time += frame_delay;
    if (period_time >= period) {
      period_time -= period;

      for (size_t i = 0; i < points.size(); i++){
        float angle = 0.01f * (rand() % 628);
        points[i].new_dir = Point(cos(angle), sin(angle));
      }      
    }

    for (size_t i = 0; i < points.size(); i++)
    {
      float t_dir_x = points[i].new_dir.x * points[i].rotate_speed + points[i].cur_dir.x * (1.0f - points[i].rotate_speed);
      float t_dir_y = points[i].new_dir.y * points[i].rotate_speed + points[i].cur_dir.y * (1.0f - points[i].rotate_speed);
      Point t_dir = normalize(Point(t_dir_x, t_dir_y));
      points[i].cur_dir = t_dir;
      points[i].pos.x += t_dir.x * points[i].speed;
      points[i].pos.y += t_dir.y * points[i].speed;

      points[i].pos = keep_on_screen(points[i].pos);
    }

    glutPostRedisplay();
  }
}

void redraw(){
  glClear(GL_COLOR_BUFFER_BIT);
  draw_background();
  // string s = "shape: " + to_string(cur_shape) + "; koef offset: " + to_string(koef_off).substr(0, 4);
  // glutSetWindowTitle(s.c_str());
}

void specialKeys(int key, int x, int y)
{
	switch (key)
	{
	// case GLUT_KEY_UP:
  //   cur_shape++;
  //   cur_shape = clamp(cur_shape, 0, shapes.size() - 1);
  //   redraw();
  //   break;
	// case GLUT_KEY_DOWN:
  //   cur_shape--;
  //   cur_shape = clamp(cur_shape, 0, shapes.size() - 1);
  //   redraw();
  //   break;
  // case GLUT_KEY_LEFT:
  //   koef_off -= 0.01f;
  //   redraw();
  //   break;	
  // case GLUT_KEY_RIGHT:
  //   koef_off += 0.01f;
  //   redraw();
  //   break;	
  }

	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
  case ' ':
    time_active = !time_active;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void mouseEvents(int button, int state, int x, int y)
{
	switch (button)
	{
    case GLUT_LEFT_BUTTON:
    {
      if (state == GLUT_DOWN)
      {
        mouseX = x;
        mouseY = y;
        mDown = true;

        float offX = (mouseX / (glutGet(GLUT_WINDOW_WIDTH) * 1.0)) * 2 - 1;
        float offY = -((mouseY / (glutGet(GLUT_WINDOW_HEIGHT) * 1.0)) * 2 - 1);      
      }
      else
      {
        mDown = false;
      }
    }
	}
}

void mouse_passive_move(int x, int y){
  mouseX = x;
  mouseY = y;
  idle_time = 0;
}

void mouseMove(int x, int y)
{
	if (mDown)
	{
    if (out_of_bounds(x, y, scr_w, scr_h)) {
      mouseX = x;
      mouseY = y;
      return;
    }
    float mouseXfold = (mouseX / (glutGet(GLUT_WINDOW_WIDTH) * 1.0)) * 2 - 1;
    float mouseYfold = -((mouseY / (glutGet(GLUT_WINDOW_HEIGHT) * 1.0)) * 2 - 1);
    mouseX = x;
    mouseY = y;
    float mouseXf = (mouseX / (glutGet(GLUT_WINDOW_WIDTH) * 1.0)) * 2 - 1;
    float mouseYf = -((mouseY / (glutGet(GLUT_WINDOW_HEIGHT) * 1.0)) * 2 - 1);
	}
}

void init() {
  glClearColor(0.1f, 0.1f, 0.1f, 1.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glEnable(GL_POINT_SMOOTH);
  // glEnable(GL_LINE_SMOOTH);

  grid = vector<vector<int>>(scr_w);
  for (size_t i = 0; i < scr_w; i++)
  {
    grid[i] = vector<int>(scr_h);
    for (size_t j = 0; j < scr_h; j++)
    {
      grid[i][j] = 0;
    }    
  }
  for (size_t i = 0; i < POINTS_CNT; i++)
  {
    float x = 0.01f * ((rand() % 200) - 100);
    float y = 0.01f * ((rand() % 200) - 100);
    points.push_back(DynamicPoint(x, y));
  }
  // points.push_back(DynamicPoint(0.9f, 0.9f));
  // points.push_back(DynamicPoint(0.9f, 0.1f));
  
  // points.push_back(DynamicPoint(0.1f, 0.1f));
  
  // points.push_back(DynamicPoint(0.1f, 0.9f));
  
  
  draw_background();
  redraw();
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowPosition(80, 80);
  glutInitWindowSize(800, 800);
  glutCreateWindow("Curves");
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
  srand(time(nullptr));

  scr_w = glutGet(GLUT_WINDOW_WIDTH);
  scr_h = glutGet(GLUT_WINDOW_HEIGHT);

  glutDisplayFunc(display);
  glutIdleFunc(Idle);
  glutMouseFunc(mouseEvents);
  glutMotionFunc(mouseMove);
  glutPassiveMotionFunc(mouse_passive_move);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(specialKeys);
  init();
  glutMainLoop();
}
