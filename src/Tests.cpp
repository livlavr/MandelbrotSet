#include <stdio.h>
#include <string.h>
#include <SFML/Graphics.hpp>
#include <getopt.h>
#include <time.h>
#include <mach/mach_time.h>

#include "custom_asserts.h"
#include "Tests.hpp"
#include "Mandelbrot.hpp"

static inline uint64_t rdtsc(void) {
    uint64_t time_value = 0;
    asm volatile("mrs %0, cntvct_el0" : "=r" (time_value));

    return time_value;
}

ErrorType TestsRun(sf::RenderWindow* window, sf::Vertex* pixels_array, const WindowParameters* window_parameters, Mandelbrot* mandelbrot_context, renderFunction* render_function) {
    warning(mandelbrot_context, NULL_PTR_ERROR);
    warning(window_parameters,  NULL_PTR_ERROR);
    warning(pixels_array,       NULL_PTR_ERROR);
    warning(window,             NULL_PTR_ERROR);

    uint64_t mstart = 0;
    uint64_t mend   = 0;
    uint64_t mtotal = 0;
    for(size_t tests_count = 0; tests_count < (size_t)mandelbrot_context->test; tests_count++) {
        mstart = mach_absolute_time();

        render_function(pixels_array, window_parameters, mandelbrot_context);

        mend    = mach_absolute_time();
        mtotal += (mend - mstart);
    }

    mach_timebase_info_data_t timebase;
    mach_timebase_info(&timebase);

    double nanos = (double)mtotal * timebase.numer / (timebase.denom * mandelbrot_context->test);
    printf("<%0.f>\n", nanos);

    return SUCCESS;
}
