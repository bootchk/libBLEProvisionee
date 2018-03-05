
#include "uuid.h"

#include <cassert>

#include "app_error.h"
#include "ble.h"	// def of sd_ble_uuid_add



// MIDI Service UUID: 03B80E5A-EDE8-4B33-A751-6CE34EC4C700
// MIDI Data I/O Characteristic UUID: 7772E5DB-3868-4112-A1A9-F2669D106BF3
// The bytes need to be in reverse order to match the UUID from the spec
// Note: They are stored in little endian, meaning that the Least Significant Byte
// is stored first.

#define BLE_UUID_MIDI_SERVICE_BASE_UUID  {0x00, 0xC7, 0xC4, 0x4E, 0xE3, 0x6C, 0x51, 0xA7, 0x33, 0x4B, 0xE8, 0xED, 0x5A, 0x0E, 0xB8, 0x03}
#define BLE_UUID_MIDI_SERVICE_UUID        0x0E5A

#define BLE_UUID_MIDI_DATA_IO_CHAR_BASE_UUID {0xF3, 0x6B, 0x10, 0x9D, 0x66, 0xF2, 0xA9, 0xA1, 0x12, 0x41, 0x68, 0x38, 0xDB, 0xE5, 0x72, 0x77}
#define BLE_UUID_MIDI_DATA_IO_CHAR_UUID       0xE5DB

namespace {

/*
 * UUIDRepresentatives that this class creates.
 */
ble_uuid_t myServiceUUIDRepresentative;
ble_uuid128_t myServiceLongUUID = BLE_UUID_MIDI_SERVICE_BASE_UUID;

ble_uuid_t myCharacteristicUUIDRepresentative;
ble_uuid128_t myCharacteristicLongUUID = BLE_UUID_MIDI_DATA_IO_CHAR_BASE_UUID;

bool wasInit = false;


void createUUIDRepresentative(
		ble_uuid_t * representative,
		ble_uuid128_t* longUUID,
		uint16_t shortUUID
		) {
	uint32_t   err_code;

	/*
	 * put long UUID in sd's vendor specific table and put returned index into UUIDRepresentative
	 */
	err_code = sd_ble_uuid_vs_add(
			longUUID, // IN
			&representative->type);	//myServiceUUIDIndex);	// OUT.  "type" is misnomer: is an index

	APP_ERROR_CHECK(err_code);

	/*
	 * Put short UUID in UUIDRepresentative
	 * "uuid" is misnomer: is a short UUID
	 */
	representative->uuid = shortUUID;

	/*
	 * assert representative has a short UUID
	 * and an index into SD internal table of vendor specific long UUID's
	 */
}

}




void Uuid::init() {

	/*
	 * !!! BLE stack must be configured to have room for two vendor specific UUID's.
     * Else these calls may fail with error 4.
	 */
	// Service
	createUUIDRepresentative(
			&myServiceUUIDRepresentative,
			&myServiceLongUUID,
			BLE_UUID_MIDI_SERVICE_UUID
			);
	// Characteristic
	createUUIDRepresentative(
			&myCharacteristicUUIDRepresentative,
			&myCharacteristicLongUUID,
			BLE_UUID_MIDI_DATA_IO_CHAR_UUID
	);

	wasInit = true;
}


ble_uuid_t* Uuid::getCustomServiceUUID() {
	assert(wasInit);
	return &myServiceUUIDRepresentative;
}

ble_uuid_t* Uuid::getCustomCharacteristicUUID() {
	// assert init was called
	return &myCharacteristicUUIDRepresentative;
}
