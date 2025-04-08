#ifndef MANDELBROT_HPP_
#define MANDELBROT_HPP_

static const double MAX_RADIUS = 3.0;
static const int    MAX_ITERATIONS_COUNT = 512;
static const double REAL_HEIGHT = 2.0;
static const double REAL_WIDTH  = 3.0;

double    drawMandelbrotDefault(sf::Vertex* PixelsArray, ViewParameters* view_parameters);
sf::Color getPixelColorDefault (double x, double y);

#endif