
#include "timerAdaptor.h"

#include <cassert>

#include "nrfLog.h"

// lib radioSoC
// timerAdaptor.h already #include "timer.h"
#include "clock/clockFacilitator.h"



/*
 * Define which implementation is adapted
 */
// Adapt null implementation.
// Use to simplify building: some objects e.g. Provisioner referenced but not called
// Provisioner calls TimerAdapter
// e.g. for main1
//#define ADAPT_NULL

// Nordic app_timer via AppTimer facade
//#define ADAPT_APP_TIMER

// radioSoC lib Timer
#define ADAPT_TIMER



#ifdef ADAPT_NULL
void TimerAdaptor::stop() { }
#endif


#ifdef ADAPT_APP_TIMER
//APP_TIMER_DEF(provisionElapsedTimerID);
// AppTimer::stop(provisionElapsedTimerID);
//AppTimer::createOneShot(&provisionElapsedTimerID, provisionElapsedTimerHandler);
#endif


#ifdef ADAPT_TIMER
void TimerAdaptor::startClocks() {

#ifdef OLD
	this interferes with SD
	ClockFacilitator::startLongClockWithSleepUntilRunning();
#else
	ClockFacilitator::startLongClockNoWaitUntilRunning();
#endif

	// No need to create timers, static defined
}


#ifdef NOT_USED
void TimerAdaptor::create() {
}
#endif

void TimerAdaptor::create(AlarmHandler onTimeoutCallback) {
	// Using radioSoC, no need to create
}

void TimerAdaptor::start(uint32_t timeout, AlarmHandler onTimeoutCallback) {
	assert(ClockFacilitator::isLongClockRunning());

	NRFLog::log("start timer");
	Timer::start(
			TimerIndex::First,	// assert first timer not in use by SleepSync
			timeout,
			onTimeoutCallback);
}


void TimerAdaptor::stop() {
	Timer::cancel(TimerIndex::First);
}

#endif

