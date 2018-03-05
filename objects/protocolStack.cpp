#include "protocolStack.h"

// SD supports many protocols
#include "softdevice.h"

// One of protocols is BLE
#include "bleProtocol.h"



bool ProtocolStack::startup() {

	if ( ! Softdevice::enable() ) return false;

	BLEProtocol::start();

	BLEProtocol::startAdvertising();

	return true;
}


void ProtocolStack::shutdown() {
	// TODO stop advertising?
	BLEProtocol::stop();
	Softdevice::disable();
}

bool ProtocolStack::isEnabled() {
	return Softdevice::isEnabled();
}
