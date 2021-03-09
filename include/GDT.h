#ifndef __GDT_H
#define __GDT_H

#include "Types.h"

class GlobalDescriptorTable {
public:

    class SegmentDescriptor {
    private:
        uint16_t limit_lo;
        uint16_t base_lo;
        uint8_t base_hi;
        uint8_t type;
        uint8_t limit_hi;
        uint8_t base_vhi;

    public:
        SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t type);

        uint32_t base();

        uint32_t limit();
    } __attribute__((packed));

private:
    SegmentDescriptor null_segment_selector;
    SegmentDescriptor unused_segment_selector;
    SegmentDescriptor code_segment_selector;
    SegmentDescriptor data_segment_selector;

public:

    GlobalDescriptorTable();

    ~GlobalDescriptorTable();

    uint16_t codeSegmentSelector();

    uint16_t dataSegmentSelector();
};

#endif