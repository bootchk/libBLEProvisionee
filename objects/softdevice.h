

#pragma once

#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"	// ble_evt_t


class Softdevice {
	// Some sort of label for protocol???
	static const int ProtocolTag = 1;

public:
	static bool enable();
	static bool isEnabled();
	static void disable();

	/*
	 * Handler of BLE events from SD, some internal handling and some propagated to app.
	 */
	static void dispatchBleEvent( ble_evt_t const * bleEvent, void * context);
};
