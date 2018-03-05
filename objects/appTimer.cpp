
#include "appTimer.h"

#include "nrfLog.h"

/*
 * Implementation by NRF SDK components/libraries/timer/app_timer.c or app_timer_ble_gzl.c ????
 *
 * Requires path above, and to components/drivers_nrf/delay
 */
#include "app_timer.h"
#include "app_error.h"

#include "nrf_drv_clock.h"

// SDK 14: see migrations guide:  configuration in sdk_config.h


void AppTimer::init() {
	// Initialize timer module, making it use the scheduler.
	// older SDK APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, nullptr);

	ret_code_t err_code;

	err_code = app_timer_init();
	APP_ERROR_CHECK(err_code);
}




void AppTimer::createRepeating(app_timer_id_t const * timerID, timerHandler aHandler ) {
	ret_code_t err_code;

	err_code = app_timer_create(timerID, APP_TIMER_MODE_REPEATED, aHandler);
	APP_ERROR_CHECK(err_code);
}

void AppTimer::createOneShot(app_timer_id_t const * timerID, timerHandler aHandler ) {
	ret_code_t err_code;

	err_code = app_timer_create(timerID, APP_TIMER_MODE_SINGLE_SHOT , aHandler);
	APP_ERROR_CHECK(err_code);
}



void AppTimer::start(app_timer_id_t timerID, int milliseconds) {
	ret_code_t err_code;

	// Macro converts from milliseconds to ticks
	uint32_t ticks = APP_TIMER_TICKS(milliseconds);
	NRFLog::logInt(ticks);

	// NULL: no context passed
	err_code = app_timer_start(timerID, ticks, NULL);
	APP_ERROR_CHECK(err_code);
}

void AppTimer::stop(app_timer_id_t timerID) {
	ret_code_t err_code;
	err_code = app_timer_stop(timerID);
	APP_ERROR_CHECK(err_code);
}

bool AppTimer::isClockRunning() {
	return nrf_drv_clock_lfclk_is_running();
}

