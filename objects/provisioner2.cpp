/*
 * This is cruft from a design where provisioner kept its own timer for timeout provisioning.
 * The new design is that the Softdevice keeps a timer that timeouts advertising, and thus provisioning.
 *
 * Part of the implementation of Provisioner.
 *
 * When it was moved here, I did not recompile, it might be broken.
 */

#include "provisioner.h"



/*
 * ISR for RTCx.
 *
 * Interrupt does not necessarily mean that our timer (compare register) elapsed:
 * overflow and other compare registers (although there aren't any in use)
 * will also interrupt.
 */
void Provisioner::provisionElapsedTimerHandler(TimerInterruptReason reason) {
	assert( Provisioner::isProvisioning() );
	//RTTLogger::log("provisioner RTC interrupt");

	switch(reason) {
	case OverflowOrOtherTimerCompare:
		/*
		 * Waked but Timer not expired.
		 * Sleep again
		 */
		break;
	case SleepTimerCompare:
		onTimerElapsed();
		break;
	}
}


void Provisioner::onTimerElapsed() {
	// Time elapsed without any client provisioning us
	SoftdeviceSleeper::setReasonForSDWake(ReasonForSDWake::TimedOut);

	provisioningSessionResult = false;

	// assert time is oneshot and thus no longer enabled
}


APIError Provisioner::provisionWithSleep() {

	APIError result;

	// Clear flag before starting session, it may succeed before we get to sleep
	SoftdeviceSleeper::setReasonForSDWake(ReasonForSDWake::Cleared);


	APIError startResult;
	startResult = start();
	if ( startResult != APIError::BLEStartedOK ) {
		RTTLogger::log("Provisioner fail start.");
		result = startResult;
	}
	else {
		//RTTLogger::log("Provisioner sleeps");

		// Blocks in low-power until timer expires or client provisions us via Softdevice
		SoftdeviceSleeper::sleepInSDUntilTimeoutOrCanceled(Provisioner::ProvisioningSessionDuration);

		// Must be a reason we are not sleeping anymore
		// TODO check the exact set of reasons we expect
		// If SD wakes us for an unknown reason, it might be left in a bad state.
		assert(SoftdeviceSleeper::getReasonForSDWake() != ReasonForSDWake::Cleared);

		shutdown();
		// assert hw resources not used by SD, can be used by app
		assert(! Provisioner::isProvisioning());

		callbackAppWithProvisioningResult();

#ifdef OBSOLETE
		if ( provisioningSessionResult )
			result = ProvisioningResult::Provisioned;
		else
			result = ProvisioningResult::NotProvisioned;
#endif
	}
	return result;
}

