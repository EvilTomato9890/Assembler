#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

#include "common/error_handlers.h"

#include <stdio.h>

//==============================================================================

struct command_data {
    char* str;
    size_t len;
    size_t command_len;
    size_t args_cnt;
}; 

struct text_data {
    command_data** commands;
    command_data* commands_container;
    size_t str_num;
};

//==============================================================================

//error_code text_verify(text_data* text);
error_type read_file_into_buffer(FILE* file_name, char** buffer_return); 
FILE* open_file(char** file_name_return);
error_type text_init(char* buff, text_data* text_return);
error_type text_dest(text_data* text);

#endif