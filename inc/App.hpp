#ifndef APP_HPP_
#define APP_HPP_

static const int FPS_STRING_LEN = 15;

struct Coordinates {
    double x = 0;
    double y = 0;
};

struct WindowParameters {
    const unsigned int WINDOW_WIDTH  = 300;
    const unsigned int WINDOW_HEIGHT = 300;
    const double MOVING_SPEED = 0.1;
    const double SCALE_DEGREE = 0.1;
    const double REAL_HEIGHT  = 2.0;
    const double REAL_WIDTH   = 3.0;
    const int    WINDOW_SIZE  = WINDOW_WIDTH * WINDOW_HEIGHT;
};

struct ViewParameters {
    Coordinates top_left_angle  = {.x = -2, .y = 1};
    double      x_shift = 0;
    double      y_shift = 0;
    double      scale   = 1;
};

void appRun(sf::RenderWindow* window, sf::Vertex* PixelsArray, WindowParameters* window_parameters, ViewParameters* view_parameters);

#endif