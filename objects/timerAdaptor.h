
#pragma once

#include <inttypes.h>

// lib radioSoC
#include <clock/timer.h>    // TimerInterruptReason


typedef void (*AlarmHandler)(TimerInterruptReason reason);


/*
 * Hides implementation of timer.
 *
 * Implementation using radioSoC library requires LongClock running.
 *
 * Some adaptions have create() do nothing.
 * Some adaptions have different signature for start.
 */
class TimerAdaptor {
public:
	/*
	 * knows how to start all clocks needed by Timer: lf and longclock
	 */
	static void startClocks();

	static void create(AlarmHandler);
	static void create();

	static void start(uint32_t, AlarmHandler);
	static void stop();
};
