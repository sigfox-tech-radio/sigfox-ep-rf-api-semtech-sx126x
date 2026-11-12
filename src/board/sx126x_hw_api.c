/*!*****************************************************************
 * \file    sx126x_rf_api.h
 * \brief   Sigfox SX126X HW API.
 *******************************************************************
 * \copyright
 *
 * Copyright 2022 UnaBiz SAS
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice, this
 *     list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation and/or
 *     other materials provided with the distribution.
 *  3. Neither the name of the copyright holder nor the names of its contributors
 *     may be used to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************/

#include "board/sx126x_hw_api.h"

#ifndef SIGFOX_EP_DISABLE_FLAGS_FILE
#include "sigfox_ep_flags.h"
#endif
#include "sigfox_error.h"
#include "sigfox_types.h"

SX126X_HW_API_status_t __attribute__((weak)) SX126X_HW_API_open(SX126X_HW_API_config_t *hw_api_config) {
    /* To be implemented by the device manufacturer */
#ifdef SIGFOX_EP_ERROR_CODES
    SX126X_HW_API_status_t status = SX126X_HW_API_ERROR;
#endif
    SIGFOX_UNUSED(hw_api_config);
    SIGFOX_RETURN();
}

SX126X_HW_API_status_t __attribute__((weak)) SX126X_HW_API_close(void) {
    /* To be implemented by the device manufacturer */
#ifdef SIGFOX_EP_ERROR_CODES
    SX126X_HW_API_status_t status = SX126X_HW_API_ERROR;
#endif
    SIGFOX_RETURN();
}

SX126X_HW_API_status_t __attribute__((weak)) SX126X_HW_API_delayMs(unsigned short delay_ms) {
    /* To be implemented by the device manufacturer */
#ifdef SIGFOX_EP_ERROR_CODES
    SX126X_HW_API_status_t status = SX126X_HW_API_ERROR;
#endif
    SIGFOX_UNUSED(delay_ms);
    SIGFOX_RETURN();
}

SX126X_HW_API_status_t __attribute__((weak)) SX126X_HW_API_get_chip_name(SX126X_HW_API_chip_name_t *chipset) {
    /* To be implemented by the device manufacturer */
#ifdef SIGFOX_EP_ERROR_CODES
    SX126X_HW_API_status_t status = SX126X_HW_API_ERROR;
#endif
    SIGFOX_UNUSED(chipset);
    SIGFOX_RETURN();
}

SX126X_HW_API_status_t __attribute__((weak)) SX126X_HW_API_get_reg_mode(SX126X_HW_API_reg_mod_t *reg_mode) {
    /* To be implemented by the device manufacturer */
#ifdef SIGFOX_EP_ERROR_CODES
    SX126X_HW_API_status_t status = SX126X_HW_API_ERROR;
#endif
    SIGFOX_UNUSED(reg_mode);
    SIGFOX_RETURN();
}

SX126X_HW_API_status_t __attribute__((weak)) SX126X_HW_API_get_xosc_cfg(SX126X_HW_API_xosc_cfg_t *xosc_cfg) {
    /* To be implemented by the device manufacturer */
#ifdef SIGFOX_EP_ERROR_CODES
    SX126X_HW_API_status_t status = SX126X_HW_API_ERROR;
#endif
    SIGFOX_UNUSED(xosc_cfg);
    SIGFOX_RETURN();
}

SX126X_HW_API_status_t __attribute__((weak)) SX126X_HW_API_get_pa_pwr_cfg(SX126X_HW_API_pa_pwr_cfg_t *pa_pwr_cfg, sfx_u32 rf_freq_in_hz, sfx_s8 expected_output_pwr_in_dbm) {
    /* To be implemented by the device manufacturer */
#ifdef SIGFOX_EP_ERROR_CODES
    SX126X_HW_API_status_t status = SX126X_HW_API_ERROR;
#endif
    SIGFOX_UNUSED(pa_pwr_cfg);
    SIGFOX_UNUSED(rf_freq_in_hz);
    SIGFOX_UNUSED(expected_output_pwr_in_dbm);
    SIGFOX_RETURN();
}

SX126X_HW_API_status_t __attribute__((weak)) SX126X_HW_API_tx_on(void) {
    /* To be implemented by the device manufacturer */
#ifdef SIGFOX_EP_ERROR_CODES
    SX126X_HW_API_status_t status = SX126X_HW_API_ERROR;
#endif
    SIGFOX_RETURN();
}

SX126X_HW_API_status_t __attribute__((weak)) SX126X_HW_API_tx_off(void) {
    /* To be implemented by the device manufacturer */
#ifdef SIGFOX_EP_ERROR_CODES
    SX126X_HW_API_status_t status = SX126X_HW_API_ERROR;
#endif
    SIGFOX_RETURN();
}

#ifdef SIGFOX_EP_BIDIRECTIONAL
SX126X_HW_API_status_t __attribute__((weak)) SX126X_HW_API_rx_on(void) {
    /* To be implemented by the device manufacturer */
#ifdef SIGFOX_EP_ERROR_CODES
    SX126X_HW_API_status_t status = SX126X_HW_API_ERROR;
#endif
    SIGFOX_RETURN();
}
#endif

#ifdef SIGFOX_EP_BIDIRECTIONAL
SX126X_HW_API_status_t __attribute__((weak)) SX126X_HW_API_rx_off(void) {
    /* To be implemented by the device manufacturer */
#ifdef SIGFOX_EP_ERROR_CODES
    SX126X_HW_API_status_t status = SX126X_HW_API_ERROR;
#endif
    SIGFOX_RETURN();
}
#endif

#if (defined SIGFOX_EP_TIMER_REQUIRED) && (defined SIGFOX_EP_LATENCY_COMPENSATION)
SX126X_HW_API_status_t __attribute__((weak)) SX126X_HW_API_get_latency(SX126X_HW_API_latency_t latency_type, sfx_u32 *latency_ms) {
    /* To be implemented by the device manufacturer */
#ifdef SIGFOX_EP_ERROR_CODES
    SX126X_HW_API_status_t status = SX126X_HW_API_ERROR;
#endif
    SIGFOX_UNUSED(latency_type);
    SIGFOX_UNUSED(latency_ms);
    SIGFOX_RETURN();
}
#endif
