
#pragma once

#include "app_timer.h"



typedef void (*timerHandler)(void *);


/*
 * Facade to Nordic app_timer
 */
class AppTimer {
public:
	static void init();
	static void createRepeating(app_timer_id_t const *, timerHandler);
	static void createOneShot(app_timer_id_t const *, timerHandler);
	static void start(app_timer_id_t, int);
	static void stop(app_timer_id_t);

	/*
	 * Is required clock running?
	 * Does not mean there are timers queued.
	 */
	static bool isClockRunning();
};
