
#include <softdeviceSleeper.h>
#include "nrf_sdm.h"	// in /softdevice/s132/headers  // nrf_clock_lf_cfg_t
#include "app_error.h"

// Implementation:
#include "timerAdaptor.h"
#include "softdevice.h"
#include "provisioner.h"
#include "nrfLog.h"

#include <cassert>

namespace {
	ReasonForSDWake reasonForSDWake;
}




void SoftdeviceSleeper::sleepInSDUntilAnyEvent() {
	uint32_t err_code;

	// Sleep using SoftDevice API when SD active.
	err_code = sd_app_evt_wait();
	APP_ERROR_CHECK(err_code);
}



void SoftdeviceSleeper::sleepInSDUntilTimeoutOrCanceled(uint32_t timeout) {

	/*
	 *  Not meaningful to call when SD already disabled.
	 *  A warning: we expect SD to be enabled, but returns immediately if not enabled.
	 */
	if (not Softdevice::isEnabled()) {
		NRFLog::log("??? sleep called with SD disabled");
	}

	/*
	 * Caller must setReasonForSDWake before enabling SD.
	 * The provisioning might have already succeeded and setReasonForSDWake == Canceled
	 */

	// Start timeout on provisioning service
	TimerAdaptor::start(timeout, Provisioner::provisionElapsedTimerHandler);

	/*
	 * Might return immediately if provisioning already succeeded and setReasonForSDWake == Canceled
	 */
	while (reasonForSDWake == ReasonForSDWake::Cleared) {
		sleepInSDUntilAnyEvent();

		NRFLog::log("Wake from sleepInSD");
		/*
		 * Woken by event:
		 * 1) that SD handled
		 * 2) timeout on expiration timer
		 * 3) other events (long clock overflow or other timers)
		 *
		 * 1) client provisioned us.
		 * SD already handled it in interrupt context
		 * and called AppHandler::onWrite
		 * which called Provisioner::onProvisioned which shutdown SD
		 *
		 * 2) timeout
		 * ISR of timer callback handler which disabled SD
		 *
		 * 3) other events which woke cpu
		 * Sleep again.
		 *
		 * !!! This depends on NRF_SDH_DISPATCH_MODEL_INTERRUPT
		 * i.e. SDH callbacks have already occurred in ISR's before
		 * execution resumes here.
		 */
	}
}


void SoftdeviceSleeper::cancelSleep() {
	setReasonForSDWake(ReasonForSDWake::Canceled);
}

ReasonForSDWake  SoftdeviceSleeper::getReasonForSDWake() {
	return reasonForSDWake;
}
void SoftdeviceSleeper:: setReasonForSDWake(ReasonForSDWake reason) {
	reasonForSDWake = reason;
}

