
#include "nrfLog.h"

#include <inttypes.h>

/*
 * Configured in sdk_config.h
 *
 * Might be configured (my preference)
 * - uses RTT
 * - not deferred
 */

/*
 * SDK 14.  See migration guide.
 * !!! Order is important:  ???
 * 1. #defind module name
 * 2. #include nrf_log.h
 * 3. invoke NRF_LOG_MODULE_REGISTER
 *
 * Not quoted.
 * Using "app" apparently conflicts with predefined var?
 *
 * I don't understand how this is per module?
 */
#define NRF_LOG_MODULE_NAME    fir

// components/libraries/experimental_log
// also includes from components/libraries/atomic
// also includes from external/fprintf
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"


NRF_LOG_MODULE_REGISTER();

void NRFLog::enable() {
	uint32_t           err_code;

	// also define NRF_LOG_ENABLED=1 in sdk_config.h
	err_code = NRF_LOG_INIT(nullptr);	// no timestamp func
	APP_ERROR_CHECK(err_code);

	NRF_LOG_DEFAULT_BACKENDS_INIT();
}

void NRFLog::log(char const * string) {
	// Use this if string is dynamic (on the stack)
	//NRF_LOG_INFO("%s",nrf_log_push(string));
	NRF_LOG_INFO("%s",string);
	NRF_LOG_FLUSH();
}

void NRFLog::logInt(int value) {
	NRF_LOG_INFO("0x%x", value);
	NRF_LOG_FLUSH();
}

void NRFLog::flush() {
	NRF_LOG_FLUSH();
}
