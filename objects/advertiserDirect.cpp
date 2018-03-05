
#include "advertiser.h"
#include "advertiserDirect.h"

#include "advertisement.h"
#include "nrfLog.h"


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
	advertisingParams.interval    = APP_ADV_INTERVAL;
	advertisingParams.timeout     = APP_ADV_TIMEOUT_IN_SECONDS;
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

	NRFLog::log("started advertising\n");

	err_code = sd_ble_gap_adv_start(&advertisingParams, APP_BLE_CONN_CFG_TAG);
	APP_ERROR_CHECK(err_code);
}

void DirectAdvertiser::stopAdvertising() {
	NRFLog::log("stop advertising\n");
	uint32_t err_code;
	err_code = sd_ble_gap_adv_stop();
	APP_ERROR_CHECK(err_code);
}

