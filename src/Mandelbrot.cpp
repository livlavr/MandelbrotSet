#include <cmath>
#include <time.h>
#include <SFML/Graphics.hpp>

#include "custom_asserts.h"
#include "App.hpp"
#include "Mandelbrot.hpp"
#include "Tests.hpp"
#include "ErrorTypes.hpp"

ErrorType mandelbrotCtr(Mandelbrot* mandelbrot_context) {

    mandelbrot_context->top_left_angle.x = -2;
    mandelbrot_context->top_left_angle.y = 1;
    mandelbrot_context->x_shift  = 0;
    mandelbrot_context->y_shift  = 0;
    mandelbrot_context->scale    = 1;
    mandelbrot_context->runs     = RUNS_DEFAULT_VALUE;
    mandelbrot_context->test     = TEST_DEFAULT_VALUE;
    mandelbrot_context->graphics = GRAPHICS_DEFAULT_VALUE;

    return SUCCESS;
}

ErrorType renderMandelbrotDefault(sf::Vertex* pixels_array, const WindowParameters* window_parameters, Mandelbrot* mandelbrot_context) {
    int index = 0;

    unsigned int WINDOW_WIDTH  = window_parameters->window_width;
    unsigned int WINDOW_HEIGHT = window_parameters->window_height;
    double       REAL_WIDTH    = window_parameters->real_width;
    double       REAL_HEIGHT   = window_parameters->real_height;
    double center_x = mandelbrot_context->top_left_angle.x + 0.5 * REAL_WIDTH  + mandelbrot_context->x_shift;
    double center_y = mandelbrot_context->top_left_angle.y - 0.5 * REAL_HEIGHT + mandelbrot_context->y_shift;
    double scale    = mandelbrot_context->scale;

    for (size_t window_x = 0; window_x < (size_t)WINDOW_WIDTH; window_x++) {
        for (size_t window_y = 0; window_y < (size_t)WINDOW_HEIGHT; window_y++) {
            double offset_x = ((double)window_x - WINDOW_WIDTH  / 2.0) / WINDOW_WIDTH;
            double offset_y = ((double)window_y - WINDOW_HEIGHT / 2.0) / WINDOW_HEIGHT;

            double real_x = center_x + offset_x * REAL_WIDTH * scale;
            double real_y = center_y + offset_y * REAL_HEIGHT * scale;

            pixels_array[index].color    = getPixelColorDefault(real_x, real_y);
            pixels_array[index].position = sf::Vector2f((double)window_x, (double)window_y);
            index++;
        }
    }

    return SUCCESS;
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
        uint8_t r = (8 * counter) % 255;
        uint8_t g = (1 * counter) % 255;
        uint8_t b = (4 * counter) % 255;

        sf::Color color{r, g, b, 255};

        return color;
    }

    return sf::Color::White;
}

ErrorType renderMandelbrotOptimized(sf::Vertex* pixels_array, const WindowParameters* window_parameters, Mandelbrot* mandelbrot_context) {
    size_t point_index = 0;
    int    points_render_flag = 0;

    double center_x    = mandelbrot_context->top_left_angle.x + 0.5 * REAL_WIDTH  + mandelbrot_context->x_shift;
    double center_y    = mandelbrot_context->top_left_angle.y - 0.5 * REAL_HEIGHT + mandelbrot_context->y_shift;
    double scale       = mandelbrot_context->scale;
    double half_height = WINDOW_HEIGHT / 2.0;
    double half_width  = WINDOW_WIDTH  / 2.0;

    Coordinates* result_coords = (Coordinates*)calloc(WINDOW_SIZE, sizeof(double));
    Coordinates  coords[UNROLL_LEVEL] = {};
    sf::Vector2f position   [UNROLL_LEVEL] = {};
    sf::Color    colors     [UNROLL_LEVEL] = {};

    size_t window_x = 0;
    size_t window_y = 0;

    for (window_x = 0; window_x < (size_t)WINDOW_WIDTH; window_x++) {
        for (window_y = 0; window_y < (size_t)WINDOW_HEIGHT; window_y += UNROLL_LEVEL) {
            for(size_t i = 0; i < UNROLL_LEVEL; i++) {
                coords[i].x = (double)window_x;
            }
            for(size_t i = 0; i < UNROLL_LEVEL; i++) {
                coords[i].x -= half_width;
            }
            for(size_t i = 0; i < UNROLL_LEVEL; i++) {
                coords[i].x /= WINDOW_WIDTH;
            }
            for(size_t i = 0; i < UNROLL_LEVEL; i++) {
                coords[i].x *= REAL_WIDTH * scale;
            }
            for(size_t i = 0; i < UNROLL_LEVEL; i++) {
                coords[i].x += center_x;
            }

            for(size_t i = 0; i < UNROLL_LEVEL; i++) {
                coords[i].y = (double)(window_y + i);
            }
            for(size_t i = 0; i < UNROLL_LEVEL; i++) {
                coords[i].y -= half_height;
            }
            for(size_t i = 0; i < UNROLL_LEVEL; i++) {
                coords[i].y /= WINDOW_HEIGHT;
            }
            for(size_t i = 0; i < UNROLL_LEVEL; i++) {
                coords[i].y *= REAL_HEIGHT * scale;
            }
            for(size_t i = 0; i < UNROLL_LEVEL; i++) {
                coords[i].y += center_y;
            }

            for(size_t i = 0; i < UNROLL_LEVEL; i++) {
                result_coords[point_index + i].x = coords[i].x;
            }
            for(size_t i = 0; i < UNROLL_LEVEL; i++) {
                result_coords[point_index + i].y = coords[i].y;
            }

            getPixelColorOptimized(coords, colors);
            for(size_t i = 0; i < UNROLL_LEVEL; i++) {
                pixels_array[point_index + i].color = colors[i];
            }
            for(size_t i = 0; i < UNROLL_LEVEL; i++) {
                pixels_array[point_index + i].position = sf::Vector2f((double)window_x, (double)(window_y + i));
            }

            point_index += UNROLL_LEVEL;
        }
    }

    FREE(result_coords);

    return SUCCESS;
}

ErrorType getPixelColorOptimized(Coordinates* coords, sf::Color* colors) {
    int counter[UNROLL_LEVEL] = {};
    int radius_vector_test = 1;
    Coordinates radius_vector_coords[UNROLL_LEVEL] = {};
    double xn[UNROLL_LEVEL] = {};
    double yn[UNROLL_LEVEL] = {};
    double radius_vector_length[UNROLL_LEVEL] = {};
    int    radius_vector_cmps[UNROLL_LEVEL] = {};
    bool   continue_color_rendering = true;

    for(size_t i = 0; i < UNROLL_LEVEL; i++) {
        radius_vector_coords[i].x = coords[i].x;
    }
    for(size_t i = 0; i < UNROLL_LEVEL; i++) {
        radius_vector_coords[i].y = coords[i].y;
    }

    while(radius_vector_test != 0 && continue_color_rendering) {
        for(size_t i = 0; i < UNROLL_LEVEL; i++) {
            xn[i] = radius_vector_coords[i].x;
        }
        for(size_t i = 0; i < UNROLL_LEVEL; i++) {
            yn[i] = radius_vector_coords[i].y;
        }

        for(size_t i = 0; i < UNROLL_LEVEL; i++) {
            radius_vector_coords[i].x = xn[i] * xn[i];
        }
        for(size_t i = 0; i < UNROLL_LEVEL; i++) {
            radius_vector_coords[i].x -= yn[i] * yn[i];
        }
        for(size_t i = 0; i < UNROLL_LEVEL; i++) {
            radius_vector_coords[i].x += coords[i].x;
        }

        for(size_t i = 0; i < UNROLL_LEVEL; i++) {
            radius_vector_coords[i].y = 2 * xn[i] * yn[i];
        }
        for(size_t i = 0; i < UNROLL_LEVEL; i++) {
            radius_vector_coords[i].y += coords[i].y;
        }

        for(size_t i = 0; i < UNROLL_LEVEL; i++) {
            radius_vector_length[i] = radius_vector_coords[i].x * radius_vector_coords[i].x;
        }
        for(size_t i = 0; i < UNROLL_LEVEL; i++) {
            radius_vector_length[i] += radius_vector_coords[i].y * radius_vector_coords[i].y;
        }
        for(size_t i = 0; i < UNROLL_LEVEL; i++) {
            radius_vector_cmps[i] = (sqrt(radius_vector_length[i]) < MAX_RADIUS);
        }

        radius_vector_test = 0;
        for(size_t i = 0; i < UNROLL_LEVEL; i++) {
            if(radius_vector_cmps[i] != 0) {
                radius_vector_test++;
                counter[i]++;
            }
        }

        continue_color_rendering = false;
        for(size_t i = 0; i < UNROLL_LEVEL; i++) {
            if(counter[i] < MAX_ITERATIONS_COUNT && radius_vector_cmps[i] != 0) {
                continue_color_rendering = true;
            }
        }

    }

    for(size_t i = 0; i < UNROLL_LEVEL; i++) {
        if(radius_vector_cmps[i] == 0) {
            uint8_t r = (8 * counter[i]) % 255;
            uint8_t g = (1 * counter[i]) % 255;
            uint8_t b = (4 * counter[i]) % 255;

            colors[i] = {r, g, b, 255};
        } else {
            colors[i] = sf::Color::Black;
        }
    }

    return SUCCESS;
}
