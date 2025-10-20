#ifndef ASSEMBLER_H_INCLUDED
#define ASSEMBLER_H_INCLUDED

#include "processor_data.h"
#include "stack.h"

#define LABELS_CNT 10

struct assembler_info_t {
	FILE* command_file; //Почему нет присваивания в структуре
	FILE* asm_file;	
	text_data text;
	char* buff;
	int labels[LABELS_CNT];
};

void test(char* file_buff);
error_code assemble(FILE* input_file, FILE* command_file);
error_code assembler_info_init(assembler_info_t* asm_info, FILE* command_file, FILE* asm_file);
error_code assembler_info_dest(assembler_info_t* asm_info);

#endif