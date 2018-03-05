
#include "gatt.h"

// Implementation
// Also require nrf_ble_gatt.c linked into project

#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"	// required by nrf_ble_gatt.h for NRF_SDH_BLE_OBSERVER
#include "nrf_ble_gatt.h"	// components/ble/nrf_ble_gatt

#include "app_error.h"


namespace {

NRF_BLE_GATT_DEF(gattInstance);

}


void Gatt::init() {
	// nullptr: app has no event handler
	ret_code_t err_code = nrf_ble_gatt_init(&gattInstance, nullptr);
	APP_ERROR_CHECK(err_code);
}
