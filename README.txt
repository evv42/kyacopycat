this is kyacopycat, a canon cat emulator

this emulator tries to provide a more enjoyable experience compared to the MAME 
emulator, notably:

- an integer scaled interface
- the ability to save and restore the system memory
- an optional layout remapping for the alphabetic keys for users of non-QWERTY 
keyboards
- if a beep occur it does not crash the system
- the emulator does not needlessly grab the mouse since this machine never ever 
came with one
- the ability to interface the emulator through the printer and serial ports

some features are yet to be implemented, notably the bell emitting a sound and 
floppy drive support, the latter being mostly redundant with the ram backup 
feature

HOW TO RUN

to run this emulator some knowledge about how a command-line interface works is 
required. once this knowledge has been acquired run the kyacopycat program 
without any option for a view of the available command options

you will notably need a canon cat system rom at the minimum, this emulator uses 
a combined rom file for convinience. such a file may be obtained by looking for 
"canon cat combined romset" on the Internet Archive

for usage of the canon cat please refer to the many documents also hosted on 
the Internet Archive

for an example: this document has been written on the emulator itself, using 
the following command:
./kyacopycat -n svram -b example_readme -o README.txt -t r174.rom

KEYBOARD

most of the keys are where you would expect them, a picture of the canon cat 
keys is included (keyboard.png)

concerning the special keys, the "USE FRONT" keys are mapped to the Alt ones, 
the "LEAP" keys to the Control ones, the "UNDO" key to the Home key and the 
"DOCUMENT/PAGE" key to the End key

HOW TO BUILD

for building this emulator you will need a UNIX system with a copy of the SDL2 
library and a working C compiler
note that the Musashi 68K emulator won't compile correctly with BSD make

please consult the config.h header and the build.sh script for more information

LICENSE

this program is licensed under the GNU GPL2 license only
