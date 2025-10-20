#include <stdio.h>
#include "common/logger.h"
#include "common/error_handlers.h"
#include "common/stack.h"
#include "cpu/processor.h"
#include "cpu/processor_data.h"

int main() {
    logger_initialize_stream(nullptr);
    error_code error = 0;

    FILE* output_file = fopen("output", "w");
    if (!output_file) {
        LOGGER_ERROR("Cannot open output for writing");
        return (int)READ_FILE_ERROR;
    }

    processor_data_t processor_data = {};
    stack_t stack = {};
    error |= processor_data_init(&processor_data, "num_commands", &stack);
    error |= execution(output_file, &processor_data);
    error |= processor_data_dest(&processor_data);

    fclose(output_file);
    RETURN_IF_ERROR((int)error);
    return 0;
}
