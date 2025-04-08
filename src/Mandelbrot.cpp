#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <SFML/Graphics.hpp>

#include "App.hpp"
#include "Mandelbrot.hpp"

double drawMandelbrotDefault(sf::Vertex* PixelsArray, ViewParameters* view_parameters) {
    int index = 0;
    for(int window_x = 0; window_x < WINDOW_WIDTH; window_x++) {
        for(int window_y = 0; window_y < WINDOW_HEIGHT; window_y++) {
            // double real_x = (view_parameters->top_left_angle.x + ((double)window_x)/WINDOW_WIDTH  * REAL_WIDTH ) * view_parameters->scale + view_parameters->x_shift;
            // double real_y = (view_parameters->top_left_angle.y - ((double)window_y)/WINDOW_HEIGHT * REAL_HEIGHT) * view_parameters->scale + view_parameters->y_shift;
            // PixelsArray[index].color = getPixelColorDefault(real_x, real_y);
            // PixelsArray[index].position = sf::Vector2f((double)window_x, (double)window_y);
            // index++;

            double real_x = (view_parameters->top_left_angle.x + ((double)window_x)/WINDOW_WIDTH  * REAL_WIDTH ) * view_parameters->scale + view_parameters->x_shift;
            double real_y = (view_parameters->top_left_angle.y - ((double)window_y)/WINDOW_HEIGHT * REAL_HEIGHT) * view_parameters->scale + view_parameters->y_shift;
            PixelsArray[index].color = getPixelColorDefault(real_x, real_y);
            PixelsArray[index].position = sf::Vector2f((double)window_x, (double)window_y);
            index++;
        }
    }
    return 0;
}

sf::Color getPixelColorDefault(double x, double y) {
    double radius_vector = 0, x0 = x, y0 = y;
    int counter = 0;

    while(radius_vector < MAX_RADIUS && counter < MAX_ITERATIONS_COUNT) {
        double xn = x, yn = y;
        x = xn*xn - yn*yn + x0;
        y = 2*xn*yn + y0;

        radius_vector = sqrt(x*x + y*y);

        counter++;
    }

    if(counter >= MAX_ITERATIONS_COUNT) {
        return sf::Color::Black;
    } else {
        return sf::Color::White;
    }

    return sf::Color::White;
}