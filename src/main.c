#include "main.h" 

/*
 * Standart colors
*/
char* transparent = "none";
char* black = "black";
char* white = "white";

int main(int argc, char** argv){
    // Validating inputs
    input_data input;
    if(randle_arguments(argc, argv, &input)){
        return 1;
    }
    // Inputs
    population_data data;
    if(randle_input_data(&input, &data)){
        return 1;
    }

    FILE* file = fopen(data.file_name, "w+");
    if(!file){
        printf("Error:\n\tfopen() returned NULL");
        return 1;
    }

    if(input.roll20_flag){
        populate_roll20_hex_grid(file, &data);
    } else {
        populate_hex_grid(file, &data);
    }

    fclose(file);
    return 0;
}

int randle_arguments(int argc, char** argv, input_data* input){
    if(argc < 3){
        printf("Error:\tNot enough arguments.\n\tIt's necessary at least 2 number arguments for width and height\n");
        return 1;
    }

    input->width = atol(argv[1]);
    input->height = atol(argv[2]);
    input->roll20_flag = TRUE;
    input->mode = HEX_ROLL20;
    return 0;
}

int randle_input_data(input_data* input, population_data* data){
    switch (input->mode){
    case HEX_ROLL20:
        data->width = input->width;
        data->height = input->height;
        data->size = roll20_standart_size;
        data->stroke_width = roll20_standart_stroke_width;
        data->stroke_color = black;
        data->background_color = transparent;
        data->scale_px = roll20_hex_width/(sqrt_three*roll20_standart_size);
        data->file_name = "hexagonal_grid_roll20.svg";

        return 0;
    case HEX_ROLL20_EXACT_SIZE:
        data->width = input->width;
        data->height = (long)(input->height*roll20_height_scale);
        data->size = roll20_standart_size*roll20_width_scale;
        data->stroke_width = roll20_standart_stroke_width;
        data->stroke_color = black;
        data->background_color = transparent;
        data->scale_px = roll20_hex_width/(sqrt_three*roll20_standart_size);
        data->file_name = "hexagonal_grid_roll20.svg";

        return 0;
    case HEX_STANDART:
    default:
        data->width = input->width;
        data->height = input->height;
        data->size = 2;
        data->stroke_width = 0.2;
        data->stroke_color = black;
        data->background_color = transparent;
        data->scale_px = 35;
        data->file_name = "hexagonal_grid.svg";

        return 0;
    }
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

    fputs(open_group, file);
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
    fputs(close_group, file);
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