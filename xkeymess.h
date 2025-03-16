/*keymaps for sdl*/

void handle_xlib_char(unsigned char sym, const unsigned char state){
	sym = sym & 0x5F;//uppercase only
	//printf("%c\n",sym);
	switch(sym){
		case 'N': change_keyreg_state(0, 0, state); break;
		case 'M': change_keyreg_state(0, 1, state); break;
		case 'K': change_keyreg_state(0, 2, state); break;
		case 'J': change_keyreg_state(0, 3, state); break;
		case 'Y': change_keyreg_state(0, 4, state); break;
		case 'T': change_keyreg_state(0, 5, state); break;
		case 'B': change_keyreg_state(1, 0, state); break;
		case 'L': change_keyreg_state(1, 2, state); break;
		case 'H': change_keyreg_state(1, 3, state); break;
		case 'U': change_keyreg_state(1, 4, state); break;
		case 'R': change_keyreg_state(1, 5, state); break;
		case 'V': change_keyreg_state(2, 0, state); break;
		case 'G': change_keyreg_state(2, 3, state); break;
		case 'I': change_keyreg_state(2, 4, state); break;
		case 'C': change_keyreg_state(3, 0, state); break;
		case 'F': change_keyreg_state(3, 3, state); break;
		case 'O': change_keyreg_state(3, 4, state); break;
		case 'E': change_keyreg_state(3, 5, state); break;
		case 'X': change_keyreg_state(4, 0, state); break;
		case 'D': change_keyreg_state(4, 3, state); break;
		case 'P': change_keyreg_state(4, 4, state); break;
		case 'W': change_keyreg_state(4, 5, state); break;
		case 'Z': change_keyreg_state(5, 0, state); break;
		case 'S': change_keyreg_state(5, 3, state); break;
		case 'Q': change_keyreg_state(5, 5, state); break;
		case 'A': change_keyreg_state(6, 3, state); break;
	}
}

/*keycodes were obtained using xev*/
void handle_xlib_code(const int scancode, const unsigned char state){
	if(scancode < 1 || scancode > XK_VoidSymbol)return;
	switch(scancode){
		case 57: change_keyreg_state(0, 0, state); break;
		case 58: change_keyreg_state(0, 1, state); break;
		case 45: change_keyreg_state(0, 2, state); break;
		case 44: change_keyreg_state(0, 3, state); break;
		case 29: change_keyreg_state(0, 4, state); break;
		case 28: change_keyreg_state(0, 5, state); break;
		case 56: change_keyreg_state(1, 0, state); break;
		case 46: change_keyreg_state(1, 2, state); break;
		case 43: change_keyreg_state(1, 3, state); break;
		case 30: change_keyreg_state(1, 4, state); break;
		case 27: change_keyreg_state(1, 5, state); break;
		case 55: change_keyreg_state(2, 0, state); break;
		case 42: change_keyreg_state(2, 3, state); break;
		case 31: change_keyreg_state(2, 4, state); break;
		case 54: change_keyreg_state(3, 0, state); break;
		case 41: change_keyreg_state(3, 3, state); break;
		case 32: change_keyreg_state(3, 4, state); break;
		case 26: change_keyreg_state(3, 5, state); break;
		case 53: change_keyreg_state(4, 0, state); break;
		case 40: change_keyreg_state(4, 3, state); break;
		case 33: change_keyreg_state(4, 4, state); break;
		case 25: change_keyreg_state(4, 5, state); break;
		case 52: change_keyreg_state(5, 0, state); break;
		case 39: change_keyreg_state(5, 3, state); break;
		case 24: change_keyreg_state(5, 5, state); break;
		case 38: change_keyreg_state(6, 3, state); break;
		
		case 10: case 87:
			change_keyreg_state(5, 7, state); break;
		case 11: case 88:
			change_keyreg_state(4, 7, state); break;
		case 12: case 89:
			change_keyreg_state(2, 7, state); break;
		case 13: case 83:
			change_keyreg_state(1, 7, state); break;
		case 14: case 84:
			change_keyreg_state(0, 7, state); break;
		case 15: case 85:
			change_keyreg_state(0, 6, state); break;
		case 16: case 79:
			change_keyreg_state(1, 6, state); break;
		case 17: case 80:
			change_keyreg_state(2, 6, state); break;
		case 18: case 81:
			change_keyreg_state(3, 6, state); break;
		case 19: case 90:
			change_keyreg_state(4, 6, state); break;

		case 59:
			change_keyreg_state(1, 1, state); break;/*,*/
		case 60: case 91:
			change_keyreg_state(2, 1, state); break;/*.*/
		case 48:
			change_keyreg_state(2, 2, state); break;/*;, :*/
		case 51:
			change_keyreg_state(3, 2, state); break;/*', "*/
		case 34: case 99:
			change_keyreg_state(5, 4, state); break;/*1/2, 1/4*/
		case 20: case 82:
			change_keyreg_state(5, 6, state); break;/*-, _*/
		case 61: case 112:
			change_keyreg_state(6, 2, state); break;/*/, ?*/
		case 35:
			change_keyreg_state(6, 4, state); break;/*[, ]*/
		case 21:
			change_keyreg_state(6, 6, state); break;/*=, +*/
		case 49:
			change_keyreg_state(7, 7, state); break;/*+-, °*/

		case 64:
			change_keyreg_state(3, 1, state); break;/*left use front*/
		case 113: case 108:
			change_keyreg_state(4, 1, state); break;/*right use front*/
		case 65:
			change_keyreg_state(5, 1, state); break;/*space*/
		case 36:
			change_keyreg_state(5, 2, state); break;/*return*/
		case 37:
			change_keyreg_state(6, 1, state); break;/*left leap*/
		case XK_Tab:
			change_keyreg_state(6, 5, state); break;/*tab*/
		case 50: case 62:
			change_keyreg_state(7, 0, state); break;/*shift*/
		case 105:
			change_keyreg_state(7, 1, state); break;/*right leap*/
		case 115: /*case XK_Menu:*/
			change_keyreg_state(7, 2, state); break;/*document, page*/
		case 66:
			change_keyreg_state(7, 3, state); break;/*lock*/
		case 22:
			change_keyreg_state(7, 4, state); break;/*erase*/
		case 110:
			change_keyreg_state(7, 6, state); break;/*undo*/
		default: fprintf(stderr,"unhandled key %d\n", scancode);
	}
}
