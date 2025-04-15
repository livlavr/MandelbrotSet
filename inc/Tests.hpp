#ifndef TESTS_HPP_
#define TESTS_HPP_

#include <unistd.h>
#include "App.hpp"

uint64_t rdtsc   (void);
void     TestsRun(sf::RenderWindow* window, sf::Vertex* pixels_array, WindowParameters* window_parameters, Mandelbrot* mandelbrot);

#endif
