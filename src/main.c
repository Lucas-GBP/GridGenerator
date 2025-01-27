#include "main.h" 

/*
 * Standart colors
*/
char* color_transparent = "none";
char* color_black = "black";
char* color_white = "white";

int main(int argc, char** argv){
    // Validating inputs
    input_data input;
    if(handling_arguments(argc, argv, &input)){
        printf("\tsomething got wrong in the input. try --help");
        return 1;
    }
    if(input.mode == HELP){
        printf(help_string);
        return 0;
    }
    // Inputs
    population_data data;
    if(handling_input_data(&input, &data)){
        return 1;
    }
    printf("Arguments verified, generating grid...\n");

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
    printf("grid generated :)\n");
    return 0;
}

int handling_arguments(int argc, char** argv, input_data* input){
    // Standart input
    input->width = 0;
    input->height = 0;
    input->roll20_flag = FALSE;
    input->mode = HEX_STANDART;
    //input buffer
    char input_buffer[max_arg_size+1];

    for(int i = 1; i < argc; i++){
        if(safe_string_copy(argv[i], input_buffer, max_arg_size)){
            return 1;
        }

        int j;
        for(j = 0; j < arg_quant+1; j++){
            if(j >= arg_quant){
                printf("Error:\n\tArgument[%d] (%s) not reconized.\n", i, input_buffer);
                return 1;
            }
            if(!strcmp(arguments[j], input_buffer)){
                break;
            }
        }

        switch (j) {
        case ARGUMENTS_WIDTH:
            i++;
            if(i >= argc){ 
                printf("Error:\n\tNot enought arguments. after -width you need to especify the size with a integer number.\n");
                return 1; 
            }
            if(safe_string_copy(argv[i], input_buffer, max_arg_size)){
                return 1;
            }
            input->width = atol(input_buffer);

            break;
        case ARGUMENTS_HEIGHT:
            i++;
            if(i >= argc){
                printf("Error:\n\tNot enought arguments. after -height you need to especify the size with a integer number.\n");
                return 1; 
            }
            if(safe_string_copy(argv[i], input_buffer, max_arg_size)){
                return 1;
            }
            input->height = atol(input_buffer);

            break;
        case ARGUMENTS_ROLL20:
            input->roll20_flag = TRUE;
            input->mode = HEX_ROLL20;

            break;
        case ARGUMENTS_ROLL20_EXACT:
            input->roll20_flag = TRUE;
            input->mode = HEX_ROLL20_EXACT_SIZE;

            break;
        case ARGUMENTS_HELP:
            input->mode = HELP;
            return 0;
        }
    }

    if(input->width < 1 || input->height < 1){
        printf("Error:\n\tinvalid grid size.\n");
        return 1;
    }
    return 0;
}

int handling_input_data(input_data* input, population_data* data){
    switch (input->mode){
    case HEX_ROLL20:
        data->width = input->width;
        data->height = input->height;
        data->size = roll20_standart_size;
        data->stroke_width = roll20_standart_stroke_width;
        data->stroke_color = color_black;
        data->background_color = color_transparent;
        data->scale_px = roll20_scale_px;
        data->file_name = "hexagonal_grid_roll20.svg";

        return 0;
    case HEX_ROLL20_EXACT_SIZE:
        data->width = input->width;
        data->height = (long)(((double)(input->height))*roll20_height_scale);
        data->size = roll20_standart_size*roll20_width_scale;
        data->stroke_width = roll20_standart_stroke_width;
        data->stroke_color = color_black;
        data->background_color = color_transparent;
        data->scale_px = roll20_scale_px;
        data->file_name = "hexagonal_grid_roll20.svg";

        return 0;
    case HEX_STANDART:
    default:
        data->width = input->width;
        data->height = input->height;
        data->size = 2;
        data->stroke_width = 0.2;
        data->stroke_color = color_black;
        data->background_color = color_transparent;
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
    const double view_box_width = (double)((double)(data->width)*width_scale+width_odd_offset+data->stroke_width/2);
    const double view_box_height = (double)((data->size*(double)(data->height*3+1)+data->stroke_width)/2);

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
                    (double)(j)*width_scale+width_odd_offset, 
                    (double)(i)*height_scale
                );
            }
        } else {
            for(long j = 0; j < data->width; j++){
                fprintf(file, 
                    use_hexagon, 
                    (double)(j)*width_scale, 
                    (double)(i)*height_scale
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
    const double view_box_width = (double)(data->width)*width_scale;
    const double view_box_height = ((data->size*(double)(data->height*3+1))/2)*roll20_factor;

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
            for(long j = 0; j < data->width-1; j++){
                fprintf(file, 
                    use_hexagon, 
                    (double)(j)*width_scale+width_odd_offset, 
                    (double)(i)*height_scale*roll20_factor
                );
            }
        } else {
            for(long j = 0; j < data->width; j++){
                fprintf(file, 
                    use_hexagon, 
                    (double)(j)*width_scale, 
                    (double)(i)*height_scale*roll20_factor
                );
            }
        }
        fputc('\n', file);
    }
    fputs("\t</g>\n", file);
    fputs(close_svg, file);
}

int safe_string_copy(char* string, char* buffer, int max_lenght){
    buffer[max_lenght] = NULL_TERMINATOR;
    int i = 0;
    for(; string[i] != NULL_TERMINATOR && i < max_lenght; i++){
        buffer[i] = string[i];
    }

    if(string[i] != NULL_TERMINATOR){
        return 1;
    }

    buffer[i] = NULL_TERMINATOR;
    return 0;
}