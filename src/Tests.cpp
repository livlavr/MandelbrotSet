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

ErrorType TestsRun(sf::RenderWindow* window, sf::Vertex* pixels_array, const WindowParameters* window_parameters, Mandelbrot* mandelbrot_context) {
    warning(mandelbrot_context, NULL_PTR_ERROR);
    warning(window_parameters,  NULL_PTR_ERROR);
    warning(pixels_array,       NULL_PTR_ERROR);
    warning(window,             NULL_PTR_ERROR);

    uint64_t mstart = 0;
    uint64_t mend   = 0;
    uint64_t mtotal = 0;
    for(size_t tests_count = 0; tests_count < (size_t)mandelbrot_context->test; tests_count++) {
        mstart = mach_absolute_time();

        RENDER_FUNCTION(pixels_array, window_parameters, mandelbrot_context);

        mend    = mach_absolute_time();
        mtotal += (mend - mstart);
    }

    mach_timebase_info_data_t timebase;
    mach_timebase_info(&timebase);

    double nanos = (double)mtotal * timebase.numer / (timebase.denom * mandelbrot_context->test);
    printf("<%0.f>\n", nanos);

    return SUCCESS;
}

ErrorType parseFlags(int argc, char** argv, Mandelbrot* mandelbrot_context) {
    warning(mandelbrot_context, NULL_PTR_ERROR);
    warning(argv,       NULL_PTR_ERROR);

    const char* short_options = "r::t::";

    const struct option long_options[] = {
        {"graphics",    no_argument, &(mandelbrot_context->graphics), YES},
        {"no-graphics", no_argument, &(mandelbrot_context->graphics), NO},
        {"runs",        required_argument, NULL, 'r'},
        {"test",        required_argument, NULL, 't'},
        {"native",      no_argument, &(mandelbrot_context->render_function), NATIVE},
        {"unroll",      no_argument, &(mandelbrot_context->render_function), UNROLL},
        {"simd",        no_argument, &(mandelbrot_context->render_function), SIMD},
        {NULL, 0, NULL, 0}
    };

    int rez = 0;

	while((rez = getopt_long(argc, argv, short_options, long_options, NULL)) != -1){
        switch(rez) {
            case 'r':
                if(optarg != NULL) {
                    mandelbrot_context->runs = atoi(optarg);
                } else {
                    mandelbrot_context->runs = RUNS_DEFAULT_VALUE;
                }
                break;
            case 't':
                if(optarg != NULL) {
                    mandelbrot_context->test = atoi(optarg);
                } else {
                    mandelbrot_context->test = TEST_DEFAULT_VALUE;
                }
                break;
            default:
                break;
        }
    }

    color_printf(GREEN_COLOR, REGULAR, "[\tTEST         = %d\t]\n", mandelbrot_context->test);
    color_printf(GREEN_COLOR, REGULAR, "[\tRUNS         = %d\t]\n", mandelbrot_context->runs);
    color_printf(GREEN_COLOR, REGULAR, "[\tRENDER       = %d\t]\n", mandelbrot_context->render_function);
    color_printf(GREEN_COLOR, REGULAR, "[\tUNROLL_LEVEL = %d\t]\n", UNROLL_LEVEL);

    return SUCCESS;
}

