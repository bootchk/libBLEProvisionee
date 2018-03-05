
#include "gap.h"

#include "nrfLog.h"
#include "advertiser.h"
#include "rssi.h"


#include <inttypes.h>

#include "ble_conn_params.h"
#include "app_error.h"	// APP_ERROR_CHECK
#include <string.h>	// memset
#include "ble_hci.h"	// HCI_REMOTE...




// Desired by peripheral.  Negotiated and set by central, so not very important.
#define MIN_CONN_INTERVAL   MSEC_TO_UNITS(500, UNIT_1_25_MS)            /**< Minimum acceptable connection interval (0.5 seconds). */
#define MAX_CONN_INTERVAL   MSEC_TO_UNITS(1000, UNIT_1_25_MS)           /**< Maximum acceptable connection interval (1 second). */
#define SLAVE_LATENCY       0                                           /**< Slave latency. */
#define CONN_SUP_TIMEOUT    MSEC_TO_UNITS(4000, UNIT_10_MS)             /**< Connection supervisory timeout (4 seconds). */



namespace {

/*
 * Any length over 6 will probably be truncated in advertisements.
 */
#define DEVICE_NAME_LEN  7
const uint8_t DeviceName[DEVICE_NAME_LEN + 1] = "Firefly";

uint16_t connectionHandle = BLE_CONN_HANDLE_INVALID;




void rememberConnectionHandle(uint16_t aConnectionHandle) {
	// Remember handle to connection, needed later?
	// TODO also remembering connection in ServiceData?
	connectionHandle = aConnectionHandle;
}


}


void GAP::onConnect(const ble_evt_t * bleEvent) {
	rememberConnectionHandle(bleEvent->evt.gap_evt.conn_handle);

	RSSI::startRSSIMeasurementOnConnection(connectionHandle);
}


void GAP::onDisconnect() {

}

void GAP::disconnect(const ble_evt_t * bleEvent) {
	uint32_t   err_code;

	// TODO assert connection handle in event equals remembered connection handle?
	err_code = sd_ble_gap_disconnect(bleEvent->evt.gattc_evt.conn_handle,
					BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
	APP_ERROR_CHECK(err_code);
}


#ifdef NOT_USED
All events handled in softdeviceHandler.cpp

void GAP::onBleEvent(const ble_evt_t * bleEvent, void* foo) {

	uint32_t                    err_code;

	//static ble_gap_sec_keyset_t s_sec_keyset;
	//ble_gap_enc_info_t        * p_enc_info;

	NRFLog::log("GAP event");
	switch (bleEvent->header.evt_id)
	{
	case BLE_GAP_EVT_CONNECTED:
		NRFLog::log("GAP connected");
		onConnect(bleEvent);

		/*
		 * Do not stop advertising.  That yields "INVALID_STATE."
		 */
		break;

	case BLE_GAP_EVT_DISCONNECTED:
		// TODO, if connection successfully set characteristic,  shutdown
		// else if not timed out, advertise again.
		// original advertising_start();
		//Advertiser::startAdvertising();

		NRFLog::log("GAP disconnected");
		// original code does not do this
		//connectionHandle = BLE_CONN_HANDLE_INVALID;
		break;

	case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
		/*
	            No security.
	            s_sec_keyset.keys_peer.p_enc_key  = NULL;
	            s_sec_keyset.keys_peer.p_id_key   = NULL;
	            s_sec_keyset.keys_peer.p_sign_key = NULL;
	            err_code                          = sd_ble_gap_sec_params_reply(connectionHandle,
	                                                                            BLE_GAP_SEC_STATUS_SUCCESS,
	                                                                            &m_sec_params,
	                                                                            &s_sec_keyset);
	            APP_ERROR_CHECK(err_code);
		 */
		NRFLog::log("security params request");
		break;

	case BLE_GAP_EVT_AUTH_STATUS:
		NRFLog::log("GAP auth status");
		break;

	// TODO why handle GATTS event here?
	case BLE_GATTS_EVT_SYS_ATTR_MISSING:
		NRFLog::log("GAP sys attr");
		err_code = sd_ble_gatts_sys_attr_set(connectionHandle, NULL, 0, 0);
		APP_ERROR_CHECK(err_code);
		break;

	case BLE_GATTS_EVT_EXCHANGE_MTU_REQUEST:	// 0x55
		NRFLog::log("GATT server MTU req");
		// TODO don't know what this value should be ???NRF_BLE_MAX_MTU_SIZE
		err_code = sd_ble_gatts_exchange_mtu_reply(connectionHandle, NRF_SDH_BLE_GATT_MAX_MTU_SIZE );
		APP_ERROR_CHECK(err_code);
		break;

	case BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST:
		NRFLog::log("GAP RW auth req");
		/*
		 * if attr_md.rd_auth = 1, then get this event.
		 * The Softdevice does not otherwise have an event for reads.
		 */
		// ??? call sd_ble_gatts_rw_authorize_reply
		break;

	case  BLE_GATTC_EVT_TIMEOUT:
		// client
		NRFLog::log("GATT client timeout");
		// The SoftDevice will disable all ATT traffic and mark the ATT connection as down,
		// but it’s up to the application to do the final disconnect with sd_ble_gap_disconnect().
		break;

	case  BLE_GATTS_EVT_TIMEOUT:
		// server
		NRFLog::log("GATT server timeout");
		break;

	case BLE_GAP_EVT_SEC_INFO_REQUEST:
		NRFLog::log("GAP sec info");
		/*
	            No security.
	            if (s_sec_keyset.keys_own.p_enc_key != NULL)
	            {
	                p_enc_info = &s_sec_keyset.keys_own.p_enc_key->enc_info;

	                err_code = sd_ble_gap_sec_info_reply(connectionHandle, p_enc_info, NULL, NULL);
	                APP_ERROR_CHECK(err_code);
	            }
	            else
	            {
	                // No keys found for this device.
	                err_code = sd_ble_gap_sec_info_reply(connectionHandle, NULL, NULL, NULL);
	                APP_ERROR_CHECK(err_code);
	            }
		 */
		break;

	case BLE_GAP_EVT_TIMEOUT:
		NRFLog::log("GAP timeout");
		if (bleEvent->evt.gap_evt.params.timeout.src == BLE_GAP_TIMEOUT_SRC_ADVERTISING)
		{
			/*
	                err_code = bsp_indication_set(BSP_INDICATE_IDLE);
	                APP_ERROR_CHECK(err_code);

	                err_code = app_button_disable();
	                APP_ERROR_CHECK(err_code);

	                if (err_code == NRF_SUCCESS)
	                {
	                    // Go to system-off mode.
	                    // (this function will not return; wakeup will cause a reset)
	                    err_code = sd_power_system_off();
	                    APP_ERROR_CHECK(err_code);
	                }
			 */
		}
		break;

	case  BLE_GAP_EVT_RSSI_CHANGED:
		/*
		 * Here we take the max rssi.
		 * We never stop taking rssi.
		 */
		int8_t receivedRssi;
		receivedRssi = bleEvent->evt.gap_evt.params.rssi_changed.rssi;
		if (receivedRssi > maxRssi)
			maxRssi = receivedRssi;
		break;

	default:
		// No implementation needed.
		NRFLog::log("event not handled by GAP");
		NRFLog::logInt(bleEvent->header.evt_id);
		break;
	}
}
#endif



void GAP::initParams() {

	uint32_t                err_code;
	ble_gap_conn_params_t   gap_conn_params;
	ble_gap_conn_sec_mode_t sec_mode;

	// security mode: no protection, open link
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

	err_code = sd_ble_gap_device_name_set(
			&sec_mode,
			DeviceName,
			6);	// strlen(DEVICE_NAME));	//DEVICE_NAME_LEN);
	//NRFLog::flush();
	APP_ERROR_CHECK(err_code);

	// TODO
	// err_code = sd_ble_gap_appearance_set(BLE_APPEARANCE_HEART_RATE_SENSOR_HEART_RATE_BELT);
	// err_code = sd_ble_gap_appearance_set(BLE_APPEARANCE_GARDEN_LIGHT); //GENERIC_LIGHT_FIXTURES);
	err_code = sd_ble_gap_appearance_set(BLE_APPEARANCE_UNKNOWN); //GENERIC_LIGHT_FIXTURES);
	APP_ERROR_CHECK(err_code);

	memset(&gap_conn_params, 0, sizeof(gap_conn_params));

	gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
	gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
	gap_conn_params.slave_latency     = SLAVE_LATENCY;
	gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

	err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
	APP_ERROR_CHECK(err_code);

}
