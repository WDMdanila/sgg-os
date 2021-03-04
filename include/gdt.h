#ifndef SGG_OS_GDT_H
#define SGG_OS_GDT_H

#include "types.h"

class GlobalDescriptorTable {
public:
    class SegmentSelector {
    public:
        SegmentSelector(uint32_t base, uint32_t limit, uint8_t flags);

        uint32_t base();

        uint32_t limit();

    private:
        uint16_t limit_lo;
        uint16_t base_lo;
        uint8_t base_hi;
        uint8_t type;
        uint8_t flags_limit_hi;
        uint8_t base_vhi;
    } __attribute__((packed));

    GlobalDescriptorTable();

    ~GlobalDescriptorTable();

    uint16_t codeSegmentSelector();

    uint16_t dataSegmentSelector();

    SegmentSelector null_segment_selector;
    SegmentSelector unused_segment_selector;
    SegmentSelector code_segment_selector;
    SegmentSelector data_segment_selector;
};

#endif //SGG_OS_GDT_H
