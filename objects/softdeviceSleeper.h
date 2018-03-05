#pragma once

#include <inttypes.h>


enum class ReasonForSDWake {
	Cleared,
	TimedOut,
	Canceled
};





/*
 * Knows how to sleep for Provisioner i.e. knows SD is enabled and coordinates with it
 */
class SoftdeviceSleeper {

private:
	/*
	 * Sleep until any one event.
	 */
	static void sleepInSDUntilAnyEvent();

public:
	/*
	 * Sleep until SD is disabled OR duration.
	 * May return before duration has elapsed.
	 * Ensures SD disabled on return.
	 * (When duration elapses, a timer's event handler disables SD.)
	 *
	 * If SD is not enabled when called, returns immediately.
	 */
	static void sleepInSDUntilTimeoutOrCanceled(uint32_t duration);

	static void cancelSleep();

	static ReasonForSDWake getReasonForSDWake();
	static void setReasonForSDWake(ReasonForSDWake reason);
};
