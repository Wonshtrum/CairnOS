#ifndef __PRINT_UDP_H__
#define __PRINT_UDP_H__

#include "utils/types.h"
#include "utils/io.h"
#include "drivers/keyboard.h"
#include "drivers/keyCodes.h"
#include "net/udp.h"

#define MAX_BUFFER_LENGTH 256


class Console_UDP: public Keyboard_event_handler, public User_datagram_protocol_handler {
private:
	char buffer[MAX_BUFFER_LENGTH];
	int32_t cursor;

public:
	Console_UDP();
	~Console_UDP();

	virtual void on_key_down(char key) override;
	virtual void on_receive(uint8_t* payload, uint32_t size);
};


#endif
