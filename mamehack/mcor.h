
#ifndef MCOR_H
#define MCOR_H

#define MEM_MAIN ":maincpu"
#define MEM_SND  ":soundcpu"

/* Main CPU memory */
#define MEM_DEAD 0x9848
#define MEM_P1_MEN 0xBDEC

/* sound notes:
0x68 0xFB -- dead
0x45 0xFF -- shot
0x45 0xFC -- level change
0x99 0xFF -- pick up human
0x68 0xFF -- grunts walking
0xe6 0xff -- game start?
0x66 ???? -- enforcer spawn
0x68 0xfb -- human dies (0x45 fb)
0x5a 0xff -- tank stuff
0xa0 0xff -- entering brain wave
*/

/* Sound memory */
#define CUR_SND 0x0B

void mcor_init(symbol_table *symtable);
void mcor_frame_update();

void mcor_send_mem(UINT32 offset, int length);

void mcor_notify_dead();
void mcor_notify_shot();


#endif
