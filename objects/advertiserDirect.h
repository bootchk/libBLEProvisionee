
#pragma once

/*
 * dvertising direct to SD (omitting Advertising Module)
 * (not direct to a peer.)
 *
 * Does not create bonds.
 */
class DirectAdvertiser {
public:
	static void init();
	static void startAdvertising();
	static void stopAdvertising();
};
