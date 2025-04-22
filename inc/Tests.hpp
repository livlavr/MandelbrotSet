#ifndef TESTS_HPP_
#define TESTS_HPP_

#include <unistd.h>
#include "App.hpp"

//=================================================================//
//============================FUNCTIONS============================//
//=================================================================//

static inline uint64_t rdtsc(void);
ErrorType TestsRun(sf::RenderWindow* window, sf::Vertex* pixels_array, const WindowParameters* window_parameters, Mandelbrot* mandelbrot_context, renderFunction* render_function);

#endif
