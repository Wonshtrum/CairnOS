#include "gdt.h"


Global_descriptor_table::Global_descriptor_table():
	null_segment_selector(0, 0, 0),
	unused_segment_selector(0, 0, 0),
	code_segment_selector(0, 64*1024*1024, 0x9A),
	data_segment_selector(0, 64*1024*1024, 0x92) {
	
	uint32_t init[2];
	init[0] = (uint32_t)this;
	init[1] = sizeof(Global_descriptor_table) << 16;

	asm volatile("lgdt (%0)": :"p" (((uint8_t*)init)+2));
}

Global_descriptor_table::~Global_descriptor_table() {}

uint16_t Global_descriptor_table::get_code_segment_selector() {
	return (uint8_t*)&code_segment_selector - (uint8_t*)this;
}
uint16_t Global_descriptor_table::get_data_segment_selector() {
	return (uint8_t*)&data_segment_selector - (uint8_t*)this;
}

Global_descriptor_table::Segment_descriptor::Segment_descriptor(uint32_t base, uint32_t span, uint8_t type) {
	uint8_t* target = (uint8_t*)this;

	if (span <= 2 << 15) {
		target[6] = 0x40;
	} else {
		if ((span & 0xFFFF) != 0xFFFF) {
			span = (span >> 12)-1;
		} else {
			span = span >> 12;
		}
		target[6] = 0xC0;
	}
	//[B, SF|S, F, B, B, B, F, F]
	target[0]  = (span >>  0) & 0xFF;
	target[1]  = (span >>  8) & 0xFF;
	target[2]  = (base >>  0) & 0xFF;
	target[3]  = (base >>  8) & 0xFF;
	target[4]  = (base >> 16) & 0xFF;
	target[5]  = flags_span_hi;
	target[6] |= (span >> 16) & 0x0F;
	target[7]  = (base >> 24) & 0xFF;
}

uint32_t Global_descriptor_table::Segment_descriptor::get_base() {
	uint8_t* target = (uint8_t*)this;
	uint32_t result = target[7];
	result = (result << 8) + target[4];
	result = (result << 8) + target[3];
	result = (result << 8) + target[2];
}

uint32_t Global_descriptor_table::Segment_descriptor::get_span() {
	uint8_t* target = (uint8_t*)this;
	uint32_t result = target[6] & 0x0F;
	result = (result << 8) + target[1];
	result = (result << 8) + target[0];
	if ((target[6] & 0xF0) == 0xC0) {
		result = (result << 12);
	}
}
