#include "protocolStack.h"

// SD supports many protocols
#include "softdevice.h"

// One of protocols is BLE
#include "bleProtocol.h"

#include "services/logger.h"



APIError ProtocolStack::startup() {

	APIError startupResult;

	APIError enableSDResult;
	enableSDResult = Softdevice::enable();
	if ( enableSDResult != APIError::SDStateChangeOK  ) {
		RTTLogger::log(" SD enable fail.");
		startupResult = enableSDResult;
	}

	BLEProtocol::start();

	BLEProtocol::startAdvertising();

	// We don't have results for start() and startAd()
	startupResult = APIError::BLEStartedOK;

	return startupResult;
}


void ProtocolStack::shutdown() {
	// TODO stop advertising?
	BLEProtocol::stop();
	Softdevice::disable();
}

bool ProtocolStack::isEnabled() {
	return Softdevice::isEnabled();
}
