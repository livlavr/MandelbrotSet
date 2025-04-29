#ifndef APP_HPP_
#define APP_HPP_

#include "ErrorTypes.hpp"

//=================================================================//
//==============================DATA===============================//
//=================================================================//

static const int   FPS_STRING_LEN = 15;
static const int   FONT_SIZE      = 24;
static const char* FONT_PATH      = "../Common/Arial.ttf";

//Window default values:
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

struct WindowParameters {
    double       moving_speed  = 0;
    double       scale_degree  = 0;
    double       real_height   = 0;
    double       real_width    = 0;
    unsigned int window_width  = 0;
    unsigned int window_height = 0;
    unsigned int window_size   = 0;
};

struct Mandelbrot;

//=================================================================//
//============================FUNCTIONS============================//
//=================================================================//

ErrorType windowCtr (WindowParameters* window_parameters);
ErrorType appRun    (sf::RenderWindow* window, sf::Vertex* pixels_array, const WindowParameters* window_parameters, Mandelbrot* mandelbrot_context);

#endif
