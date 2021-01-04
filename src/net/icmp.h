#ifndef __ICMP_H__
#define __ICMP_H__

#include "utils/types.h"
#include "utils/io.h"
#include "net/ipv4.h"


struct ICMP_frame {
	uint8_t type;
	uint8_t code;
	uint16_t crc;
	uint32_t data;
} __attribute__((packed));


class Internet_control_message_protocol: public Internet_protocol_handler {
public:
	Internet_control_message_protocol(Internet_protocol_provider* backend);
	~Internet_control_message_protocol();

	virtual bool on_receive(uint32_t src_ip, uint32_t dst_ip, uint8_t* payload, uint32_t size);
	void echo_reply(uint32_t dst_ip);
};


#endif
