
#pragma once

#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"	// required by nrf_ble_gatt.h for NRF_SDH_BLE_OBSERVER
#include "nrf_ble_gatt.h"	// components/ble/nrf_ble_gatt


class AppHandler {
public:
	static void onWriteCharacteristic(const ble_gatts_evt_write_t *);
	static void onConnect();
	static void onDisconnect();
};
