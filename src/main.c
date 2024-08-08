#include "main.h" 

//#define STANDART_DATA
//#define STANDART_ROLL20_DATA
#define ROLL20_DATA

/*
 * Standart colors
*/
char* transparent = "none";
char* black = "black";
char* white = "white";

int main(int argc, char** argv){
    // Validating inputs
    if(argc < 3){
        printf("Error:\tNot enough arguments.\n\tIt's necessary at least 2 number arguments for width and height\n");
        return 1;
    }

    // Inputs
    #ifdef STANDART_DATA
    population_data data = {
        .width = atol(argv[1]),
        .height = atol(argv[2]),
        .size = 2,
        .stroke_width = 0.2,
        .stroke_color = black,
        .background_color = transparent,
        .scale_px = 35
    };
    #endif
    #ifdef STANDART_ROLL20_DATA
    population_data data = {
        .width = (long)(atol(argv[1])),
        .height = (long)(atol(argv[2])),
        .size = roll20_standart_size,
        .stroke_width = roll20_standart_stroke_width,
        .stroke_color = black,
        .background_color = transparent,
        .scale_px = roll20_hex_width/(sqrt_three*roll20_standart_size)
    };
    #endif
    #ifdef ROLL20_DATA
    population_data data = {
        .width = (long)(atol(argv[1])),
        .height = (long)(atol(argv[2])*roll20_height_scale),
        .size = roll20_standart_size*roll20_width_scale,
        .stroke_width = roll20_standart_stroke_width,
        .stroke_color = black,
        .background_color = transparent,
        .scale_px = roll20_hex_width/(sqrt_three*roll20_standart_size)
    };
    #endif

    FILE* file = fopen("file.svg", "w+");
    if(!file){
        printf("Error:\n\tfopen() returned NULL");
        return 1;
    }
    populate_roll20_hex_grid(file, &data);

    fclose(file);
    return 0;
}

void populate_hex_grid(FILE* file, population_data* data){
    // Hexagon Blueprint
    const double l_sqrt3_per2 = (double)(sqrt_three*data->size/2);
    const double l_per2 = (double)(data->size/2);
    // Calculated constants
    const double width_scale = (double)(sqrt_three*data->size);
    const double width_odd_offset = l_sqrt3_per2;
    const double height_scale = (double)(3*l_per2);
    const double view_box_width = (double)(data->width*width_scale+width_odd_offset+data->stroke_width/2);
    const double view_box_height = (double)((data->size*(data->height*3+1)+data->stroke_width)/2);

    fprintf(file, 
        header, 
        view_box_width, view_box_height,
        view_box_width*data->scale_px, view_box_height*data->scale_px
    );
    // Hexagon
    fprintf(
        file, empty_hexagon,
        l_sqrt3_per2, (double)(0),
        l_sqrt3_per2, l_per2,
        (double)(0), (double)data->size,
        (double)(-1*l_sqrt3_per2), l_per2,
        (double)(-1*l_sqrt3_per2), (double)(-1*l_per2),
        (double)(0), (double)(-1*data->size),
        data->stroke_color, data->stroke_width
    );
    // Rectangle
    fprintf(file, empty_rectangle,
        view_box_width, view_box_height, data->background_color
    );

    fputs("\t<g>\n", file);
    for(long i = 0; i < data->height; i++){
        if(i%2){
            for(long j = 0; j < data->width; j++){
                fprintf(file, 
                    use_hexagon, 
                    (double)(j*width_scale+width_odd_offset), 
                    (double)(i*height_scale)
                );
            }
        } else {
            for(long j = 0; j < data->width; j++){
                fprintf(file, 
                    use_hexagon, 
                    (double)(j*width_scale), 
                    (double)(i*height_scale)
                );
            }
        }
        fputc('\n', file);
    }
    fputs("\t</g>\n", file);
    fputs(close_svg, file);
};

void populate_roll20_hex_grid(FILE* file, population_data* data){
    // Hexagon Blueprint
    const double l_sqrt3_per2 = (double)(sqrt_three*data->size/2);
    const double l_per2 = (double)(data->size/2);
    // Calculated constants
    const double width_scale = (double)(sqrt_three*data->size);
    const double width_odd_offset = l_sqrt3_per2;
    const double height_scale = (double)(3*l_per2);
    const double view_box_width = (double)(data->width*width_scale+width_odd_offset+data->stroke_width/2);
    const double view_box_height = (double)((data->size*(data->height*3+1)+data->stroke_width)/2)*roll20_factor;

    fprintf(file, 
        header, 
        view_box_width, view_box_height,
        view_box_width*data->scale_px, view_box_height*data->scale_px
    );
    // Hexagon
    fprintf(
        file, empty_hexagon,
        l_sqrt3_per2, (double)(0)*roll20_factor,
        l_sqrt3_per2, l_per2*roll20_factor,
        (double)(0), (double)data->size*roll20_factor,
        (double)(-1*l_sqrt3_per2), l_per2*roll20_factor,
        (double)(-1*l_sqrt3_per2), (double)(-1*l_per2)*roll20_factor,
        (double)(0), (double)(-1*data->size)*roll20_factor,
        data->stroke_color, data->stroke_width*roll20_factor
    );
    // Rectangle
    fprintf(file, empty_rectangle,
        view_box_width, view_box_height, data->background_color
    );

    fputs("\t<g>\n", file);
    for(long i = 0; i < data->height; i++){
        if(i%2){
            for(long j = 0; j < data->width; j++){
                fprintf(file, 
                    use_hexagon, 
                    (double)(j*width_scale+width_odd_offset), 
                    (double)(i*height_scale*roll20_factor)
                );
            }
        } else {
            for(long j = 0; j < data->width; j++){
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
}