#include "manuf/smtc_dbpsk.h"

void dbpsk_encode_buffer(const uint8_t* data_in, int bpsk_pld_len_in_bits, uint8_t* data_out)
{
    uint8_t in_byte = 0x00;
    uint8_t out_byte = 0x00;
    int data_in_bytecount = bpsk_pld_len_in_bits >> 3;
    in_byte = *data_in++;
    uint8_t current = 0;

    // Process full bytes
    while (--data_in_bytecount >= 0) {
        for ( int i = 0; i < 8; ++i) {
                out_byte = ( out_byte << 1) | current;
                if ((in_byte & 0x80) == 0) {
                    current = current ^ 0x01;
                }
                in_byte <<= 1;
        }
        in_byte = *data_in++;
        *data_out++ = out_byte;
    }
    // Process remaining bits
    for (int i = 0; i < ( bpsk_pld_len_in_bits & 7 ); ++i) {
        out_byte = ( out_byte << 1 ) | current;
        if ((in_byte & 0x80 ) == 0) {
            current = current ^ 0x01;
        }
        in_byte <<= 1;
    }
    // Process last data bit
    out_byte = ( out_byte << 1 ) | current;
    if ((bpsk_pld_len_in_bits & 7 ) == 7) {
        *data_out++ = out_byte;
    }
    // Add duplicate bit and store
    out_byte = ( out_byte << 1 ) | current;
    *data_out = out_byte << ( 7 - ( ( bpsk_pld_len_in_bits + 1 ) & 7 ) );
}
