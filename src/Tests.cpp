#include <stdio.h>
#include <string.h>
#include <SFML/Graphics.hpp>
#include <getopt.h>

#include "Tests.hpp"

uint64_t rdtsc(void)
{
    uint64_t time_value;
    asm volatile("mrs %0, cntvct_el0" : "=r" (time_value));
    return time_value;
}

void TestsRun(sf::RenderWindow* window, sf::Vertex* pixels_array, WindowParameters* window_parameters, Mandelbrot* mandelbrot) {
    printf("MYAU\n");
}
