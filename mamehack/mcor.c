
#include <stdio.h>
#include <string.h>
#include "debug/express.h"
#include "mcor_send.h"
#include "mcor.h"

symbol_table *mcor_sym;
int last_dead = 0;
int last_shot = 0;

void mcor_init(symbol_table *symtable) {
	printf("--== THE CHURCH OF ROBOTRON ==--\n");
	mcor_sym = symtable;
	mcor_send_init(MCOR_SERVER, MCOR_PORT);
}

void mcor_frame_update(){
	//mcor_send_mem(MEM_DEAD, 1);
	mcor_notify_dead();
	mcor_notify_shot();
}

void mcor_notify_dead(){
	enum expression_space space = EXPSPACE_PROGRAM_LOGICAL;
	int value = (int)mcor_sym->memory_value(MEM_MAIN, space, MEM_DEAD, 1);
	int men = (int)mcor_sym->memory_value(MEM_MAIN, space, MEM_P1_MEN, 1);
	if((value == 1) && (last_dead != 1)){
		char buff[255];
		sprintf(buff, "DEAD %d", men);
		//mcor_send("DEAD", 4);
		mcor_send(buff, strlen(buff));
	}
	last_dead = value;
}

void mcor_notify_shot(){
	enum expression_space space = EXPSPACE_PROGRAM_LOGICAL;
	int value = (int)mcor_sym->memory_value(MEM_SND, space, CUR_SND, 1);
	if((value == 0x45) && (last_shot != 0x45)){
		mcor_send("SHOT ON", 7);
	}
	else if((value != 0x45) && (last_shot == 0x45)){
		mcor_send("SHOT OFF", 8);
	}
	last_shot = value;
}

/* Send a UDP packet with the given offset/length */
void mcor_send_mem(UINT32 offset, int length){
	char name[] = MEM_MAIN;
	enum expression_space space = EXPSPACE_PROGRAM_LOGICAL;
	UINT64 value = mcor_sym->memory_value(name, space, offset, length);
	//todo: don't send ascii, just fling bytes
	char buff[255];
	sprintf(buff, "%02X %d", offset, (int)value);
	mcor_send(buff, strlen(buff));
}

/////////////////////////////////////////////////////////////////////////////////
//boneyard
	// see also debugcpu.c:
	// static UINT64 expression_read_memory(void *param, const char *name, expression_space spacenum, UINT32 address, int size)
	// mcor_sym->memory_value(name, space, offset, size); 	//see debug/express.c
	/*
	enum expression_space
	{
		    EXPSPACE_INVALID,
			EXPSPACE_PROGRAM_LOGICAL,
			EXPSPACE_DATA_LOGICAL,
			EXPSPACE_IO_LOGICAL,
			EXPSPACE_SPACE3_LOGICAL,
			EXPSPACE_PROGRAM_PHYSICAL,
			EXPSPACE_DATA_PHYSICAL,
			EXPSPACE_IO_PHYSICAL,
			EXPSPACE_SPACE3_PHYSICAL,
			EXPSPACE_OPCODE,
			EXPSPACE_RAMWRITE,
			EXPSPACE_REGION
	};
	*/
