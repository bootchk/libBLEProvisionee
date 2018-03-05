
#pragma once

#include <inttypes.h>

// Parent
#include "service.h"

/*
 * A property that Bluetooth and Softdevice proxy to the outside world.
 */
class Characteristic {
public:
	/*
	 * Tell Softdevice to proxy
	 */
	static uint32_t addProxy(ServiceData*, ServiceDataInit*, ble_uuid_t*, unsigned int);

	static bool isValidWrite(const ble_gatts_evt_write_t *);
};
