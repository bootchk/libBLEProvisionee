
#pragma once


/*
 * Modes are mutually exclusive.
 * Protocols use radio in sequence, not concurrently
 */
enum class ProtocolMode {
	BLE,   			// Bluetooth
	Alternate       // some other protocol (AKA proprietary)
};


class ProtocolSwitch {
public:
	static ProtocolMode protocolMode();
	static void setMode(ProtocolMode);
	static bool didChangeMode();

};
