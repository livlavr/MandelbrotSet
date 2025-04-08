#ifndef APP_HPP_
#define APP_HPP_

static const double MOVING_SPEED  = 0.1;
static const double SCALE_DEGREE  = 0.1;
static const int    WINDOW_WIDTH  = 300;
static const int    WINDOW_HEIGHT = 200;
static const double REAL_HEIGHT = 2.0;
static const double REAL_WIDTH  = 3.0;
static       int    WINDOW_SIZE   = WINDOW_WIDTH * WINDOW_HEIGHT;

struct Coordinates {
    double x = 0;
    double y = 0;
};

struct ViewParameters {
    Coordinates top_left_angle  = {.x = -2, .y = 1};
    // Coordinates bot_right_angle = {.x = (double)WINDOW_WIDTH, .y = (double)WINDOW_HEIGHT};
    double      x_shift = 0;
    double      y_shift = 0;
    double      scale   = 1;
};

void appRun(sf::RenderWindow* window, sf::Vertex* PixelsArray, ViewParameters* view_parameters);

#endif