#include "arp.h"


Address_resolution_protocol::Address_resolution_protocol(Ethernet_frame_provider* backend):
	Ethernet_frame_handler(backend, 0x0806),
	num_entries(0) {}
Address_resolution_protocol::~Address_resolution_protocol() {}

bool Address_resolution_protocol::update_cache(uint32_t ip, uint64_t mac) {
	for (uint32_t i = 0 ; i < num_entries ; i++) {
		if (ip_cache[i] == ip) {
			mac_cache[i] = mac;
			return true;
		}
	}
	if (num_entries >= MAX_CACHE_ENTRIES) {
		return false;
	}
	ip_cache[num_entries] = ip;
	mac_cache[num_entries] = mac;
	num_entries++;
	return true;
}

bool Address_resolution_protocol::on_receive(uint8_t* payload, uint32_t size) {
	if (size < sizeof(ARP_frame)) {
		return false;
	}

	ARP_frame* arp = (ARP_frame*)payload;
	if (arp->hardware_type == 0x0100
	&& arp->protocol == 0x0008
	&& arp->hardware_addr_size == 6
	&& arp->protocol_addr_size == 4
	&& arp->dst_ip == backend->get_ip()) {
		switch (arp->command) {
			case 0x0100:	// request
				arp->command = 0x0200;
				arp->dst_ip = arp->src_ip;
				arp->dst_mac = arp->src_mac;
				arp->src_ip = backend->get_ip();
				arp->src_mac = backend->get_mac();
				return true;
			case 0x0200:	// response
				update_cache(arp->src_ip, arp->src_mac);
				break;
		}
	}

	return false;
}

void Address_resolution_protocol::request_mac(uint32_t ip) {
	ARP_frame arp;
	arp.hardware_type = 0x0100;	// ethernet
	arp.protocol = 0x0008;		// ipv4
	arp.hardware_addr_size = 6;	// mac
	arp.protocol_addr_size = 4;	// ipv4
	arp.command = 0x0100;		// request
	arp.src_mac = backend->get_mac();
	arp.src_ip = backend->get_ip();
	arp.dst_mac = MAC_BROADCAST;
	arp.dst_ip = ip;

	send(arp.dst_mac, (uint8_t*)&arp, sizeof(ARP_frame));
}

void Address_resolution_protocol::broadcast_mac(uint32_t mask) {
	ARP_frame arp;
	arp.hardware_type = 0x0100;	// ethernet
	arp.protocol = 0x0008;		// ipv4
	arp.hardware_addr_size = 6;	// mac
	arp.protocol_addr_size = 4;	// ipv4
	arp.command = 0x0200;		// "reponse"
	arp.src_mac = backend->get_mac();
	arp.src_ip = backend->get_ip();
	arp.dst_mac = MAC_BROADCAST;
	arp.dst_ip = broadcast_ip(backend->get_ip(), mask);

	send(arp.dst_mac, (uint8_t*)&arp, sizeof(ARP_frame));
}

uint64_t Address_resolution_protocol::get_mac(uint32_t ip) {
	for (uint32_t i = 0 ; i < num_entries ; i++) {
		if (ip_cache[i] == ip) {
			return mac_cache[i];
		}
	}
	return MAC_BROADCAST;
}

uint64_t Address_resolution_protocol::resolve(uint32_t ip) {
	uint64_t result = get_mac(ip);
	if (result == MAC_BROADCAST) {
		print_str("MAC not in cache\n");
		request_mac(ip);
	} else {
		print_str("MAC in cache\n");
	}
	while (result == MAC_BROADCAST) {
		result = get_mac(ip);
	}
	return result;
}
