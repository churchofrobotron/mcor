
#include <stdio.h>
#include <string.h>
#include "debug/express.h"
#include "mcor_send.h"
#include "mcor.h"

symbol_table *mcor_sym;

void mcor_init(symbol_table *symtable) {
	printf("--== THE CHURCH OF ROBOTRON ==--\n");
	mcor_sym = symtable;
	mcor_send_init(MCOR_SERVER, MCOR_PORT);
}

void mcor_frame_update(){
	mcor_send_mem(0x9848, 1);
}

/* Send a UDP packet with the given offset/length */
void mcor_send_mem(UINT32 offset, int length){
	char name[] = ":maincpu";
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
