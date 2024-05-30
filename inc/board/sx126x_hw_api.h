/*!*****************************************************************
 * \file    sx126x_hw_api.h
 * \brief   Sigfox sx126x HW api.
 *******************************************************************
 * \copyright
 *
 * Copyright (c) 2022, UnaBiz SAS
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  1 Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  2 Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  3 Neither the name of UnaBiz SAS nor the names of its contributors may be
 *    used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY
 * THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************/
 
#ifndef __SX126X_HW_H__
#define __SX126X_HW_H__

#ifdef USE_SIGFOX_EP_FLAGS_H
#include "sigfox_ep_flags.h"
#endif
#include "sigfox_types.h"

/*** SX126X HW API structures ***/


#ifdef ERROR_CODES
/*!******************************************************************
 * \enum SX126X_HW_API_status_t
 * \brief SX126X HW driver error codes.
 *******************************************************************/
typedef enum {
    SX126X_HW_API_SUCCESS = 0,
    SX126X_HW_API_ERROR,
    // Additional custom error codes can be added here (up to sfx_u32).
    // They will be logged in the library error stack if the ERROR_STACK flag is defined (SIGFOX_ERROR_SOURCE_HW base).
    // Last index.
    SX126X_HW_API_ERROR_LAST
} SX126X_HW_API_status_t;
#else
typedef void SX126X_HW_API_status_t;
#endif

/*!******************************************************************
 * \enum SX126X_HW_API_chip_name_t
 * \brief TODO
 *******************************************************************/
typedef enum
{
	SX126X_HW_API_CHIP_NAME_SX1261    = 0x00,
	SX126X_HW_API_CHIP_NAME_SX1262    = 0x01,
	SX126X_HW_API_CHIP_NAME_LAST,
} SX126X_HW_API_chip_name_t;

/*!******************************************************************
 * \enum SX126X_HW_API_reg_mod_t
 * \brief Sx126x regulator mode
 *******************************************************************/
typedef enum
{
	SX126X_HW_API_REG_MODE_LDO  = 0x00,  // default
	SX126X_HW_API_REG_MODE_DCDC = 0x01,
	SX126X_HW_API_REG_MODE_LAST,
} SX126X_HW_API_reg_mod_t;


/*!******************************************************************
 * \enum SX126X_HW_API_tcxo_ctrl_t
 * \brief TODO
 *******************************************************************/
typedef enum
{
	SX126X_HW_API_TCXO_CTRL_1_6V = 0x00,
	SX126X_HW_API_TCXO_CTRL_1_7V ,
	SX126X_HW_API_TCXO_CTRL_1_8V ,
	SX126X_HW_API_TCXO_CTRL_2_2V ,
	SX126X_HW_API_TCXO_CTRL_2_4V ,
	SX126X_HW_API_TCXO_CTRL_2_7V ,
	SX126X_HW_API_TCXO_CTRL_3_0V ,
	SX126X_HW_API_TCXO_CTRL_3_3V ,
	SX126X_HW_API_TCXO_CTRL_LAST,
} SX126X_HW_API_tcxo_ctrl_t;

/*!******************************************************************
 * \enum SX126X_HW_API_oscillator_type_t
 * \brief SX126X oscillator configuration.
 *******************************************************************/
typedef struct {
	sfx_u8  tcxo_is_radio_controlled;
	SX126X_HW_API_tcxo_ctrl_t supply_voltage;
	sfx_u32 startup_time_in_tick;
} SX126X_HW_API_xosc_cfg_t;

/*!******************************************************************
 * \enum SX126X_HW_API_pa_pwr_cfg_t
 * \brief TODO
 *******************************************************************/
typedef struct {
	sfx_u8 pa_duty_cycle;
	sfx_u8 hp_max;
	sfx_u8 device_sel;
	sfx_u8 pa_lut;
} SX126X_HW_API_pa_cfg_params_t;

/*!******************************************************************
 * \enum SX126X_HW_API_pa_pwr_cfg_t
 * \brief TODO
 *******************************************************************/
typedef struct {
	sfx_s8 power;
	SX126X_HW_API_pa_cfg_params_t pa_config;
}SX126X_HW_API_pa_pwr_cfg_t;

#if (defined TIMER_REQUIRED) && (defined LATENCY_COMPENSATION)
/*!******************************************************************
 * \enum SX126X_HW_API_latency_t
 * \brief SX126X hardware functions latency delay type.
 *******************************************************************/
typedef enum {
    SX126X_HW_API_LATENCY_RESET = 0,
    SX126X_HW_API_LATENCY_WAKEUP,
    SX126X_HW_API_LATENCY_LAST
} SX126X_HW_API_latency_t;
#endif

/********************************
 * \brief SX126X driver callback functions.
 * \fn SX126X_HW_irq_cb_t To be called when a rising edge is detected on the DIO9 pin.
 *******************************/
typedef void (SX126X_HW_irq_cb_t)(void);

/*** SX126X HW API functions ***/

/*!******************************************************************
 * \fn SX126X_HW_API_status_t SX126X_HW_API_open(SX126X_HW_irq_cb_t SX126X_HW_irq_cb_t);
 * \brief Open the SX126X hardware interface. This function is called during RF_API_open() function of the manufacturer layer.
 * \param[in]  	SX126X_HW_irq_cb_t: GPIO interrupt callback that must be called on SX126X GPIOx pin interrupt.
 * \param[out] 	none
 * \retval		Function execution status.
 *******************************************************************/
SX126X_HW_API_status_t SX126X_HW_API_open(SX126X_HW_irq_cb_t callback);

/*!******************************************************************
 * \fn SX126X_HW_API_status_t SX126X_HW_API_close(void)
 * \brief Close the SX126X hardware interface. This function is called during RF_API_close() function of the manufacturer layer.
 * \param[in]  	none
 * \param[out] 	none
 * \retval		Function execution status.
 *******************************************************************/
SX126X_HW_API_status_t SX126X_HW_API_close(void);

/*!******************************************************************
 * \fn SX126X_HW_API_status_t SX126X_HW_API_delayMs(unsigned short delay_ms);
 * \brief Delay. This function is called during multiples function of the manufacturer layer.
 * \param[in]  	delay_ms: delay in ms
 * \param[out] 	none
 * \retval		Function execution status.
 *******************************************************************/
SX126X_HW_API_status_t SX126X_HW_API_delayMs(unsigned short delay_ms);

/*!******************************************************************
 * \fn SX126X_HW_API_status_t SX126X_HW_API_get_chip_name(SX126X_HW_API_chip_name_t *chipset);
 * \brief Get the chip name
 * \param[out] 	chipset: pointer to chip name structure
 * \retval		Function execution status.
 *******************************************************************/
SX126X_HW_API_status_t SX126X_HW_API_get_chip_name(SX126X_HW_API_chip_name_t *chipset);

/*!******************************************************************
 * \fn SX126X_HW_API_status_t SX126X_HW_API_get_reg_mode(SX126X_HW_API_reg_mod_t *reg_mode);
 * \brief Get regulator mode configuration
 * \param[out] 	reg_mode: pointer to regulator mode structure
 * \retval		Function execution status.
 *******************************************************************/
SX126X_HW_API_status_t SX126X_HW_API_get_reg_mode(SX126X_HW_API_reg_mod_t *reg_mode);


/*!******************************************************************
 * \fn SX126X_HW_API_status_t SX126X_HW_API_get_xosc_cfg(SX126X_HW_API_xosc_cfg_t *xosc_type);
 * \brief Get the XOSC configuration
 * \param[out] 	xosc_cfg: pointer to oscillator structure
 * \retval		Function execution status.
 *******************************************************************/
SX126X_HW_API_status_t SX126X_HW_API_get_xosc_cfg(SX126X_HW_API_xosc_cfg_t *xosc_cfg);

/*!******************************************************************
 * \fn SX126X_HW_API_status_t SX126X_HW_API_get_pa_pwr_cfg(SX126X_HW_API_pa_pwr_cfg_t *pa_pwr_cfg_t, sfx_u32 rf_freq_in_hz, sfx_s8 expected_output_pwr_in_dbm);
 * \brief Get the SX126x PA power and configuration according to hardware matching. For more informations see SX126x datasheet (chapter 13.1.14 / 13.4.4) or contact Semtech.
 * \param[in]  	rf_freq_in_hz: RF frequency in Hz
 * \param[in]  	expected_output_pwr_in_dbm: Expected output power in dBm
 * \param[out] 	pa_pwr_cfg_t: Pointer to PA power configuration structure
 * \retval		Function execution status.
 *******************************************************************/
SX126X_HW_API_status_t SX126X_HW_API_get_pa_pwr_cfg(SX126X_HW_API_pa_pwr_cfg_t *pa_pwr_cfg, sfx_u32 rf_freq_in_hz, sfx_s8 expected_output_pwr_in_dbm);


/*!******************************************************************
 * \fn SX126X_HW_API_status_t SX126X_HW_API_tx_on(void);
 * \brief Radio chipset will be start the TX.
 * \param[in]  	none
 * \param[out] 	none
 * \retval		Function execution status.
 *******************************************************************/
SX126X_HW_API_status_t SX126X_HW_API_tx_on(void);

/*!******************************************************************
 * \fn SX126X_HW_API_status_t SX126X_HW_API_tx_off(void);
 * \brief Radio chipset just stopped the TX.
 * \param[in]  	none
 * \param[out] 	none
 * \retval		Function execution status.
 *******************************************************************/
SX126X_HW_API_status_t SX126X_HW_API_tx_off(void);

#ifdef BIDIRECTIONAL
/*!******************************************************************
 * \fn SX126X_HW_API_status_t SX126X_HW_API_rx_on(void);
 * \brief Radio chipset will be start the RX.
 * \param[in]  	none
 * \param[out] 	none
 * \retval		Function execution status.
 *******************************************************************/
SX126X_HW_API_status_t SX126X_HW_API_rx_on(void);
#endif

#ifdef BIDIRECTIONAL
/*!******************************************************************
 * \fn SX126X_HW_API_status_t SX126X_HW_API_rx_off(void);
 * \brief Radio chipset just stopped the RX.
 * \param[in]  	none
 * \param[out] 	none
 * \retval		Function execution status.
 *******************************************************************/
SX126X_HW_API_status_t SX126X_HW_API_rx_off(void);
#endif

#if (defined TIMER_REQUIRED) && (defined LATENCY_COMPENSATION)
/*!******************************************************************
 * \fn SX126X_HW_API_status_t SX126X_HW_API_get_latency(SX126X_HW_API_latency_t latency_type, sfx_u32 *latency_ms)
 * \brief Read HW functions latency in milliseconds.
 * \param[in]	latency_type: Type of latency to get.
 * \param[out] 	latency_ms: Pointer to integer that will contain the latency in milliseconds.
 * \retval		Function execution status.
 *******************************************************************/
SX126X_HW_API_status_t SX126X_HW_API_get_latency(SX126X_HW_API_latency_t latency_type, sfx_u32 *latency_ms);
#endif

#ifdef ERROR_CODES
/*!******************************************************************
 * \fn void SX126X_HW_API_stack_error(void)
 * \brief Generic macro which calls the error stack function for SX126X_HW_API errors (if enabled).
 * \param[in]   none
 * \param[out]  none
 * \retval      none
 *******************************************************************/
#ifdef ERROR_STACK
#define SX126X_HW_API_stack_error(void) SIGFOX_ERROR_stack(SIGFOX_ERROR_SOURCE_HW_API, sx126x_hw_api_status)
#else
#define SX126X_HW_API_stack_error(void)
#endif
#endif

#ifdef ERROR_CODES
/*!******************************************************************
 * \fn void SX126X_HW_API_check_status(error)
 * \brief Generic macro to check an SX126X_HW_API function status and exit.
 * \param[in]   error: High level error code to rise.
 * \param[out]  none
 * \retval      none
 *******************************************************************/
#define SX126X_HW_API_check_status(error) { if (sx126x_hw_api_status != SX126X_HW_API_SUCCESS) { SX126X_HW_API_stack_error(); EXIT_ERROR(error) } }
#endif

#endif /* __SX126X_RF_API_H__ */
