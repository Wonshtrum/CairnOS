#include "driver.h"


Driver::Driver() {}
Driver::~Driver() {}
void Driver::activate() {}
uint32_t Driver::reset() { return 0; }
void Driver::deactivate() {}


Driver_manager* Driver_manager::instance;
Driver_manager::Driver_manager() {
	instance = this;
	num_drivers = 0;
}
Driver_manager::~Driver_manager() {}

Driver_manager* Driver_manager::get() {
	return instance;
}

bool Driver_manager::add_driver(Driver* driver) {
	if (num_drivers >= MAX_NUM_DRIVERS - 1) {
		return false;
	}
	drivers[num_drivers++] = driver;
	return true;
}

void Driver_manager::activate_all() {
	for (uint32_t i = 0 ; i < num_drivers ; i++) {
		drivers[i]->activate();
	}
	print_hex((uint8_t)num_drivers);
	print_str(" DRIVER(S) ACTIVATED\n");
}

Driver* Driver_manager::debug_get(uint32_t index) {
	return drivers[index];
}
