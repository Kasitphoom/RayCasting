#include <iostream>
#include <unordered_set>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <cmath>
#include <cstring>
#include <cstdint>
#include <unistd.h>
#include <map>
#include <chrono> // for high_resolution_clock and duration_cast
#include <thread> // for sleep_for
#include <ctime>

extern "C"
{
    int getPlayerHAngel(int mouseX, int mouseInitX, int mouse_speed);
    int getPlayerVAngel(int mouseY, int mouseInitY, int mouse_speed);
    int checkAdd360deg(int playerAngle);
    int modulo(int a, int b);
    int pressWx(double dy, std::unordered_set<KeySym>::iterator it, std::unordered_set<KeySym>::iterator end);
    int pressWy(double dx, std::unordered_set<KeySym>::iterator it, std::unordered_set<KeySym>::iterator end);
    double mul_double(double a, double b);
    double add_double(double a, double b);
    double sub_double(double a, double b);
    double div_double(double a, double b);
    int mul_int(int a, int b);
    int add_int(int a, int b);
    int sub_int(int a, int b);
    int div_int(int a, int b);
    int bgt(int a, int b);
    int compareZero(int a);
    int logicalOR(int a, int b);
    int logicalAND(int a, int b);
    int logicalXOR(int a, int b);
    int greaterThan(int a, int b);
    int lessThan(int a, int b);
    int GTE(int a, int b);
    int LTE(int a, int b);
    int EQ(int a, int b);
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
const int map_size = 8;          // square map size
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
    {0, 0x000000}, // black
    {1, 0x0000FF}, // blue
    {2, 0x00FF00}, // green
    {3, 0x00000000}, // red
    {4, 0xFF0000}, // yellow
    {5, 0xFFD800}, // orange
    {8, 0xEEEEEE}, // white
};

char timeLable[7] = "Time: ";
std::string timerText = "00:00";
int timerDuration = 2 * 60;

// Initialize variables for minutes and seconds
int minutes, seconds;

int skull120[120*60]={


0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

};

// Structure holding player data
struct
{
    double x, y;           // coordinates
    double vx, vy;         // velocities
    double ang_h, ang_v;   // horizontal and vertical angle
    double friction = 0.1; // friction coefficient for movement; set to 0.01 and go ice skating :)
    double accel = 0.01;   // acceleration coefficient for movement
} player;

struct Sprite // All variables per sprite
{
    int type;      // static, key, enemy
    int state;     // on off
    int map;       // texture to show
    float x, y, z; // position
};

Sprite sp[4];

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
        sintab[i] = sin(mul_double(add_double(i, 0.001), mul_double(0.1, torad)));

    // fisheye correction term (cosine of ray angle relative to the screen center, e.g. res_X/2)
    for (int i = 0; i < res_X; i++)
        fisheye[i] = cos(mul_double(mul_double(0.1, (sub_double(i, div_double(res_X, 2)))), div_double(mul_double(torad, fov), res_X)));

    for (int i = 0; i < map_size; i++) // make walls on the map edges
    {
        map[i][0] = 1;
        map[i][sub_int(map_size, 1)] = 1;
        map[0][i] = 1;
        map[sub_int(map_size, 1)][i] = 1;
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

    // 1 == brick
    // 2 == door
    // 3 == fnaf floor
    // 4 == exit

    map[1][1] = 4;
    map[1][2] = 0;
    map[1][3] = 0;
    map[1][4] = 1;
    map[1][5] = 0;
    map[1][6] = 0;
    map[2][1] = 1;
    map[2][2] = 1;
    map[2][3] = 0;
    map[2][4] = 1;
    map[2][5] = 0;
    map[2][6] = 0;
    map[3][1] = 0;
    map[3][2] = 1;
    map[3][3] = 2;
    map[3][4] = 1;
    map[3][5] = 0;
    map[3][6] = 0;
    map[4][1] = 0;
    map[4][2] = 0;
    map[4][3] = 0;
    map[4][4] = 0;
    map[4][5] = 0;
    map[4][6] = 0;
    map[5][1] = 0;
    map[5][2] = 0;
    map[5][3] = 0;
    map[5][4] = 0;
    map[5][5] = 0;
    map[5][6] = 0;
    map[6][1] = 0;
    map[6][2] = 0;
    map[6][3] = 0;
    map[6][4] = 0;
    map[6][5] = 0;
    map[6][6] = 0;

    // map[1][4] = 1;
    // map[1][6] = 4;
    // map[2][2] = 1;
    // map[2][6] = 4;
    // map[3][2] = 1;
    // map[3][3] = 1;
    // map[3][4] = 1;
    // map[3][5] = 0;
    // map[3][6] = 2;
    // map[4][2] = 1;
    // map[5][2] = 1;
    // map[5][4] = 1;
    // map[5][5] = 1;
    // map[6][5] = 1;

    // map[3][1] = 1;
    // map[2][4] = 2;
    // map[7][7] = 1; // some random blocks;

    for (int x = 0; x < map_size; x++) // random floor/ceiling textures
        for (int y = 0; y < map_size; y++)
        {
            //map[x][y] += 256 * (2);
            //map[x][y] += 65536 * (2);
            map[x][y] = add_int(map[x][y], mul_int(256, 2));
            map[x][y] = add_int(map[x][y], mul_int(65536, 2));
        }

    player.x = 6;
    player.y = 6.5;
    player.ang_h = 450; // put the player somewhere in the middle, angle is in 0.1 degree increments

    for (int x = 0; x < 32; x++) // texture generation
        for (int y = 0; y < 32; y++)
        {
            //textures[x + y * 32] = (12 - 8 * ((y % 6 == 0)  ((x + 4 * (y / 6)) % 16 == 0)) + rand() % 2) + (4 + 4 * ((y % 6 == 0)  ((x + 4 * (y / 6)) % 16 == 0))) * 256; // brick texture;last term is color (4=red, 8=gray)
            //textures[x + y * 32] = (12 - 8 * logicalOR(compareZero(modulo(y,6)), compareZero(modulo((x + 4 * (y / 6)) , 16))) + modulo(rand(), 2)) + (4 + 4 * logicalOR(compareZero(modulo(y,6)), compareZero(modulo((x + 4 * (y / 6)) , 16)))) * 256; // brick texture;last term is color (4=red, 8=gray)
            //use add_int, mul_int, sub_int, div_int to replace +, *, -, / respectively
            textures[add_int(x, mul_int(y, 32))] = add_int(sub_int(12, mul_int(8, logicalOR(compareZero(modulo(y, 6)), compareZero(modulo(add_int(x, mul_int(4, div_int(y, 6))), 16))))), mul_int(add_int(4, mul_int(4, logicalOR(compareZero(modulo(y, 6)), compareZero(modulo(add_int(x, mul_int(4, div_int(y, 6))), 16))))), 256)); // brick texture;last term is color (4=red, 8=gray)
            
            //textures[x + y * 32 + 2048] = 8 - 4 * (((y > 16) ^ ((x + 4 * (y / 31)) < 16))) + rand() % 2 + (0 + 8 * (((y % 16) >= 8) ^ ((x % 16) >= 8))) * 256; // large brick texture;last term is color (1=blue)
            //textures[x + y * 32 + 2048] = 8 - 4 * (logicalXOR(greaterThan(y, 16), lessThan((x + 4 * (y / 31)), 16))) + modulo(rand(), 2) + (0 + 8 * logicalXOR(GTE(modulo(y, 16), 8), GTE(modulo(x, 16), 8))) * 256; // large brick texture;last term is color (1=blue)
            //use add_int, mul_int, sub_int, div_int to replace +, *, -, / respectively
            textures[add_int(add_int(x, mul_int(y, 32)) , 2048)] = add_int(sub_int(8, mul_int(4, logicalXOR(greaterThan(y, 16), lessThan(add_int(x, mul_int(4, div_int(y, 31))), 16)))), add_int(modulo(rand(), 2), mul_int(add_int(0, mul_int(8, logicalXOR(GTE(modulo(y, 16), 8), GTE(modulo(x, 16), 8)))), 256))); // large brick texture;last term is color (1=blue)


            //textures[x + y * 32 + 1024] = (12 + rand() % 2) + (16*(((x==0||x==31||(15<=x&&x<=16))(2>=y||y>=30))((x==12||x==19)&&(16<=y&&y<=17)))) * 256; //ending            
            bool condition1 = logicalOR(compareZero(x), EQ(x, 31));
            bool condition2 = logicalAND(LTE(15, x), LTE(x, 16));
            bool condition3 = logicalOR(GTE(2, y), GTE(y, 30));
            bool condition4 = logicalOR(EQ(x, 12), EQ(x, 19));
            bool condition5 = logicalAND(LTE(16, y), LTE(y, 17));
            //textures[x + y * 32 + 1024] = (12 + modulo(rand(), 2)) + (5 * logicalOR(logicalOR(logicalOR(condition1, condition2), condition3), logicalAND(condition4, condition5))) * 256;
            textures[add_int(x , add_int(mul_int(y , 32) , 1024))] =  add_int(( add_int(12,modulo(rand(), 2)) ),mul_int(( mul_int(5,logicalOR(logicalOR(logicalOR(condition1, condition2), condition3), logicalAND(condition4, condition5))) ),256))   ;
            //use add_int, mul_int, sub_int, div_int to replace +, *, -, / respectively
                                        
            //textures[x + y * 32 + 3072] = (12 + rand() % 2) + (4 + 4*((6<=x) && (x<=27) && (y>=6))) * 256;         
            //textures[x + y * 32 + 3072] = (12 + modulo(rand(), 2)) + (4 + 4*(logicalAND(logicalAND(LTE(6,x), LTE(x,27)), GTE(y,6)))) * 256;
            //use add_int, mul_int, sub_int, div_int to replace +, *, -, / respectively
            textures[add_int(add_int(x, mul_int(y, 32)), 3072)] = add_int(add_int(12, modulo(rand(), 2)), mul_int(add_int(4, mul_int(4, logicalAND(logicalAND(LTE(6,x), LTE(x,27)), GTE(y,6)))) , 256));
        }
    getCurrentMousePosition(display, mouseInitX, mouseInitY);

    // initialize sprite1
    sp[0].type = 1;
    sp[0].state = 1;
    sp[0].map = 0;
    sp[0].x = mul_double(1.5, 32);
    sp[0].y = mul_double(1.5, 32);
    sp[0].z = 0;
}

void cast() // main ray casting function
{
    // some speedup might be possible by declaring all variables beforehand here instead of inside the loops

    for (int xs = 0; xs < res_X; xs++) // go through all screen columns
    {

        // ray angle = player angle +-half of FoV at screen edges;
        // add 360 degrees to avoid negative values when using lookup table later
        int r_angle = (int)(3600 + player.ang_h + (xs - res_X / 2) * fov / res_X);

        // ray has a velocity of 1. Now we calculate its horizontal and vertical components;
        // horizontal uses cosine (e.g. sin(a+90 degrees))
        // use %3600 to wrap the angles to 0-360 degree range
        double r_vx = sintab[(r_angle + 900) % 3600];
        // we will ned an integer step to navigate the map; +1/-1 depending on sign of r_vx
        int r_ivx = (r_vx > 0) ? 1 : -1;

        // now the same for vertical components
        double r_vy = sintab[r_angle % 3600];
        int r_ivy = (r_vy > 0) ? 1 : -1;

        // initial position of the ray; precise and integer values
        // ray starts from player position; tracing is done on doubles (x,y), map checks on integers(ix,iy)
        double r_x = player.x;
        double r_y = player.y;
        int r_ix = (int)r_x;
        int r_iy = (int)r_y;
        double r_dist = 0; // travelled distance
        double t1, t2;     // time to intersect next vertical/horizontal grid line;

        // printf("ADDR\nr_y: %p\nr_vy: %p\nr_iy: %p\nr_ivy: %p\nr_x: %p\nr_vx: %p\nr_ix: %p\nr_ivx: %p\nr_dist: %p\nt1: %p\nt2: %p\n", &r_y, &r_vy, &r_iy, &r_ivy, &r_x, &r_vx, &r_ix, &r_ivx, &r_dist, &t1, &t2);

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
        int lm1 = -(hmap[x] + horizon_pos > res_Y / 2 ? res_Y / 2 : hmap[x] + horizon_pos);
        // lower limit of the wall, capped at -half vertical resolution (middle of the screen=0)
        int lm2 = (hmap[x] - horizon_pos > res_Y / 2 ? res_Y / 2 : hmap[x] - horizon_pos);

        // array offset for putting characters
        int offset = x;   // we draw on the column x
        double character; // the number of the character from gradient to draw
        int color;        // the color of the character to draw

        for (int y = -res_Y / 2; y < res_Y / 2; y++) // go along the whole screen column, drawing either wall or floor/ceiling
        {
            int ang = (int)(3600 + player.ang_h + (x - res_X / 2) * fov / res_X); // calculate ray angle; needed for floor
            double dx = sintab[(ang + 900) % 3600];                               // steps in x and y direction, the same as in tracing, needed for floor
            double dy = sintab[ang % 3600];

            if (y >= lm1 && y <= lm2) // are we drawing a wall?
            {
                int crdx = tmap[x];                                      // we get texture x coordinate from coordinate buffer made in tracing step
                int crdy = 16 + (int)(14 * (y + horizon_pos) / hmap[x]); // texture y coordinate depends on y, horizon position and height
                int crd = crdx + 32 * crdy + 1024 * typemap[x];          // calculate coordinate to use in 1-d texture buffer
                character = textures[crd] % 256;                         // get texture pixel (1st byte)
                color = textures[crd] / 256;                             // get texture color (2nd byte)

                character = character * lmap[x]; // multiply by the brightness value of light map
            }
            else // floor/ceiling?
            {
                // calculate distance to the floor pixel; y and horizon_pos are in pixels whole, 0.1 is added here to avoid division by 0
                double dz = (res_Y / 2) / (fabs(y + horizon_pos) + 0.1) / fisheye[x];

                int crdx = (int)(1024 + 32.0 * (player.x + dx * dz)) % 32; // floor/ceiling texture coordinates
                int crdy = (int)(1024 + 32.0 * (player.y + dy * dz)) % 32; // 1024 is here just to avoid negative numbers
                int mcx = (int)(player.x + dx * dz) % map_size;            // floor/ceiling map coordinates
                int mcy = (int)(player.y + dy * dz) % map_size;
                int crd = crdx + 32 * crdy; // base texture coordinate

                if (y > (-horizon_pos))
                    crd += 1024 * ((map[mcx][mcy] / 256) % 256); // 2nd byte = floor type
                else
                    crd += 1024 * ((map[mcx][mcy] / 65536) % 256); // 3rd byte = ceiling type

                character = textures[crd] % 256; // get texture pixel (1st byte)
                color = textures[crd] / 256;     // get texture color (2nd byte)

                // add dithering to avoid ugly edges
                character += ((abs(y) % 2) + (abs(x) % 2));

                // simple vertical gradient, use lower brightness than walls for better contrast
                character *= (1.0 / res_Y * abs(y + horizon_pos));
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

            offset += res_X; // go down by 1 row
        }
        // end of column
    } // end of drawing
}

// draw screen

void drawScreen()
{
    // draw a skull on the middle of the screen nothing else
    for (int x = 0; x < res_X; x++)
    {
        // array offset for putting characters
        int offset = x; // we draw on the column x
        int character;  // the number of the character from gradient to draw
        int color = 0;  // the color of the character to draw

        for (int y = 0; y < res_Y; y++)
        { // go along the whole screen column, drawing either wall or floor/ceiling
            int index = y * res_X + x;
            // std::cout << "index: " << index << std::endl;

            if (skull120[index] == 1)
            {
                color = 8;
            }
            else
            {
                color = 0;
            }
            // save the character in character buffer
            char_buff[offset] = char_grad[7];
            // save the color in color buffer
            color_buff[offset] = color;

            offset += res_X; // go down by 1 row
        }
        // end of column
    } // end of drawing
}

void drawSprite()
{

    // double sx = sp[0].x - player.x;
    // double sy = sp[0].y - player.y;
    // double sz = sp[0].z;

    // double CS = cos(player.ang_h * torad);
    // double SN = sin(player.ang_h * torad);

    // double a = sy * CS - sx * SN;
    // double b = sx * SN + sy * SN;
    // sx = a;
    // sy = b;

    // sx = (sx*108.0/sy)+(120/2);
    // sy = (sz*108.0/sy)+( 80/2);
}

void MouseEvent() // handles keyboard, mouse controls and player movement; windows-specific
{
    getCurrentMousePosition(display, mouseX, mouseY);
    // set player angles according to mouse position. 500 and 20 are arbitraty values that just work OK
    // player.ang_h = getPlayerHAngel(mouseX, mouseInitX, mouse_speed);
    player.ang_h = 500.0 * (mouseX - mouseInitX) / mouse_speed; // player horizontal angle
    // player.ang_v = getPlayerVAngel(mouseY, mouseInitY, mouse_speed);
    // std::cout << player.ang_h << std::endl;
    player.ang_v = 20.0 * (mouseY - mouseInitY) / mouse_speed; // player vertical angle
    // std::cout << player.ang_v << std::endl;

    horizon_pos = (int)player.ang_v; // position of the horizon, for looking up/down 0=in the middle

    // int checkToAdd360deg;
    if (player.ang_h < 3600)
        player.ang_h += 3600;

    // std::cout << "player.ang_h: " << player.ang_h << std::endl;
    // checkToAdd360deg = checkAdd360deg(player.ang_h);
    // std::cout << "checkToAdd360deg: " << checkToAdd360deg << std::endl;
    // player.ang_h += checkToAdd360deg; // if player angle is less than 360 degrees, add 360 degrees so its never negative
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
        //player.vx += dy;
        //player.vy += dx;
        player.vx = add_double(player.vx , dy);
        player.vy = add_double(player.vy , dx);
    }
    // player.vx += pressWx(dy,pressedKeys.find(XK_w),pressedKeys.end());
    // player.vy += pressWy(dx,pressedKeys.find(XK_w),pressedKeys.end());
    if (pressedKeys.find(XK_s) != pressedKeys.end())
    {
        // player.vx -= dy;
        // player.vy -= dx;
        player.vx = sub_double(player.vx , dy);
        player.vy = sub_double(player.vy , dx);
    }
    if (pressedKeys.find(XK_a) != pressedKeys.end())
    {
        // player.vx += dx;
        // player.vy -= dy;
        player.vx = add_double(player.vx , dx);
        player.vy = sub_double(player.vy , dy);
    }
    if (pressedKeys.find(XK_d) != pressedKeys.end())
    {
        // player.vx -= dx;
        // player.vy += dy;
        player.vx = sub_double(player.vx , dx);
        player.vy = add_double(player.vy , dy);
    }
    // std::cout << "vx, y" << (int)(player.x + 1 * player.vx) << " , " << (int)player.y << std::endl;
    // std::cout << "vy, x" << (int)(player.y + 1 * player.vy) << " , " << (int)player.x << std::endl;

    //Original
    // if ((map[(int)(player.x + 2.0 * player.vx)][(int)player.y] % 256 == 4) || (map[(int)player.x][(int)(player.y + 2.0 * player.vy)] % 256 == 4))
    // {
    //     GameState = 1;
    //     // F_exit = 1;
    //     // std::cout << "You win!" << std::endl;
    //     // closeX();
    //     // exit(0);
    // }

    if ((modulo(map[(int)(add_double(player.x , mul_double(2 , player.vx)))][(int)player.y] , 256) == 4) || (modulo(map[(int)player.x][(int)(add_double(player.y , mul_double(2 , player.vy)))] , 256) == 4))
    {
        GameState = 1;
        // F_exit = 1;
        // std::cout << "You win!" << std::endl;
        // closeX();
        // exit(0);
    }
    

    // if (map[(int)(player.x + 2 * player.vx)][(int)player.y] % 256 == 2)
    // {
    //     map[(int)(player.x + 2 * player.vx)][(int)player.y] = 131584; // found door on x axis
    //     // std::cout << "Yes Door" << std::endl;
    // }

    if (modulo(map[(int)(add_double(player.x , mul_double(2 , player.vx)))][(int)player.y] , 256) == 2)
    {
        map[(int)(add_double(player.x , mul_double(2 , player.vx)))][(int)player.y] = 131584; // found door on x axis
        // std::cout << "Yes Door" << std::endl;
    }
    // if (map[(int)player.x][(int)(player.y + 2 * player.vy)] % 256 == 2)
    // {
    //     map[(int)player.x][(int)(player.y + 2 * player.vy)] = 131584; // found door on y axis
    //     // std::cout << "Yes Door" << std::endl;
    // }
    if (modulo(map[(int)player.x][(int)(add_double(player.y , mul_double(2 , player.vy)))] , 256) == 2)
    {
        map[(int)player.x][(int)(add_double(player.y , mul_double(2 , player.vy)))] = 131584; // found door on y axis
        // std::cout << "Yes Door" << std::endl;
    }

    
    // if (map[(int)(player.x + 1 * player.vx)][(int)player.y] % 256 > 0)
    //     player.vx = -player.vx / 2; // collisions in x axis - bounce back with half the velocity

    //std::cout << "Result 1: "<< modulo(map[(int)(add_double(player.x , mul_double(1, player.vx)))][(int)player.y], 256)<< std::endl;
    //std::cout << "Result 1:" << greaterThan(modulo(map[(int)(add_double(player.x , mul_double(1, player.vx)))][(int)player.y], 256) , 0) << std::endl;
    //if (modulo(map[(int)(add_double(player.x , mul_double(1, player.vx)))][(int)player.y], 256) > 0) {
        // std::cout << "X collision: "<< mapIDx << std::endl;
        //double conditionX = greaterThan(modulo(map[(int)(add_double(player.x , mul_double(1, player.vx)))][(int)player.y], 256) , 0);
        //player.vx = div_double(-player.vx , 1.0*conditionX +1); // collisions in x axis - bounce back with half the velocity
        //std::cout << "Actual: "<< 1 << std::endl;
    //} //else {
        //std::cout << "Actual: "<< 0 << std::endl;
    //}
    double conditionX = greaterThan(modulo(map[(int)(add_double(player.x , mul_double(1, player.vx)))][(int)player.y], 256) , 0);
    // std::cout << "conditionX: "<< conditionX << std::endl;
    // std::cout << "value: " << div_double(-player.vx , (double)(1.0*conditionX + 1.0)) << std::endl;
    // player.vx = div_double((1-(conditionX*2))*player.vx , (double)(1.0*conditionX + 1.0)); // collisions in x axis - bounce back with half the velocity
    player.vx = div_double(mul_double(sub_double(1,mul_double(conditionX,2.0)),player.vx) , (double)(add_double(mul_double(1.0,conditionX) , 1.0))); // collisions in x axis - bounce back with half the velocity

    
    //std::cout << "Result 2: "<< modulo(map[(int)player.x][(int)(add_double(player.y , mul_double(1 , player.vy)))] , 256) << std::endl;
    //std::cout << "Result 2:" << greaterThan(modulo(map[(int)player.x][(int)(add_double(player.y , mul_double(1 , player.vy)))] , 256) , 0) << std::endl;
    //if (modulo(map[(int)player.x][(int)(add_double(player.y , mul_double(1 , player.vy)))] , 256) > 0)
        // std::cout << map[(int)player.x][(int)(player.y + 1 * player.vy)] % 256 << std::endl;
        //player.vy = div_double(-player.vy , 2.0); // collisions in y axis
    // else {
    //     std::cout << map[(int)player.x][(int)(player.y + 1 * player.vy)] << std::endl;
    // }
    double conditionY = greaterThan(modulo(map[(int)player.x][(int)(add_double(player.y , mul_double(1 , player.vy)))] , 256) , 0);
    player.vy = div_double(mul_double(sub_double(1,mul_double(conditionY,2.0)),player.vy) , (double)(add_double(mul_double(1.0,conditionY) , 1.0))); // collisions in x axis - bounce back with half the velocity

    player.x = add_double(player.x,player.vx); // update x,y values with x,y velocities -> original: player.x += player.vx;
    player.y = add_double(player.y,player.vy); // original: player.y += player.vy;
    player.vx = mul_double(player.vx, sub_double(1 , player.friction)); // friction reduces velocity values -> original: player.vx *= (1 -player.friction);
    player.vy = mul_double(player.vy, sub_double(1 , player.friction)); // original: player.vy *= (1 -player.friction);
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

void setTimer() {
    while (timerDuration > 0) {
        // Calculate minutes and seconds
        minutes = timerDuration / 60;
        seconds = timerDuration % 60;

        // Display the time in "mm:ss" format
        //std::cout << minutes << ":";
        // if (seconds < 10) {
        //     std::cout << "0"; // Add leading zero for single-digit seconds
        // }
        //std::cout << seconds << std::endl;
        timerText = (seconds < 10) ? std::to_string(minutes) + ":0" + std::to_string(seconds) : std::to_string(minutes) + ":" + std::to_string(seconds);
        //std::cout << timerText << std::endl;

        // Decrement the timer duration
        --timerDuration;

        // // Sleep for 1 second
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    GameState = 1;
}

void displayTimer() {
    for (int i = 0; i < 6; i++)
    {
        int entityX = res_X - 20 + i;
        int entityY = -29;

        if (entityX >= 0 && entityX < res_X && entityY >= -res_Y / 2 && entityY < res_Y / 2)
        {
            // The entity is within the screen bounds, so draw a yellow dot at its position
            int offset = entityX + (entityY + res_Y / 2) * res_X;
            char_buff[offset] = timeLable[i]; // Character for a dot
            color_buff[offset] = 5;  // Color value for yellow
        }
    }
    for (int i = 0; i < 4; i++)
    {
        int entityX = res_X - 14 + i;
        int entityY = -29;

        if (entityX >= 0 && entityX < res_X && entityY >= -res_Y / 2 && entityY < res_Y / 2)
        {
            // The entity is within the screen bounds, so draw a yellow dot at its position
            int offset = entityX + (entityY + res_Y / 2) * res_X;
            char_buff[offset] = timerText[i]; // Character for a dot
            color_buff[offset] = 5;  // Color value for yellow
        }
    }
}

int main()
{
    // std::cout << 11%4 << std::endl;
    // std::cout << modulo(11,4) << std::endl;
    // int x = 31;
    // // std::cout << (15<=x<=16) << std::endl;
    std::cout << mul_double(1.5,1.5) << " "<<add_double(1.5,1.5) << std::endl;

    initializeX();
    initGame();
    std::thread timerThread(setTimer);

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
            displayTimer();
            
        }
        else if (GameState == 1)
        {
            drawScreen();
        }
        displayText(display, win, gc);
    }
    timerThread.join();

    return 0;
}