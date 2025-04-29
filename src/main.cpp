#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <SFML/Graphics.hpp>

#include "App.hpp"
#include "Tests.hpp"
#include "Mandelbrot.hpp"

int main(int argc, char** argv){
    //Calloc context structures
    WindowParameters* window_parameters = (WindowParameters*)calloc(1, sizeof(WindowParameters));
    Mandelbrot* mandelbrot_context = (Mandelbrot*)calloc(1, sizeof(Mandelbrot));

    //Set default context
    windowCtr(window_parameters);
    mandelbrotCtr(mandelbrot_context);

    //Parse flags
    parseFlags(argc, argv, mandelbrot_context);

    // create the window
    sf::RenderWindow window(sf::VideoMode({window_parameters->window_width, window_parameters->window_height}), "Mandelbrot Set");

    // create a triangle strip
    sf::Vertex* pixels_array = (sf::Vertex*)calloc(window_parameters->window_size, sizeof(sf::Vertex));

    if(mandelbrot_context->test) {
        TestsRun(&window, pixels_array, window_parameters, mandelbrot_context);
    } else {
        appRun(&window, pixels_array, window_parameters, mandelbrot_context);
    }
}
