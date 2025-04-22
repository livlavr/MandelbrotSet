#ifndef APP_HPP_
#define APP_HPP_

#include "ErrorTypes.hpp"

//=================================================================//
//==============================DATA===============================//
//=================================================================//

static const int   FPS_STRING_LEN = 15;
static const int   FONT_SIZE      = 24;
static const char* FONT_PATH      = "../Common/Arial.ttf";

static const int GRAPHICS_DEFAULT_VALUE = 1;   //Flags default values
static const int RUNS_DEFAULT_VALUE     = 1;
static const int TEST_DEFAULT_VALUE     = 0;


static const int YES                    = 1;
static const int NO                     = 0;

static const double WINDOW_WIDTH  = 1000;
static const double WINDOW_HEIGHT = 800;
static const double WINDOW_SIZE   = WINDOW_WIDTH * WINDOW_HEIGHT;
static const double MOVING_SPEED  = 0.1;
static const double SCALE_DEGREE  = 0.1;
static const double REAL_HEIGHT   = 2.0;
static const double REAL_WIDTH    = 3.0;

//=================================================================//
//=============================STRUCTS=============================//
//=================================================================//

struct Coordinates {
    double x = 0;
    double y = 0;
};

struct WindowParameters {
    unsigned int window_width  = 0;
    unsigned int window_height = 0;
    unsigned int window_size   = 0;
    double       moving_speed  = 0;
    double       scale_degree  = 0;
    double       real_height   = 0;
    double       real_width    = 0;
};

struct Mandelbrot {
    Coordinates top_left_angle = {};
    double x_shift  = 0;
    double y_shift  = 0;
    double scale    = 0;
    int    runs     = 0;
    int    test     = 0;
    int    graphics = 0;
};

//=================================================t================//
//============================FUNCTIONS============================//
//=================================================================//

typedef ErrorType (renderFunction) (sf::Vertex*, const WindowParameters*, Mandelbrot*);

ErrorType parseFlags    (int argc, char** argv, Mandelbrot* mandelbrot_context);
ErrorType windowCtr     (WindowParameters* window_parameters);
ErrorType appRun        (sf::RenderWindow* window, sf::Vertex* pixels_array, const WindowParameters* window_parameters, Mandelbrot* mandelbrot_context);
#endif
