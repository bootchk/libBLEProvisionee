
#pragma once

#include "ble_types.h"

/*
 * Knows:
 * - UUID's come in two forms:
 * -- long
 * -- short
 * - how to register a long UUID with the SD (associate with short UUID or get index?)
 *
 * Hack: not generic,  two fixed UUID's
 */

// From SDK
/**@brief Add a Vendor Specific base UUID.
 *
 * @details This call enables the application to add a vendor specific base UUID to the BLE stack's table, for later
 * use with all other modules and APIs. This then allows the application to use the shorter, 24-bit @ref ble_uuid_t
 * format when dealing with both 16-bit and 128-bit UUIDs without having to check for lengths and having split code
 * paths. This is accomplished by extending the grouping mechanism that the Bluetooth SIG standard base UUID uses
 * for all other 128-bit UUIDs. The type field in the @ref ble_uuid_t structure is an index (relative to
 * @ref BLE_UUID_TYPE_VENDOR_BEGIN) to the table populated by multiple calls to this function, and the UUID field
 * in the same structure contains the 2 bytes at indexes 12 and 13. The number of possible 128-bit UUIDs available to
 * the application is therefore the number of Vendor Specific UUIDs added with the help of this function times 65536,
 * although restricted to modifying bytes 12 and 13 for each of the entries in the supplied array.
 *
 * @note Bytes 12 and 13 of the provided UUID will not be used internally, since those are always replaced by
 * the 16-bit uuid field in @ref ble_uuid_t.
 *
 * @note If a UUID is already present in the BLE stack's internal table, the corresponding index will be returned in
 * p_uuid_type along with an NRF_SUCCESS error code.
 *
 * @param[in]  p_vs_uuid    Pointer to a 16-octet (128-bit) little endian Vendor Specific UUID disregarding
 *                          bytes 12 and 13.
 * @param[out] p_uuid_type  Pointer to a uint8_t where the type field in @ref ble_uuid_t corresponding to this UUID will be stored.
 *
 * @retval ::NRF_SUCCESS Successfully added the Vendor Specific UUID.
 * @retval ::NRF_ERROR_INVALID_ADDR If p_vs_uuid or p_uuid_type is NULL or invalid.
 * @retval ::NRF_ERROR_NO_MEM If there are no more free slots for VS UUIDs.
 */
class Uuid {
public:
	static void init();
	static ble_uuid_t* getCustomServiceUUID();
	static ble_uuid_t* getCustomCharacteristicUUID();
};
