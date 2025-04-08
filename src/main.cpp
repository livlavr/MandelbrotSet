#include <stdio.h>
#include <stdlib.h>
#include <SFML/Graphics.hpp>

#include "App.hpp"
#include "Mandelbrot.hpp"

int main()
{
//set default window parameters
ViewParameters view_parameters = {};

// create the window
sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Mandelbrot Set");

// create a triangle strip
sf::Vertex* PixelsArray = (sf::Vertex*)calloc(WINDOW_SIZE, sizeof(sf::Vertex));

appRun(&window, PixelsArray, &view_parameters);

}