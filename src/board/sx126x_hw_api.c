#include "board/sx126x_hw_api.h"

#ifndef SIGFOX_EP_DISABLE_FLAGS_FILE
#include "sigfox_ep_flags.h"
#endif
#include "sigfox_error.h"
#include "sigfox_types.h"

SX126X_HW_API_status_t __attribute__((weak)) SX126X_HW_API_open(SX126X_HW_API_config_t *hw_api_config) {
    /* To be implemented by the device manufacturer */
#ifdef SIGFOX_EP_ERROR_CODES
    SX126X_HW_API_status_t status = SX126X_HW_API_SUCCESS;
#endif
    SIGFOX_UNUSED(hw_api_config);
    SIGFOX_RETURN();
}

SX126X_HW_API_status_t __attribute__((weak)) SX126X_HW_API_close(void) {
    /* To be implemented by the device manufacturer */
#ifdef SIGFOX_EP_ERROR_CODES
    SX126X_HW_API_status_t status = SX126X_HW_API_SUCCESS;
#endif
    SIGFOX_RETURN();
}

SX126X_HW_API_status_t __attribute__((weak)) SX126X_HW_API_delayMs(unsigned short delay_ms) {
    /* To be implemented by the device manufacturer */
#ifdef SIGFOX_EP_ERROR_CODES
    SX126X_HW_API_status_t status = SX126X_HW_API_SUCCESS;
#endif
    SIGFOX_UNUSED(delay_ms);
    SIGFOX_RETURN();
}

SX126X_HW_API_status_t __attribute__((weak)) SX126X_HW_API_get_chip_name(SX126X_HW_API_chip_name_t *chipset) {
    /* To be implemented by the device manufacturer */
#ifdef SIGFOX_EP_ERROR_CODES
    SX126X_HW_API_status_t status = SX126X_HW_API_SUCCESS;
#endif
    SIGFOX_UNUSED(chipset);
    SIGFOX_RETURN();
}

SX126X_HW_API_status_t __attribute__((weak)) SX126X_HW_API_get_reg_mode(SX126X_HW_API_reg_mod_t *reg_mode) {
    /* To be implemented by the device manufacturer */
#ifdef SIGFOX_EP_ERROR_CODES
    SX126X_HW_API_status_t status = SX126X_HW_API_SUCCESS;
#endif
    SIGFOX_UNUSED(reg_mode);
    SIGFOX_RETURN();
}

SX126X_HW_API_status_t __attribute__((weak)) SX126X_HW_API_get_xosc_cfg(SX126X_HW_API_xosc_cfg_t *xosc_cfg) {
    /* To be implemented by the device manufacturer */
#ifdef SIGFOX_EP_ERROR_CODES
    SX126X_HW_API_status_t status = SX126X_HW_API_SUCCESS;
#endif
    SIGFOX_UNUSED(xosc_cfg);
    SIGFOX_RETURN();
}

SX126X_HW_API_status_t __attribute__((weak)) SX126X_HW_API_get_pa_pwr_cfg(SX126X_HW_API_pa_pwr_cfg_t *pa_pwr_cfg, sfx_u32 rf_freq_in_hz, sfx_s8 expected_output_pwr_in_dbm) {
    /* To be implemented by the device manufacturer */
#ifdef SIGFOX_EP_ERROR_CODES
    SX126X_HW_API_status_t status = SX126X_HW_API_SUCCESS;
#endif
    SIGFOX_UNUSED(pa_pwr_cfg);
    SIGFOX_UNUSED(rf_freq_in_hz);
    SIGFOX_UNUSED(expected_output_pwr_in_dbm);
    SIGFOX_RETURN();
}

SX126X_HW_API_status_t __attribute__((weak)) SX126X_HW_API_tx_on(void) {
    /* To be implemented by the device manufacturer */
#ifdef SIGFOX_EP_ERROR_CODES
    SX126X_HW_API_status_t status = SX126X_HW_API_SUCCESS;
#endif
    SIGFOX_RETURN();
}

SX126X_HW_API_status_t __attribute__((weak)) SX126X_HW_API_tx_off(void) {
    /* To be implemented by the device manufacturer */
#ifdef SIGFOX_EP_ERROR_CODES
    SX126X_HW_API_status_t status = SX126X_HW_API_SUCCESS;
#endif
    SIGFOX_RETURN();
}

#ifdef SIGFOX_EP_BIDIRECTIONAL
SX126X_HW_API_status_t __attribute__((weak)) SX126X_HW_API_rx_on(void) {
    /* To be implemented by the device manufacturer */
#ifdef SIGFOX_EP_ERROR_CODES
    SX126X_HW_API_status_t status = SX126X_HW_API_SUCCESS;
#endif
    SIGFOX_RETURN();
}
#endif

#ifdef SIGFOX_EP_BIDIRECTIONAL
SX126X_HW_API_status_t __attribute__((weak)) SX126X_HW_API_rx_off(void) {
    /* To be implemented by the device manufacturer */
#ifdef SIGFOX_EP_ERROR_CODES
    SX126X_HW_API_status_t status = SX126X_HW_API_SUCCESS;
#endif
    SIGFOX_RETURN();
}
#endif

#if (defined SIGFOX_EP_TIMER_REQUIRED) && (defined SIGFOX_EP_LATENCY_COMPENSATION)
SX126X_HW_API_status_t __attribute__((weak)) SX126X_HW_API_get_latency(SX126X_HW_API_latency_t latency_type, sfx_u32 *latency_ms) {
    /* To be implemented by the device manufacturer */
#ifdef SIGFOX_EP_ERROR_CODES
    SX126X_HW_API_status_t status = SX126X_HW_API_SUCCESS;
#endif
    SIGFOX_UNUSED(latency_type);
    SIGFOX_UNUSED(latency_ms);
    SIGFOX_RETURN();
}
#endif
