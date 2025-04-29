#ifndef MANDELBROT_HPP_
#define MANDELBROT_HPP_

#include <arm_neon.h>
#include "ErrorTypes.hpp"

//=================================================================//
//==============================DATA===============================//
//=================================================================//

//Flags default values:
static const int RENDER_FUNCTION_DEFAULT_VALUE = 0;
static const int GRAPHICS_DEFAULT_VALUE        = 1;
static const int RUNS_DEFAULT_VALUE            = 1;
static const int TEST_DEFAULT_VALUE            = 0;

//Yes & No
static const int YES = 1;
static const int NO  = 0;

//Render functions
static const int NATIVE = 0;
static const int UNROLL = 1;
static const int SIMD   = 2;

//Define needs to pass this variable from enviroment
static const double MAX_RADIUS                 = 3.0;
static const int    MAX_ITERATIONS_COUNT       = 256;
static const int    ARM_NEON_ITERATIONS_COUNT  = 2; //Don't change, it'll kill you!

struct Coordinates {
    double x = 0;
    double y = 0;
};

struct Mandelbrot {
    Coordinates top_left_angle = {};
    double x_shift  = 0;
    double y_shift  = 0;
    double scale    = 0;
    int    runs     = 0;
    int    test     = 0;
    int    graphics = 0;
    int    render_function = 0;
};

//=================================================================//
//============================FUNCTIONS============================//
//=================================================================//
ErrorType parseFlags(int argc, char** argv, Mandelbrot* mandelbrot_context);

ErrorType renderMandelbrotDefault  (sf::Vertex* pixels_array, const WindowParameters* window_parameters, Mandelbrot* mandelbrot_context);
ErrorType renderMandelbrotOptimized(sf::Vertex* pixels_array, const WindowParameters* window_parameters, Mandelbrot* mandelbrot_context);
ErrorType renderMandelbrotArmNeon  (sf::Vertex* pixels_array, const WindowParameters* window_parameters, Mandelbrot* mandelbrot_context);

ErrorType mandelbrotCtr            (Mandelbrot* mandelbrot_context);
sf::Color getPixelColorDefault     (double x, double y);
ErrorType getPixelColorOptimized   (double* x_coords, double* y_coords, sf::Color* colors);
ErrorType getPixelColorArmNeon     (float64x2_t* x_coords_ptr, float64x2_t* y_coords_ptr, sf::Color* colors);

typedef ErrorType (*renderFunction) (sf::Vertex*, const WindowParameters*, Mandelbrot*);
static renderFunction render_funciton_type[3] = {renderMandelbrotDefault, renderMandelbrotOptimized, renderMandelbrotArmNeon};

#define RENDER_FUNCTION render_funciton_type[mandelbrot_context->render_function]

#endif
