# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [v2.0](https://github.com/sigfox-tech-radio/sigfox-ep-rf-api-semtech-sx126x/releases/tag/v2.0) - 22 Nov 2024

### Added

* New **SX126X_RF_API_get_and_clear_irq_status** method to be compatible with stm32wlxx_hal_subghz (#5).

### Changed

* Upgrade to **sigfox-ep-lib v4.0**
* Use sx126x_rf_api_obj target object to build sx126x_rf_api library.
* Remove unifdef dependency in all cmake with linked target.
* Add **SX126X_HW_API_config_t** structure in HW API open function with **radio configuration pointer** to manage multi-RC front-ends.
* Call **sx126x_clear_device_errors** in any case (#6).
* Add missing **"sigfox_types.h"** include file (#3).

### Known limitations

* **LBT** not implemented
* **Modulated CW** not supported for type approval addon.

## [v1.3](https://github.com/sigfox-tech-radio/sigfox-ep-rf-api-semtech-sx126x/releases/tag/v1.3) - 30 May 2024

### Added

* Add weak attribute to functions templates.
* Add **SFX_UNUSED** macro to remove extra warnings.
* Add bidirectional flag on HW API RX on and RX off functions.

### Fixed

* Initialize local latency variables to 0.
* Fallback on XOSC mode after sending or recevice.
* Clear device errors on wakeup.

### Removed

* Remove sx126x_hw_api.c from **SX126X_RF_API_SOURCES** in `CMakeLists.txt`.

## [v1.2](https://github.com/sigfox-tech-radio/sigfox-ep-rf-api-semtech-sx126x/releases/tag/v1.2) - 22 Mar 2024

### Fixed

* SX1262 **hp_max** configuration issue.
* BIDIRECTIONAL flag compilation issue.

## [v1.1](https://github.com/sigfox-tech-radio/sigfox-ep-rf-api-semtech-sx126x/releases/tag/v1.1) - 15 Dec 2023

### General

* Remove `sx126x.patch` file.

### Known limitations

* **LBT** not implemented.

## [v1.0](https://github.com/sigfox-tech-radio/sigfox-ep-rf-api-semtech-sx126x/releases/tag/v1.0) - 04 Dec 2023

### General

* First version of the SX126x RF API implementation example.

### Known limitations

* **LBT** not implemented.
