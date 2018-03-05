
#include "softdevice.h"

// Derived from ble_app_template, which presumably is valid for SDK 14 and above
// but includes some logic which may not be necessary if you pare the example down.


// Needed if strategy is to dispatch to other facade objects
#include "service.h"
#include "gap.h"
#include "appHandler.h"
#include "rssi.h"


// #include "nrfLog.h"
#include "services/logger.h"

// ???
#define APP_FEATURE_NOT_SUPPORTED       BLE_GATT_STATUS_ATTERR_APP_BEGIN + 2    /**< Reply when unsupported features are requested. */



/*
 * Handler of BLE events from BLEObserver (an SDK concept.)
 * Registered with and called from the BLE Stack event interrupt handler.
 *
 *
 * Alternate strategies:
 * 1. Dispatch BLE stack event to all modules with a BLE stack event handler.
 * Sequentially call each module's event handler.  Many modules are observing same event.
 *
 * 2 Handle all events here, even though they belong to another module.
 *
 * Each action has two components:
 * - keep internal state
 * - keep external application state
 *
 * Currently, this maintains semantics original to ble_app_template,
 * plus some added handling of events.
 * I don't understand all of the internal logic
 */


void Softdevice::dispatchBleEvent( ble_evt_t const * bleEvent, void * context)
{
	RTTLogger::log("bleEvent\n");

#ifdef STRATEGY_1
	// !!! original code passes the context, to be cast
	// ?? Here I pass the one serviceData?
	Service::onBleEvent(&serviceData, bleEvent);

    //lkk ble_conn_params_on_ble_evt(bleEvent);

    // ??? GAP registers its own observer?
	// GAP::onBleEvent(bleEvent, context);
#else

	ret_code_t err_code = NRF_SUCCESS;

	switch (bleEvent->header.evt_id)
	{
	case BLE_GAP_EVT_DISCONNECTED:
		RTTLogger::log("Disconnected.");

		// app logic
		// LED indication will be changed when advertising starts.

		// Tell the facade, the SD Gap module has already handled it
		GAP::onDisconnect();

		// Tell app
		AppHandler::onDisconnect();
		break;

	case BLE_GAP_EVT_CONNECTED:
		RTTLogger::log("Connected.");
		// app logic
		//err_code = bsp_indication_set(BSP_INDICATE_CONNECTED);
		//APP_ERROR_CHECK(err_code);

		// Tell facades, need remember handle to connection
		GAP::onConnect(bleEvent);

		AppHandler::onConnect();
		break;

#ifndef S140
	case BLE_GAP_EVT_PHY_UPDATE_REQUEST:
	{
		RTTLogger::log("PHY update request.");
		ble_gap_phys_t const phys =
		{
				tx_phys : BLE_GAP_PHY_AUTO,
				rx_phys : BLE_GAP_PHY_AUTO,
		};
		err_code = sd_ble_gap_phy_update(bleEvent->evt.gap_evt.conn_handle, &phys);
		APP_ERROR_CHECK(err_code);
	} break;
#endif

	case BLE_GATTC_EVT_TIMEOUT:
		// Disconnect on GATT Client timeout event.
		RTTLogger::log("GATT Client Timeout.");
		GAP::disconnect(bleEvent);
		break;

	case BLE_GATTS_EVT_TIMEOUT:
		// Disconnect on GATT Server timeout event.
		RTTLogger::log("GATT Server Timeout.");

		// Tell facade, it will tell SD and another event will come from SD
		GAP::disconnect(bleEvent);
		break;

	case BLE_EVT_USER_MEM_REQUEST:
		// Internal
		// see queued writes
		RTTLogger::log("User mem req.");
		err_code = sd_ble_user_mem_reply(bleEvent->evt.gattc_evt.conn_handle, NULL);
		APP_ERROR_CHECK(err_code);
		break;

	case BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST:
	{
		RTTLogger::log("gatts rw auth req.");
		ble_gatts_evt_rw_authorize_request_t  req;
		ble_gatts_rw_authorize_reply_params_t auth_reply;

		req = bleEvent->evt.gatts_evt.params.authorize_request;

		if (req.type != BLE_GATTS_AUTHORIZE_TYPE_INVALID)
		{
			if ((req.request.write.op == BLE_GATTS_OP_PREP_WRITE_REQ)     ||
					(req.request.write.op == BLE_GATTS_OP_EXEC_WRITE_REQ_NOW) ||
					(req.request.write.op == BLE_GATTS_OP_EXEC_WRITE_REQ_CANCEL))
			{
				if (req.type == BLE_GATTS_AUTHORIZE_TYPE_WRITE)
				{
					auth_reply.type = BLE_GATTS_AUTHORIZE_TYPE_WRITE;
				}
				else
				{
					auth_reply.type = BLE_GATTS_AUTHORIZE_TYPE_READ;
				}
				auth_reply.params.write.gatt_status = APP_FEATURE_NOT_SUPPORTED;
				err_code = sd_ble_gatts_rw_authorize_reply(bleEvent->evt.gatts_evt.conn_handle,
						&auth_reply);
				APP_ERROR_CHECK(err_code);
			}
		}
	} break; // BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST

	/*
	 * Starting here, I added events to the original
	 * Some are needed by my app,
	 * some don't need to be handled.
	 */
	case BLE_GATTS_EVT_WRITE:	// 0x50
		RTTLogger::log("gatts write\n");

		// TODO filter writes to internal characteristics here?
		// Not all writes are to characteristics the app owns

		// Tell app
		// Passing address inside event.  Event must not be destroyed.  Caller must copy any data it wants.
		AppHandler::onWriteCharacteristic(&bleEvent->evt.gatts_evt.params.write);
		break;

	case BLE_GATTS_EVT_EXCHANGE_MTU_REQUEST:	// 0x55
		RTTLogger::log("GATT server MTU req");

		// For now, assume SD handles internally

#ifdef NOT_USED
		// TODO don't know why this was not in the original?
		// TODO don't know what this value should be ???NRF_BLE_MAX_MTU_SIZE
		//
		err_code = sd_ble_gatts_exchange_mtu_reply(bleEvent->evt.gattc_evt.conn_handle, NRF_SDH_BLE_GATT_MAX_MTU_SIZE );
		APP_ERROR_CHECK(err_code);
#endif
		break;

	case BLE_GAP_EVT_CONN_PARAM_UPDATE:	// 0x12
		RTTLogger::log("GAP parm update");
		// assume SD handled this internally
		break;

	case BLE_GAP_EVT_TIMEOUT:	// 0x1B
		RTTLogger::log("GAP advertising timeout");
		break;

	case  BLE_GAP_EVT_RSSI_CHANGED:
		RTTLogger::log("RSSI changed");

		RSSI::updateFromEvent(bleEvent);

		break;

	default:
		// No implementation needed?

		RTTLogger::log("event not handled");
		RTTLogger::log((uint32_t)bleEvent->header.evt_id);
		break;
	}

#endif

}

#ifdef NOT_USED
void dispatchSysEvent(ble_evt_t * bleEvent) {

}
#endif

