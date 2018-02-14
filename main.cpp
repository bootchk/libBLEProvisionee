/*
 * Using Provisioner
 * Using a Timer between provisionings.
 */


// Facades
#include "provisioner.h"
#include "nrfLog.h"
#include "softdevice.h"

// radioSoc
#include "clock/sleeper.h"


void provisioningFailedCallback() {
	NRFLog::log("provision fail");
}


void provisioningSuccededCallback(
		ProvisionedValueType provisionedValue,
		int8_t rssi) {
	NRFLog::log("provision succeed");
}


void sleepUntilNextProvisioningSession() {
	Sleeper::sleepDuration(800000);	// 2.4s
}


int main(void)
{
	NRFLog::enable();

	Provisioner::init(provisioningSuccededCallback, provisioningFailedCallback);

	/*
	 * Start LongClock so we we can use Timer,
	 * even when SD is disabled.
	 *
	 * Provisioner knows how to start clocks compatible with SD
	 */
	Provisioner::startClocks();


	while(true) {

		// Here you do "normal" app, including using radio if not provisioning

		// Start a provisioning session.
		Provisioner::provisionWithSleep();

		/*
		 * On wake:
		 * SD is disabled.
		 * Provisioning succeeded or failed.
		 */

		NRFLog::log("Sleeping between provisionings.");
		sleepUntilNextProvisioningSession();

		// Instead of sleeping, your app does other work, or uses radio for another protocol
	}
}
