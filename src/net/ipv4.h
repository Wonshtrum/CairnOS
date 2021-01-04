#ifndef __IPV4_H__
#define __IPV4_H__

#include "utils/types.h"
#include "utils/bits.h"
#include "memory/memory.h"
#include "net/ethernet.h"
#include "net/ethernetFrame.h"
#include "net/arp.h"


struct IPv4_frame {
	uint8_t header_length: 4;	// no options -> 
	uint8_t version: 4;		// ipv4 -> 4
	uint8_t tos;
	uint16_t total_length;
	uint16_t identification;
	uint8_t flags: 3;
	uint16_t offset: 13;
	uint8_t ttl;
	uint8_t protocol;
	uint16_t crc;
	uint32_t src_ip;
	uint32_t dst_ip;
} __attribute__((packed));


class Internet_protocol_provider;
class Internet_protocol_handler {
protected:
	Internet_protocol_provider* backend;
	uint8_t protocol;

public:
	Internet_protocol_handler(Internet_protocol_provider* backend, uint8_t protocol);
	~Internet_protocol_handler();

	virtual bool on_receive(uint32_t src_ip, uint32_t dst_ip, uint8_t* payload, uint32_t size);
	void send(uint32_t dst_ip, uint8_t* payload, uint32_t size);
};


class Internet_protocol_provider: public Ethernet_frame_handler {
friend class Internet_protocol_handler;
private:
	Internet_protocol_handler* handlers[256];
	Address_resolution_protocol* arp;
	uint32_t gw_ip;
	uint32_t mask;
	
public:
	Internet_protocol_provider(Ethernet_frame_provider* backend, Address_resolution_protocol* arp, uint32_t gw_ip, uint32_t mask);
	~Internet_protocol_provider();

	virtual bool on_receive(uint8_t* buffer, uint32_t size) override;
	void send(uint32_t dst_ip, uint8_t protocol, uint8_t* buffer, uint32_t size);

	static uint16_t check_sum(uint16_t* data, uint32_t size);
};


#endif
