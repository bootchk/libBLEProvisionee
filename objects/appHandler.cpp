
#include "appHandler.h"



#include "provisioner.h"
#include "characteristic.h"

#include <cassert>

// from radioSoC library
#include "services/logger.h"
//#include "nrfLog.h"




void AppHandler::onWriteCharacteristic(const ble_gatts_evt_write_t * aWrite) {

	RTTLogger::log("Write characteristic.");

	/*
	 * The write is "wild": OTA and might be flawed
	 * or not one that app recognizes.
	 */
	if ( Characteristic::isValidWrite(aWrite)) {
		/*
		 * Convert raw data into type that Provisioner provides
		 */
		// OLD when use byte, no conversion necessary:   uint8_t value = aWrite->data[0];

		// isValidWrite already ensures length is proper i.e. 4

		// TODO move deserialization
		ProvisionedValueType provision;
		// Deserialize
		// Must match serialization in client
		provision.value = aWrite->data[0];
		provision.index = aWrite->data[1];
		provision.offset = aWrite->data[2];
		provision.tss = aWrite->data[3];

		RTTLogger::log("Characteristic: ");
		RTTLogger::log(" Value ");
		RTTLogger::log(provision.value);
		RTTLogger::log(" Index ");
		RTTLogger::log(provision.index);
		RTTLogger::log(" Offset ");
		RTTLogger::log(provision.offset);
		RTTLogger::log(" Tss ");
		RTTLogger::log(provision.tss);


		/*
		 * Propagate up to app.  Here app is "provisioner."
		 * Here, Provisioner will shutdown BLE.
		 */
		Provisioner::onProvisioned(provision);
	}
	else {
		/*
		 * Unexpected:
		 * Not a standard or unknown characteristic,
		 * and not one that my app has defined.
		 */
		RTTLogger::log("Write to unexpected characteristic.");

	}
}


void AppHandler::onConnect() {
	// App doesn't care about connections
	RTTLogger::log("Connected.\n");
}

/*
 * This doesn' seem to come, when we shutdown without calling disconnect()
 */
void AppHandler::onDisconnect(){
	// App doesn't care about disconnections
	RTTLogger::log("Disconnected.\n");
}

// TODO app should care about advertisement timeouts.
// e.g. it indicates there is no client e.g. provisioner existing in RF range.


