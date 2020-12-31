#ifndef __DRIVER_H__
#define __DRIVER_H__

#include "utils/types.h"
#include "utils/io.h"

#define MAX_NUM_DRIVERS 256


class Driver {
public:
	Driver();
	~Driver();

	virtual void activate();
	virtual uint32_t reset();
	virtual void deactivate();
};


class Driver_manager {
private:
	static Driver_manager* instance;
	Driver* drivers[MAX_NUM_DRIVERS];
	uint8_t num_drivers;

public:
	Driver_manager();
	~Driver_manager();

	bool add_driver(Driver* driver);
	void activate_all();

	static Driver_manager* get();
};


#endif
