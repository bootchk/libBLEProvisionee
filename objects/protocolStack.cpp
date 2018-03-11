#include "protocolStack.h"

// SD supports many protocols
#include "softdevice.h"

// One of protocols is BLE
#include "bleProtocol.h"



ProvisioningResult ProtocolStack::startup() {

	ProvisioningResult enableSDResult;
	enableSDResult = Softdevice::enable();
	if ( enableSDResult != ProvisioningResult::SDEnabledSuccessfully  ) return enableSDResult;

	BLEProtocol::start();

	BLEProtocol::startAdvertising();

	// We don't have results for start() and startAd()
	return enableSDResult;
}


void ProtocolStack::shutdown() {
	// TODO stop advertising?
	BLEProtocol::stop();
	Softdevice::disable();
}

bool ProtocolStack::isEnabled() {
	return Softdevice::isEnabled();
}
