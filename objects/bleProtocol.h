
#pragma once

/*
 * Facade to the overall Nordic suite for BLE.
 *
 * Understands BLE modules on top of SD.
 * Inits them in proper order.
 */
class BLEProtocol {
public:
	static void start();
	static void stop();

	static void startAdvertising();
	static void stopAdvertising();

};
