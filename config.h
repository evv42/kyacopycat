/*
 * configuration header for kyacopycat compile-time options
 * 
 */

/* debugging options (all output to stdout) */

/*enable disassembling trace
 * also need to enable M68K_INSTRUCTION_HOOK in musashi m68kconf.h and recompile it for that option
 */
//#define DEBUG_I

/*enable output of a PNM image
 *after DEBUG_V_TRG triggers of the 6.5ms timer interrupt
 *will not work properly if combined with any other degug option
 */
//#define DEBUG_V
//#define DEBUG_V_TRG 2500

/*enable display of unhandled memory reads and writes, respectively*/
//#define DEBUG_UR
//#define DEBUG_UW

/*enable display of watchdog value before reset*/
//#define DEBUG_WATCHDOG

/*enable display of the cpu reads to the keyboard buffers*/
//#define DEBUG_KEYB

/*enable display of the duart range reads/writes*/
//#define DEBUG_UART

/*
 * timing option
 * 
 * with UNIX_TIMING enabled the timing will be sligtly more accurate,
 * but will leverage functions not available on some platforms.
 * 
 * if disabled, kyacopycat will rely on the millisecond-based SDL 64-bit timing functions,
 * make sure you are using SDL 2.0.18 or later if so.
 */
#define UNIX_TIMING

/*
 * serial option
 * with SERIAL_IO enabled the emulator will be able to communicate on its serial interface through a read and write file,
 * but will leverage functions not available on some platforms.
 */
#define SERIAL_IO

/*must be a multiple of 131072 for the cat to use it*/
#define RAM_SIZE 262144

/*makes video faster at the expense of possible video artifacts*/
#define FASTER_VIDEO

#define INLINE inline
