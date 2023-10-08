#include <iostream>
#include <unordered_set>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <cmath>
#include <cstring>

Display* display;
Window root;
Window win;
GC gc;
int screen;

int mouseX = 0;
int mouseY = 0;
int playerX = 0;
int playerY = 0;

const int res_X = 120;           // resolution x
const int res_Y = 60;            // resolution y; right-click on the window title, select properties and set the proper size
const int fov = 600;             // field of view, in 0.1 degree increments (60 degrees)
const int map_size = 16;         // square map size
const double torad = M_PI / 180; // degrees to radians conversion factor
double sintab[3600];             // lookup table of sine values, every 0.1 degree
double fisheye[res_X];           // lookup table for fisheye correction, 1 value for every screen column
const int mouse_speed = 100;     // mouse speed division

// Buffers
char char_buff[res_X * res_Y];     // screen character buffer
// WORD color_buff[res_X * res_Y];    // screen color buffer
char char_grad[11] = " .-,=+*#%@"; // character gradient used to denote brightness
int hmap[res_X];                   // map of heights of wall columns
double lmap[res_X];                // map of light/brightness
int tmap[res_X];                   // map of texture coordinates of wall columns
int typemap[res_X];                // wall type (if we want various textures etc.)

// World map
int map[map_size][map_size]; // world map

// Structure holding player data
struct
{
    double x, y;           // coordinates
    double vx, vy;         // velocities
    double ang_h, ang_v;   // horizontal and vertical angle
    double friction = 0.1; // friction coefficient for movement; set to 0.01 and go ice skating :)
    double accel = 0.01;   // acceleration coefficient for movement
} player;

int horizon_pos = 0; // position of the horizon, in characters; 0=middle of the screen

// Textures
int textures[32 * 32 * 3]; // array containing 3 32x32 textures (1024 pixels); first byte is character, second one is color

// Global flags
int F_exit = 0; // turns to 1 when player presses esc.

std::unordered_set<KeySym> pressedKeys;

void handleKeyPress(KeySym key) {
    // Handle key press here
    if (pressedKeys.find(XK_w) != pressedKeys.end()) {
        playerY--;
    }
    if (pressedKeys.find(XK_s) != pressedKeys.end()) {
        playerY++;
    }
    if (pressedKeys.find(XK_a) != pressedKeys.end()) {
        playerX--;
    }
    if (pressedKeys.find(XK_d) != pressedKeys.end()) {
        playerX++;
    }
    std::cout << "Mouse at (" << mouseX << ", " << mouseY << ")" << std::endl;
    std::cout << "Player at (" << playerX << ", " << playerY << ")" << std::endl;
}

void handleKeyRelease(KeySym key) {
    // Handle key release here
}

void handlePointerMotion(XEvent& event) {
    // Extract cursor position
    mouseX = event.xmotion.x_root;
    mouseY = event.xmotion.y_root;
    std::cout << "Mouse at (" << mouseX << ", " << mouseY << ")" << std::endl;
}

void closeX() {
    XCloseDisplay(display);
}

void initializeX() {
    display = XOpenDisplay(nullptr);
    if (!display) {
        std::cerr << "Error: Could not open X display." << std::endl;
        exit(1);
    }

    
    screen = DefaultScreen(display);
    root = RootWindow(display, screen);
    gc = XCreateGC(display, root, 0, nullptr);

    // Create a window to capture input events
    XSetWindowAttributes swa;
    swa.event_mask = KeyPressMask | KeyReleaseMask | PointerMotionMask | ButtonPressMask | ButtonReleaseMask;
    win = XCreateWindow(display, root, 0, 0, 600, 600, 0, DefaultDepth(display, screen), InputOutput, DefaultVisual(display, screen), CWEventMask, &swa);

    XSetForeground(display, gc, WhitePixel(display, screen));

    XFontStruct* font = XLoadQueryFont(display, "fixed");

    if (!font) {
        std::cerr << "Error: Could not load font." << std::endl;
        XCloseDisplay(display);
        return;
    }

    XMapWindow(display, win);
    XFlush(display);
}

int main() {
    initializeX();
    
    XEvent event;

    while (true) {
        

        XNextEvent(display, &event);

        if (event.type == KeyPress) {
            KeySym key = XLookupKeysym(&event.xkey, 0);
            pressedKeys.insert(key);
            handleKeyPress(key);
        } else if (event.type == KeyRelease) {
            KeySym key = XLookupKeysym(&event.xkey, 0);
            pressedKeys.erase(key);
            handleKeyRelease(key);
        } else if (event.type == MotionNotify){
            handlePointerMotion(event);
        }

        // Handle other events as needed
        XDrawString(display, win, gc, 0, 20, "Hello, X11!", strlen("Hello, X11!"));
        XFlush(display);
    }

    return 0;
}
