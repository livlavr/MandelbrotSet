#ifndef MANDELBROT_HPP_
#define MANDELBROT_HPP_

static const double MAX_RADIUS = 3.0;
static const int    MAX_ITERATIONS_COUNT = 500;

double    drawMandelbrotDefault(sf::Vertex* pixels_array, WindowParameters* window_parameters, Mandelbrot* mandelbrot);
sf::Color getPixelColorDefault (double x, double y);

#endif
