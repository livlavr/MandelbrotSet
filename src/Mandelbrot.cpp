#include <cmath>
#include <SFML/Graphics.hpp>

#include "App.hpp"
#include "Mandelbrot.hpp"

double drawMandelbrotDefault(sf::Vertex* PixelsArray, WindowParameters* window_parameters, ViewParameters* view_parameters) {
    int index = 0;
    double center_x = view_parameters->top_left_angle.x + 0.5 * window_parameters->REAL_WIDTH  + view_parameters->x_shift;
    double center_y = view_parameters->top_left_angle.y - 0.5 * window_parameters->REAL_HEIGHT + view_parameters->y_shift;
    double scale = view_parameters->scale;

    for (int window_x = 0; window_x < window_parameters->WINDOW_WIDTH; window_x++) {
        for (int window_y = 0; window_y < window_parameters->WINDOW_HEIGHT; window_y++) {
            double offset_x = ((double)window_x - window_parameters->WINDOW_WIDTH / 2.0) / window_parameters->WINDOW_WIDTH;
            double offset_y = ((double)window_y - window_parameters->WINDOW_HEIGHT / 2.0) / window_parameters->WINDOW_HEIGHT;

            double real_x = center_x + offset_x * window_parameters->REAL_WIDTH * scale;
            double real_y = center_y + offset_y * window_parameters->REAL_HEIGHT * scale;

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
