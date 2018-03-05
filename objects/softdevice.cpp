
#include "softdevice.h"

#include <inttypes.h>

// #include "app_error.h"	// APP_ERROR_CHECK


/*
 * Since our use of SD is not critical, return false if fails for any reason
 */
#define returnFalseOnSDError()  { if (err_code != NRF_SUCCESS) { return false; } }



#ifdef OLDER_SDK
// Needed for SDK earlier than 14 ?

//#include "nrf_sdm.h"	// in /softdevice/s132/headers  // nrf_clock_lf_cfg_t
//#include "ble.h"
//#include "ble_srv_common.h"
//SDK13 #include "softdevice_handler.h"  // in softdevice/common/softdevice_handler // SOFTDEVICE_HANDLER_INIT
//#include "nrf_sdh.h"
//#include "nrf_sdh_ble.h"

// When changing this number remember to adjust the RAM settings
#define CENTRAL_LINK_COUNT      0      // < Number of central links used by the application.
#define PERIPHERAL_LINK_COUNT   1      // < Number of peripheral links used by the application.

#define IS_SRVC_CHANGED_CHARACT_PRESENT 0
/*
 * Whether or not to include the service_changed characteristic.
 * If not enabled, the server's database cannot be changed for the lifetime of the device
 */


#endif



#define APP_BLE_OBSERVER_PRIO   3  /**< Application's BLE observer priority. You shouldn't need to modify this value. */



bool Softdevice::enable() {
	uint32_t           err_code;

	// NRFLog::log("Enable SD\n");

	#ifdef SDK13
	// Struct instance that describes lf clock, copied from board/pca10040.h
	nrf_clock_lf_cfg_t clock_lf_cfg =  {
			.source        = NRF_CLOCK_LF_SRC_XTAL,            \
			.rc_ctiv       = 0,                                \
			.rc_temp_ctiv  = 0,                                \
			.xtal_accuracy = NRF_CLOCK_LF_XTAL_ACCURACY_20_PPM};


	SOFTDEVICE_HANDLER_INIT(&clock_lf_cfg, nullptr);

	// Enable BLE stack
	ble_enable_params_t ble_enable_params;
	err_code = softdevice_enable_get_default_config(
			CENTRAL_LINK_COUNT,
			PERIPHERAL_LINK_COUNT,
			&ble_enable_params);
	APP_ERROR_CHECK(err_code);

	// ??? Might be necessary for later SD versions?
	ble_enable_params.gatts_enable_params.service_changed = IS_SRVC_CHANGED_CHARACT_PRESENT;

	// Nordic deprecated to check the ram settings against the used number of links

	err_code = softdevice_enable(&ble_enable_params);
	APP_ERROR_CHECK(err_code);

	uint8_t sd_is_enabled = 0;
	sd_softdevice_is_enabled(&sd_is_enabled);
	//sd_softdevice_is_ble_enabled(&sd_is_enabled);
	if ( !sd_is_enabled ) {
		NRFLog::log("SOFT DEVICE IS NOT ENABLED\r\n");
	}

	err_code = softdevice_ble_evt_handler_set(dispatchBleEvent);
	APP_ERROR_CHECK(err_code);

	// Register with the SoftDevice handler module for SYS events.
	// See NRF_SOC_EVTS enum.  Events like hfclkstarted and radio events
	err_code = softdevice_sys_evt_handler_set(dispatchSysEvent);
	APP_ERROR_CHECK(err_code);

	NRFLog::flush();
#else
	err_code = nrf_sdh_enable_request();
	returnFalseOnSDError();

	ASSERT(nrf_sdh_is_enabled());



	/*
	 * Value to be returned by ..cfg_set.
	 * A recommended process is to capture value in debugger,
	 * then change your .ld script to allocate enough RAM for BLE stack
	 * (adjust application RAM start in ld to be this value.)
	 */
	uint32_t ram_start = 0;

	/*
	 * Configure BLE stack using sdk_config.h file.
	 *
	 * See migration guide to SDK14.
	 * Alternatives are described there.
	 *
	 * If you add capability to code (e.g. add a characteristic), you might need to change sdk_config.h
	 * run through debugger, then change .ld file.
	 */
	err_code = nrf_sdh_ble_default_cfg_set(Softdevice::ProtocolTag, &ram_start);
	returnFalseOnSDError();

	// Enable BLE stack.
	err_code = nrf_sdh_ble_enable(&ram_start);
	returnFalseOnSDError();

	/*
	 * Register a handler for BLE events.
	 *
	 * SDK 14.  See "Migration guide."
	 *
	 * Params:
	 * name of observer.  A valid C name.  Macro defines an instance, never used by other code.
	 * priority of observer
	 * handler function
	 * optional parameter to handler (typically instance of service?
	 */
	//NRF_SDH_BLE_OBSERVER(m_ble_observer, APP_BLE_OBSERVER_PRIO, ble_evt_handler, NULL);
	NRF_SDH_BLE_OBSERVER(bleObserver, APP_BLE_OBSERVER_PRIO, dispatchBleEvent, nullptr);
#endif
	return true;
}



bool Softdevice::isEnabled() {
	return nrf_sdh_is_enabled();
}


void Softdevice::disable() {
	uint32_t   err_code;

	// assert BLEProtocol already stopped

	//NRFLog::log("Disable SD\n");
	err_code = nrf_sdh_disable_request();

	/*
	 * Not checking error.
	 * TODO are there any we can recover from?
	 */
	(void) err_code;

	ASSERT(!nrf_sdh_is_enabled());
}

