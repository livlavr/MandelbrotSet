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

    uint64_t start_ticks = 0;
    uint64_t end_ticks   = 0;
    uint64_t total_ticks = 0;
    for(size_t tests_count = 0; tests_count < WARM_UP_TESTS_COUNT; tests_count++) {
        RENDER_FUNCTION(pixels_array, window_parameters, mandelbrot_context);
    }

    for(size_t tests_count = 0; tests_count < (size_t)mandelbrot_context->test; tests_count++) {
        start_ticks = rdtsc();

        RENDER_FUNCTION(pixels_array, window_parameters, mandelbrot_context);

        end_ticks    = rdtsc();
        total_ticks = (end_ticks - start_ticks);
        printf("%lu %llu\n", tests_count, total_ticks);
    }

    return SUCCESS;
}

ErrorType parseFlags(int argc, char** argv, Mandelbrot* mandelbrot_context) {
    warning(mandelbrot_context, NULL_PTR_ERROR);
    warning(argv,       NULL_PTR_ERROR);

    const char* short_options = "t::";

    const struct option long_options[] = {
        {"graphics",    no_argument, &(mandelbrot_context->graphics), YES},
        {"no-graphics", no_argument, &(mandelbrot_context->graphics), NO},
        {"test",        required_argument, NULL, 't'},
        {"native",      no_argument, &(mandelbrot_context->render_function), NATIVE},
        {"unroll",      no_argument, &(mandelbrot_context->render_function), UNROLL},
        {"simd",        no_argument, &(mandelbrot_context->render_function), SIMD},
        {NULL, 0, NULL, 0}
    };

    int rez = 0;

	while((rez = getopt_long(argc, argv, short_options, long_options, NULL)) != -1){
        switch(rez) {
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

    // color_printf(GREEN_COLOR, REGULAR, "[\tTEST         = %d\t]\n", mandelbrot_context->test);
    // color_printf(GREEN_COLOR, REGULAR, "[\tRENDER       = %d\t]\n", mandelbrot_context->render_function);
    // color_printf(GREEN_COLOR, REGULAR, "[\tUNROLL_LEVEL = %d\t]\n", UNROLL_LEVEL);

    return SUCCESS;
}

