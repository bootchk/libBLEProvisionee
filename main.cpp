/*
 * Using Provisioner
 * Using a Timer between provisionings.
 */


// Facades
#include "provisioner.h"
#include "softdevice.h"

#include "nrfLog.h"
#include "services/logger.h"

// radioSoc
#include "clock/sleeperObs.h"

static bool provisionSessionOverFlag;

void provisioningFailedCallback() {
	NRFLog::log("provision fail");
	provisionSessionOverFlag = true;
}


void provisioningSuccededCallback(
		ProvisionedValueType provisionedValue,
		int8_t rssi) {
	NRFLog::log("provision succeed");
	provisionSessionOverFlag = true;
}


void sleepUntilNextProvisioningSession() {
	//Sleeper::sleepDuration(800000);	// 2.4s
	Sleeper::sleepDuration(800);	// ? s
}






int main(void)
{
	NRFLog::enable();

	Provisioner::init(provisioningSuccededCallback, provisioningFailedCallback);

	while(true) {

		// Here you do "normal" app, including using radio if not provisioning

		provisionSessionOverFlag = false;

		// returns immediately
		APIError result = Provisioner::start();
		if (result == APIError::BLEStartedOK ) {

			while(true) {
				Provisioner::sleep();
				// did callback set flag?
				if (provisionSessionOverFlag) {
					break;
				}
				else {
					// Expected: wake for BLE events
					RTTLogger::log("Wake before provision session over.");
				}
			}
		}
		else {
			RTTLogger::log("Failed  provision::start.");
			// No need to call errorCallback
			// but must schedule a new task, provisioning is not happening
		}

		/*
		 * On wake:
		 * SD is disabled.
		 * Provisioning succeeded or failed.
		 */

		Provisioner::shutdown();
		RTTLogger::log("Between provisionings.");

		// Instead of sleeping, your app does other work, or uses radio for another protocol
	}
}

#ifdef OBSOLETE
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
		ProvisioningResult result;
		result = Provisioner::provisionWithSleep();
		(void) result;
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
#endif
