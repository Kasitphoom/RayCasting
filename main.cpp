#include <iostream>
#include <unordered_set>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <cmath>
#include <cstring>
#include <cstdint>
#include <unistd.h>
#include <map>

extern "C"
{
    int getPlayerHAngel(int mouseX, int mouseInitX, int mouse_speed);
    int getPlayerVAngel(int mouseY, int mouseInitY, int mouse_speed);
    int checkAdd360deg(int playerAngle);
    int modulo(int a, int b);
    int pressWx(double dy, std::unordered_set<KeySym>::iterator it, std::unordered_set<KeySym>::iterator end);
    int pressWy(double dx, std::unordered_set<KeySym>::iterator it, std::unordered_set<KeySym>::iterator end);

    //draw
    int calculateLM1(int hmapX, int horizonPos, int resY);
    int calculateLM2(int hmap_x, int horizon_pos, int res_Y);
    int calculateAngle(int player_ang_h, int x, int res_X, int fov);
    int mod(int dividend, int divisor);
    int calculate_crdy(int y, int horizon_pos, int hmap);
    int calculate_crd(int crdx, int crdy, int typemap);
    int divide(int a, int b);
    double mul_double(double a, double b);
    double add_double(double a, double b);
    double sub_double(double a, double b);
    double div_double(double a, double b);
    //double absolutef(double a);
    int absolute(int a);
    int add_int(int a, int b);
    int sub_int(int a, int b);
    int mul_int(int a, int b);
    int div_int(int a, int b);

    
}

Display *display;
Window root;
Window win;
GC gc;
int screen;

int mouseX = 0;
int mouseY = 0;
int mouseInitX = 0;
int mouseInitY = 0;
int playerX = 0;
int playerY = 0;

int CHAR_WIDTH = 0;
int CHAR_HEIGHT = 0;

const int res_X = 120;           // resolution x
const int res_Y = 60;            // resolution y; right-click on the window title, select properties and set the proper size
const int fov = 600;             // field of view, in 0.1 degree increments (60 degrees)
const int map_size = 16;          // square map size
const double torad = M_PI / 180; // degrees to radians conversion factor
double sintab[3600];             // lookup table of sine values, every 0.1 degree
double fisheye[res_X];           // lookup table for fisheye correction, 1 value for every screen column
const int mouse_speed = 100;     // mouse speed division


int GameState = 0;

// Buffers
char char_buff[res_X * res_Y];      // screen character buffer
uint16_t color_buff[res_X * res_Y]; // screen color buffer
char char_grad[11] = " .-,=+*#%@";  // character gradient used to denote brightness
int hmap[res_X];                    // map of heights of wall columns
double lmap[res_X];                 // map of light/brightness
int tmap[res_X];                    // map of texture coordinates of wall columns
int typemap[res_X];                 // wall type (if we want various textures etc.)

// World map
int map[map_size][map_size]; // world map
std::map<int, int> colorMap = {
    {0, 0x000000},
    {1, 0x0000FF},
    {2, 0x00FF00},
    {4, 0xFF0000},
    {8, 0xEEEEEE},
    {16, 0xFFD800}
    };


int skull[res_X*res_Y] = {
    //120 0 per line
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};



// Structure holding player data
struct
{   
    double x, y;           // coordinates
    double vx, vy;         // velocities
    double ang_h, ang_v;   // horizontal and vertical angle
    double friction = 0.1; // friction coefficient for movement; set to 0.01 and go ice skating :)
    double accel = 0.1;   // acceleration coefficient for movement
} player;

int horizon_pos = 0; // position of the horizon, in characters; 0=middle of the screen

// Textures
int textures[32 * 32 * 4]; // array containing 3 32x32 textures (1024 pixels); first byte is character, second one is color

// Global flags
int F_exit = 0; // turns to 1 when player presses esc.

std::unordered_set<KeySym> pressedKeys;

void getCurrentMousePosition(Display *display, int &mouseX, int &mouseY)
{
    Window root, child;
    int rootX, rootY, winX, winY;
    unsigned int mask;

    if (XQueryPointer(display, DefaultRootWindow(display), &root, &child, &rootX, &rootY, &winX, &winY, &mask))
    {
        mouseX = rootX;
        mouseY = rootY;
    }
}

void handleKeyPress(KeySym key)
{
    // Handle key press here
}

void handleKeyRelease(KeySym key)
{
    // Handle key release here
}

void closeX()
{
    XCloseDisplay(display);
}

void initializeX()
{
    display = XOpenDisplay(nullptr);
    if (!display)
    {
        std::cerr << "Error: Could not open X display." << std::endl;
        exit(1);
    }

    screen = DefaultScreen(display);
    int screenWidth = XDisplayWidth(display, screen);
    int screenHeight = XDisplayHeight(display, screen);

    root = RootWindow(display, screen);

    // Create a window to capture input events
    XSetWindowAttributes swa;
    swa.event_mask = KeyPressMask | KeyReleaseMask | PointerMotionMask | ButtonPressMask | ButtonReleaseMask;
    win = XCreateWindow(display, root, 0, 0, screenWidth, screenHeight, 0, DefaultDepth(display, screen), InputOutput, DefaultVisual(display, screen), CWEventMask, &swa);

    XSetWindowBackground(display, win, BlackPixel(display, screen));

    XFontStruct *font = XLoadQueryFont(display, "fixed");

    CHAR_WIDTH = font->max_bounds.width;
    CHAR_HEIGHT = font->max_bounds.ascent;

    if (!font)
    {
        std::cerr << "Error: Could not load font." << std::endl;
        XCloseDisplay(display);
        return;
    }

    XMapWindow(display, win);
    XFlush(display);
}

void initGame()
{
    // precalculate sine values. Important, we add 0.001 to avoid even angles where sin=0 or cos=0
    for (int i = 0; i < 3600; i++)
        sintab[i] = sin((i + 0.001) * 0.1 * torad);

    // fisheye correction term (cosine of ray angle relative to the screen center, e.g. res_X/2)
    for (int i = 0; i < res_X; i++)
        fisheye[i] = cos(0.1 * (i - res_X / 2) * torad * fov / res_X);

    for (int i = 0; i < map_size; i++) // make walls on the map edges
    {
        map[i][0] = 1;
        map[i][map_size - 1] = 1;
        map[0][i] = 1;
        map[map_size - 1][i] = 1;
    };



    // map = {
    //     {1, 1, 1, 1, 1, 1, 1, 1},
    //     {1, 0, 0, 0, 1, 0, 4, 1},
    //     {1, 0, 1, 0, 0, 0, 4, 1},
    //     {1, 0, 1, 1, 1, 0, 2, 1},
    //     {1, 0, 1, 0, 0, 0, 0, 1},
    //     {1, 0, 1, 0, 1, 1, 0, 1},
    //     {1, 0, 0, 0, 0, 1, 0, 1},
    //     {1, 1, 1, 1, 1, 1, 1, 1}
    // };

    //1 == brick
    //2 == door
    //3 == fnaf floor
    //4 == exit
    //5 == key
    // 5,6 and 6,6 is start position
    int player_position = 0;
    int exit = 4;
    map[0][1] = 1;  map[0][2] = 1;  map[0][3] = 1;                  map[0][4] = 1; map[0][5] = 1; map[0][6] = 1; map[0][7] = 1; map[0][8] = 1; map[0][9] = 1; map[0][10] = 1; map[0][11] = 1; map[0][12] = 1; map[0][13] = 1; map[0][14] = 1; map[0][15] = 1; 
    map[1][1] = 1;  map[1][2] = 0;  map[1][3] = 0;                  map[1][4] = 1; map[1][5] = 0; map[1][6] = 1; map[1][7] = 0; map[1][8] = 1; map[1][9] = 2; map[1][10] = 1; map[1][11] = 2; map[1][12] = 1; map[1][13] = 0; map[1][14] = 0; map[1][15] = 1;
    map[2][1] = 1;  map[2][2] = 0;  map[2][3] = 0;                  map[2][4] = 1; map[2][5] = 0; map[2][6] = 1; map[2][7] = 0; map[2][8] = 0; map[2][9] = 1; map[2][10] = 1; map[2][11] = 1; map[2][12] = 1; map[2][13] = 0; map[2][14] = 0; map[2][15] = 1;
    map[3][1] = 1;  map[3][2] = 0;  map[3][3] = player_position;    map[3][4] = 2; map[3][5] = 0; map[3][6] = 0; map[3][7] = 0; map[3][8] = 0; map[3][9] = 1; map[3][10] = 0; map[3][11] = 2; map[3][12] = exit; map[3][13] = 1; map[3][14] = 0; map[3][15] = 1;
    map[4][1] = 1;  map[4][2] = 0;  map[4][3] = player_position;    map[4][4] = 1; map[4][5] = 0; map[4][6] = 0; map[4][7] = 0; map[4][8] = 0; map[4][9] = 0; map[4][10] = 0; map[4][11] = 1; map[4][12] = 1; map[4][13] = 1; map[4][14] = 0; map[4][15] = 1;
    map[5][1] = 1;  map[5][2] = 0;  map[5][3] = 0;                  map[5][4] = 1; map[5][5] = 1; map[5][6] = 1; map[5][7] = 0; map[5][8] = 0; map[5][9] = 0; map[5][10] = 0; map[5][11] = 0; map[5][12] = 0; map[5][13] = 0; map[5][14] = 0; map[5][15] = 1;
    map[6][1] = 1;  map[6][2] = 0;  map[6][3] = 0;                  map[6][4] = 1; map[6][5] = 0; map[6][6] = 1; map[6][7] = 0; map[6][8] = 1; map[6][9] = 0; map[6][10] = 1; map[6][11] = 0; map[6][12] = 0; map[6][13] = 0; map[6][14] = 0; map[6][15] = 1;
    map[7][1] = 1;  map[7][2] = 0;  map[7][3] = 0;                  map[7][4] = 2; map[7][5] = 0; map[7][6] = 1; map[7][7] = 0; map[7][8] = 1; map[7][9] = 0; map[7][10] = 1; map[7][11] = 0; map[7][12] = 0; map[7][13] = 0; map[7][14] = 0; map[7][15] = 1;
    map[8][1] = 1;  map[8][2] = 0;  map[8][3] = 0;                  map[8][4] = 1; map[8][5] = 1; map[8][6] = 1; map[8][7] = 0; map[8][8] = 1; map[8][9] = 0; map[8][10] = 1; map[8][11] = 1; map[8][12] = 1; map[8][13] = 1; map[8][14] = 0; map[8][15] = 1;
    map[9][1] = 1;  map[9][2] = 0;  map[9][3] = 0;                  map[9][4] = 1;  map[9][5] = 1; map[9][6] = 1; map[9][7] = 0; map[9][8] = 1; map[9][9] = 0; map[9][10] = 2; map[9][11] = 0; map[9][12] = 0; map[9][13] = 1; map[9][14] = 0; map[9][15] = 1;
    map[10][1] = 1; map[10][2] = 0; map[10][3] = 0;                 map[10][4] = 1; map[10][5] = 1; map[10][6] = 1; map[10][7] = 0; map[10][8] = 1; map[10][9] = 0; map[10][10] = 1; map[10][11] = 0; map[10][12] = 0; map[10][13] = 1; map[10][14] = 0; map[10][15] = 1;
    map[11][1] = 1; map[11][2] = 0; map[11][3] = 0;                 map[11][4] = 1; map[11][5] = 1; map[11][6] = 1; map[11][7] = 0; map[11][8] = 2; map[11][9] = 0; map[11][10] = 1; map[11][11] = 1; map[11][12] = 0; map[11][13] = 1; map[11][14] = 0; map[11][15] = 1;
    map[12][1] = 1; map[12][2] = 0; map[12][3] = 0;                 map[12][4] = 1; map[12][5] = 0; map[12][6] = 1; map[12][7] = 0; map[12][8] = 1; map[12][9] = 0; map[12][10] = 1; map[12][11] = 1; map[12][12] = 0; map[12][13] = 1; map[12][14] = 0; map[12][15] = 1;
    map[13][1] = 1; map[13][2] = 0; map[13][3] = 0;                 map[13][4] = 2; map[13][5] = 0; map[13][6] = 1; map[13][7] = 1; map[13][8] = 1; map[13][9] = 0; map[13][10] = 1; map[13][11] = 1; map[13][12] = 0; map[13][13] = 1; map[13][14] = 0; map[13][15] = 1;
    map[14][1] = 1; map[14][2] = 0; map[14][3] = 0;                 map[14][4] = 1; map[14][5] = 0; map[14][6] = 1; map[14][7] = 1; map[14][8] = 1; map[14][9] = 0; map[14][10] = 1; map[14][11] = 1; map[14][12] = 0; map[14][13] = 1; map[14][14] = 0; map[14][15] = 1;
    map[15][1] = 1; map[15][2] = 1; map[15][3] = 1;                 map[15][4] = 1; map[15][5] = 1; map[15][6] = 1; map[15][7] = 1; map[15][8] = 1; map[15][9] = 1; map[15][10] = 1; map[15][11] = 1; map[15][12] = 1; map[15][13] = 1; map[15][14] = 1; map[15][15] = 1;

    

   

    // craete exit
    map[map_size - 2][map_size - 2] = 4;



    for (int x = 0; x < map_size; x++) // random floor/ceiling textures
        for (int y = 0; y < map_size; y++)
        {
            map[x][y] += 256 * (2);
            map[x][y] += 65536 * (2);
        }

    player.x = 2;
    player.y = 2;
    player.ang_h = 450; // put the player somewhere in the middle, angle is in 0.1 degree increments

    for (int x = 0; x < 32; x++) // texture generation
        for (int y = 0; y < 32; y++)
        {
            textures[x + y * 32] = (12 - 8 * ((y % 6 == 0) || ((x + 4 * (y / 6)) % 16 == 0)) + rand() % 2) + (4 + 4 * ((y % 6 == 0) || ((x + 4 * (y / 6)) % 16 == 0))) * 256; // brick texture;last term is color (4=red, 8=gray)
            // textures[x + y * 32 + 1024] = 8 - 4 * (((y > 16) ^ ((x + 4 * (y / 31)) < 16 ))) + rand() % 2 + (1 + 1 * ((y > 16) ^ ((x + 4 * (y / 31)) < 16 ))) * 256;
            textures[x + y * 32 + 2048] = 8 - 4 * (((y > 16) ^ ((x + 4 * (y / 31)) < 16))) + rand() % 2 + (0 + 8 * (((y % 16) >= 8) ^ ((x % 16) >= 8))) * 256; // large brick texture;last term is color (1=blue)
            textures[x + y * 32 + 1024] = ((12) + rand() % 2) + (16*(((x==0||x==31||(15<=x&&x<=16))||(2>=y||y>=30))||((x==12||x==19)&&(16<=y&&y<=17)))) * 256; //ending
            // std::cout << x << ":  " << (15<=x<=16) << std::endl;
            //                                                                            ^                             
            //                                                                            |
            //                                                                          Border                                          
            textures[x + y * 32 + 3072] = ((12) + rand() % 2) + (4 + 4*((6<=x) && (x<=27) && (y>=6))) * 256;             
        }
    getCurrentMousePosition(display, mouseInitX, mouseInitY);
}

void cast() // main ray casting function
{
    // some speedup might be possible by declaring all variables beforehand here instead of inside the loops

    for (int xs = 0; xs < res_X; xs++) // go through all screen columns
    {   
        
        // ray angle = player angle +-half of FoV at screen edges;
        // add 360 degrees to avoid negative values when using lookup table later

        int r_angle = (int)(3600 + player.ang_h + (xs - res_X / 2) * fov / res_X); // USE ASM

        // ray has a velocity of 1. Now we calculate its horizontal and vertical components;
        // horizontal uses cosine (e.g. sin(a+90 degrees))
        // use %3600 to wrap the angles to 0-360 degree range

        double r_vx = sintab[(r_angle + 900) % 3600];  // USE ASM

        // we will ned an integer step to navigate the map; +1/-1 depending on sign of r_vx
        int r_ivx = (r_vx > 0) ? 1 : -1; // USE ASM

        // now the same for vertical components
        double r_vy = sintab[r_angle % 3600]; // USE ASM
        int r_ivy = (r_vy > 0) ? 1 : -1; // USE ASM

        // initial position of the ray; precise and integer values
        // ray starts from player position; tracing is done on doubles (x,y), map checks on integers(ix,iy)
        double r_x = player.x;
        double r_y = player.y;
        int r_ix = (int)r_x;
        int r_iy = (int)r_y;
        double r_dist = 0; // travelled distance
        double t1, t2;     // time to intersect next vertical/horizontal grid line;

        //printf("ADDR\nr_y: %p\nr_vy: %p\nr_iy: %p\nr_ivy: %p\nr_x: %p\nr_vx: %p\nr_ix: %p\nr_ivx: %p\nr_dist: %p\nt1: %p\nt2: %p\n", &r_y, &r_vy, &r_iy, &r_ivy, &r_x, &r_vx, &r_ix, &r_ivx, &r_dist, &t1, &t2);

        // ray tracing; we check only intersections with horizontal/vertical grid lines, so maximum of 2*map_size is possible
        for (int i = 0; i < 2 * map_size; i++)
        {
            if ((map[r_ix][r_iy] % 256) > 0)
                break; // map>0 is a wall; hit a wall? end tracing

            // calculate time to intersect next vertical grid line;
            // distance to travel is the difference between double and int coordinate, +1 if moving to the right
            // example: x=0.3, map x=0, moving to the right, next grid is x=1 and distance is 1-0.3=0.7
            // to get time, divide the distance by speed in that direction
            t1 = (r_ix - r_x + (r_vx > 0)) / r_vx;
            // the same for horizontal lines
            t2 = (r_iy - r_y + (r_vy > 0)) / r_vy;

            // now we select the lower of two times, e.g. the closest intersection
            if (t1 < t2)
            {                                    // intersection with vertical line
                r_y += r_vy * t1;                // update y position
                r_ix += r_ivx;                   // update x map position by +-1
                r_x = r_ix - (r_vx < 0) * r_ivx; // we are on vertical line -> x coordinate = integer coordinate
                r_dist += t1;                    // increment distance by velocity (=1) * time
            }
            else
            { // intersection with horizontal line
                r_x += r_vx * t2;
                r_iy += r_ivy;
                r_y = r_iy - (r_vy < 0) * r_ivy;
                r_dist += t2;
            }
        }
        // end of tracing; the distance is updated during steps, so there is no need to use slow pythagorean theory to calculate it
        // record wall height; it is inversely proportional to distance; apply fisheye correction term
        hmap[xs] = (int)(res_Y / 2 / r_dist / fisheye[xs]);

        // record the wall type; subtract 1 so map[x][y]=1 means wall type 0
        typemap[xs] = map[r_ix][r_iy] % 256 - 1;

        // record the texture coordinate - it is the fractional part of x/y coordinate * texture size
        tmap[xs] = (t1 < t2) ? 32 * fabs(r_y - (int)(r_y)) : 32 * fabs(r_x - (int)(r_x));

        // wolfenstein 3D style lightning - Norts/south walls are brighter
        lmap[xs] = (t1 < t2) ? 1 : 0.5;

        // calculate brightness; it is proportional to height, 5.0 is arbitrary constant
        lmap[xs] *= 5.0 / res_Y * hmap[xs];

        // add to wall brightness to improve the contrast between walls and floor
        lmap[xs] = lmap[xs] + 0.2;
    }
}

void draw()
{
    // go through the screen, column by column 
    for (int x = 0; x < res_X; x++)
    {
        // upper limit of the wall, capped at half vertical resolution (middle of the screen=0)
        //int lm1 = -(hmap[x] + horizon_pos > res_Y / 2 ? res_Y / 2 : hmap[x] + horizon_pos);
        int lm1 = calculateLM1(hmap[x], horizon_pos, res_Y);

        // lower limit of the wall, capped at -half vertical resolution (middle of the screen=0)
        //int lm2 = (hmap[x] - horizon_pos > res_Y / 2 ? res_Y / 2 : hmap[x] - horizon_pos);
        int lm2 = calculateLM2(hmap[x], horizon_pos, res_Y);

        // array offset for putting characters
        int offset = x;   // we draw on the column x
        double character; // the number of the character from gradient to draw
        int color;        // the color of the character to draw

        for (int y = -res_Y / 2; y < res_Y / 2; y++) // go along the whole screen column, drawing either wall or floor/ceiling
        {
            //int ang = (int)(3600 + player.ang_h + (x - res_X / 2) * fov / res_X); // calculate ray angle; needed for floor
            int ang = calculateAngle(player.ang_h, x, res_X, fov);
            //double dx = sintab[(ang + 900) % 3600];                               // steps in x and y direction, the same as in tracing, needed for floor
            double dx = sintab[mod(ang + 900, 3600)];
            //double dy = sintab[ang % 3600];
            double dy = sintab[mod(ang, 3600)];


            if (y >= lm1 && y <= lm2) // are we drawing a wall?
            {
                int crdx = tmap[x];                                      // we get texture x coordinate from coordinate buffer made in tracing step
                //int crdy = 16 + (14 * (y + horizon_pos) / hmap[x]); // texture y coordinate depends on y, horizon position and height
                int crdy = calculate_crdy(y, horizon_pos, hmap[x]);
                //int crd = crdx + 32 * crdy + 1024 * typemap[x];          // calculate coordinate to use in 1-d texture buffer
                int crd = calculate_crd(crdx, crdy, typemap[x]);

                character = mod(textures[crd],256);                         // get texture pixel (1st byte)
                color = divide(textures[crd],256);                             // get texture color (2nd byte)

                //character = character * lmap[x]; // multiply by the brightness value of light map
                character = mul_double(character, lmap[x]);
            }
            else // floor/ceiling?
            {
                // calculate distance to the floor pixel; y and horizon_pos are in pixels whole, 0.1 is added here to avoid division by 0
                //double dz = (res_Y / 2.0) / (fabs(y + horizon_pos) + 0.1) / fisheye[x];
                double dz = div_double(div_double(div_double(res_Y,2.0) , add_double (absolute(add_double(y , horizon_pos)), 0.1)), fisheye[x]);

                //int crdx = (int)(1024 + 32.0 * (player.x + dx * dz)) % 32; // floor/ceiling texture coordinates
                int crdx = mod(add_double(1024 , mul_double(32.0 , add_double(player.x , mul_double(dx , dz)))), 32);
                //int crdy = (int)(1024 + 32 * (player.y + dy * dz)) % 32; // 1024 is here just to avoid negative numbers
                int crdy = mod(add_double(1024 , mul_double(32.0 , add_double(player.y , mul_double(dy , dz)))), 32);
                //int mcx = (int)(player.x + dx * dz) % map_size;            // floor/ceiling map coordinates
                int mcx = mod(add_double(player.x , mul_double(dx , dz)), map_size);
                //int mcy = (int)(player.y + dy * dz) % map_size;
                int mcy = mod(add_double(player.y , mul_double(dy , dz)), map_size);
                //int crd = crdx + 32 * crdy; // base texture coordinate
                int crd = calculate_crd(crdx, crdy, 0);

                if (y > (-horizon_pos))
                    //crd += 1024 * ((map[mcx][mcy] / 256) % 256); // 2nd byte = floor type
                    crd = add_double(crd, mul_double(1024, mod(divide(map[mcx][mcy], 256), 256)));
                else
                    //crd += 1024 * ((map[mcx][mcy] / 65536) % 256); // 3rd byte = ceiling type
                    crd = add_double(crd, mul_double(1024, mod(divide(map[mcx][mcy], 65536), 256)));

                //character = textures[crd] % 256; // get texture pixel (1st byte)
                character = mod(textures[crd], 256);
                //color = textures[crd] / 256;     // get texture color (2nd byte)
                color = divide(textures[crd], 256);

                // add dithering to avoid ugly edges
                //character += ((abs(y) % 2) + (abs(x) % 2));
                character = add_double(character, add_double(mod(absolute(y), 2), mod(absolute(x), 2)));

                // simple vertical gradient, use lower brightness than walls for better contrast
                //character *= (1.0 / res_Y * abs(y + horizon_pos));
                character = mul_double(character, mul_double(div_double(1.0, res_Y), absolute(add_double(y, horizon_pos))));
            }

            // limit the value to the limits of character gradient (especially important if there are multiple brightness modifiers)
            if (character > 9)
                character = 9;
            if (character < 0)
                character = 0;

            // save the character in character buffer
            char_buff[offset] = char_grad[(int)character];
            // save the color in color buffer
            color_buff[offset] = color;

            //offset += res_X; // go down by 1 row
            offset = add_double(offset, res_X);
        }
        // end of column

        
    } // end of drawing
}


//draw screen

void drawScreen(){
    //draw a skull on the middle of the screen nothing else
    for (int x = 0; x < res_X; x++)
    {
        // array offset for putting characters
        int offset = x;   // we draw on the column x
        int character; // the number of the character from gradient to draw
        int color = 0;        // the color of the character to draw
        
        for (int y = 0; y < res_Y; y++){// go along the whole screen column, drawing either wall or floor/ceiling
            //int index = y*res_X + x;
            int index = add_int(mul_int(y,res_X),x);
            // std::cout << "index: " << index << std::endl;

            if (skull[index]==1){
                color = 8;
            }
            else{
                color = 0;
            }
            // save the character in character buffer
            char_buff[offset] = char_grad[7];
            // save the color in color buffer
            color_buff[offset] = color;

            //offset += res_X; // go down by 1 row
            offset = add_int(offset, res_X);
        }
        // end of column
    } // end of drawing
}


void MouseEvent() // handles keyboard, mouse controls and player movement; windows-specific
{
    getCurrentMousePosition(display, mouseX, mouseY);
    // set player angles according to mouse position. 500 and 20 are arbitraty values that just work OK
    player.ang_h = getPlayerHAngel(mouseX, mouseInitX, mouse_speed);
    // player.ang_h = 500.0 * (mouseX - mouseInitX) / mouse_speed; //player horizontal angle
    player.ang_v = getPlayerVAngel(mouseY, mouseInitY, mouse_speed);
    // player.ang_v = 20.0 * (mouseY - mouseInitY) / mouse_speed;  // player vertical angle

    horizon_pos = (int)player.ang_v; // position of the horizon, for looking up/down 0=in the middle

    int checkToAdd360deg;
    // if (player.ang_h < 3600)
    //     player.ang_h += 3600;

    // std::cout << "player.ang_h: " << player.ang_h << std::endl;
    checkToAdd360deg = checkAdd360deg(player.ang_h);
    // std::cout << "checkToAdd360deg: " << checkToAdd360deg << std::endl;
    player.ang_h += checkToAdd360deg; // if player angle is less than 360 degrees, add 360 degrees so its never negative
}

void handleEvent()
{
    XEvent event;

    while (XPending(display))
    {
        XNextEvent(display, &event);
        if (event.type == KeyPress)
        {
            KeySym key = XLookupKeysym(&event.xkey, 0);
            pressedKeys.insert(key);
        }
        else if (event.type == KeyRelease)
        {
            KeySym key = XLookupKeysym(&event.xkey, 0);
            pressedKeys.erase(key);
        }

        if (event.type == MotionNotify)
        {
            MouseEvent();
        }
    }
}

void updateMovement()
{
    double dx = player.accel * sintab[(int)player.ang_h % 3600];         // x step in the direction player is looking;
    double dy = player.accel * sintab[((int)player.ang_h + 900) % 3600]; // y step in the direction player is looking

    if (pressedKeys.find(XK_w) != pressedKeys.end())
    {
        player.vx += dy;
        player.vy += dx;
    }
    // player.vx += pressWx(dy,pressedKeys.find(XK_w),pressedKeys.end());
    // player.vy += pressWy(dx,pressedKeys.find(XK_w),pressedKeys.end());
    if (pressedKeys.find(XK_s) != pressedKeys.end())
    {
        player.vx -= dy;
        player.vy -= dx;
    }
    if (pressedKeys.find(XK_a) != pressedKeys.end())
    {
        player.vx += dx;
        player.vy -= dy;
    }
    if (pressedKeys.find(XK_d) != pressedKeys.end())
    {
        player.vx -= dx;
        player.vy += dy;
    }
    // std::cout << "vx, y" << (int)(player.x + 1 * player.vx) << " , " << (int)player.y << std::endl;
    // std::cout << "vy, x" << (int)(player.y + 1 * player.vy) << " , " << (int)player.x << std::endl;
    if ((map[(int)(player.x + 2 * player.vx)][(int)player.y] % 256 == 4) || (map[(int)player.x][(int)(player.y + 2 * player.vy)] % 256 == 4)){
        GameState = 1;
        // F_exit = 1;
        // std::cout << "You win!" << std::endl;
        // closeX();
        // exit(0);
    }

    if (map[(int)(player.x + 2 * player.vx)][(int)player.y] % 256 == 2){
        map[(int)(player.x + 2 * player.vx)][(int)player.y] = 131584;//found door on x axis
        // std::cout << "Yes Door" << std::endl;
    }
    if (map[(int)player.x][(int)(player.y + 2 * player.vy)] % 256 == 2) {
        map[(int)player.x][(int)(player.y + 2 * player.vy)] = 131584; //found door on y axis
        // std::cout << "Yes Door" << std::endl;
    }
    if (map[(int)(player.x + 1 * player.vx)][(int)player.y] % 256 > 0)
        //std::cout << "X collision: "<< mapIDx << std::endl;
        player.vx = -player.vx / 2; // collisions in x axis - bounce back with half the velocity
    if (map[(int)player.x][(int)(player.y + 1 * player.vy)] % 256 > 0)
        // std::cout << map[(int)player.x][(int)(player.y + 1 * player.vy)] % 256 << std::endl;
        player.vy = -player.vy / 2; // collisions in y axis
    // else {
    //     std::cout << map[(int)player.x][(int)(player.y + 1 * player.vy)] << std::endl;
    // }
    
    player.x += player.vx;          // update x,y values with x,y velocities
    player.y += player.vy;
    player.vx *= (1 - player.friction); // friction reduces velocity values
    player.vy *= (1 - player.friction);
}

void displayText(Display *display, Window win, GC gc)
{
    // Create a graphics context (GC) for drawing
    gc = XCreateGC(display, win, 0, NULL);

    // Set the foreground color (you can choose your own color)
    XSetForeground(display, gc, BlackPixel(display, screen));

    // Loop through the character buffer and draw each character on the window
    for (int y = 0; y < res_Y; ++y)
    {
        for (int x = 0; x < res_X; ++x)
        {
            int index = y * res_X + x;
            char ch = char_buff[index];
            int color = color_buff[index];

            XSetForeground(display, gc, BlackPixel(display, screen));
            XFillRectangle(display, win, gc, x * CHAR_WIDTH, y * CHAR_HEIGHT * 2, CHAR_WIDTH, CHAR_HEIGHT * 2);

            // Set the foreground color based on color_buff
            XSetForeground(display, gc, colorMap[color]);

            // Draw the character on the window

            XDrawString(display, win, gc, x * CHAR_WIDTH, y * CHAR_HEIGHT * 2, &ch, 1);
        }
    }

    // Flush the changes to the window
    XFlush(display);

    // Don't forget to free the GC when you're done
    XFreeGC(display, gc);
}

// void displayScreen(Display *display, Window win, GC gc)
// {
//     // Create a graphics context (GC) for drawing
//     gc = XCreateGC(display, win, 0, NULL);

//     // Set the foreground color (you can choose your own color)
//     XSetForeground(display, gc, BlackPixel(display, screen));

//     // Loop through the character buffer and draw each character on the window
//     for (int y = 0; y < res_Y; ++y)
//     {
//         for (int x = 0; x < res_X; ++x)
//         {
//             int index = y * res_X + x;
//             char ch = char_buff[index];
//             int color = color_buff[index];

//             XSetForeground(display, gc, BlackPixel(display, screen));
//             XFillRectangle(display, win, gc, x * CHAR_WIDTH, y * CHAR_HEIGHT * 2, CHAR_WIDTH, CHAR_HEIGHT * 2);

//             // Set the foreground color based on color_buff
//             XSetForeground(display, gc, colorMap[color]);

//             // Draw the character on the window

//             XDrawString(display, win, gc, x * CHAR_WIDTH, y * CHAR_HEIGHT * 2, &ch, 1);
//         }
//     }

//     // Flush the changes to the window
//     XFlush(display);

//     // Don't forget to free the GC when you're done
//     XFreeGC(display, gc);
// }

// int main()
// {   
//     // std::cout << 11%4 << std::endl;
//     // std::cout << modulo(11,4) << std::endl;
//     // int x = 31;
//     // std::cout << (15<=x<=16) << std::endl;
    

//     initializeX();
//     initGame();

//     XEvent event;

//     while (true)
//     {
//         handleEvent();
//         updateMovement();
//         cast();
//         std::cout << GameState << std::endl;
//         if (GameState == 0){
//             draw();
            
//         }
//         else if (GameState == 1){
//             drawScreen();
//         }
//         displayText(display, win, gc);
//     }

//     return 0;
// }


int main()
{
    // std::cout << 11%4 << std::endl;
    // std::cout << modulo(11,4) << std::endl;
    // int x = 31;
    // // std::cout << (15<=x<=16) << std::endl;
    std::cout << mul_double(1.5,1.5) << " "<<add_double(1.5,1.5) << std::endl;

    initializeX();
    initGame();

    XEvent event;

    while (true)
    {
        handleEvent();
        updateMovement();
        cast();
        //std::cout << GameState << std::endl;
        if (GameState == 0)
        {
            draw();
            int entityX = res_X - 2;
            int entityY = 2;

            if (entityX >= 0 && entityX < res_X && entityY >= -res_Y / 2 && entityY < res_Y / 2)
            {
                // The entity is within the screen bounds, so draw a yellow dot at its position
                int offset = entityX + (entityY + res_Y / 2) * res_X;
                char_buff[offset] = 'F'; // Character for a dot
                color_buff[offset] = 1;  // Color value for yellow
            }
        }
        else if (GameState == 1)
        {
            drawScreen();
        }
        displayText(display, win, gc);
    }

    return 0;
}
