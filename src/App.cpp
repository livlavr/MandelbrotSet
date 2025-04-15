#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <stdlib.h>
#include <SFML/Graphics.hpp>
#include <getopt.h>

#include "custom_asserts.h"
#include "color_printf.h"
#include "App.hpp"
#include "Mandelbrot.hpp"

ErrorType appCtr(WindowParameters* window_parameters, Mandelbrot* mandelbrot) {
    window_parameters->WINDOW_WIDTH  = 800;
    window_parameters->WINDOW_HEIGHT = 800;
    window_parameters->WINDOW_SIZE   = window_parameters->WINDOW_WIDTH * window_parameters->WINDOW_HEIGHT;
    window_parameters->MOVING_SPEED  = 0.1;
    window_parameters->SCALE_DEGREE  = 0.1;
    window_parameters->REAL_HEIGHT   = 2.0;
    window_parameters->REAL_WIDTH    = 3.0;

    return SUCCESS;
}

ErrorType appRun(sf::RenderWindow* window, sf::Vertex* pixels_array, WindowParameters* window_parameters, Mandelbrot* mandelbrot) {
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
        while (const std::optional event = window->pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window->close();
            }

            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                    window->close();
                }
                else if (keyPressed->scancode == sf::Keyboard::Scancode::H) {
                    mandelbrot->x_shift -= window_parameters->MOVING_SPEED * window_parameters->REAL_WIDTH * mandelbrot->scale;
                } else if (keyPressed->scancode == sf::Keyboard::Scancode::L) {
                    mandelbrot->x_shift += window_parameters->MOVING_SPEED * window_parameters->REAL_WIDTH * mandelbrot->scale;
                } else if (keyPressed->scancode == sf::Keyboard::Scancode::J) {
                    mandelbrot->y_shift -= window_parameters->MOVING_SPEED * window_parameters->REAL_HEIGHT * mandelbrot->scale;
                } else if (keyPressed->scancode == sf::Keyboard::Scancode::K) {
                    mandelbrot->y_shift += window_parameters->MOVING_SPEED * window_parameters->REAL_HEIGHT * mandelbrot->scale;
                }
                else if (keyPressed->scancode == sf::Keyboard::Scancode::W) {
                    mandelbrot->scale   -= window_parameters->SCALE_DEGREE * mandelbrot->scale;
                } else if (keyPressed->scancode == sf::Keyboard::Scancode::S) {
                    mandelbrot->scale   += window_parameters->SCALE_DEGREE * mandelbrot->scale;
                }
            }

            else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {     //TODO Delete or make it
                sf::Vector2i localPosition = sf::Mouse::getPosition(*window);
                int x = localPosition.x;
                int y = localPosition.y;
                printf("<%d | %d>\n", x, y);
            }
        }

        // draw it
        window->clear();
        drawMandelbrotDefault(pixels_array, window_parameters, mandelbrot);

        float current_time = clock.restart().asSeconds();
        float fps = 1.0f / current_time;

        snprintf(fps_string, FPS_STRING_LEN, "FPS: %1.f\n", fps);
        text.setString(fps_string);

        window->draw(pixels_array, window_parameters->WINDOW_SIZE, sf::PrimitiveType::Points);
        window->draw(text);
        window->display();
    }
}

ErrorType parseFlags(int argc, char** argv, Mandelbrot* mandelbrot) {
    warning(mandelbrot, NULL_PTR_ERROR);
    warning(argv,       NULL_PTR_ERROR);

    const char* short_options = "r::t:";

    const struct option long_options[] = {
        {"graphics",    no_argument, &(mandelbrot->graphics), 1},
        {"no-graphics", no_argument, &(mandelbrot->graphics), 0},
        {"runs",  optional_argument, NULL, 'r'},
        {"test",  required_argument, NULL, 't'},
        {NULL, 0, NULL, 0}
    };

    int rez = 0;

	while((rez = getopt_long(argc, argv, short_options, long_options, NULL)) != -1){
        switch(rez) {
            case 'r':
                if(optarg != NULL) {
                    mandelbrot->runs = atoi(optarg);
                } else {
                    mandelbrot->runs = RUNS_DEFAULT_VALUE;
                }
                break;
            case 't':
                if(optarg != NULL) {
                    mandelbrot->test = atoi(optarg);
                } else {
                    mandelbrot->test = TEST_DEFAULT_VALUE;
                }
                break;
            default:
                break;
        }
    }

    color_printf(GREEN_COLOR, REGULAR, "[ TEST = %d ]\n", mandelbrot->test);
    color_printf(GREEN_COLOR, REGULAR, "[ RUNS = %d ]\n", mandelbrot->runs);

    return SUCCESS;
}
