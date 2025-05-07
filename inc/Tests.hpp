#ifndef TESTS_HPP_
#define TESTS_HPP_

#include <unistd.h>
#include "App.hpp"

static const size_t WARM_UP_TESTS_COUNT = 100;

//=================================================================//
//============================FUNCTIONS============================//
//=================================================================//

static inline uint64_t rdtsc(void);
ErrorType TestsRun(sf::RenderWindow* window, sf::Vertex* pixels_array, const WindowParameters* window_parameters, Mandelbrot* mandelbrot_context);

#endif
