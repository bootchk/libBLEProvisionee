
#include "provisioner.h"
#include <cassert>

// Implementation facade classes
#include "protocolStack.h"


#include "rssi.h"

//#include "nrfLog.h"
#include "services/logger.h"



enum class ProvisioningSessionResult {
	Unknown,
	WasProvisioned,
	NotProvisioned
};


namespace {


bool isProvisioningFlag = false;

// Results of provisioning session
ProvisioningSessionResult provisioningSessionResult = ProvisioningSessionResult::Unknown;
ProvisionedValueType provision;

ProvisioningSucceedCallback succeedCallback = nullptr;
ProvisioningFailCallback failCallback = nullptr;

// TODO implement this for errors returned by SD after start advertising???
// ProvisioningFailCallback errorCallback = nullptr;



// !!! in ISR context, app must return shortly
void callbackAppWithProvisioningResult() {
	switch (provisioningSessionResult ) {
	case ProvisioningSessionResult::WasProvisioned:
		succeedCallback(provision, RSSI::getConnectionRSSI() );
		break;
	case ProvisioningSessionResult::NotProvisioned:
		failCallback();
		break;
	case ProvisioningSessionResult::Unknown:
		// Should not be called with Unknown result
		assert(false);
	}
}


}	// namespace


ProvisionedValueType Provisioner::getProvisionedValue(){ return provision; }



void Provisioner::init(ProvisioningSucceedCallback aSucceedCallback, ProvisioningFailCallback aFailCallback) {
	succeedCallback = aSucceedCallback;
	failCallback = aFailCallback;

	// OLD: TimerAdaptor::create(provisionElapsedTimerHandler);

	assert(! Provisioner::isProvisioning());	// enabled but not started
}




bool Provisioner::isProvisioning() { return isProvisioningFlag; }


/*
 * start() and shutdown() should be mirror images
 */

APIError Provisioner::start() {
	// RTTLogger::log(" Provisioner start.");

	// provisioning sessions are one at a time
	assert(!isProvisioning());

	// assert self initialized
	assert(succeedCallback != nullptr);

	provisioningSessionResult = ProvisioningSessionResult::Unknown;

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
 * Called from SD, from a handler.
 * In ISR context.
 * You can't shutdown SD at such a time.
 * Because it returns to the SD's chain of handlers.
 */
void Provisioner::onProvisioned(ProvisionedValueType aProvision) {
	assert(isProvisioning());

	// We did not timeout, cancel any timer.   OLD: TimerAdaptor::stop();

	RSSI::captureLastRSSI();

	/*
	 * Semantics are one-shot: any one provisioning disconnects and ends provisioning session.
	 * But that is responsibility of app, to disable SD when provisioning succeeds.
	 */
	// OLD: Tell SoftdeviceSleeper to quit its sleeping loop.
	// SoftdeviceSleeper::setReasonForSDWake(ReasonForSDWake::Canceled);

	provision = aProvision;
	provisioningSessionResult = ProvisioningSessionResult::WasProvisioned;

	/*
	 * Context is ISR.
	 * Continuation is return to Softdevice handler.
	 *
	 * OLD: eventually to after call to SoftdeviceSleeper in provisionWithSleep().
	 * There we will callback app (when not in ISR context)
	 *
	 * NEW: callback app with result.
	 * App will schedule next task (pend interrupt that will execute after this ISR finishes.)
	 */
	callbackAppWithProvisioningResult();
}


void Provisioner::onAdvertisingTimeout() {
	provisioningSessionResult = ProvisioningSessionResult::NotProvisioned;
	callbackAppWithProvisioningResult();
}

void Provisioner::onBLEError() {
	// TODO the app might want to do something else if there are persistent errors.
	// Not a separate callback for errors: fail means nothing in range, or error
	provisioningSessionResult = ProvisioningSessionResult::NotProvisioned;
	callbackAppWithProvisioningResult();
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
