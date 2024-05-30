/*!*****************************************************************
 * \file    sx126x_hal.c
 * \brief   SX126X library low level functions.
 *******************************************************************/
#include "sx126x_hal.h"

sx126x_hal_status_t __attribute__((weak)) sx126x_hal_reset( const void* context )
{
	/*TODO To be implemented by the device manufacturer */
    SFX_UNUSED(context);
    return SX126X_HAL_STATUS_OK;
}

sx126x_hal_status_t __attribute__((weak)) sx126x_hal_wakeup( const void* context )
{
	/*TODO To be implemented by the device manufacturer */
    SFX_UNUSED(context);    
    return SX126X_HAL_STATUS_OK;
}

sx126x_hal_status_t __attribute__((weak)) sx126x_hal_write( const void* context, const uint8_t* command, const uint16_t command_length,
                                      const uint8_t* data, const uint16_t data_length )
{
    /*TODO To be implemented by the device manufacturer */
    SFX_UNUSED(context);
    SFX_UNUSED(command);
    SFX_UNUSED(command_length);
    SFX_UNUSED(data);
    SFX_UNUSED(data_length);
    return SX126X_HAL_STATUS_OK;
}


sx126x_hal_status_t __attribute__((weak)) sx126x_hal_read( const void* context, const uint8_t* command, const uint16_t command_length,
                                     uint8_t* data, const uint16_t data_length )

{
    /*TODO To be implemented by the device manufacturer */
    SFX_UNUSED(context);
    SFX_UNUSED(command);
    SFX_UNUSED(command_length);
    SFX_UNUSED(data);
    SFX_UNUSED(data_length);
    return SX126X_HAL_STATUS_OK;
}

