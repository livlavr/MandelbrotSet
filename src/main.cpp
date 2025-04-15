#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <SFML/Graphics.hpp>

#include "App.hpp"
#include "Tests.hpp"
#include "Mandelbrot.hpp"

int main(int argc, char** argv){
    //set default window parameters
    WindowParameters* window_parameters = (WindowParameters*)calloc(1, sizeof(WindowParameters));
    //set default view parameters
    Mandelbrot* mandelbrot = (Mandelbrot*)calloc(1, sizeof(Mandelbrot));
    appCtr(window_parameters, mandelbrot);

//     parseFlags(argc, argv, &mandelbrot);
//
//     // create the window
//     sf::RenderWindow window(sf::VideoMode({window_parameters->WINDOW_WIDTH, window_parameters->WINDOW_HEIGHT}), "Mandelbrot Set");
//
//     // create a triangle strip
//     sf::Vertex* pixels_array = (sf::Vertex*)calloc(window_parameters.WINDOW_SIZE, sizeof(sf::Vertex));
//
//     if(mandelbrot)
//     TestsRun(&window, pixels_array, &window_parameters, &mandelbrot);
//     appRun(&window, pixels_array, &window_parameters, &mandelbrot);
}
