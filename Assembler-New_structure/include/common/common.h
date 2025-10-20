#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

//==============================================================================

enum command_num {
	cmdNOTHING = 0,
	cmdPUSH    = 1,
	cmdADD     = 2,
	cmdSUB     = 3,
	cmdDEL     = 4,
	cmdMULT    = 5,
	cmdPOW     = 6,
	cmdSQRT    = 7,
	cmdOUT     = 8,
	cmdPOPR    = 9,
	cmdPUSHR   = 10,
	cmdHLT     = 11,
	cmdJE      = 12,
	cmdJA      = 13,
	cmdJB      = 14,
	cmdJUMP    = 15,
	cmdIN      = 16,
	cmdLBL     = 17
};

#define COMMAND_TYPES_NUM 17

//==============================================================================

struct command_info {
	const char* cmd_name;
	command_num cmd;
	int args_num;
	
};
/*
command_info command_arr[] = {   					
								{"PUSH", cmdPUSH,  1},  		
							    {"ADD",  cmdADD,   0},   
							    {"SUB",  cmdSUB,   0},
								{"DEL",  cmdDEL,   0},
								{"MULT", cmdMULT,  0},
								{"POW",  cmdPOW,   0},
								{"SQRT", cmdSQRT,  0},
								{"OUT",  cmdOUT,   0},
								{"PUSHR",cmdPUSHR, 1},
								{"POPR", cmdPOPR,  1},
								{"HLT",  cmdHLT,   0},
								{"JE",   cmdJE,    1},
								{"JB",   cmdJB,    1},
								{"JA",   cmdJA,    1},
								{"JUMP", cmdJUMP,  1},
								{"IN",   cmdIN,    0}
							};
*/
#endif