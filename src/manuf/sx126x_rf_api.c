/*!*****************************************************************
 * \file    sx126x_rf_api.c
 * \brief   Sigfox sx126x RF API implementation.
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

#include "manuf/sx126x_rf_api.h"
#include "manuf/smtc_dbpsk.h"

#include "sigfox_error.h"
#include "sigfox_types.h"
#include "manuf/mcu_api.h"
#include "manuf/rf_api.h"
#include "sx126x.h"
// SX126X hardware driver.
#include "board/sx126x_hw_api.h"

/*** SX126X RF API local macros ***/

#define T_RADIO_DELAY_ON 0
#define T_RADIO_BIT_LATENCY_ON (2)
#define T_RADIO_BIT_LATENCY_OFF (9)

/*** SX126X RF API local structures ***/
#ifdef ASYNCHRONOUS
typedef struct {
    RF_API_process_cb_t process_cb;
    RF_API_error_cb_t error_cb;
    RF_API_tx_cplt_cb_t tx_cplt_cb;
#ifdef BIDIRECTIONAL
    RF_API_rx_data_received_cb_t rx_data_received_cb;
#endif
#if (defined REGULATORY) && (defined SPECTRUM_ACCESS_LBT)
    RF_API_channel_free_cb_t channel_free_cb;
#endif
} callback_t;
#endif

typedef struct {
#ifdef ASYNCHRONOUS
    callback_t callbacks;
#endif
    sfx_bool tx_done_flag;
    sfx_bool rx_done_flag;
    sfx_bool error_flag;
    volatile sfx_bool irq_flag;
    volatile sfx_bool irq_en;
    sfx_u16 backup_bit_rate_bps_patch;
}sx126x_ctx_t;


/*** SX126X RF API local global variables ***/

#ifdef VERBOSE
static const sfx_u8 SX126X_RF_API_VERSION[] = "v1.1";
#endif

static sx126x_ctx_t sx126x_ctx = {
#ifdef ASYNCHRONOUS
        .callbacks.process_cb = SFX_NULL,
        .callbacks.error_cb = SFX_NULL,
        .callbacks.tx_cplt_cb = SFX_NULL,
#ifdef BIDIRECTIONAL
        .callbacks.rx_data_received_cb = SFX_NULL,
#endif
#if (defined REGULATORY) && (defined SPECTRUM_ACCESS_LBT)
        .callbacks.channel_free_cb = SFX_NULL,
#endif
#endif
        .tx_done_flag   = 0,
        .rx_done_flag   = 0,
        .error_flag     = 0,
        .irq_flag = 0,
        .irq_en = SFX_FALSE,
        .backup_bit_rate_bps_patch = 0,
};


/*** SX126X RF API local functions ***/
static void SX126X_irq(void) {
    if (sx126x_ctx.irq_en == 1) {
    	sx126x_ctx.irq_flag = 1;
#ifdef ASYNCHRONOUS
        if (sx126x_ctx.callbacks.process_cb != SFX_NULL)
        	sx126x_ctx.callbacks.process_cb();
#endif
    }
}

/*** SX126X RF API functions ***/

#if (defined ASYNCHRONOUS) || (defined LOW_LEVEL_OPEN_CLOSE)
/*******************************************************************/
RF_API_status_t SX126X_RF_API_open(RF_API_config_t *rf_api_config) {
#ifdef ERROR_CODES
    RF_API_status_t status = RF_API_SUCCESS;
    SX126X_HW_API_status_t sx126x_hw_api_status = SX126X_HW_API_SUCCESS;
#endif

#ifdef ASYNCHRONOUS
    sx126x_ctx.callbacks.process_cb = rf_api_config->process_cb;
    sx126x_ctx.callbacks.error_cb = rf_api_config->error_cb;
#endif
#ifdef ERROR_CODES
    sx126x_hw_api_status = SX126X_HW_API_open(&SX126X_irq);
    SX126X_HW_API_check_status(SX126X_RF_API_ERROR_DRIVER_SX126X_HW_API);
#else
    SX126X_HW_API_open(&SX126X_irq);
#endif
#ifdef ERROR_CODES
errors:
#endif
    RETURN();
}
#endif

#ifdef LOW_LEVEL_OPEN_CLOSE
/*******************************************************************/
RF_API_status_t SX126X_RF_API_close(void) {
#ifdef ERROR_CODES
    RF_API_status_t status = RF_API_SUCCESS;
    SX126X_HW_API_status_t sx126x_hw_api_status = SX126X_HW_API_SUCCESS;
#endif
#ifdef ERROR_CODES
    sx126x_hw_api_status = SX126X_HW_API_close();
    SX126X_HW_API_check_status(SX126X_RF_API_ERROR_DRIVER_SX126X_HW_API);
#else
    SX126X_HW_API_close();
#endif
#ifdef ERROR_CODES
errors:
#endif
    RETURN();
}
#endif

/*******************************************************************/
RF_API_status_t SX126X_RF_API_process(void) {
#ifdef ERROR_CODES
    RF_API_status_t status = RF_API_SUCCESS;
    SX126X_HW_API_status_t sx126x_hw_api_status = SX126X_HW_API_SUCCESS;
#endif
    sx126x_status_t sx126x_status;
    sx126x_irq_mask_t sx126x_irq_mask;

    if (sx126x_ctx.irq_flag != 1)
        EXIT_ERROR(SX126X_RF_API_ERROR_STATE);
    sx126x_ctx.irq_flag = 0;

    sx126x_status = sx126x_get_and_clear_irq_status( SFX_NULL, &sx126x_irq_mask );
    if (sx126x_status != SX126X_STATUS_OK)
    	EXIT_ERROR(SX126X_RF_API_ERROR_CHIP_IRQ);
    if (sx126x_irq_mask & SX126X_IRQ_TX_DONE) {
    #ifdef ERROR_CODES
    	sx126x_hw_api_status = SX126X_HW_API_tx_off();
    	SX126X_HW_API_check_status(SX126X_RF_API_ERROR_DRIVER_SX126X_HW_API);
    #else
    	SX126X_HW_API_tx_off();
    #endif
            sx126x_ctx.tx_done_flag = 1;
    #ifdef ASYNCHRONOUS
            if (sx126x_ctx.callbacks.tx_cplt_cb != SFX_NULL)
            	sx126x_ctx.callbacks.tx_cplt_cb();
    #endif
    }
    if (sx126x_irq_mask & SX126X_IRQ_RX_DONE) {
#ifdef ERROR_CODES
        sx126x_hw_api_status = SX126X_HW_API_rx_off();
        SX126X_HW_API_check_status(SX126X_RF_API_ERROR_DRIVER_SX126X_HW_API);
#else
        SX126X_HW_API_rx_off();
#endif
        sx126x_ctx.rx_done_flag = 1;
#if (defined ASYNCHRONOUS) && (defined BIDIRECTIONAL)
        if (sx126x_ctx.callbacks.rx_data_received_cb != SFX_NULL)
        	sx126x_ctx.callbacks.rx_data_received_cb();
#endif
    }
    RETURN();
errors:
#ifdef ASYNCHRONOUS
#ifdef ERROR_CODES
            sx126x_ctx.callbacks.error_cb(RF_API_ERROR);
#else
            sx126x_ctx.callbacks.error_cb();
#endif
#endif
    RETURN();
}

/*******************************************************************/
RF_API_status_t SX126X_RF_API_wake_up(void) {
#ifdef ERROR_CODES
    RF_API_status_t status = RF_API_SUCCESS;
    SX126X_HW_API_status_t sx126x_hw_api_status = SX126X_HW_API_SUCCESS;
#endif
    sx126x_status_t sx126x_status;
    SX126X_HW_API_reg_mod_t reg_mode;
    SX126X_HW_API_xosc_cfg_t xosc_cfg;
    sx126x_errors_mask_t errors_mask;

    sx126x_status = sx126x_reset(SFX_NULL);
    if ( sx126x_status != SX126X_STATUS_OK)
        EXIT_ERROR(SX126X_RF_API_ERROR_CHIP_RESET);
    sx126x_status = sx126x_wakeup(SFX_NULL);
    if ( sx126x_status != SX126X_STATUS_OK)
        EXIT_ERROR(SX126X_RF_API_ERROR_CHIP_WAKEUP);
    sx126x_status = sx126x_init_retention_list(SFX_NULL);
    if ( sx126x_status != SX126X_STATUS_OK)
        EXIT_ERROR(SX126X_RF_API_ERROR_CHIP_RETENTION);
#ifdef ERROR_CODES
    sx126x_hw_api_status = SX126X_HW_API_get_reg_mode(&reg_mode);
    SX126X_HW_API_check_status(SX126X_RF_API_ERROR_DRIVER_SX126X_HW_API);
#else
    SX126X_HW_API_get_reg_mode(&reg_mode);
#endif
    sx126x_status = sx126x_set_reg_mode( SFX_NULL, reg_mode);
    if ( sx126x_status != SX126X_STATUS_OK)
        EXIT_ERROR(SX126X_RF_API_ERROR_CHIP_REGMODE);
    sx126x_status = sx126x_set_dio2_as_rf_sw_ctrl(SFX_NULL, 0x01);
    if ( sx126x_status != SX126X_STATUS_OK)
        EXIT_ERROR(SX126X_RF_API_ERROR_CHIP_RF_SW_CTRL);
#ifdef ERROR_CODES
    sx126x_hw_api_status = SX126X_HW_API_get_xosc_cfg(&xosc_cfg);
    SX126X_HW_API_check_status(SX126X_RF_API_ERROR_DRIVER_SX126X_HW_API);
#else
    SX126X_HW_API_get_xosc_cfg(&xosc_cfg);
#endif
    if (xosc_cfg.tcxo_is_radio_controlled == 0x01) {
    	switch (xosc_cfg.supply_voltage) {
    		case SX126X_HW_API_TCXO_CTRL_1_6V :
    			sx126x_status = sx126x_set_dio3_as_tcxo_ctrl(SFX_NULL, SX126X_TCXO_CTRL_1_6V, xosc_cfg.startup_time_in_tick);
    			break;
    		case SX126X_HW_API_TCXO_CTRL_1_7V :
    			sx126x_status = sx126x_set_dio3_as_tcxo_ctrl(SFX_NULL, SX126X_TCXO_CTRL_1_7V, xosc_cfg.startup_time_in_tick);
    			break;
    		case SX126X_HW_API_TCXO_CTRL_1_8V :
    			sx126x_status = sx126x_set_dio3_as_tcxo_ctrl(SFX_NULL, SX126X_TCXO_CTRL_1_8V, xosc_cfg.startup_time_in_tick);
    			break;
    		case SX126X_HW_API_TCXO_CTRL_2_2V :
    			sx126x_status = sx126x_set_dio3_as_tcxo_ctrl(SFX_NULL, SX126X_TCXO_CTRL_2_2V, xosc_cfg.startup_time_in_tick);
    			break;
    		case SX126X_HW_API_TCXO_CTRL_2_4V :
    			sx126x_status = sx126x_set_dio3_as_tcxo_ctrl(SFX_NULL, SX126X_TCXO_CTRL_2_4V, xosc_cfg.startup_time_in_tick);
    			break;
    		case SX126X_HW_API_TCXO_CTRL_2_7V :
    			sx126x_status = sx126x_set_dio3_as_tcxo_ctrl(SFX_NULL, SX126X_TCXO_CTRL_2_7V, xosc_cfg.startup_time_in_tick);
    			break;
    		case SX126X_HW_API_TCXO_CTRL_3_0V :
    			sx126x_status = sx126x_set_dio3_as_tcxo_ctrl(SFX_NULL, SX126X_TCXO_CTRL_3_0V, xosc_cfg.startup_time_in_tick);
    			break;
    		case SX126X_HW_API_TCXO_CTRL_3_3V :
    			sx126x_status = sx126x_set_dio3_as_tcxo_ctrl(SFX_NULL, SX126X_TCXO_CTRL_3_3V, xosc_cfg.startup_time_in_tick);
    			break;
    		default:
    			sx126x_status = SX126X_STATUS_UNKNOWN_VALUE;
    	}
        if ( sx126x_status != SX126X_STATUS_OK)
            EXIT_ERROR(SX126X_RF_API_ERROR_CHIP_TCXO_CTRL);
        sx126x_status = sx126x_cal( SFX_NULL, SX126X_CAL_ALL );
        if ( sx126x_status != SX126X_STATUS_OK)
            EXIT_ERROR(SX126X_RF_API_ERROR_CHIP_CAL);
    }
    sx126x_status = sx126x_set_standby(SFX_NULL, SX126X_STANDBY_CFG_XOSC );
    if (sx126x_status != SX126X_STATUS_OK)
    	EXIT_ERROR(SX126X_RF_API_ERROR_CHIP_STBY);
    sx126x_status = sx126x_set_dio_irq_params(SFX_NULL, SX126X_IRQ_ALL, SX126X_IRQ_TX_DONE | SX126X_IRQ_RX_DONE, SX126X_IRQ_NONE, SX126X_IRQ_NONE );
    if (sx126x_status != SX126X_STATUS_OK)
    	EXIT_ERROR(SX126X_RF_API_ERROR_CHIP_IRQ);
    sx126x_status = sx126x_clear_irq_status(SFX_NULL, SX126X_IRQ_ALL );
    if (sx126x_status != SX126X_STATUS_OK)
    	EXIT_ERROR(SX126X_RF_API_ERROR_CHIP_IRQ);
    sx126x_status = sx126x_get_device_errors(SFX_NULL, &errors_mask);
    if ( (sx126x_status != SX126X_STATUS_OK) || (errors_mask != 0))
    	EXIT_ERROR(SX126X_RF_API_ERROR_CHIP);
    sx126x_ctx.irq_en = 1;

#ifdef ERROR_CODES
    sx126x_hw_api_status = SX126X_HW_API_delayMs(150);
    SX126X_HW_API_check_status(SX126X_RF_API_ERROR_DRIVER_SX126X_HW_API);
#else
    SX126X_HW_API_delayMs(150);
#endif

errors:
	RETURN();
}

/*******************************************************************/
RF_API_status_t SX126X_RF_API_sleep(void) {
#ifdef ERROR_CODES
    RF_API_status_t status = RF_API_SUCCESS;
#endif
    sx126x_status_t sx126x_status;

    sx126x_status = sx126x_set_sleep(SFX_NULL, SX126X_SLEEP_CFG_COLD_START);
    if (sx126x_status != SX126X_STATUS_OK)
    	EXIT_ERROR(SX126X_RF_API_ERROR_CHIP_SLEEP);

errors:
    RETURN();
}

/*******************************************************************/
RF_API_status_t SX126X_RF_API_init(RF_API_radio_parameters_t *radio_parameters) {
#ifdef ERROR_CODES
    RF_API_status_t status = RF_API_SUCCESS;
    SX126X_HW_API_status_t sx126x_hw_api_status = SX126X_HW_API_SUCCESS;
#endif
    sx126x_status_t sx126x_status;
    sx126x_mod_params_bpsk_t sx126x_mod_params_bpsk;
    sx126x_mod_params_gfsk_t sx126x_mod_params_gfsk;
    sx126x_pa_cfg_params_t sx126x_pa_cfg_params;
    SX126X_HW_API_pa_pwr_cfg_t sx126x_hw_api_pa_pwr_cfg;

    sx126x_status = sx126x_set_rf_freq(SFX_NULL, radio_parameters->frequency_hz);
    if (sx126x_status != SX126X_STATUS_OK)
    	EXIT_ERROR(SX126X_RF_API_ERROR_CHIP_FREQ);
    switch (radio_parameters->modulation) {
        case RF_API_MODULATION_DBPSK :
        	sx126x_status = sx126x_set_pkt_type(SFX_NULL, SX126X_PKT_TYPE_BPSK);
            if (sx126x_status != SX126X_STATUS_OK)
            	EXIT_ERROR(SX126X_RF_API_ERROR_CHIP_PKT);
            sx126x_mod_params_bpsk.br_in_bps = radio_parameters->bit_rate_bps;
            sx126x_mod_params_bpsk.pulse_shape = SX126X_DBPSK_PULSE_SHAPE;
            sx126x_status = sx126x_set_bpsk_mod_params(SFX_NULL, &sx126x_mod_params_bpsk);
            if (sx126x_status != SX126X_STATUS_OK)
            	EXIT_ERROR(SX126X_RF_API_ERROR_CHIP_MOD);
        	break;
        case RF_API_MODULATION_GFSK :
        	sx126x_status = sx126x_set_pkt_type(SFX_NULL, SX126X_PKT_TYPE_GFSK);
            if (sx126x_status != SX126X_STATUS_OK)
            	EXIT_ERROR(SX126X_RF_API_ERROR_CHIP_PKT);
            sx126x_mod_params_gfsk.br_in_bps = radio_parameters->bit_rate_bps;
#ifdef BIDIRECTIONAL
            sx126x_mod_params_gfsk.fdev_in_hz = radio_parameters->deviation_hz;
#endif
            sx126x_mod_params_gfsk.pulse_shape = SX126X_GFSK_PULSE_SHAPE_BT_1;
            sx126x_mod_params_gfsk.bw_dsb_param = SX126X_GFSK_BW_4800;
            sx126x_status = sx126x_set_gfsk_mod_params(SFX_NULL, &sx126x_mod_params_gfsk);
            if ( sx126x_status != SX126X_STATUS_OK)
                EXIT_ERROR(SX126X_RF_API_ERROR_CHIP_MOD);
        	break;
        case RF_API_MODULATION_NONE :
        	break;
        default:
        	break;
    }
    if (radio_parameters->rf_mode == RF_API_MODE_TX) {
    	sx126x_ctx.backup_bit_rate_bps_patch = radio_parameters->bit_rate_bps;
#ifdef ERROR_CODES
    	sx126x_hw_api_status = SX126X_HW_API_get_pa_pwr_cfg(&sx126x_hw_api_pa_pwr_cfg, radio_parameters->frequency_hz, radio_parameters->tx_power_dbm_eirp);
        SX126X_HW_API_check_status(SX126X_RF_API_ERROR_DRIVER_SX126X_HW_API);
#else
    	SX126X_HW_API_get_pa_pwr_cfg(&sx126x_hw_api_pa_pwr_cfg, radio_parameters->frequency_hz, radio_parameters->tx_power_dbm_eirp);
#endif
    	sx126x_pa_cfg_params.device_sel = sx126x_hw_api_pa_pwr_cfg.pa_config.device_sel;
    	sx126x_pa_cfg_params.hp_max = sx126x_hw_api_pa_pwr_cfg.pa_config.device_sel;
    	sx126x_pa_cfg_params.pa_duty_cycle = sx126x_hw_api_pa_pwr_cfg.pa_config.pa_duty_cycle;
    	sx126x_pa_cfg_params.pa_lut = sx126x_hw_api_pa_pwr_cfg.pa_config.pa_lut;
        sx126x_status = sx126x_set_pa_cfg( SFX_NULL, &sx126x_pa_cfg_params);
        if ( sx126x_status != SX126X_STATUS_OK)
            EXIT_ERROR(SX126X_RF_API_ERROR_CHIP_PA);
        sx126x_status = sx126x_set_tx_params( SFX_NULL, sx126x_hw_api_pa_pwr_cfg.power, SX126X_RAMP_40_US);
        if ( sx126x_status != SX126X_STATUS_OK)
            EXIT_ERROR(SX126X_RF_API_ERROR_CHIP_TX_CFG);
    }
    if (radio_parameters->rf_mode == RF_API_MODE_RX) {
    	sx126x_status = sx126x_cfg_rx_boosted( SFX_NULL, 0x01);
        if ( sx126x_status != SX126X_STATUS_OK)
            EXIT_ERROR(SX126X_RF_API_ERROR_CHIP_RX);
    }
errors:
    RETURN();
}

/*******************************************************************/
RF_API_status_t SX126X_RF_API_de_init(void) {
#ifdef ERROR_CODES
    RF_API_status_t status = RF_API_SUCCESS;
    SX126X_HW_API_status_t sx126x_hw_api_status = SX126X_HW_API_SUCCESS;
#endif
    //sx126x_status_t sx126x_status;

#ifdef ERROR_CODES
    sx126x_hw_api_status = SX126X_HW_API_rx_off();
    SX126X_HW_API_check_status(SX126X_RF_API_ERROR_DRIVER_SX126X_HW_API);
#else
    SX126X_HW_API_rx_off();
#endif
#ifdef ERROR_CODES
    sx126x_hw_api_status = SX126X_HW_API_tx_off();
    SX126X_HW_API_check_status(SX126X_RF_API_ERROR_DRIVER_SX126X_HW_API);
#else
    SX126X_HW_API_tx_off();
#endif
#ifdef ERROR_CODES
errors:
#endif
    RETURN();
}

/*******************************************************************/
RF_API_status_t SX126X_RF_API_send(RF_API_tx_data_t *tx_data) {
#ifdef ERROR_CODES
    RF_API_status_t status = RF_API_SUCCESS;
    SX126X_HW_API_status_t sx126x_hw_api_status = SX126X_HW_API_SUCCESS;
#endif
    sx126x_status_t sx126x_status;
    sfx_u8 buffer[SIGFOX_UL_BITSTREAM_SIZE_BYTES + 1];
    sfx_u8 i;
    sx126x_pkt_params_bpsk_t sx126x_pkt_params_bpsk;

#ifdef ASYNCHRONOUS
    sx126x_ctx.callbacks.tx_cplt_cb = tx_data->cplt_cb;
#endif
    sx126x_ctx.tx_done_flag = 0;
    sx126x_ctx.error_flag = 0;
    for (i = 0; i < tx_data->bitstream_size_bytes; i++) {
        buffer[i] = tx_data->bitstream[i];
    }
    buffer[tx_data->bitstream_size_bytes] = 0x80;
    dbpsk_encode_buffer(buffer, tx_data->bitstream_size_bytes * 8 + 2, buffer);
    /*Set the BPSK packet param*/
    sx126x_pkt_params_bpsk.pld_len_in_bits = dbpsk_get_pld_len_in_bits(tx_data->bitstream_size_bytes * 8);
    sx126x_pkt_params_bpsk.pld_len_in_bytes = dbpsk_get_pld_len_in_bytes(tx_data->bitstream_size_bytes * 8);
    if (sx126x_ctx.backup_bit_rate_bps_patch == 100) {
    	sx126x_pkt_params_bpsk.ramp_down_delay = SX126X_SIGFOX_DBPSK_RAMP_DOWN_TIME_100_BPS;
    	sx126x_pkt_params_bpsk.ramp_up_delay = SX126X_SIGFOX_DBPSK_RAMP_UP_TIME_100_BPS;
    } else if (sx126x_ctx.backup_bit_rate_bps_patch == 600) {
        sx126x_pkt_params_bpsk.ramp_down_delay = SX126X_SIGFOX_DBPSK_RAMP_DOWN_TIME_600_BPS;
        sx126x_pkt_params_bpsk.ramp_up_delay = SX126X_SIGFOX_DBPSK_RAMP_UP_TIME_600_BPS;
    }
    sx126x_status = sx126x_set_bpsk_pkt_params(SFX_NULL, &sx126x_pkt_params_bpsk);
    if ( sx126x_status != SX126X_STATUS_OK)
        EXIT_ERROR(SX126X_RF_API_ERROR_CHIP_PKT);
    sx126x_status = sx126x_write_buffer(SFX_NULL, 0, buffer, sx126x_pkt_params_bpsk.pld_len_in_bytes);
    if ( sx126x_status != SX126X_STATUS_OK)
        EXIT_ERROR(SX126X_RF_API_ERROR_CHIP_BUFFER);

#ifdef ERROR_CODES
    sx126x_hw_api_status = SX126X_HW_API_tx_on();
    SX126X_HW_API_check_status(SX126X_RF_API_ERROR_DRIVER_SX126X_HW_API);
#else
    SX126X_HW_API_tx_on();
#endif
    sx126x_status = sx126x_set_tx( SFX_NULL, 5000);
    if ( sx126x_status != SX126X_STATUS_OK)
        EXIT_ERROR(SX126X_RF_API_ERROR_CHIP_TX);
#ifndef ASYNCHRONOUS
    while(sx126x_ctx.tx_done_flag != 1) {
        if (sx126x_ctx.irq_flag == 1) {
#ifdef ERROR_CODES
            status = SX126X_RF_API_process();
            CHECK_STATUS(RF_API_SUCCESS);
#else
            SX126X_RF_API_process();
#endif
            if(sx126x_ctx.error_flag == 1) {
                EXIT_ERROR(RF_API_ERROR);
            }
        }
    }
#endif
errors:
    RETURN();
}

#ifdef BIDIRECTIONAL
/*******************************************************************/
RF_API_status_t SX126X_RF_API_receive(RF_API_rx_data_t *rx_data) {
#ifdef ERROR_CODES
    RF_API_status_t status = RF_API_SUCCESS;
    SX126X_HW_API_status_t sx126x_hw_api_status = SX126X_HW_API_SUCCESS;
#ifndef ASYNCHRONOUS
    MCU_API_status_t mcu_api_status = MCU_API_SUCCESS;
#endif
#endif
    sx126x_status_t sx126x_status;
    sx126x_pkt_params_gfsk_t sx126x_pkt_params_gfsk;
    sfx_u8 sync_world[2] = SIGFOX_DL_FT;
#ifdef ASYNCHRONOUS
    sx126x_ctx.callbacks.rx_data_received_cb = rx_data->data_received_cb;
#else
    sfx_bool timer_has_elapsed;
#endif
    sx126x_ctx.rx_done_flag = 0;
    sx126x_ctx.error_flag = 0;
    sx126x_pkt_params_gfsk.address_filtering = SX126X_GFSK_ADDRESS_FILTERING_DISABLE;
    sx126x_pkt_params_gfsk.crc_type = SX126X_GFSK_CRC_OFF;
    sx126x_pkt_params_gfsk.dc_free = SX126X_GFSK_DC_FREE_OFF;
    sx126x_pkt_params_gfsk.header_type = SX126X_GFSK_PKT_FIX_LEN;
    sx126x_pkt_params_gfsk.pld_len_in_bytes = SIGFOX_DL_PHY_CONTENT_SIZE_BYTES;
    sx126x_pkt_params_gfsk.preamble_detector = SX126X_GFSK_PREAMBLE_DETECTOR_MIN_16BITS;
    sx126x_pkt_params_gfsk.preamble_len_in_bits = 16;
    sx126x_pkt_params_gfsk.sync_word_len_in_bits = SIGFOX_DL_FT_SIZE_BYTES * 8;
    sx126x_status = sx126x_set_gfsk_pkt_params(SFX_NULL, &sx126x_pkt_params_gfsk);
    if ( sx126x_status != SX126X_STATUS_OK)
        EXIT_ERROR(SX126X_RF_API_ERROR_CHIP_PKT);
    sx126x_status = sx126x_set_gfsk_sync_word(SFX_NULL, sync_world, 2);
    if ( sx126x_status != SX126X_STATUS_OK)
        EXIT_ERROR(SX126X_RF_API_ERROR_CHIP_SYNC);
#ifdef ERROR_CODES
    sx126x_hw_api_status = SX126X_HW_API_rx_on();
    SX126X_HW_API_check_status(SX126X_RF_API_ERROR_DRIVER_SX126X_HW_API);
#else
    SX126X_HW_API_rx_on();
#endif
    sx126x_status = sx126x_set_rx_with_timeout_in_rtc_step(SFX_NULL, 0xFFFFFF);
    if ( sx126x_status != SX126X_STATUS_OK)
        EXIT_ERROR(SX126X_RF_API_ERROR_CHIP_RX);
#ifndef ASYNCHRONOUS
    while(1) {
        if (sx126x_ctx.irq_flag == 1) {
#ifdef ERROR_CODES
            status = SX126X_RF_API_process();
            CHECK_STATUS(RF_API_SUCCESS);
#else
            SX126X_RF_API_process();
#endif
            if(sx126x_ctx.error_flag == 1) {
                EXIT_ERROR(RF_API_ERROR);
            }
            if(sx126x_ctx.rx_done_flag == 1) {
                rx_data->data_received = SFX_TRUE;
                break;
            }
        }
#ifdef ERROR_CODES
        mcu_api_status = MCU_API_timer_status(MCU_API_TIMER_INSTANCE_T_RX, &timer_has_elapsed);
        MCU_API_check_status(SX126X_RF_API_ERROR_DRIVER_MCU_API);
#else
        MCU_API_timer_status(MCU_API_TIMER_INSTANCE_T_RX, &timer_has_elapsed);
#endif
        if (timer_has_elapsed == SFX_TRUE) {
            break;
        }
    }
#endif
errors:
    RETURN();
}
#endif

#ifdef BIDIRECTIONAL
/*******************************************************************/
RF_API_status_t SX126X_RF_API_get_dl_phy_content_and_rssi(sfx_u8 *dl_phy_content, sfx_u8 dl_phy_content_size, sfx_s16 *dl_rssi_dbm) {
#ifdef ERROR_CODES
    RF_API_status_t status = RF_API_SUCCESS;
#endif
    sx126x_status_t sx126x_status;
    sx126x_pkt_status_gfsk_t sx126x_pkt_status_gfsk;
    sx126x_rx_buffer_status_t sx126x_rx_buffer_status;
#ifdef PARAMETERS_CHECK
    // Check parameters.
    if ((dl_phy_content == SFX_NULL) || (dl_rssi_dbm == SFX_NULL)) {
        EXIT_ERROR(SX126X_RF_API_ERROR_NULL_PARAMETER);
    }
    if (dl_phy_content_size > SIGFOX_DL_PHY_CONTENT_SIZE_BYTES) {
        EXIT_ERROR(SX126X_RF_API_ERROR_BUFFER_SIZE);
    }
#endif
    if (sx126x_ctx.rx_done_flag != SFX_TRUE)
            EXIT_ERROR(SX126X_RF_API_ERROR_STATE);
    sx126x_status = sx126x_get_gfsk_pkt_status(SFX_NULL, &sx126x_pkt_status_gfsk);
    if ( sx126x_status != SX126X_STATUS_OK)
        EXIT_ERROR(SX126X_RF_API_ERROR_CHIP_PKT);
    if (sx126x_pkt_status_gfsk.rx_status.pkt_received == 1) {
        *dl_rssi_dbm = (sfx_s16)sx126x_pkt_status_gfsk.rssi_avg;
        sx126x_status = sx126x_get_rx_buffer_status(SFX_NULL, &sx126x_rx_buffer_status);
        if ( sx126x_status != SX126X_STATUS_OK)
            EXIT_ERROR(SX126X_RF_API_ERROR_CHIP_BUFFER);
        sx126x_read_buffer(SFX_NULL,sx126x_rx_buffer_status.buffer_start_pointer, dl_phy_content,
                dl_phy_content_size);
    }
errors:
    RETURN();
}
#endif

#if (defined REGULATORY) && (defined SPECTRUM_ACCESS_LBT)
/*******************************************************************/
RF_API_status_t SX126X_RF_API_carrier_sense(RF_API_carrier_sense_parameters_t *carrier_sense_params) {
    /* To be implemented by the device manufacturer */
#ifdef ERROR_CODES
    RF_API_status_t status = RF_API_SUCCESS;
#endif
    RETURN();
}
#endif

#if (defined TIMER_REQUIRED) && (defined LATENCY_COMPENSATION)
/*******************************************************************/
RF_API_status_t SX126X_RF_API_get_latency(RF_API_latency_t latency_type, sfx_u32 *latency_ms) {
#ifdef ERROR_CODES
    RF_API_status_t status = RF_API_SUCCESS;
    SX126X_HW_API_status_t sx126x_hw_api_status = SX126X_HW_API_SUCCESS;
#endif
    sfx_u32 latency_tmp;

    switch(latency_type) {
        case RF_API_LATENCY_WAKE_UP :
            *latency_ms = 150;
#ifdef ERROR_CODES
            sx126x_hw_api_status = SX126X_HW_API_get_latency(SX126X_HW_API_LATENCY_RESET, &latency_tmp);
            SX126X_HW_API_check_status(SX126X_RF_API_ERROR_DRIVER_SX126X_HW_API);
#else
            SX126X_HW_API_get_latency(SX126X_HW_API_LATENCY_RESET, &latency_tmp);
#endif
            (*latency_ms) += latency_tmp;
#ifdef ERROR_CODES
            sx126x_hw_api_status = SX126X_HW_API_get_latency(SX126X_HW_API_LATENCY_WAKEUP, &latency_tmp);
            SX126X_HW_API_check_status(SX126X_RF_API_ERROR_DRIVER_SX126X_HW_API);
#else
            SX126X_HW_API_get_latency(SX126X_HW_API_LATENCY_RESET, &latency_tmp);
#endif
            (*latency_ms) += latency_tmp;
            break;
        case RF_API_LATENCY_INIT_TX:
            *latency_ms = T_RADIO_DELAY_ON;
            break;
        case RF_API_LATENCY_SEND_START:
            *latency_ms = (T_RADIO_BIT_LATENCY_ON * 1000) / sx126x_ctx.backup_bit_rate_bps_patch;
            break;
        case RF_API_LATENCY_SEND_STOP:
            *latency_ms = (T_RADIO_BIT_LATENCY_OFF * 1000) / sx126x_ctx.backup_bit_rate_bps_patch;
            break;
        case RF_API_LATENCY_DE_INIT_TX:
            *latency_ms = 0;
            break;
        case RF_API_LATENCY_SLEEP :
            *latency_ms = 0;
            break;
#ifdef BIDIRECTIONAL
        case RF_API_LATENCY_INIT_RX:
            *latency_ms = 0;
            break;
        case RF_API_LATENCY_RECEIVE_START:
            *latency_ms = 0;
            break;
        case RF_API_LATENCY_RECEIVE_STOP:
            *latency_ms = 5;
            break;
        case RF_API_LATENCY_DE_INIT_RX:
            *latency_ms = 0;
            break;
#endif
        default :
            *latency_ms = 0;
    }
#ifdef ERROR_CODES
errors:
#endif
    RETURN();
}
#endif

#ifdef CERTIFICATION
/*******************************************************************/
RF_API_status_t SX126X_RF_API_start_continuous_wave(void) {
#ifdef ERROR_CODES
    RF_API_status_t status = RF_API_SUCCESS;
#endif
    //TODO
    RETURN();
}
#endif

#ifdef VERBOSE
/*******************************************************************/
RF_API_status_t SX126X_RF_API_get_version(sfx_u8 **version, sfx_u8 *version_size_char) {
#ifdef ERROR_CODES
    RF_API_status_t status = RF_API_SUCCESS;
#endif
    (*version) = (sfx_u8*) SX126X_RF_API_VERSION;
    (*version_size_char) = (sfx_u8) sizeof(SX126X_RF_API_VERSION);
    RETURN();
}
#endif

#ifdef ERROR_CODES
/*******************************************************************/
void SX126X_RF_API_error(void) {
    SX126X_RF_API_de_init();
    SX126X_RF_API_sleep();
}
#endif

/*** SX126X RF API functions mapping ***/

#ifndef DYNAMIC_RF_API

#if (defined ASYNCHRONOUS) || (defined LOW_LEVEL_OPEN_CLOSE)
/*******************************************************************/
RF_API_status_t RF_API_open(RF_API_config_t *rf_api_config) {
    return SX126X_RF_API_open(rf_api_config);
}
#endif

#ifdef LOW_LEVEL_OPEN_CLOSE
/*******************************************************************/
RF_API_status_t RF_API_close(void) {
    return SX126X_RF_API_close();
}
#endif

#ifdef ASYNCHRONOUS
/*******************************************************************/
RF_API_status_t RF_API_process(void) {
    return SX126X_RF_API_process();
}
#endif

/*******************************************************************/
RF_API_status_t RF_API_wake_up(void) {
    return SX126X_RF_API_wake_up();
}

/*******************************************************************/
RF_API_status_t RF_API_sleep(void) {
    return SX126X_RF_API_sleep();
}

/*******************************************************************/
RF_API_status_t RF_API_init(RF_API_radio_parameters_t *radio_parameters) {
    return SX126X_RF_API_init(radio_parameters);
}

/*******************************************************************/
RF_API_status_t RF_API_de_init(void) {
    return SX126X_RF_API_de_init();
}

/*******************************************************************/
RF_API_status_t RF_API_send(RF_API_tx_data_t *tx_data) {
    return SX126X_RF_API_send(tx_data);
}

#ifdef BIDIRECTIONAL
/*******************************************************************/
RF_API_status_t RF_API_receive(RF_API_rx_data_t *rx_data) {
    return SX126X_RF_API_receive(rx_data);
}
#endif

#ifdef BIDIRECTIONAL
/*******************************************************************/
RF_API_status_t RF_API_get_dl_phy_content_and_rssi(sfx_u8 *dl_phy_content, sfx_u8 dl_phy_content_size, sfx_s16 *dl_rssi_dbm) {
    return SX126X_RF_API_get_dl_phy_content_and_rssi(dl_phy_content, dl_phy_content_size, dl_rssi_dbm);
}
#endif

#if (defined REGULATORY) && (defined SPECTRUM_ACCESS_LBT)
/*******************************************************************/
RF_API_status_t RF_API_carrier_sense(RF_API_carrier_sense_parameters_t *carrier_sense_params) {
    return SX126X_RF_API_carrier_sense(carrier_sense_params);
}
#endif

#if (defined TIMER_REQUIRED) && (defined LATENCY_COMPENSATION)
/*******************************************************************/
RF_API_status_t RF_API_get_latency(RF_API_latency_t latency_type, sfx_u32 *latency_ms) {
    return SX126X_RF_API_get_latency(latency_type, latency_ms);
}
#endif

#ifdef CERTIFICATION
/*******************************************************************/
RF_API_status_t RF_API_start_continuous_wave(void) {
    return SX126X_RF_API_start_continuous_wave();
}
#endif

#ifdef VERBOSE
/*******************************************************************/
RF_API_status_t RF_API_get_version(sfx_u8 **version, sfx_u8 *version_size_char) {
    return SX126X_RF_API_get_version(version, version_size_char);
}
#endif

#ifdef ERROR_CODES
/*******************************************************************/
void RF_API_error(void) {
    SX126X_RF_API_error();
}
#endif

#endif /* DYNAMIC_RF_API */
