#include "protocolStack.h"

// SD supports many protocols
#include "softdevice.h"

// One of protocols is BLE
#include "bleProtocol.h"

#include "services/logger.h"



ProvisioningResult ProtocolStack::startup() {

	ProvisioningResult startupResult;

	ProvisioningResult enableSDResult;
	enableSDResult = Softdevice::enable();
	if ( enableSDResult != ProvisioningResult::SDEnabledOK  ) {
		RTTLogger::log(" SD enable fail.");
		startupResult = enableSDResult;
	}

	BLEProtocol::start();

	BLEProtocol::startAdvertising();

	// We don't have results for start() and startAd()
	startupResult = ProvisioningResult::BLEStartedOK;

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
