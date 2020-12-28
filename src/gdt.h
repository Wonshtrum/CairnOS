#ifndef __GDT_H__
#define __GDT_H__

#include "types.h"


class Global_descriptor_table {
public:
	class Segment_descriptor {
	private:
		uint16_t span_lo;
		uint16_t base_lo;
		uint8_t base_hi;
		uint8_t type;
		uint8_t flags_span_hi;
		uint8_t base_vhi;
	public:
		Segment_descriptor(uint32_t base, uint32_t span, uint8_t type);
		uint32_t get_base();
		uint32_t get_span();
	} __attribute__((packed));

	Segment_descriptor null_segment_selector;
	Segment_descriptor unused_segment_selector;
	Segment_descriptor code_segment_selector;
	Segment_descriptor data_segment_selector;

	Global_descriptor_table();
	~Global_descriptor_table();

	uint16_t get_code_segment_selector();
	uint16_t get_data_segment_selector();
};


#endif
