#ifndef MANDELBROT_HPP_
#define MANDELBROT_HPP_

#include <arm_neon.h>
#include "ErrorTypes.hpp"

//=================================================================//
//==============================DATA===============================//
//=================================================================//
static const double MAX_RADIUS                = 3.0;
static const int    MAX_ITERATIONS_COUNT      = 256;
static const int    UNROLL_LEVEL              = 2;
static const int    ARM_NEON_ITERATIONS_COUNT = 2; // Don't change!

//=================================================================//
//============================FUNCTIONS============================//
//=================================================================//
ErrorType renderMandelbrotDefault  (sf::Vertex* pixels_array, const WindowParameters* window_parameters, Mandelbrot* mandelbrot_context);
ErrorType renderMandelbrotOptimized(sf::Vertex* pixels_array, const WindowParameters* window_parameters, Mandelbrot* mandelbrot_context);
ErrorType renderMandelbrotArmNeon  (sf::Vertex* pixels_array, const WindowParameters* window_parameters, Mandelbrot* mandelbrot_context);

sf::Color getPixelColorDefault  (double x, double y);
ErrorType getPixelColorOptimized(double* x_coords, double* y_coords, sf::Color* colors);
ErrorType getPixelColorArmNeon  (float64x2_t* x_coords_ptr, float64x2_t* y_coords_ptr, sf::Color* colors);
ErrorType mandelbrotCtr         (Mandelbrot* mandelbrot_context);

#endif
