#ifndef __ETHERNET_H__
#define __ETHERNET_H__

#include "utils/types.h"
#include "drivers/driver.h"

#define MAC_BROADCAST 0xFFFFFFFFFFFF


class Driver_ethernet;
class Net_data_handler {
protected:
	Driver_ethernet* backend;

public:
	Net_data_handler(Driver_ethernet* backend);
	~Net_data_handler();

	virtual bool on_receive(uint8_t* buffer, uint32_t size);
	virtual void send(uint8_t* buffer, uint32_t size);
};


class Driver_ethernet: public Driver {
protected:
	Net_data_handler* handler;

public:
	Driver_ethernet();
	~Driver_ethernet();

	virtual uint64_t get_mac();
	virtual void receive();
	virtual void send(uint8_t* buffer, uint32_t size);

	void set_handler(Net_data_handler* handler);
};


#endif
