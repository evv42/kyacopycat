/*keymaps for sdl*/

void handle_sdl_sym(unsigned char sym, const unsigned char state){
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

void handle_sdl_scancode(const int scancode, const unsigned char state){
	if(scancode < 1 || scancode > SDL_NUM_SCANCODES)return;
	switch(scancode){
		case SDL_SCANCODE_N: change_keyreg_state(0, 0, state); break;
		case SDL_SCANCODE_M: change_keyreg_state(0, 1, state); break;
		case SDL_SCANCODE_K: change_keyreg_state(0, 2, state); break;
		case SDL_SCANCODE_J: change_keyreg_state(0, 3, state); break;
		case SDL_SCANCODE_Y: change_keyreg_state(0, 4, state); break;
		case SDL_SCANCODE_T: change_keyreg_state(0, 5, state); break;
		case SDL_SCANCODE_B: change_keyreg_state(1, 0, state); break;
		case SDL_SCANCODE_L: change_keyreg_state(1, 2, state); break;
		case SDL_SCANCODE_H: change_keyreg_state(1, 3, state); break;
		case SDL_SCANCODE_U: change_keyreg_state(1, 4, state); break;
		case SDL_SCANCODE_R: change_keyreg_state(1, 5, state); break;
		case SDL_SCANCODE_V: change_keyreg_state(2, 0, state); break;
		case SDL_SCANCODE_G: change_keyreg_state(2, 3, state); break;
		case SDL_SCANCODE_I: change_keyreg_state(2, 4, state); break;
		case SDL_SCANCODE_C: change_keyreg_state(3, 0, state); break;
		case SDL_SCANCODE_F: change_keyreg_state(3, 3, state); break;
		case SDL_SCANCODE_O: change_keyreg_state(3, 4, state); break;
		case SDL_SCANCODE_E: change_keyreg_state(3, 5, state); break;
		case SDL_SCANCODE_X: change_keyreg_state(4, 0, state); break;
		case SDL_SCANCODE_D: change_keyreg_state(4, 3, state); break;
		case SDL_SCANCODE_P: change_keyreg_state(4, 4, state); break;
		case SDL_SCANCODE_W: change_keyreg_state(4, 5, state); break;
		case SDL_SCANCODE_Z: change_keyreg_state(5, 0, state); break;
		case SDL_SCANCODE_S: change_keyreg_state(5, 3, state); break;
		case SDL_SCANCODE_Q: change_keyreg_state(5, 5, state); break;
		case SDL_SCANCODE_A: change_keyreg_state(6, 3, state); break;
		
		case SDL_SCANCODE_1: case SDL_SCANCODE_KP_1:
			change_keyreg_state(5, 7, state); break;
		case SDL_SCANCODE_2: case SDL_SCANCODE_KP_2:
			change_keyreg_state(4, 7, state); break;
		case SDL_SCANCODE_3: case SDL_SCANCODE_KP_3:
			change_keyreg_state(2, 7, state); break;
		case SDL_SCANCODE_4: case SDL_SCANCODE_KP_4:
			change_keyreg_state(1, 7, state); break;
		case SDL_SCANCODE_5: case SDL_SCANCODE_KP_5:
			change_keyreg_state(0, 7, state); break;
		case SDL_SCANCODE_6: case SDL_SCANCODE_KP_6:
			change_keyreg_state(0, 6, state); break;
		case SDL_SCANCODE_7: case SDL_SCANCODE_KP_7:
			change_keyreg_state(1, 6, state); break;
		case SDL_SCANCODE_8: case SDL_SCANCODE_KP_8:
			change_keyreg_state(2, 6, state); break;
		case SDL_SCANCODE_9: case SDL_SCANCODE_KP_9:
			change_keyreg_state(3, 6, state); break;
		case SDL_SCANCODE_0: case SDL_SCANCODE_KP_0:
			change_keyreg_state(4, 6, state); break;

		case SDL_SCANCODE_COMMA:
			change_keyreg_state(1, 1, state); break;/*,*/
		case SDL_SCANCODE_PERIOD: case SDL_SCANCODE_KP_PERIOD:
			change_keyreg_state(2, 1, state); break;/*.*/
		case SDL_SCANCODE_APOSTROPHE:
			change_keyreg_state(2, 2, state); break;/*;, :*/
		case SDL_SCANCODE_BACKSLASH:
			change_keyreg_state(3, 2, state); break;/*', "*/
		case SDL_SCANCODE_LEFTBRACKET: case SDL_SCANCODE_PAGEUP:
			change_keyreg_state(5, 4, state); break;/*1/2, 1/4*/
		case SDL_SCANCODE_MINUS: case SDL_SCANCODE_KP_MINUS:
			change_keyreg_state(5, 6, state); break;/*-, _*/
		case SDL_SCANCODE_SLASH: case SDL_SCANCODE_KP_DIVIDE:
			change_keyreg_state(6, 2, state); break;/*/, ?*/
		case SDL_SCANCODE_RIGHTBRACKET:
			change_keyreg_state(6, 4, state); break;/*[, ]*/
		case SDL_SCANCODE_EQUALS:
			change_keyreg_state(6, 6, state); break;/*=, +*/
		case SDL_SCANCODE_GRAVE:
			change_keyreg_state(7, 7, state); break;/*+-, °*/

		case SDL_SCANCODE_LALT:
			change_keyreg_state(3, 1, state); break;/*Left Use Front*/
		case SDL_SCANCODE_RALT:
			change_keyreg_state(4, 1, state); break;/*Right Use Front*/
		case SDL_SCANCODE_SPACE:
			change_keyreg_state(5, 1, state); break;/*Space*/
		case SDL_SCANCODE_RETURN:
			change_keyreg_state(5, 2, state); break;/*Return*/
		case SDL_SCANCODE_LCTRL:
			change_keyreg_state(6, 1, state); break;/*Left Leap*/
		case SDL_SCANCODE_TAB:
			change_keyreg_state(6, 5, state); break;/*Tab*/
		case SDL_SCANCODE_LSHIFT: case SDL_SCANCODE_RSHIFT:
			change_keyreg_state(7, 0, state); break;/*Shift*/
		case SDL_SCANCODE_RCTRL:
			change_keyreg_state(7, 1, state); break;/*Right Leap*/
		case SDL_SCANCODE_END: case SDL_SCANCODE_RGUI:
			change_keyreg_state(7, 2, state); break;/*Document, Page*/
		case SDL_SCANCODE_CAPSLOCK:
			change_keyreg_state(7, 3, state); break;/*Lock*/
		case SDL_SCANCODE_BACKSPACE:
			change_keyreg_state(7, 4, state); break;/*Erase*/
		case SDL_SCANCODE_HOME:
			change_keyreg_state(7, 6, state); break;/*Undo*/
		default: fprintf(stderr,"unhandled key %d\n", scancode);
	}
}
