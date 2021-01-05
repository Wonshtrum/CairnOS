#include "keyCodes.h"


char decode_key(char key, bool shift) {
	if (!shift) {
		switch (key) {
			case 0x10: return 'a';
			case 0x11: return 'z';
			case 0x12: return 'e';
			case 0x13: return 'r';
			case 0x14: return 't';
			case 0x15: return 'y';
			case 0x16: return 'u';
			case 0x17: return 'i';
			case 0x18: return 'o';
			case 0x19: return 'p';
			case 0x1e: return 'q';
			case 0x1f: return 's';
			case 0x20: return 'd';
			case 0x21: return 'f';
			case 0x22: return 'g';
			case 0x23: return 'h';
			case 0x24: return 'j';
			case 0x25: return 'k';
			case 0x26: return 'l';
			case 0x27: return 'm';
			case 0x2c: return 'w';
			case 0x2d: return 'x';
			case 0x2e: return 'c';
			case 0x2f: return 'v';
			case 0x30: return 'b';
			case 0x31: return 'n';
			case 0x32: return ',';
			case 0x33: return ';';
			case 0x34: return ':';
			case 0x35: return '!';
			case 0x39: return ' ';
			case 0x1c: return '\n';
			case 0x0e: return DELETE_KEY;
			case 0x2a: return SHIFT_KEY;
			default:   return '_';
		}
	} else {
		switch (key) {
			case 0x10: return 'A';
			case 0x11: return 'Z';
			case 0x12: return 'E';
			case 0x13: return 'R';
			case 0x14: return 'T';
			case 0x15: return 'Y';
			case 0x16: return 'U';
			case 0x17: return 'I';
			case 0x18: return 'O';
			case 0x19: return 'P';
			case 0x1e: return 'Q';
			case 0x1f: return 'S';
			case 0x20: return 'D';
			case 0x21: return 'F';
			case 0x22: return 'G';
			case 0x23: return 'H';
			case 0x24: return 'J';
			case 0x25: return 'K';
			case 0x26: return 'L';
			case 0x27: return 'M';
			case 0x2c: return 'W';
			case 0x2d: return 'X';
			case 0x2e: return 'C';
			case 0x2f: return 'V';
			case 0x30: return 'B';
			case 0x31: return 'N';
			case 0x32: return '?';
			case 0x33: return '.';
			case 0x34: return '/';
			default:   return decode_key(key, false);
		}
	}
}
