
#pragma once

#include <inttypes.h>

#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"	// ble_evt_t

/*
 * Knows about RSSI in Softdevice.
 * Knows it is on a connection.
 * Understands events pertinent.
 */
class RSSI {
public:
	static void startRSSIMeasurementOnConnection(uint16_t aConnectionHandle);

	// Returns -127 or the RSSI of the last transaction in any connection
	// Can only be called while SD enabled and connection is alive?
	static int8_t  getLastRSSIOnConnection(uint16_t aConnectionHandle);

	static int8_t maxRSSI();
	static int8_t lastRSSI();

	static void captureLastRSSI();

	static void updateFromEvent( ble_evt_t const * bleEvent);

	// Get representative RSSI from connection, converted to unsigned inverse RSSI
	static int8_t getConnectionRSSI();
};
