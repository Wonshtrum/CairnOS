#include "driver.h"


Driver::Driver(): activated(false) {}
Driver::~Driver() {}
void Driver::activate() { activated = true; }
uint32_t Driver::reset() { return 0; }
void Driver::deactivate() { activated = false; }
char* Driver::get_name() { return "Unnamed driver"; }

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
	if (num_drivers >= MAX_NUM_DRIVERS) {
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
	print_str(" DRIVER(S) ACTIVATED:\n");
	for (uint32_t i = 0 ; i < num_drivers ; i++) {
		print_str(" - ");
		print_str(drivers[i]->get_name());
		print_str("\n");
	}
}

Driver* Driver_manager::debug_get(uint32_t index) {
	return drivers[index];
}
