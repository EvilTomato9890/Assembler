#include "input.h"
#include "asserts.h"
#include "logger.h"
#include "error_handlers.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//==============================================================================

static const int MAX_FILE_REQUESTS = 10;

static size_t count_string_and_prepare_buffer(char* buff); 
static size_t get_command_from_buffer(command_data* command_ptr, char** buff);
static error_type get_file_size(FILE* curr_file, long* length); 

FILE* open_file(char** file_name_return) {
    printf("Введите имя файла:\n");
    char file_name[100] = {};
    scanf("%99s", &file_name);

    int cnt = 0;
    FILE* input_file = 0;
    while((input_file = fopen(file_name, "r")) == nullptr && cnt < MAX_FILE_REQUESTS) {
        printf("Ошибка открытия файла, попробуйте еще раз\n"
               "Чтобы завершить ввод напишите \"exit\"\n");
        printf("Введите имя файла:\n");
        scanf("%99s", &file_name);
        if(strcmp(file_name, "exit") == 0) return nullptr;
        cnt++;
    }
    *file_name_return = file_name;
    return input_file;
}

error_type text_dest(text_data* text) {
    HARD_ASSERT(text != nullptr, "text is nullptr");
    LOGGER_DEBUG("text_dest started");

    free(text->commands);
    text->commands = NULL;
    free(text->commands_container);
    return NO_ERROR;
}

/*
error_code text_verify(text_data* text) {
    if(text == nullptr)       return NULL_ARG_ERROR;
    if(text->str == nullptr)   return NULL_BUFF_ERROR;
    if(text->command_len > 10) return LARGE_CMD_LEN;
    if(text->args_cnt > 10)    return LARGE_ARGS_NUM;
    return NO_ERROR;
}*/

error_type read_file_into_buffer(FILE* curr_file, char** buffer_return) { 
    LOGGER_INFO("Reading file started");
    HARD_ASSERT(curr_file != nullptr, "File not found");
    HARD_ASSERT(buffer_return != nullptr, "Return ptr is nullptr");

    error_type error = NO_ERROR;
    long length = 0;
    error = get_file_size(curr_file, &length);
    if(error != NO_ERROR) {
        return error;
    }
    LOGGER_INFO("Filesize - %llu", length);

    char* buffer = (char*)calloc(length + 1, sizeof(char));
    if(buffer == nullptr) {
        LOGGER_ERROR("Memory allocation failed");
        return MEMORY_ALLOC_ERROR;
    }
    LOGGER_DEBUG("Memory allocation succes, taken %ld bytes", (long)((length + 1) * sizeof(char)));

    buffer[length] = '\0';
    size_t symbols = fread(buffer, sizeof(buffer[0]), length, curr_file);
    if(length > symbols) {
        LOGGER_ERROR("Failed to read file");
        free(buffer);
        return READ_FILE_ERROR;
    }

    LOGGER_INFO("Buffer returned");
    *buffer_return = buffer;
    return error;
}

static error_type get_file_size(FILE* curr_file, long* length) {
    LOGGER_DEBUG("get_file_size started");
    HARD_ASSERT(curr_file != nullptr, "File not found");
    HARD_ASSERT(length != nullptr, "Return ptr is nullptr");

    int fseek_error = fseek(curr_file, 0, SEEK_END);
    if(fseek_error != 0) {
        LOGGER_ERROR("Fseek droped"); 
        return PROCESSING_FILE_ERROR;
    }
    *length = ftell(curr_file);
    fseek_error = fseek(curr_file, 0, SEEK_SET);
    if(fseek_error != 0 && *length > 0) {
        LOGGER_ERROR("Fseek droped");
        return PROCESSING_FILE_ERROR;
    }
    return NO_ERROR;
}

static size_t count_string_and_prepare_buffer(char* buff) {
    LOGGER_DEBUG("count_string_and_prepare_buffer started");
    HARD_ASSERT(buff != nullptr, "Buff is nullptr");
    
    size_t strings_num = 0;
    size_t index = 0;
    size_t buff_len = 0;

    while(buff[index] != '\0') {
        if(buff[index] == '\n') {
            strings_num++;
            buff[index] = '\0';
        }
        index++;
        buff_len++;
    }

    if (buff_len <= 1) LOGGER_WARNING("Very small size of buffer");
    return strings_num;
}

static size_t get_command_from_buffer(command_data* command_ptr, char** buff) {
    HARD_ASSERT(buff != nullptr, "Buff is nullptr");
    HARD_ASSERT(command_ptr != nullptr, "Commands is nullptr");
    char* ptr_to_space = strchr(*buff, ' ');

    command_ptr->str      = *buff;

    if(ptr_to_space == nullptr) command_ptr->command_len = strlen(*buff);
    else                        command_ptr->command_len = (size_t)(ptr_to_space - *buff);

    while((ptr_to_space = strchr(*buff, ' ')) != nullptr) {
        command_ptr->args_cnt++;
        *buff = ptr_to_space + 1;
    }
    *buff = strchr(*buff, '\0') + 1;

    return (size_t)(*buff - command_ptr->str - 1);
}

error_type text_init(char* buff, text_data* text_return) {
    LOGGER_DEBUG("text_init started");
    if (buff == nullptr) LOGGER_WARNING("Buff is nullptr");
    
    error_type error = NO_ERROR;

    size_t strings_num = count_string_and_prepare_buffer(buff);
    text_data text = {.commands = (command_data**)calloc(strings_num, sizeof(command_data*)), \
                      .str_num = strings_num};
    LOGGER_INFO("strings_num: %d", strings_num);
    command_data* arr_struct_addreses = (command_data*)calloc(text.str_num, sizeof(command_data));
    if(text.commands == nullptr || arr_struct_addreses == nullptr) {
        LOGGER_ERROR("Memory allocation failed");
        free(arr_struct_addreses);
        free(text.commands);
        return MEMORY_ALLOC_ERROR;
    }


    for(size_t i = 0; i < text.str_num; i++) {
        text.commands[i] = arr_struct_addreses + i;
        text.commands[i]->len = get_command_from_buffer(text.commands[i], &buff);    
    }
    *text_return = text;
    text_return->commands_container = arr_struct_addreses;

    return error;
}
