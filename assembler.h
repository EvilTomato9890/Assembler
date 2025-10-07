#ifndef ASSEMBLER_H_INCLUDED
#define ASSEMBLER_H_INCLUDED

void test(char* file_buff);
error_code assemble(FILE* input_file, FILE* command_file);

#endif