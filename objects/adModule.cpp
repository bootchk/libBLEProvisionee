
#include "advertiser.h"
#include "adModule.h"


#include "nrf_sdh_soc.h"	// dependency of ble_advertising.h macros
#include "nrf_sdh_ble.h"

#include "ble_advertising.h" // header for Nordic Advertising module
#include "app_error.h"	// APP_ERROR_CHECK

// facade
#include "uuid.h"

// Use macro to create instance
// TODO does this work in cpp or need extern "C" ?
BLE_ADVERTISING_DEF(adModuleInstance);





namespace {

/*
 * Handle events from module.
 *
 * Events just indicate stage of advertising that module sequences through.
 *
 * BLE_ADV_EVT_IDLE indicates last stage of sequence.
 * Alternatives are to sleep, or to restart advertising at start of sequence.
 *
 */
static void on_adv_evt(ble_adv_evt_t ble_adv_evt)
{
    // ret_code_t err_code;

    switch (ble_adv_evt)
    {
        case BLE_ADV_EVT_FAST:
            //NRF_LOG_INFO("Fast advertising.");
            //err_code = bsp_indication_set(BSP_INDICATE_ADVERTISING);
            //APP_ERROR_CHECK(err_code);
            break;

        case BLE_ADV_EVT_IDLE:
        	// TODO original was:
            //sleep_mode_enter();
            break;

        default:
            break;
    }
}
}


void AdModule::init() {
	uint32_t             err_code;
	ble_advertising_init_t init;

	memset(&init, 0, sizeof(init));

	init.advdata.name_type               = BLE_ADVDATA_FULL_NAME;
	init.advdata.include_appearance      = false;	// true;
	init.advdata.flags                   = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
	init.advdata.uuids_complete.uuid_cnt = 1;	// OLD sizeof(m_adv_uuids) / sizeof(m_adv_uuids[0]);
	init.advdata.uuids_complete.p_uuids  = Uuid::getCustomServiceUUID();  // OLD m_adv_uuids;

	init.config.ble_adv_fast_enabled  = true;
	init.config.ble_adv_fast_interval = APP_ADV_INTERVAL;
	init.config.ble_adv_fast_timeout  = APP_ADV_TIMEOUT_IN_SECONDS;

	init.evt_handler = on_adv_evt;

	err_code = ble_advertising_init(&adModuleInstance, &init);
	APP_ERROR_CHECK(err_code);

	ble_advertising_conn_cfg_tag_set(&adModuleInstance, APP_BLE_CONN_CFG_TAG);
}


void AdModule::startAdvertising(bool erase_bonds) {

	if (erase_bonds == true)
	{
		// TODO  delete_bonds();
		// Advertising is started by PM_EVT_PEERS_DELETED_SUCEEDED evetnt
	}
	else
	{
		ret_code_t err_code = ble_advertising_start(&adModuleInstance, BLE_ADV_MODE_FAST);
		APP_ERROR_CHECK(err_code);
	}
}


void AdModule::restartWithoutWhitelist() {
	uint32_t             err_code;

	err_code = ble_advertising_restart_without_whitelist(&adModuleInstance);
	if (err_code != NRF_ERROR_INVALID_STATE)
	{
		APP_ERROR_CHECK(err_code);
	}
}



#ifdef NOT_USED
void AdModule::getAdModuleInstance() {

}
#endif

