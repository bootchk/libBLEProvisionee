

#include <cassert>
#include <inttypes.h>
#include "../objects/nrfLog.h"


/*
 * External hacks
 */
// define BLE_STACK_SUPPORT_REQD in project properties>C++Compiler/Settings/Preprocessor

// hack softdevice/s132/headers/nrf_svc.h to "bx r14" : : "I" ((uint16_t)number) : "r0"

// paths to include files in project properties>C++Compiler/Settings/Includes

// Add linker file to Project Properties>C++ Build>Settings>Tool Settings>Cross ARM C Linker>General>Script Files>Add...
// Add path to included linker file
// Project Properties>C++ Build>Settings>Tool Settings>Cross ARM C Linker>Libraries-> Library search path>Add...

// in libraries/util
// #include "nordic_common.h"

// nrf.h depends on
// toolchain/cmsis/include
// toolchain
//#include "nrf.h"	// in /drivers



// Softdevice (closed library) headers
// s110 is BLE peripheral only but deprecated?

// s132 is for BLE central/peripheral without ANT
//#include "nrf_sdm.h"	// in /softdevice/s132/headers



#include "bleProtocol.h"

#include "app_error.h"


// classes interface to softdevice
#include "softdevice.h"
#include "gatt.h"
#include "gap.h"

#include "advertiser.h"
#include "service.h"
//#include "connection.h"

#include "uuid.h"




void BLEProtocol::start() {

	// require caller enabled SD
	assert(Softdevice::isEnabled());

	// Softdevice does not require app_timer (it uses its own timers?)
	// but some modules (Connection) do?

	/*
	 * This sequence is as found in ble_app_template.
	 * Except it omits stuff like security?
	 */
	GAP::initParams();
	Gatt::init();

	Uuid::init();	// Must precede AdModule and Service

	Advertiser::init();

	// Creating service also creates its characteristics
	uint32_t err_code = Service::init();
	APP_ERROR_CHECK(err_code);

#ifdef USE_CONNECTIONS_MODULE
	AppTimer::init();
	// Connection Module requires app_timer.  AppTimer::init() call must precede??
	Connection::initParams();
#endif
}


void BLEProtocol::stop() {
	// Stop any impending connection parameters update.
#ifdef USE_CONNECTIONS_MODULE
	Connection::stopPendingConnectionNegotiations();
#endif
}


void BLEProtocol::startAdvertising() {
	Advertiser::startAdvertising(false);
}


// not used
void BLEProtocol::stopAdvertising() {
	Advertiser::stopAdvertising(false);
}

