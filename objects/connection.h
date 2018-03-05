
#pragma once

// #include "service.h"


/*
 * Facade to Connection Parameters module.
 *
 * Module implements BT negotiation of connection parameters.
 *
 * Not needed unless your peripheral wants to negotiate with central for better connection.
 *
 * !!! Depends on app_timer module.
 */
class Connection {
public:
	static void initParams();
	static void stopPendingConnectionNegotiations();
};
