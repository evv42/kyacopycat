#include "config.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <stdlib.h>

#ifdef SERIAL_IO
#include <poll.h>
#endif

#ifdef UNIX_TIMING
#include <sys/time.h>
#include <unistd.h>
#endif

#include "m68k.h"
#include "m68kmess.h"

void change_keyreg_state(unsigned char y, unsigned char x, unsigned char state);

#ifdef SDL_UI
#include "SDL.h"
#include "sdlkeymess.h"
#endif


/*inverted*/
#define BYTE_TO_BIN_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BIN(byte)  \
  ((byte) & 0x80 ? '0' : '1'), \
  ((byte) & 0x40 ? '0' : '1'), \
  ((byte) & 0x20 ? '0' : '1'), \
  ((byte) & 0x10 ? '0' : '1'), \
  ((byte) & 0x08 ? '0' : '1'), \
  ((byte) & 0x04 ? '0' : '1'), \
  ((byte) & 0x02 ? '0' : '1'), \
  ((byte) & 0x01 ? '0' : '1')


#define DISP_W 672
#define DISP_H 344
/* this is the complicated math
#define MAIN_CLOCK 19968000.0
#define CPU_CLOCK (MAIN_CLOCK/4.0)
#define KTOBF (MAIN_CLOCK/65536.0)
#define TIMER (KTOBF/2.0)
#define CPU_CYCLES (int)(CPU_CLOCK/TIMER)
*/
/*which is the same as*/
#define CPU_CYCLES 32768
#define RAM_MASK RAM_SIZE-1

/*canon cat data structures*/
  
void* cpu;

unsigned char main_rom[262144];
unsigned char main_ram[RAM_SIZE];
unsigned char svram[8192];
unsigned char svrom[131072];

unsigned char opr = 0;/*0x840001 (w)*/
unsigned char watchdog = 0;/*counting 6.5ms timer overflows, reset by software setting 3rd bit of opr*/

unsigned char pr_cont = 0;/*0x80000e (w)*/
unsigned char kb_wr = 0;/*0x800002 (w)*/
unsigned char kb_rd[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};/*data of keyboard marix buffers*/
unsigned char dip = 0xFF;/*configuration switches*/

unsigned char duart_ipcr= 0;/*0x810009 (r)*/
/*
 * the canon cat first checks the isr for the serial ports control
 * for simplification purposes the tx buffers and counter/timer are permanantly ready
 * for control of the rx buffers the ready flag first need to be set on the isr then the status register
 */
unsigned char duart_isr = 0x19;/*0x81000b (r)*/
unsigned char duart_sra = 0x04;/*0x810003 (r)*/
unsigned char duart_rba = 0x00;/*0x810007 (r)*/
/*unsigned char duart_imr = 0;/*0x81000b (w)*/

/*convininence data structures*/

FILE* printer_f = NULL;
#ifdef SERIAL_IO
FILE* serial_fr = NULL;
FILE* serial_fw = NULL;
struct pollfd fds[1];
unsigned char translate = 0;
#endif
unsigned char framebuffer[DISP_W*DISP_H];
uint16_t trg = 0;
unsigned char vram_touched = 0;
unsigned char v_zero = 0;
unsigned char v_one = 0;
unsigned char v_blank = 0;

#ifdef FASTER_VIDEO
INLINE void update_pixel(unsigned int p, unsigned char value){
	for(unsigned char i = 0; i<8; i++)framebuffer[(p*8)+i] = value & (0x80 >> i) ? v_zero : v_one;
	/*framebuffer[p*8] = value & 0x80 ? v_zero : v_one;
	framebuffer[(p*8)+1] = value & 0x40 ? v_zero : v_one;
	framebuffer[(p*8)+2] = value & 0x20 ? v_zero : v_one;
	framebuffer[(p*8)+3] = value & 0x10 ? v_zero : v_one;
	framebuffer[(p*8)+4] = value & 0x08 ? v_zero : v_one;
	framebuffer[(p*8)+5] = value & 0x04 ? v_zero : v_one;
	framebuffer[(p*8)+6] = value & 0x02 ? v_zero : v_one;
	framebuffer[(p*8)+7] = value & 0x01 ? v_zero : v_one;*/
}
#endif
void update_framebuffer(){
	for(int p=0; p<28896; p++){
		for(unsigned char i = 0; i<8; i++)framebuffer[(p*8)+i] = main_ram[p] & (0x80 >> i) ? v_zero : v_one;
		/*framebuffer[i*8] = main_ram[i] & 0x80 ? v_zero : v_one;
		framebuffer[(i*8)+1] = main_ram[i] & 0x40 ? v_zero : v_one;
		framebuffer[(i*8)+2] = main_ram[i] & 0x20 ? v_zero : v_one;
		framebuffer[(i*8)+3] = main_ram[i] & 0x10 ? v_zero : v_one;
		framebuffer[(i*8)+4] = main_ram[i] & 0x08 ? v_zero : v_one;
		framebuffer[(i*8)+5] = main_ram[i] & 0x04 ? v_zero : v_one;
		framebuffer[(i*8)+6] = main_ram[i] & 0x02 ? v_zero : v_one;
		framebuffer[(i*8)+7] = main_ram[i] & 0x01 ? v_zero : v_one;*/
	}
}

void change_keyreg_state(unsigned char y, unsigned char x, unsigned char state){
	if(y > 7 || x > 7)return;
	unsigned char mask = 1 << x;
	if(state)kb_rd[y] = kb_rd[y] | mask;
	else kb_rd[y] = kb_rd[y] & (~mask);
}

unsigned char cat_keyb(){
	if(pr_cont == 0x09)return dip;
#ifdef DEBUG_KEYB
	printf("keyb_read:%02x, %02x\n", pr_cont, kb_wr);
#endif
	switch(kb_wr){
		case 0x01: return kb_rd[0];
		case 0x02: return kb_rd[1];
		case 0x04: return kb_rd[2];
		case 0x08: return kb_rd[3];
		case 0x10: return kb_rd[4];
		case 0x20: return kb_rd[5];
		case 0x40: return kb_rd[6];
		case 0x80: return kb_rd[7];
	}
	return 0xFF;
}

INLINE unsigned int m68k_read_memory_8(unsigned int addr){
#ifdef DEBUG_UART
		if((addr & 0x810000) == 0x810000)printf("r uart %06x\n",addr);
#endif
	unsigned int ret = 0;
	/*rom/svram area (two msb = 00)*/
	if(!(addr & 0xE40000))return main_rom[addr];
	else if((addr & 0xE70001) == 0x40000)return svram[(addr >>1)&0x1FFF];
	else if((addr & 0xE40001) == 0x200001)return svrom[(addr >>1)&0x1FFFF];
	else if(!(addr & 0xC00000))return 0x00;
	
	/*main ram area  (two msb = 01)*/
	else if(addr >= 0x400000 && addr < (0x400000 + RAM_SIZE))return main_ram[(addr & (RAM_SIZE-1))];
	else if((addr & 0xC00000) == 0x400000)return 0x00;
	
	/*the rest*/
	else if(addr == 0x80000a)return cat_keyb();
	else if(addr == 0x80000e)return 0x00;
	else if(addr == 0x810003){
		ret = duart_sra;
		duart_sra = duart_sra & 0x04;
	}else if(addr == 0x810007)return duart_rba;
	else if(addr == 0x810009){
		ret = duart_ipcr;//DUART input port change register
		duart_ipcr=0;
	}
	else if(addr == 0x81000b){
		ret = duart_isr;//DUART isr
		duart_isr = duart_isr & 0x3B;
	}
	else if(addr == 0x81001b)return 0x10;/*duart input port, the cat checks if the printer is busy on bit 4*/
	/*else if(addr == 0x81001d)duart_isr = duart_isr | 0x08;
	else if(addr == 0x81001f)duart_isr = duart_isr & 0xF7;*/
	/*else if(addr == 0x81001d)printf("sound on ");
	else if(addr == 0x81001f)printf("sound off\n");*/
	else if(addr >= 0x870000)return 0xFF;
	else{
#ifdef DEBUG_UR
		printf("unimplemented read %06x\n",addr);
#endif
		ret = 0x00;
	}
//	printf("ftch%08X:%02X\n",addr,ret);
	return ret;
}

INLINE void m68k_write_memory_8(unsigned int addr, unsigned int value){
	value = value & 0xFF;
#ifdef DEBUG_UART
		if((addr & 0x810000) == 0x810000)printf("w uart %06x = %02x\n",addr, value);
#endif
	if(addr >= 0x400000 && addr < (0x400000 + RAM_SIZE)){
		main_ram[addr & (RAM_SIZE-1)]=value;
		if(addr < 0x4070E0){
			vram_touched = 1;
#ifdef FASTER_VIDEO
			update_pixel(addr & (RAM_SIZE-1), value);
#endif
		}
	}
	else if((addr & 0xE70001) == 0x40000)svram[(addr >>1)&0x1FFF] = value;
	else if(addr == 0x840001){
		opr = value;
		if(!(opr & 0x04)){
			v_blank = 1;
		}else if(opr & 0x10){
			v_blank = 0; v_zero=0xFF; v_one=0;
		}else{
			v_blank = 0; v_zero=0; v_one=0xFF;
		}

		if(value & 0x8){
#ifdef DEBUG_WATCHDOG
			printf("%d\n",watchdog);
			fflush(stdout);
#endif
			watchdog = 0;
		}
	}
	else if(addr == 0x80000e)pr_cont = value;
	else if(addr == 0x800002)kb_wr = value;
	else if(addr == 0x800004 && printer_f != NULL){
		fputc(value,printer_f);
	}
#ifdef SERIAL_IO
	else if(addr == 0x810007 && serial_fw != NULL){
		if(translate && value == '\r')value='\n';
		fputc(value,serial_fw);
		fflush(serial_fw);
	}
#endif
	else if(addr == 0x81000b)return;/*duart_imr = value;*/
	else if(addr >= 0x870000)return;
#ifdef DEBUG_UW
	else printf("unimplemented write %06x = %02x\n",addr, value);
#endif
}

#ifdef DEBUG_I
void op(unsigned int addr){
	char str[256];
	unsigned int code;
	m68k_disassemble(str,addr,M68K_CPU_TYPE_68000);
	printf("instruction fetch: %s\n",str);
}
#endif

int crash_halp(char* str){
	fprintf(stderr,"%s",str);
	printf("\nthis is kyacopycat, a canon cat emulator\n \
the invocation is :\n \
\tkyacopycat [-s scale] [-d dip] [-x speed] [-p printer] [-c svrom] [-n svram] [-r] [-b ram] rom\n\n \
where: <default marked this way>\n\n \
scale is the scaling of the window (integer only) <1>\n \
dip is the value of the cat setting switches <0xFF (normal mode, us)>\n \
speed is the speed of the emulator <1.0>\n \
printer is a file to redirect the centronics printer output of the cat <none>\n \
svrom is the spellcheck rom file <none>\n \
svram is the non-volatile parametre/user dictionary ram file (created if not there) <none>\n \
the r option disables the remapping of the alphabetic keys\n \
ram is the main ram of the cat, if specified the state of the machine will be saved and restored using the warm boot procedure (nmi) <none>\n \
rom is the main rom of the cat (this is the only required parametre)\n");
#ifdef SERIAL_IO
	printf("\n this executable has been compiled with the serial i/o option,\n \
use the [-i file] and [-o file] flags to redirect the cat serial port\n \
additionally, the [-t] opetion replaces the LF of the serial input with CR and does the reverse with the serial output\n");
#endif
	return str[0];
}

INLINE int crash(char* str){
	fprintf(stderr,"%s",str);
	return str[0];
}

int main(int argc, char** argv){

FILE* in_rom_f;
int rom_file = 1;
unsigned char scale = 1;
unsigned char remap = 1;
unsigned char retv;
double speed = 1.0;
char title[256];
FILE* svram_f = NULL;
FILE* ram_f = NULL;
FILE* svrom_f = NULL;

if(argc < 2)return crash_halp("not enough arguments\n");

while(argv[rom_file] != NULL && argv[rom_file][0] == '-'){
	if(isalpha(argv[rom_file][1]))switch (argv[rom_file][1]){
		case 's':
			if(argv[rom_file + 1] == NULL || atoi(argv[rom_file + 1]) <= 0 || atoi(argv[rom_file + 1]) > 16){
				if(atoi(argv[rom_file]+2) <= 0 || atoi(argv[rom_file]+2) > 16)return crash_halp("invalid scale\n");
				scale = atoi(argv[rom_file]+2);
				break;
			}
			rom_file++;
			scale = atoi(argv[rom_file]);
			break;
		case 'd':
			if(argv[rom_file + 1] == NULL || strtol(argv[rom_file + 1],NULL,0) == 0){
				if(strtol(argv[rom_file]+2,NULL,0) == 0)return crash_halp("invalid dip switch\n");
				dip = strtol(argv[rom_file]+2,NULL,0);
				break;
			}
			rom_file++;
			dip = strtol(argv[rom_file],NULL,0);
			break;
		case 'x':
			if(argv[rom_file + 1] == NULL || strtod(argv[rom_file + 1],NULL) == 0.0){
				if(strtod(argv[rom_file]+2,NULL) == 0.0)return crash_halp("invalid speed\n");
				speed = strtod(argv[rom_file]+2, NULL);
				break;
			}
			rom_file++;
			speed = strtod(argv[rom_file],NULL);
			break;
		case 'p':
			if(argv[rom_file + 1] == NULL && argv[rom_file + 2] == NULL) break;
			rom_file++;
			printer_f = fopen(argv[rom_file],"ab");
			if(printer_f == NULL)return crash_halp("not a valid printer file\n");
			break;
#ifdef SERIAL_IO
		case 'i':
			if(argv[rom_file + 1] == NULL && argv[rom_file + 2] == NULL) break;
			rom_file++;
			serial_fr = fopen(argv[rom_file],"r+b");
			if(serial_fr == NULL)return crash_halp("not a valid serial input file\n");
			break;
		case 'o':
			if(argv[rom_file + 1] == NULL && argv[rom_file + 2] == NULL) break;
			rom_file++;
			serial_fw = fopen(argv[rom_file],"w+b");
			if(serial_fw == NULL)return crash_halp("not a valid serial output file\n");
			break;
		case 't':
			translate = 1;
			break;
#endif
		case 'c':
			if(argv[rom_file + 1] == NULL && argv[rom_file + 2] == NULL) break;
			rom_file++;
			svrom_f = fopen(argv[rom_file],"rb");
			if(svrom_f == NULL)return crash_halp("not a valid svrom file\n");
			break;
		case 'n':
			if(argv[rom_file + 1] == NULL && argv[rom_file + 2] == NULL) break;
			rom_file++;
			svram_f = fopen(argv[rom_file],"r+b");
			if(svram_f == NULL)svram_f = fopen(argv[rom_file],"w+b");
			if(svram_f == NULL)return crash_halp("not a valid svram file\n");
			break;
		case 'b':
			if(argv[rom_file + 1] == NULL && argv[rom_file + 2] == NULL) break;
			rom_file++;
			ram_f = fopen(argv[rom_file],"r+b");
			if(ram_f == NULL)ram_f = fopen(argv[rom_file],"w+b");
			if(ram_f == NULL)return crash_halp("not a valid ram file\n");
			break;
		case 'r':
			remap = 0;
			break;
	}
	rom_file++;
}

/*try to open the file*/
in_rom_f = fopen(argv[rom_file],"rb");
if(in_rom_f == NULL)return crash_halp("not a valid file\n");

/*get the rom*/
retv = fread(main_rom, 262144, 1, in_rom_f);
if(retv != 1)return crash_halp("can't read main rom\n");
fclose(in_rom_f);

/*get the svrom if any*/
if(svrom_f != NULL){
	retv = fread(svrom, 131072, 1, svrom_f);
	if(retv != 1)return crash_halp("can't read svrom\n");
	fclose(svrom_f);
}

/*get the svram if any*/
if(svram_f != NULL){
	retv = fread(svram, 8192, 1, svram_f);
	if(retv != 1)fprintf(stderr,"can't read svram, assuming new file\n");
}

/*get the ram if any*/
if(ram_f != NULL){
	retv = fread(main_ram, RAM_SIZE, 1, ram_f);
	if(retv != 1)fprintf(stderr,"can't read main ram, assuming new file\n");
	else m68k_set_irq(7);
}

#ifdef SERIAL_IO
/*set polling for serial if any*/
if(serial_fr != NULL){
	fds[0].fd = fileno(serial_fr);
	fds[0].events = POLLIN;
	setvbuf(serial_fr, NULL, _IONBF,0);
}
if(serial_fw != NULL)setvbuf(serial_fw, NULL, _IONBF,0);
#endif

/*init emulated cpu*/
m68k_init();
m68k_set_cpu_type(M68K_CPU_TYPE_68000);
m68k_pulse_reset();
m68k_get_context(cpu);

#ifdef SDL_UI
/*behold, the sdl horrors*/
SDL_Renderer *renderer;
SDL_Texture *texture;
SDL_Event event;

SDL_Init(SDL_INIT_VIDEO);
snprintf(title, 256, "kya! copycat da! (%dx scale, %4.2F%% speed)", scale, speed*100);
SDL_Window *window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, DISP_W * scale, DISP_H * scale, 0);
if (!window)return crash("SDL window");

renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
if (!renderer)return crash("SDL renderer");
SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB332, SDL_TEXTUREACCESS_STATIC, DISP_W, DISP_H);
if (!texture)return crash("SDL texture");
/*end of the sdl horrors, you can take a breath*/
#endif

unsigned char run = 1;

#ifdef FASTER_VIDEO
update_framebuffer();
vram_touched = 1;
#endif

while(run){
#ifdef UNIX_TIMING
	struct timeval start, end;
	gettimeofday(&start, NULL);
#elif defined(SDL_UI)
	Uint64 start, end;
	start = SDL_GetTicks64();
#endif

	m68k_execute(CPU_CYCLES);/*will execute for 6.5ms, trigger timer afterwards*/
	watchdog++;
	trg++;

	/*watchdog is set to an arbitrary value*/
	if(watchdog > 64){
		fprintf(stderr, "watchdog has been triggered, the cat has crashed :/\n");
		run = 0;
	}
	
	{
		//simulate the 6.5ms timer interrupt condition
		duart_isr |= 0x80;
		duart_ipcr = 0x40 | ((trg%2) << 2);
		m68k_set_irq(1);
	}
	
#ifdef DEBUG_V
	if(trg == DEGUG_V_TRG){
		printf("P1 %d %d \n", DISP_W, DISP_H);
		for(int i=0; i<28896; i++)printf(BYTE_TO_BIN_PATTERN,BYTE_TO_BIN(main_ram[i]));
		return 1;
	}
#endif

#ifdef SDL_UI
	/*
	 * the display logic would be annoying to implement and perform poorly so this is the compromise
	 * the refresh is only done when a write to video memory has occured
	 */
	if(!v_blank){
		if( /*(trg % 2) &&*/ vram_touched ){
		#ifndef FASTER_VIDEO
		update_framebuffer();
		#endif
		SDL_UpdateTexture(texture, NULL, framebuffer, DISP_W);
		/* Scales texture up to window size */
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
		vram_touched = 0;
		}
	}else{
		if(v_blank == 1){
			SDL_RenderClear(renderer);
			v_blank++;
		}
	}

	if (SDL_PollEvent(&event)) {
		switch(event.type){
			case SDL_QUIT:
				run = 0;

			/*separate treatment whether the key is an alphabetic char or not to accomodate layouts a bit*/
			case SDL_KEYDOWN:
			case SDL_KEYUP: {
				const unsigned char state = (event.type == SDL_KEYUP);
				const unsigned int sym = event.key.keysym.sym;
				if( remap && ((sym <= 0x5A && sym >=0x41) || (sym <= 0x7A && sym >=0x61)) )handle_sdl_sym(sym, state);
				else handle_sdl_scancode(event.key.keysym.scancode, state);
			} break;
		}
	}
#endif

#ifdef SERIAL_IO
		if(serial_fr != NULL && !(duart_sra & 1)){
			poll(fds,1,1);
			if(fds[0].revents & POLLIN){
				int c = fgetc(serial_fr);
				if(c == EOF){
					fprintf(stderr, "reached EOF on serial input, closing it\n");
					fclose(serial_fr);
					serial_fr = NULL;
				}else{
					if(translate && c == '\n')c = '\r';
					duart_rba = c;
					duart_sra |= 0x01;
					duart_isr |= 0x02;
				}
				/*m68k_set_irq(1);*/
			}
		}
#endif

#ifdef UNIX_TIMING
	uint64_t then = (start.tv_sec * 1000000) + start.tv_usec;
	gettimeofday(&end, NULL);
	uint64_t now = (end.tv_sec * 1000000) + end.tv_usec;
	uint32_t total_usec = now-then;
	if(total_usec < (int)(6541/speed))usleep((int)(6541/speed) - total_usec);
#elif defined(SDL_UI)
	end = SDL_GetTicks64();
	Uint32 total_ms = end-start;
	if(total_ms < (int)(6/speed))SDL_Delay((int)(6/speed)-total_ms);
#endif
}

/*save svram if any*/
if(svram_f != NULL){
	rewind(svram_f);
	retv = fwrite(svram, 8192, 1, svram_f);
	if(retv != 1)fprintf(stderr,"couldn't write svram\n");
	fclose(svram_f);
}

/*save ram if any*/
if(ram_f != NULL){
	rewind(ram_f);
	retv = fwrite(main_ram, RAM_SIZE, 1, ram_f);
	if(retv != 1)fprintf(stderr,"couldn't write ram\n");
	fclose(ram_f);
}

if(printer_f != NULL)fclose(printer_f);

#ifdef SERIAL_IO
if(serial_fr != NULL)fclose(serial_fr);
if(serial_fw != NULL)fclose(serial_fw);
#endif

return 0;

}
