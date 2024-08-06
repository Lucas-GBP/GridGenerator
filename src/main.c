#include <stdio.h>
#include <stdlib.h>

// Math Constansts
const double sqrt_three = 1.732050807568877293527446341505872366942805253810380628055806;
// Roll 20 magical numbers
const double roll20_factor = 1.02776101388*1.000424809; // Roll20 height distortion factor
const double roll20_size = 2; // [unit]
const double roll20_hex_width = 75.33098153*0.9982343843; // [pixel]
const double roll20_hex_scale_px = roll20_hex_width/(sqrt_three*roll20_size); // Total hexagon width in [pixel/unit]
const double roll20_stroke_width = 0.05; // [unit]
// SVG's elements
const char* header = "<svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" viewBox=\"0 0 %f %f\" width=\"%fpx\" height=\"%fpx\">\n";
const char* close_svg = "</svg>\n";
const char* empty_hexagon = "\t<defs>\n\t\t<path id=\"H\" d=\"M%f %fl%f %fl%f %fl%f %fl%f %fl%f %fZ\" fill=\"none\" stroke=\"%s\" stroke-width=\"%.2f\"/>\n\t</defs>\n";
const char* empty_rectangle = "\t<rect x=\"0\" y=\"0\" width=\"%f\" height=\"%f\" fill=\"%s\"/>\n";
const char* use_hexagon = "\t\t<use xlink:href=\"#H\" x=\"%f\" y=\"%f\"/>\n";

int main(int argc, char** argv){
    // Validating inputs
    if(argc < 3){
        printf("Error:\tNot enough arguments.\n\tIt's necessary at least 2 number arguments for width and height\n");
        return 1;
    }

    // Inputs
    const long width = atol(argv[1]);
    const long height = atol(argv[2]);
    const int size = 2;
    const double stroke_width =  roll20_stroke_width;
    const char* stroke_color = "black";
    const char* background_color = "none";
    const double scale_px = roll20_hex_scale_px;
    // Hexagon Blueprint
    const double l_sqrt3_per2 = (double)(sqrt_three*size/2);
    const double l_per2 = (double)(size/2);
    // Calculated constants
    const double width_scale = (double)(sqrt_three*size);
    const double width_odd_offset = l_sqrt3_per2;
    const double height_scale = (double)(3*l_per2);
    const double view_box_width = (double)(width*width_scale+width_odd_offset+stroke_width/2);
    const double view_box_height = (double)((size*(height*3+1)+stroke_width)/2)*roll20_factor;

    // Creating file
    FILE* file = fopen("file.svg", "w+");
    if(!file){
        printf("Error:\nfopen() returned NULL");
        return 1;
    }

    /*
    * Populating file
    */
    //Header
    fprintf(file, 
        header, 
        view_box_width, view_box_height,
        view_box_width*scale_px, view_box_height*scale_px
    );
    // Hexagon
    fprintf(
        file, empty_hexagon,
        l_sqrt3_per2, (double)(0)*roll20_factor,
        l_sqrt3_per2, l_per2*roll20_factor,
        (double)(0), (double)size*roll20_factor,
        (double)(-1*l_sqrt3_per2), l_per2*roll20_factor,
        (double)(-1*l_sqrt3_per2), (double)(-1*l_per2)*roll20_factor,
        (double)(0), (double)(-1*size)*roll20_factor,
        stroke_color, stroke_width*roll20_factor
    );
    // Rectangle
    fprintf(file, empty_rectangle,
        view_box_width, view_box_height, background_color
    );

    fputs("\t<g>\n", file);
    for(long i = 0; i < height; i++){
        if(i%2){
            for(long j = 0; j < width; j++){
                fprintf(file, 
                    use_hexagon, 
                    (double)(j*width_scale+width_odd_offset), 
                    (double)(i*height_scale*roll20_factor)
                );
            }
        } else {
            for(long j = 0; j < width; j++){
                fprintf(file, 
                    use_hexagon, 
                    (double)(j*width_scale), 
                    (double)(i*height_scale*roll20_factor)
                );
            }
        }
        fputc('\n', file);
    }
    fputs("\t</g>\n", file);

    fputs(close_svg, file);
    fclose(file);

    return 0;
}