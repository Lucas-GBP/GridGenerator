#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define NULL_TERMINATOR '\0'

/*
 * User input arguments
 */
const char* arguments[] = {
    "-width",
    "-height",
    "--roll20",
    "--roll20_exact",
    "--help"
};
const int arg_quant = 5;    // Quantity of possible arguments
const int max_arg_size = 32;// Biggest argument string size
// TODO: make the help string :P
const char* help_string = "You want help.\n";
enum ARGUMENTS {
    ARGUMENTS_WIDTH,
    ARGUMENTS_HEIGHT,
    ARGUMENTS_ROLL20,
    ARGUMENTS_ROLL20_EXACT,
    ARGUMENTS_HELP
};

/*
 * Math Constansts
 */
const double sqrt_three = 1.732050807568877293527446341505872366942805253810380628055806;
const double sqrt_tree_2 = sqrt_three/2;

/*
 * Roll 20 magical numbers
 */
const double roll20_standart_cell_size = 70.0;      // [pixel]
const double roll20_standart_stroke_width = 0.025;  // Standart stroke width [unit]
const double roll20_standart_size = 1.0;            // Standart Hexagon side lenght [unit]
// TODO: Tentar eliminar a constante height_scale
const double roll20_height_scale = 1.12;            // Roll20 height cell
const double roll20_width_scale = 0.930868864;      // Roll20 width cell quantity ratio
const double roll20_factor = 1.028197616008545;     // Roll20 height distortion factor
// Standart Hexagon width in roll20 [pixel]
const double roll20_hex_width = roll20_standart_cell_size/roll20_width_scale;
// [pixel/unit]
const double roll20_scale_px = roll20_hex_width/(sqrt_three*roll20_standart_size);

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
const char* standart_file_name = "hexagonal_grid_%dx%d.svg";
const char* standart_roll20_file_name = "hexagonal_grid_roll20_%dx%d.svg";

/*
 * Structs
 */
typedef int8_t bool;

enum MODES {
    HEX_STANDART,
    HEX_ROLL20,
    HEX_ROLL20_EXACT_SIZE,
    HELP
};

typedef struct {
    long width;             // Horizontal quantity of cells
    long height;            // Vertical quantity of cells
    bool roll20_flag;       //
    enum MODES mode;        //
} input_data;

typedef struct {
    long width;             // Quantity of horizontal cells
    long height;            // Quantity of vertical cells
    double size;            // Width of the cell [unit]
    double stroke_width;    // Stroke width [unit]
    char* stroke_color;     // Hex-code in string of stroke color
    char* background_color; // Hex-code in string of background color
    double scale_px;        // Total hexagon width in [pixel/unit]
    char* file_name;        // File Name
} population_data;

/*
 * Functions
 */
int handling_arguments(int argc, char** argv, input_data* input);
int handling_input_data(input_data* input, population_data* data);
void populate_hex_grid(FILE* file, population_data* data);
void populate_roll20_hex_grid(FILE* file, population_data* data);
int safe_string_copy(char* string, char* buffer, int max_lenght);

#endif