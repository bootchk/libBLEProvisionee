
#include "connection.h"


#include <inttypes.h>
#include <string.h>  // memset

#include "ble_conn_params.h"
#include "app_timer.h"	// only for the ..TICKS macro?




#define FIRST_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(5000)
/**< Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (5 seconds). */
#define NEXT_CONN_PARAMS_UPDATE_DELAY    APP_TIMER_TICKS(30000)
/**< Time between each call to sd_ble_gap_conn_param_update after the first call (30 seconds). */
#define MAX_CONN_PARAMS_UPDATE_COUNT     3
/**< Number of attempts before giving up the connection parameter negotiation. */



namespace {

void connectionParmsErrorHandler(uint32_t error) {
	APP_ERROR_HANDLER(error);
}

}


void Connection::initParams() {

	uint32_t               err_code;
	ble_conn_params_init_t connection_params_init;

	memset(&connection_params_init, 0, sizeof(connection_params_init));

	// Since memset to zero, comment out all subsequent setting to NULL

	// connection_params_init.p_conn_params                  = NULL;
	connection_params_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
	connection_params_init.next_conn_params_update_delay  = NEXT_CONN_PARAMS_UPDATE_DELAY;
	connection_params_init.max_conn_params_update_count   = MAX_CONN_PARAMS_UPDATE_COUNT;
	// Since we chose no notification, no need for handle???
	// If service/characteristic has notification enabled, need like  = m_hrs.hrm_handles.cccd_handle;
	// where m_hrs is a ServiceData, either global or passed in
	// connection_params_init.start_on_notify_cccd_handle    =  NULL;
	connection_params_init.disconnect_on_fail             = true;
	// Since we disconnect on fail, no need for event handler to catch failed negotiation event
	// connection_params_init.evt_handler                    = NULL;
	connection_params_init.error_handler                  = connectionParmsErrorHandler;

	err_code = ble_conn_params_init(&connection_params_init);
	APP_ERROR_CHECK(err_code);

}

void Connection::stopPendingConnectionNegotiations() {
	uint32_t    err_code;

	err_code = ble_conn_params_stop();
	APP_ERROR_CHECK(err_code);
}



