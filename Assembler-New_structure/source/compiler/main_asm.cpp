#include <stdio.h>
#include "common/logger.h"
#include "common/error_handlers.h"
#include "common/input.h"
#include "compiler/assembler.h"

int main() {
    logger_initialize_stream(nullptr);
    error_code error = 0;

    char* file_name = nullptr;
    FILE* input_file = open_file(&file_name);
    RETURN_IF_ERROR((int)error);

    FILE* command_file = fopen("num_commands", "w");
    if (!command_file) {
        LOGGER_ERROR("Cannot open num_commands for writing");
        return (int)READ_FILE_ERROR;
    }

    error |= assemble(input_file, command_file);

    fclose(command_file);
    fclose(input_file);

    RETURN_IF_ERROR((int)error);
    return 0;
}
