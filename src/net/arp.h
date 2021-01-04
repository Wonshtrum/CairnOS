#ifndef __APR_H__
#define __APR_H__

#include "utils/types.h"
#include "utils/io.h"
#include "net/utils.h"
#include "net/ethernetFrame.h"
#include "net/ethernet.h"

#define MAX_CACHE_ENTRIES 100


struct ARP_frame {
	uint16_t hardware_type;
	uint16_t protocol;
	uint8_t hardware_addr_size;	// mac  -> 6
	uint8_t protocol_addr_size;	// ipv4 -> 4
	uint16_t command;
	uint64_t src_mac: 48;
	uint32_t src_ip;
	uint64_t dst_mac: 48;
	uint32_t dst_ip;
} __attribute__((packed));


class Address_resolution_protocol: public Ethernet_frame_handler {
private:
	uint32_t ip_cache[MAX_CACHE_ENTRIES];
	uint64_t mac_cache[MAX_CACHE_ENTRIES];
	uint32_t num_entries;

public:
	Address_resolution_protocol(Ethernet_frame_provider* backend);
	~Address_resolution_protocol();

	bool update_cache(uint32_t ip, uint64_t mac);

	virtual bool on_receive(uint8_t* payload, uint32_t size) override;
	void request_mac(uint32_t ip);
	uint64_t get_mac(uint32_t ip);
	uint64_t resolve(uint32_t ip);
	void broadcast_mac(uint32_t mask);
};


#endif
