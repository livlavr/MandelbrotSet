#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <SFML/Graphics.hpp>

#include "App.hpp"
#include "Mandelbrot.hpp"

void appRun(sf::RenderWindow* window, sf::Vertex* PixelsArray, WindowParameters* window_parameters, ViewParameters* view_parameters) {
    sf::Clock clock;

    sf::Font font("../common/Arial.ttf");
    sf::Text text(font);
    text.setCharacterSize(24);
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
                    view_parameters->x_shift -= window_parameters->MOVING_SPEED * window_parameters->REAL_WIDTH * view_parameters->scale;
                } else if (keyPressed->scancode == sf::Keyboard::Scancode::L) {
                    view_parameters->x_shift += window_parameters->MOVING_SPEED * window_parameters->REAL_WIDTH * view_parameters->scale;
                } else if (keyPressed->scancode == sf::Keyboard::Scancode::J) {
                    view_parameters->y_shift -= window_parameters->MOVING_SPEED * window_parameters->REAL_HEIGHT * view_parameters->scale;
                } else if (keyPressed->scancode == sf::Keyboard::Scancode::K) {
                    view_parameters->y_shift += window_parameters->MOVING_SPEED * window_parameters->REAL_HEIGHT * view_parameters->scale;
                }
                else if (keyPressed->scancode == sf::Keyboard::Scancode::W) {
                    view_parameters->scale   -= window_parameters->SCALE_DEGREE * view_parameters->scale;
                } else if (keyPressed->scancode == sf::Keyboard::Scancode::S) {
                    view_parameters->scale   += window_parameters->SCALE_DEGREE * view_parameters->scale;
                }
            }

            else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                sf::Vector2i localPosition = sf::Mouse::getPosition(*window);
                int x = localPosition.x;
                int y = localPosition.y;
                printf("<%d | %d>\n", x, y);
            }
        }
        // draw it
        window->clear();
        drawMandelbrotDefault(PixelsArray, window_parameters, view_parameters);

        float current_time = clock.restart().asSeconds();
        float fps = 1.0f / current_time;

        snprintf(fps_string, 15, "FPS: %1.f\n", fps);
        text.setString(fps_string);

        window->draw(PixelsArray, window_parameters->WINDOW_SIZE, sf::PrimitiveType::Points);
        window->draw(text);
        window->display();
    }
}