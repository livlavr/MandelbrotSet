#include <cmath>
#include <time.h>
#include <stdalign.h>
#include <SFML/Graphics.hpp>

#include "custom_asserts.h"
#include "App.hpp"
#include "Mandelbrot.hpp"
#include "Tests.hpp"
#include "ErrorTypes.hpp"

ErrorType mandelbrotCtr(Mandelbrot* mandelbrot_context) {
    mandelbrot_context->top_left_angle.x = -2;
    mandelbrot_context->top_left_angle.y = 1;
    mandelbrot_context->x_shift          = 0;
    mandelbrot_context->y_shift          = 0;
    mandelbrot_context->scale            = 1;
    mandelbrot_context->graphics         = GRAPHICS_DEFAULT_VALUE;
    mandelbrot_context->runs             = RUNS_DEFAULT_VALUE;
    mandelbrot_context->test             = TEST_DEFAULT_VALUE;
    mandelbrot_context->render_function  = RENDER_FUNCTION_DEFAULT_VALUE;

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

    alignas(16) double       x_coords   [UNROLL_LEVEL] = {};
    alignas(16) double       y_coords   [UNROLL_LEVEL] = {};
    alignas(16) sf::Vector2f position   [UNROLL_LEVEL] = {};
    alignas(16) sf::Color    colors     [UNROLL_LEVEL] = {};

    size_t window_x = 0;
    size_t window_y = 0;

    for (window_x = 0; window_x < (size_t)WINDOW_WIDTH; window_x++) {
        for (window_y = 0; window_y < (size_t)WINDOW_HEIGHT; window_y += UNROLL_LEVEL) {
            for(size_t i = 0; i < UNROLL_LEVEL; i++) {
                x_coords[i] = (double)window_x;
            }
            for(size_t i = 0; i < UNROLL_LEVEL; i++) {
                x_coords[i] -= half_width;
            }
            for(size_t i = 0; i < UNROLL_LEVEL; i++) {
                x_coords[i] /= WINDOW_WIDTH;
            }
            for(size_t i = 0; i < UNROLL_LEVEL; i++) {
                x_coords[i] *= REAL_WIDTH * scale;
            }
            for(size_t i = 0; i < UNROLL_LEVEL; i++) {
                x_coords[i] += center_x;
            }

            for(size_t i = 0; i < UNROLL_LEVEL; i++) {
                y_coords[i] = (double)(window_y + i);
            }
            for(size_t i = 0; i < UNROLL_LEVEL; i++) {
                y_coords[i] -= half_height;
            }
            for(size_t i = 0; i < UNROLL_LEVEL; i++) {
                y_coords[i] /= WINDOW_HEIGHT;
            }
            for(size_t i = 0; i < UNROLL_LEVEL; i++) {
                y_coords[i] *= REAL_HEIGHT * scale;
            }
            for(size_t i = 0; i < UNROLL_LEVEL; i++) {
                y_coords[i] += center_y;
            }

            getPixelColorOptimized(x_coords, y_coords, colors);
            for(size_t i = 0; i < UNROLL_LEVEL; i++) {
                pixels_array[point_index + i].color = colors[i];
            }
            for(size_t i = 0; i < UNROLL_LEVEL; i++) {
                pixels_array[point_index + i].position = sf::Vector2f((double)window_x, (double)(window_y + i));
            }

            point_index += UNROLL_LEVEL;
        }
    }

    return SUCCESS;
}

ErrorType getPixelColorOptimized(double* x_coords, double* y_coords, sf::Color* colors) {
    int  radius_vector_test = 1;
    bool continue_rendering = true;
    alignas(16) int counter[UNROLL_LEVEL] = {};
    alignas(16) double radius_vector_x_coords[UNROLL_LEVEL] = {};
    alignas(16) double radius_vector_y_coords[UNROLL_LEVEL] = {};
    alignas(16) double xn[UNROLL_LEVEL] = {};
    alignas(16) double yn[UNROLL_LEVEL] = {};
    alignas(16) double radius_vector_length[UNROLL_LEVEL] = {};
    alignas(16) int    radius_vector_cmps[UNROLL_LEVEL] = {};

    for(size_t i = 0; i < UNROLL_LEVEL; i++) {
        radius_vector_x_coords[i] = x_coords[i];
    }
    for(size_t i = 0; i < UNROLL_LEVEL; i++) {
        radius_vector_y_coords[i] = y_coords[i];
    }

    while(radius_vector_test != 0 && continue_rendering) {
        for(size_t i = 0; i < UNROLL_LEVEL; i++) {
            xn[i] = radius_vector_x_coords[i];
        }
        for(size_t i = 0; i < UNROLL_LEVEL; i++) {
            yn[i] = radius_vector_y_coords[i];
        }

        for(size_t i = 0; i < UNROLL_LEVEL; i++) {
            radius_vector_x_coords[i] = xn[i] * xn[i];
        }
        for(size_t i = 0; i < UNROLL_LEVEL; i++) {
            radius_vector_x_coords[i] -= yn[i] * yn[i];
        }
        for(size_t i = 0; i < UNROLL_LEVEL; i++) {
            radius_vector_x_coords[i] += x_coords[i];
        }

        for(size_t i = 0; i < UNROLL_LEVEL; i++) {
            radius_vector_y_coords[i] = 2 * xn[i] * yn[i];
        }
        for(size_t i = 0; i < UNROLL_LEVEL; i++) {
            radius_vector_y_coords[i] += y_coords[i];
        }

        for(size_t i = 0; i < UNROLL_LEVEL; i++) {
            radius_vector_length[i] = radius_vector_x_coords[i] * radius_vector_x_coords[i];
        }
        for(size_t i = 0; i < UNROLL_LEVEL; i++) {
            radius_vector_length[i] += radius_vector_y_coords[i] * radius_vector_y_coords[i];
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

        continue_rendering = false;
        for(size_t i = 0; i < UNROLL_LEVEL; i++) {
            if(counter[i] < MAX_ITERATIONS_COUNT && radius_vector_cmps[i] != 0) {
                continue_rendering = true;
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

ErrorType renderMandelbrotArmNeon(sf::Vertex* pixels_array, const WindowParameters* window_parameters, Mandelbrot* mandelbrot_context) {
    size_t point_index = 0;
    int    points_render_flag = 0;

    double* x_coords_array = (double*)calloc(2, sizeof(double));
    double* y_coords_array = (double*)calloc(2, sizeof(double));

    double center_x    = mandelbrot_context->top_left_angle.x + 0.5 * REAL_WIDTH  + mandelbrot_context->x_shift;
    double center_y    = mandelbrot_context->top_left_angle.y - 0.5 * REAL_HEIGHT + mandelbrot_context->y_shift;
    double scale       = mandelbrot_context->scale;
    alignas(16) float64x2_t scaled_real_width_v  = vmovq_n_f64((float64_t)(REAL_WIDTH * scale));
    alignas(16) float64x2_t scaled_real_height_v = vmovq_n_f64((float64_t)(REAL_HEIGHT * scale));
    alignas(16) float64x2_t window_width_v       = vmovq_n_f64((float64_t)WINDOW_WIDTH);
    alignas(16) float64x2_t window_height_v      = vmovq_n_f64((float64_t)WINDOW_HEIGHT);
    alignas(16) float64x2_t center_x_v           = vmovq_n_f64((float64_t)center_x);
    alignas(16) float64x2_t center_y_v           = vmovq_n_f64((float64_t)center_y);
    alignas(16) float64x2_t half_width_v         = vmovq_n_f64((float64_t)(WINDOW_WIDTH  / 2.0));
    alignas(16) float64x2_t half_height_v        = vmovq_n_f64((float64_t)(WINDOW_HEIGHT / 2.0));

    alignas(16) float64x2_t x_coords = {};
    alignas(16) float64x2_t y_coords = {};
    alignas(16) sf::Vector2f position   [ARM_NEON_ITERATIONS_COUNT] = {};
    alignas(16) sf::Color    colors     [ARM_NEON_ITERATIONS_COUNT] = {};

    size_t window_x = 0;
    size_t window_y = 0;

    for (window_x = 0; window_x < (size_t)WINDOW_WIDTH; window_x++) {
        for (window_y = 0; window_y < (size_t)WINDOW_HEIGHT; window_y += ARM_NEON_ITERATIONS_COUNT) {
            x_coords = vmovq_n_f64((float64_t)window_x);
            x_coords = vsubq_f64(x_coords, half_width_v);

            x_coords = vdivq_f64(x_coords, window_width_v);

            x_coords = vmulq_f64(x_coords, scaled_real_width_v);
            x_coords = vaddq_f64(x_coords, center_x_v);

            y_coords = vsetq_lane_f64((float64_t)(window_y), y_coords, 0);
            y_coords = vsetq_lane_f64((float64_t)(window_y + 1), y_coords, 1);

            y_coords = vsubq_f64(y_coords, half_height_v);
            y_coords = vdivq_f64(y_coords, window_height_v);
            y_coords = vmulq_f64(y_coords, scaled_real_height_v);
            y_coords = vaddq_f64(y_coords, center_y_v);

            getPixelColorArmNeon(&x_coords, &y_coords, colors);
            for(size_t i = 0; i < ARM_NEON_ITERATIONS_COUNT; i++) {
                pixels_array[point_index + i].color = colors[i];
            }
            for(size_t i = 0; i < ARM_NEON_ITERATIONS_COUNT; i++) {
                pixels_array[point_index + i].position = sf::Vector2f((double)window_x, (double)(window_y + i));
            }

            point_index += ARM_NEON_ITERATIONS_COUNT;
        }
    }

    return SUCCESS;
}

ErrorType getPixelColorArmNeon(float64x2_t* x_coords_ptr, float64x2_t* y_coords_ptr, sf::Color* colors) {
    uint64_t radius_vector_test = 1;
    uint64_t continue_rendering = 1;

    uint64_t counter[2] = {};
    uint64_t radius_vector_cmps[2] = {};

    alignas(16) float64x2_t xn  = {};
    alignas(16) float64x2_t yn  = {};
    alignas(16) float64x2_t x_coords = *x_coords_ptr;
    alignas(16) float64x2_t y_coords = *y_coords_ptr;
    alignas(16) float64x2_t radius_vector_x_coords = *x_coords_ptr;
    alignas(16) float64x2_t radius_vector_y_coords = *y_coords_ptr;
    alignas(16) float64x2_t radius_vector_length_v = {};
    alignas(16) uint64x2_t  counter_v              = {};
    alignas(16) uint64x2_t  length_mask_v          = {};
    alignas(16) uint64x2_t  iteration_mask_v       = {};
    alignas(16) uint64x2_t  combined_mask          = {};
    alignas(16) float64x2_t two_const_v            = vmovq_n_f64(2.0);
    alignas(16) float64x2_t max_radius_v           = vmovq_n_f64(MAX_RADIUS);
    alignas(16) uint64x2_t  max_iterations_count_v = vmovq_n_u64(MAX_ITERATIONS_COUNT);

    while(continue_rendering) {
        xn = radius_vector_x_coords;
        yn = radius_vector_y_coords;

        radius_vector_x_coords = vmulq_f64(xn, xn);
        radius_vector_x_coords = vsubq_f64(radius_vector_x_coords, vmulq_f64(yn, yn));
        radius_vector_x_coords = vaddq_f64(radius_vector_x_coords, x_coords);

        radius_vector_y_coords = vmulq_f64(two_const_v, vmulq_f64(xn, yn));
        radius_vector_y_coords = vaddq_f64(radius_vector_y_coords, y_coords);

        radius_vector_length_v = vmulq_f64(radius_vector_x_coords, radius_vector_x_coords);
        radius_vector_length_v = vaddq_f64(radius_vector_length_v, vmulq_f64(radius_vector_y_coords, radius_vector_y_coords));

        length_mask_v          = vcltq_f64(vsqrtq_f64(radius_vector_length_v), max_radius_v); // cmp_mask[i] = sqrt(radius_vector_length[i]) < MAX_RADIUS
        length_mask_v          = vshrq_n_u64(length_mask_v, 63);
        counter_v              = vaddq_u64(counter_v, length_mask_v);                         // counter[i] = counter[i] + cmp_mask[i] ~ if(radius_vector_length[i] < MAX_RADIUS) { counter[i]++ }

        iteration_mask_v       = vcltq_u64(counter_v, max_iterations_count_v);                // counter[i] < MAX_ITERATIONS_COUNT
        iteration_mask_v       = vshrq_n_u64(iteration_mask_v, 63);
        combined_mask          = vandq_u64(iteration_mask_v, length_mask_v);                  // bitwise AND
        continue_rendering     = vaddvq_u64(combined_mask);                                   // continue_rendering = SUM(combined_mask[i]) (if(all combined_mask[i] equal to zero): continue_rendering = false)
    }

    vst1q_u64((uint64_t*)counter, counter_v);
    vst1q_u64((uint64_t*)radius_vector_cmps, length_mask_v);

    for(size_t i = 0; i < ARM_NEON_ITERATIONS_COUNT; i++) {
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
