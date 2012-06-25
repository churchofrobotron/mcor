
#ifndef MCOR_H
#define MCOR_H

void mcor_init(symbol_table *symtable);
void mcor_frame_update();
void mcor_send_mem(UINT32 offset, int length);

#endif
