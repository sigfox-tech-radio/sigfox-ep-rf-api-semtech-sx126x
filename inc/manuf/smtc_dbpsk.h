#ifndef __SMTC_DBPSK_H__
#define __SMTC_DBPSK_H__
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
* \brief Perform differential encoding for DBPSK
* modulation.
*
* \param [in] data_in
Buffer with input data
*
* \param [in] bpsk_pld_len_in_bits Length of the input
* BPSK frame, in bits
*
* \param [out] data_out Buffer for output data (can
* optionally be the same as data_in, but must have space
* for bpsk_pld_len_in_bits + 2 bits)
*/
void dbpsk_encode_buffer(const uint8_t* data_in, int bpsk_pld_len_in_bits, uint8_t* data_out);

/*!
* \brief Given the length of a BPSK frame, in bits,
* calculate the space necessary to hold the frame after
* differential encoding, in bytes.
*
* \param [in] bpsk_pld_len_in_bits Length of a BPSK frame,
* in bits
*
* \returns
Space required
* for DBPSK frame, after addition of start/stop bits
* [bytes]
*/
static inline int dbpsk_get_pld_len_in_bytes(int bpsk_pld_len_in_bits)
{
    return ( bpsk_pld_len_in_bits + 9 ) >> 3;
}

/*!
* \brief Given the length of a BPSK frame, in bits,
* calculate the space necessary to hold the frame after
* differential encoding, in bits.
*
* \param [in] bpsk_pld_len_in_bits Length of a BPSK frame,
* in bits
*
* \returns
Space required
* for DBPSK frame, after addition of start/stop bits [bits]
*/
static inline int dbpsk_get_pld_len_in_bits(int bpsk_pld_len_in_bits)
{
// Hold the last bit one extra bit-time
    return bpsk_pld_len_in_bits + 2;
}

#ifdef __cplusplus
}
#endif

#endif
// __SMTC_DBPSK_H__
