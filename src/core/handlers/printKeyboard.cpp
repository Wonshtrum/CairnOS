#include "printKeyboard.h"


Print_keyboard::Print_keyboard() {}
Print_keyboard::~Print_keyboard() {}

void Print_keyboard::on_key_down(char key) {
	print_str("D");
	print_hex((uint8_t)key);
	print_str(" ");
}

void Print_keyboard::on_key_up(char key) {
	print_str("U");
	print_hex((uint8_t)key);
	print_str(" ");
}
