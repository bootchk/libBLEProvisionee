

#pragma once

#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"	// ble_evt_t

#include "provisioner.h"

class Softdevice {
	// Some sort of label for protocol???
	static const int ProtocolTag = 1;

public:
	static APIError enable();
	static APIError disable();
	static bool isEnabled();

	/*
	 * Handler of BLE events from SD, some internal handling and some propagated to app.
	 */
	static void dispatchBleEvent( ble_evt_t const * bleEvent, void * context);
};
