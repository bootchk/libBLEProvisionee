
#pragma once

/*
 * Facade to NRF Advertising module
 * (which is distinct from using lower level API to create advertisements etc.)
 */
class AdModule {
public:
	static void init();

	static void startAdvertising(bool);
	static void stopAdvertising(bool);

	static void restartWithoutWhitelist();

};
