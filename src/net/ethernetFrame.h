#ifndef __ETHER_FRAME_H__
#define __ETHER_FRAME_H__

#include "utils/types.h"
#include "utils/io.h"
#include "memory/memory.h"
#include "net/ethernet.h"

#define MAX_ETHER_TYPE_HANDLERS 100


struct Ethernet_frame_header {
	uint64_t dst_mac: 48;
	uint64_t src_mac: 48;
	uint16_t ether_type;
} __attribute__((packed));
typedef uint32_t Ethernet_frame_footer;


class Ethernet_frame_provider;
class Ethernet_frame_handler {
protected:
	uint16_t ether_type;
	Ethernet_frame_provider* backend;

public:
	Ethernet_frame_handler(Ethernet_frame_provider* backend, uint16_t ether_type);
	~Ethernet_frame_handler();

	uint32_t get_mac();
	uint32_t get_ip();
	virtual bool on_receive(uint8_t* payload, uint32_t size);
	void send(uint64_t dst_mac, uint8_t* payload, uint32_t size);

	uint16_t get_ether_type();
};


class Ethernet_frame_provider: public Net_data_handler {
friend Ethernet_frame_handler;
private:
	Ethernet_frame_handler* handlers[MAX_ETHER_TYPE_HANDLERS];
	uint32_t num_handlers;

public:
	Ethernet_frame_provider(Driver_ethernet* backend);
	~Ethernet_frame_provider();

	virtual bool on_receive(uint8_t* buffer, uint32_t size) override;
	void send(uint64_t dst_mac, uint16_t ether_type, uint8_t* buffer, uint32_t size);

	bool add_handler(Ethernet_frame_handler* handler);
};


#endif
