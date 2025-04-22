#ifndef MANDELBROT_HPP_
#define MANDELBROT_HPP_

#include "ErrorTypes.hpp"

//=================================================================//
//==============================DATA===============================//
//=================================================================//
static const double MAX_RADIUS           = 3.0;
static const int    MAX_ITERATIONS_COUNT = 256;
static const int    UNROLL_LEVEL         = 16;

//=================================================================//
//============================FUNCTIONS============================//
//=================================================================//
ErrorType mandelbrotCtr             (Mandelbrot* mandelbrot_context);
ErrorType renderMandelbrotOptimized (sf::Vertex* pixels_array, const WindowParameters* window_parameters, Mandelbrot* mandelbrot_context);
ErrorType renderMandelbrotDefault   (sf::Vertex* pixels_array, const WindowParameters* window_parameters, Mandelbrot* mandelbrot_context);
ErrorType getPixelColorOptimized         (Coordinates* coords, sf::Color* colors);
sf::Color getPixelColorDefault      (double x, double y);

#endif
