#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <stdlib.h>
#include <SFML/Graphics.hpp>
#include <mach/mach_time.h>
#include <getopt.h>

#include "custom_asserts.h"
#include "color_printf.h"
#include "App.hpp"
#include "Mandelbrot.hpp"
#include "ErrorTypes.hpp"

ErrorType windowCtr(WindowParameters* window_parameters) {
    window_parameters->window_width  = WINDOW_WIDTH;
    window_parameters->window_height = WINDOW_HEIGHT;
    window_parameters->window_size   = WINDOW_WIDTH * WINDOW_HEIGHT;
    window_parameters->moving_speed  = MOVING_SPEED;
    window_parameters->scale_degree  = SCALE_DEGREE;
    window_parameters->real_height   = REAL_HEIGHT;
    window_parameters->real_width    = REAL_WIDTH;

    return SUCCESS;
}

ErrorType appRun(sf::RenderWindow* window, sf::Vertex* pixels_array, const WindowParameters* window_parameters, Mandelbrot* mandelbrot_context) {
    warning(window_parameters, NULL_PTR_ERROR);
    warning(pixels_array,      NULL_PTR_ERROR);

    sf::Clock clock;

    sf::Font font(FONT_PATH);
    sf::Text text(font);
    text.setCharacterSize(FONT_SIZE);
    text.setFillColor(sf::Color::Red);
    text.setStyle(sf::Text::Bold);
    char* fps_string = (char*)calloc(FPS_STRING_LEN, sizeof(char));

    // run the main loop
    while (window->isOpen()) {
        // handle events
        while (const std::optional<sf::Event> event = window->pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window->close();
            }

            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                    window->close();
                }
                else if (keyPressed->scancode == sf::Keyboard::Scancode::A) {
                    mandelbrot_context->x_shift -= window_parameters->moving_speed * window_parameters->real_width * mandelbrot_context->scale;
                } else if (keyPressed->scancode == sf::Keyboard::Scancode::D) {
                    mandelbrot_context->x_shift += window_parameters->moving_speed * window_parameters->real_width * mandelbrot_context->scale;
                } else if (keyPressed->scancode == sf::Keyboard::Scancode::W) {
                    mandelbrot_context->y_shift -= window_parameters->moving_speed * window_parameters->real_height * mandelbrot_context->scale;
                } else if (keyPressed->scancode == sf::Keyboard::Scancode::S) {
                    mandelbrot_context->y_shift += window_parameters->moving_speed * window_parameters->real_height * mandelbrot_context->scale;
                }
                else if (keyPressed->scancode == sf::Keyboard::Scancode::K) {
                    mandelbrot_context->scale   -= window_parameters->scale_degree * mandelbrot_context->scale;
                } else if (keyPressed->scancode == sf::Keyboard::Scancode::J) {
                    mandelbrot_context->scale   += window_parameters->scale_degree * mandelbrot_context->scale;
                }
            }
        }

        // draw it
        window->clear();

        // renderMandelbrotDefault(pixels_array, window_parameters, mandelbrot_context);
        // renderMandelbrotOptimized(pixels_array, window_parameters, mandelbrot_context);
        renderMandelbrotArmNeon(pixels_array, window_parameters, mandelbrot_context);


        float current_time = clock.restart().asSeconds();
        float fps = 1.0f / current_time;

        snprintf(fps_string, FPS_STRING_LEN, "FPS: %1.f\n", fps);
        text.setString(fps_string);

        window->draw(pixels_array, window_parameters->window_size, sf::PrimitiveType::Points);
        window->draw(text);
        window->display();
    }

    return SUCCESS;
}

ErrorType parseFlags(int argc, char** argv, Mandelbrot* mandelbrot_context) {
    warning(mandelbrot_context, NULL_PTR_ERROR);
    warning(argv,       NULL_PTR_ERROR);

    const char* short_options = "r::t:";

    const struct option long_options[] = {
        {"graphics",    no_argument, &(mandelbrot_context->graphics), YES},
        {"no-graphics", no_argument, &(mandelbrot_context->graphics), NO},
        {"runs",        optional_argument, NULL, 'r'},
        {"test",        required_argument, NULL, 't'},
        {"native",      required_argument, NULL, 'n'},
        {"optimized",   required_argument, NULL, 'o'},
        {"simd",        required_argument, NULL, 's'},
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
            case 'n':
                if(optarg != NULL) {
                    mandelbrot_context->test = atoi(optarg);
                } else {
                    mandelbrot_context->test = TEST_DEFAULT_VALUE;
                }
                break;
            case 'o':
                if(optarg != NULL) {
                    mandelbrot_context->test = atoi(optarg);
                } else {
                    mandelbrot_context->test = TEST_DEFAULT_VALUE;
                }
                break;
            case 's':
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

    color_printf(GREEN_COLOR, REGULAR, "[ TEST = %d ]\n", mandelbrot_context->test);
    color_printf(GREEN_COLOR, REGULAR, "[ RUNS = %d ]\n", mandelbrot_context->runs);

    return SUCCESS;
}
