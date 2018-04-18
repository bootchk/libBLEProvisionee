
#include "provisioner.h"
#include <cassert>

// Implementation facade classes
#include "protocolStack.h"


#include "rssi.h"

//#include "nrfLog.h"
#include "services/logger.h"





namespace {

bool isProvisioningFlag = false;
bool provisioningSessionResult = false;
ProvisionedValueType provision;

ProvisioningSucceedCallback succeedCallback = nullptr;
ProvisioningFailCallback failCallback = nullptr;

// TODO implement this for errors returned by SD after start advertising???
// ProvisioningFailCallback errorCallback = nullptr;



// assert not in interrupt context, app is not realtime constrained.
void callbackAppWithProvisioningResult() {
	if (provisioningSessionResult ) {
		 // assert SoftdeviceSleeper::getReasonForSDWake() == ReasonForSDWake::Canceled

		succeedCallback(provision, RSSI::getConnectionRSSI() );
	}
	else {
		failCallback();
	}
}


}	// namespace


ProvisionedValueType Provisioner::getProvisionedValue(){ return provision; }



void Provisioner::init(ProvisioningSucceedCallback aSucceedCallback, ProvisioningFailCallback aFailCallback) {
	succeedCallback = aSucceedCallback;
	failCallback = aFailCallback;

	// TimerAdaptor::create(provisionElapsedTimerHandler);

	assert(! Provisioner::isProvisioning());	// enabled but not started
}



/*
 * start() and shutdown() should be mirror images
 */

APIError Provisioner::start() {
	// RTTLogger::log(" Provisioner start.");

	// provisioning sessions are one at a time
	assert(!isProvisioning());

	// assert self initialized
	assert(succeedCallback != nullptr);

	APIError startResult;
	startResult = ProtocolStack::startup();
	if ( startResult == APIError::BLEStartedOK  ) {
		isProvisioningFlag = true;
	}
	return startResult;
}

void Provisioner::shutdown() {
	//RTTLogger::log("Provisioner shutdown");

	// Might be in a connection.
	// Assume SD handles that case.

	ProtocolStack::shutdown();

	isProvisioningFlag = false;

	assert(! Provisioner::isProvisioning());

	assert(!ProtocolStack::isEnabled());

	// Ensure not affect LF clock
	// assert(AppTimer::isClockRunning());
}








/*
 * onProvisioned() and onTimerElapsed() are two reasons to end a session
 * Should be similar implementations
 */


/*
 * Called from SD, from a handler.
 * In interrupt context.
 * You can't shutdown SD at such a time?
 * Because it returns to the SD's chain of handlers.
 */
void Provisioner::onProvisioned(ProvisionedValueType aProvision) {
	assert(isProvisioning());

	// We did not timeout, cancel timer.
	// TimerAdaptor::stop();

	RSSI::captureLastRSSI();

	/*
	 * Tell SoftdeviceSleeper to quit its sleeping loop.
	 * Semantics are one-shot: any one provisioning ends sleep and session.
	 */
	// SoftdeviceSleeper::setReasonForSDWake(ReasonForSDWake::Canceled);

	provision = aProvision;
	provisioningSessionResult = true;

	/*
	 * Continuation is return to Softdevice handler,
	 * then eventually to after call to SoftdeviceSleeper in provisionWithSleep().
	 * There we will callback app (when not in interrupt context)
	 */
}






#ifdef OBSOLETE

#include <softdeviceSleeper.h>
#include "timerAdaptor.h"

void Provisioner::startClocks(){
	// delegate
	TimerAdaptor::startClocks();
}

void Provisioner::sleep() {
	SoftdeviceSleeper::sleepInSDUntilAnyEvent();
}
#endif






bool Provisioner::isProvisioning() {
	return isProvisioningFlag;
}












void Provisioner::onAdvertisingTimeout() {
	// Require results is still false, to ensure callback is the failCallback
	assert(provisioningSessionResult == false);
	callbackAppWithProvisioningResult();
}

void Provisioner::onBLEError() {
	// Not a separate callback for errors: fail means nothing in range, or error
	assert(provisioningSessionResult == false);
	callbackAppWithProvisioningResult();
}

