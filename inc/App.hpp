#ifndef APP_HPP_
#define APP_HPP_

#include "ErrorTypes.hpp"

static const int   FPS_STRING_LEN = 15;
static const int   FONT_SIZE      = 24;
static const char* FONT_PATH = "../common/Arial.ttf";

static const int GRAPHICS_DEFAULT_VALUE = 1;   //Flags default values
static const int RUNS_DEFAULT_VALUE     = 10;
static const int TEST_DEFAULT_VALUE     = 0;

struct Coordinates {
    double x = 0;
    double y = 0;
};

struct WindowParameters {
    const unsigned int WINDOW_WIDTH  = 800;
    const unsigned int WINDOW_HEIGHT = 800;
    const unsigned int WINDOW_SIZE   = WINDOW_WIDTH * WINDOW_HEIGHT;
    const double       MOVING_SPEED = 0.1;
    const double       SCALE_DEGREE = 0.1;
    const double       REAL_HEIGHT  = 2.0;
    const double       REAL_WIDTH   = 3.0;
};

struct Mandelbrot {
    Coordinates top_left_angle  = {.x = -2, .y = 1};
    double x_shift  = 0;
    double y_shift  = 0;
    double scale    = 1;
    int    runs     = RUNS_DEFAULT_VALUE;
    int    test     = TEST_DEFAULT_VALUE;
    int    graphics = GRAPHICS_DEFAULT_VALUE;
};

ErrorType appRun(sf::RenderWindow* window, sf::Vertex* pixels_array, WindowParameters* window_parameters, Mandelbrot* mandelbrot);
ErrorType parseFlags(int argc, char** argv, Mandelbrot* mandelbrot);
ErrorType appCtr(WindowParameters* window_parameters, Mandelbrot* mandelbrot);

#endif
