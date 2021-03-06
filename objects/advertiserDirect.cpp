
#include "advertiser.h"
#include "advertiserDirect.h"

#include "advertisement.h"


#include "services/logger.h"
//#include "nrfLog.h"


#include <inttypes.h>
#include <cassert>

#include "ble_advdata.h"
#include "app_error.h"	// APP_ERROR_CHECK

namespace {
static ble_gap_adv_params_t advertisingParams;	// Created by init(), used by start()
}





namespace {

/*
 * This is the only "mode" of advertising.
 */
void initParams() {
	memset(&advertisingParams, 0, sizeof(advertisingParams));

	advertisingParams.type        = BLE_GAP_ADV_TYPE_ADV_IND;
	advertisingParams.p_peer_addr = NULL; // Undirected advertisement.
	advertisingParams.fp          = BLE_GAP_ADV_FP_ANY;
	advertisingParams.interval    = Advertiser::AdvertisingIntervalIn625uSec;
	advertisingParams.timeout     = Advertiser::AdvertisingTimeoutInSeconds;
}

}

void DirectAdvertiser::init() {
	/*
	 * Init the advertisement AND params of advertising behavior
	 */
	Advertisement::init();

	initParams();
}


/*
 * No access to Advertisement, already set in the Softdevice.
 *
 * eraseBonds parameter not used
 */
void DirectAdvertiser::startAdvertising() {
	uint32_t err_code;

	assert(Advertisement::isInit());

	//RTTLogger::log(" Advertising start.");

	err_code = sd_ble_gap_adv_start(&advertisingParams, Advertiser::ConnectionConfigTag);
	APP_ERROR_CHECK(err_code);
}

void DirectAdvertiser::stopAdvertising() {
	//RTTLogger::log("stop advertising\n");
	uint32_t err_code;
	err_code = sd_ble_gap_adv_stop();
	APP_ERROR_CHECK(err_code);
}

