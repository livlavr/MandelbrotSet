#include <stdio.h>
#include <stdlib.h>
#include <SFML/Graphics.hpp>

#include "App.hpp"
#include "Mandelbrot.hpp"

int main(){
//set default window parameters
WindowParameters window_parameters = {};

//set default view parameters
ViewParameters view_parameters = {};

// create the window
sf::RenderWindow window(sf::VideoMode({window_parameters.WINDOW_WIDTH, window_parameters.WINDOW_HEIGHT}), "Mandelbrot Set");

// create a triangle strip
sf::Vertex* PixelsArray = (sf::Vertex*)calloc(window_parameters.WINDOW_SIZE, sizeof(sf::Vertex));

appRun(&window, PixelsArray, &window_parameters, &view_parameters);

}