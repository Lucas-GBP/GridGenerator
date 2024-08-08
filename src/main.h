#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define TRUE 1
#define FALSE 0

/*
 * Math Constansts
 */
const double sqrt_three = 1.732050807568877293527446341505872366942805253810380628055806;
const double sqrt_tree_2 = sqrt_three/2.0;

/*
 * Roll 20 magical numbers
 */
const double roll20_standart_cell_size = 70.0;      // [pixel]
const double roll20_standart_stroke_width = 0.05;   // [unit]
const double roll20_standart_size = 2.0;            // [unit]
const double roll20_width_scale = 0.926;            // Roll20 width cell quantity ratio
const double roll20_height_scale = 1.12;            // Roll20 height cell quantity ratio
const double roll20_hex_width = 75.1979759663;      // [pixel]
const double roll20_factor = 1.028197616008545;     // Roll20 height distortion factor

/*
 * SVG's elements
 */
const char* header = "<svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" viewBox=\"0 0 %f %f\" width=\"%fpx\" height=\"%fpx\">\n";
const char* close_svg = "</svg>\n";
const char* open_group = "\t<g>\n";
const char* close_group = "\t</g>\n";
const char* empty_hexagon = "\t<defs>\n\t\t<path id=\"H\" d=\"M%f %fl%f %fl%f %fl%f %fl%f %fl%f %fZ\" fill=\"none\" stroke=\"%s\" stroke-width=\"%.2f\"/>\n\t</defs>\n";
const char* empty_rectangle = "\t<rect x=\"0\" y=\"0\" width=\"%f\" height=\"%f\" fill=\"%s\"/>\n";
const char* use_hexagon = "\t\t<use xlink:href=\"#H\" x=\"%f\" y=\"%f\"/>\n";

/*
 * Structs
 */
typedef int8_t bool;

enum MODES {
    STANDART,
    ROLL20,
    ROLL20_EXACT_SIZE
};

struct input_data {
    long width;             // Horizontal quantity of cells
    long height;            // Vertical quantity of cells
    bool roll20_flag;       //s
    bool roll20_exact_flag; //
};
typedef struct input_data input_data;

struct population_data {
    long width;             // Quantity of cells
    long height;            // Quantity of cells
    double size;            // Width of the cell [unit]
    double stroke_width;    // Stroke width [unit]
    char* stroke_color;     // Hex-code in string of stroke color
    char* background_color; // Hex-code in string of background color
    double scale_px;        // Total hexagon width in [pixel/unit]
};
typedef struct population_data population_data;

/*
 * Functions
 */
int randle_arguments(int argc, char** argv, input_data* input);
void randle_input_data(input_data* input, population_data* data);
void populate_hex_grid(FILE* file, population_data* data);
void populate_roll20_hex_grid(FILE* file, population_data* data);

#endif