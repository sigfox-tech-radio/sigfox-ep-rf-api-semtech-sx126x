#include "board/sx126x_hw_api.h"
#include "sigfox_error.h"

SX126X_HW_API_status_t __attribute__((weak)) SX126X_HW_API_open(SX126X_HW_irq_cb_t callback)
{
    /*TODO To be implemented by the device manufacturer */
#ifdef ERROR_CODES
    SX126X_HW_API_status_t status = SX126X_HW_API_SUCCESS;
#endif
    SFX_UNUSED(callback);
    // Configure all hardware pin of SX126X chipset.
    // Configure SPI peripherial.
    // Configure interrupt pin to handle radio interrupt.
    // The callback function must be called when such event occurs.
    RETURN();
}

SX126X_HW_API_status_t __attribute__((weak)) SX126X_HW_API_close(void)
{
    /*TODO To be implemented by the device manufacturer */
#ifdef ERROR_CODES
    SX126X_HW_API_status_t status = SX126X_HW_API_SUCCESS;
#endif
    //Release all pins and peripherial opened.
    RETURN();
}

SX126X_HW_API_status_t __attribute__((weak)) SX126X_HW_API_delayMs(unsigned short delay_ms)
{
    /*TODO To be implemented by the device manufacturer */
#ifdef ERROR_CODES
    SX126X_HW_API_status_t status = SX126X_HW_API_SUCCESS;
#endif
    SFX_UNUSED(delay_ms);
    RETURN();
}

SX126X_HW_API_status_t __attribute__((weak)) SX126X_HW_API_get_chip_name(SX126X_HW_API_chip_name_t *chipset)
{
    /*TODO To be implemented by the device manufacturer */
#ifdef ERROR_CODES
    SX126X_HW_API_status_t status = SX126X_HW_API_SUCCESS;
#endif
    SFX_UNUSED(chipset);
    RETURN();
}

SX126X_HW_API_status_t __attribute__((weak)) SX126X_HW_API_get_reg_mode(SX126X_HW_API_reg_mod_t *reg_mode)
{
#ifdef ERROR_CODES
    SX126X_HW_API_status_t status = SX126X_HW_API_SUCCESS;
#endif
    SFX_UNUSED(reg_mode);
    RETURN();
}

SX126X_HW_API_status_t __attribute__((weak)) SX126X_HW_API_get_xosc_cfg(SX126X_HW_API_xosc_cfg_t *xosc_cfg)
{
    /*TODO To be implemented by the device manufacturer */
#ifdef ERROR_CODES
    SX126X_HW_API_status_t status = SX126X_HW_API_SUCCESS;
#endif
    SFX_UNUSED(xosc_cfg);
    RETURN();
}

SX126X_HW_API_status_t __attribute__((weak)) SX126X_HW_API_get_pa_pwr_cfg(SX126X_HW_API_pa_pwr_cfg_t *pa_pwr_cfg, sfx_u32 rf_freq_in_hz, sfx_s8 expected_output_pwr_in_dbm)
{
    /*TODO To be implemented by the device manufacturer */
#ifdef ERROR_CODES
    SX126X_HW_API_status_t status = SX126X_HW_API_SUCCESS;
#endif
    SFX_UNUSED(pa_pwr_cfg);
    SFX_UNUSED(rf_freq_in_hz);
    SFX_UNUSED(expected_output_pwr_in_dbm);
    RETURN();
}

SX126X_HW_API_status_t __attribute__((weak)) SX126X_HW_API_tx_on(void)
{
    /*TODO To be implemented by the device manufacturer */
#ifdef ERROR_CODES
    SX126X_HW_API_status_t status = SX126X_HW_API_SUCCESS;
#endif
    RETURN();
}

SX126X_HW_API_status_t __attribute__((weak)) SX126X_HW_API_tx_off(void)
{
    /*TODO To be implemented by the device manufacturer */
#ifdef ERROR_CODES
    SX126X_HW_API_status_t status = SX126X_HW_API_SUCCESS;
#endif
    RETURN();
}

#ifdef BIDIRECTIONAL
SX126X_HW_API_status_t __attribute__((weak)) SX126X_HW_API_rx_on(void)
{
    /*TODO To be implemented by the device manufacturer */
#ifdef ERROR_CODES
    SX126X_HW_API_status_t status = SX126X_HW_API_SUCCESS;
#endif
    RETURN();
}
#endif

#ifdef BIDIRECTIONAL
SX126X_HW_API_status_t __attribute__((weak)) SX126X_HW_API_rx_off(void)
{
    /*TODO To be implemented by the device manufacturer */
#ifdef ERROR_CODES
    SX126X_HW_API_status_t status = SX126X_HW_API_SUCCESS;
#endif
    RETURN();
}
#endif

#if (defined TIMER_REQUIRED) && (defined LATENCY_COMPENSATION)
SX126X_HW_API_status_t __attribute__((weak)) SX126X_HW_API_get_latency(SX126X_HW_API_latency_t latency_type, sfx_u32 *latency_ms)
{
    /*TODO To be implemented by the device manufacturer */
#ifdef ERROR_CODES
    SX126X_HW_API_status_t status = SX126X_HW_API_SUCCESS;
#endif
    SFX_UNUSED(latency_type);
    SFX_UNUSED(latency_ms);
    RETURN();
}
#endif
