
#include "characteristic.h"

#include <string.h>	// memset

#include "provisioner.h"   // ProvisionedValueType

// from radioSoC library
#include "services/logger.h"


uint32_t Characteristic::addProxy(
		ServiceData * serviceData,
		ServiceDataInit * unused_servicefDataInit,
		ble_uuid_t*  aUUID,
		unsigned int characteristicLength
		)
{
	//static uint32_t battery_level_char_add(ble_bas_t * p_bas, const ble_bas_init_t * p_bas_init)
	//{
	uint32_t            err_code = NRF_SUCCESS;

	ble_gatts_char_md_t characteristicMetadata;
	//ble_gatts_attr_md_t cccd_md;
	ble_gatts_attr_t    attr_char_value;

	ble_gatts_attr_md_t attributeMetadata;

	// uint8_t             initialValue;

	//uint8_t             encoded_report_ref[BLE_SRV_ENCODED_REPORT_REF_LEN];
	//uint8_t             init_len;

#ifdef NOT_USED
	if (serviceData->isNotificationSupported)
	{
		memset(&cccd_md, 0, sizeof(cccd_md));

		// According to BAS_SPEC_V10, the read operation on cccd should be possible without
		// authentication.
		BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
		cccd_md.write_perm = serviceDataInit->battery_level_char_attr_md.cccd_write_perm;
		cccd_md.vloc       = BLE_GATTS_VLOC_STACK;
	}
#endif

	memset(&characteristicMetadata, 0, sizeof(characteristicMetadata));

	// Since we zeroed it, only need to write non-zero values

	characteristicMetadata.char_props.read   = 1;	// readable
	characteristicMetadata.char_props.write_wo_resp = 1;   // writeable
	/*
	characteristicMetadata.char_props.notify = (serviceData->isNotificationSupported) ? 1 : 0;
	characteristicMetadata.p_char_user_desc  = NULL;
	characteristicMetadata.p_char_pf         = NULL;
	characteristicMetadata.p_user_desc_md    = NULL;
	characteristicMetadata.p_cccd_md         = (serviceData->isNotificationSupported) ? &cccd_md : NULL;
	characteristicMetadata.p_sccd_md         = NULL;
	*/

	memset(&attributeMetadata, 0, sizeof(attributeMetadata));

	//attributeMetadata.read_perm  = serviceDataInit->battery_level_char_attr_md.read_perm;
	//attributeMetadata.write_perm = serviceDataInit->battery_level_char_attr_md.write_perm;

	//BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attributeMetadata.write_perm);

	// open (anyone, no security?) permission to read and write
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attributeMetadata.read_perm);
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attributeMetadata.write_perm);
	attributeMetadata.vloc       = BLE_GATTS_VLOC_STACK;
	attributeMetadata.rd_auth    = 0;
	attributeMetadata.wr_auth    = 0;
	attributeMetadata.vlen       = 0;

	//initialValue = serviceDataInit->initialValue;

	memset(&attr_char_value, 0, sizeof(attr_char_value));

	attr_char_value.p_uuid    = aUUID;
	attr_char_value.p_attr_md = &attributeMetadata;
	attr_char_value.init_len  = characteristicLength;
	attr_char_value.init_offs = 0;
	attr_char_value.max_len   = characteristicLength;
	// ??? the "Custom Char" tutorial uses NULL
	attr_char_value.p_value   = NULL;	// &initialValue;

	err_code = sd_ble_gatts_characteristic_add(
			serviceData->serviceHandle,
			&characteristicMetadata,
			&attr_char_value,
			&serviceData->characteristicHandles);

	return err_code;


	/* Omit HID stuff
	if (serviceDataInit->p_report_ref != NULL)
	{
		// Add Report Reference descriptor
		BLE_UUID_BLE_ASSIGN(ble_uuid, BLE_UUID_REPORT_REF_DESCR);

		memset(&attr_md, 0, sizeof(attr_md));

		attr_md.read_perm = serviceDataInit->battery_level_report_read_perm;
		BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);

		attr_md.vloc    = BLE_GATTS_VLOC_STACK;
		attr_md.rd_auth = 0;
		attr_md.wr_auth = 0;
		attr_md.vlen    = 0;

		init_len = ble_srv_report_ref_encode(encoded_report_ref, serviceDataInit->p_report_ref);

		memset(&attr_char_value, 0, sizeof(attr_char_value));

		attr_char_value.p_uuid    = &ble_uuid;
		attr_char_value.p_attr_md = &attr_md;
		attr_char_value.init_len  = init_len;
		attr_char_value.init_offs = 0;
		attr_char_value.max_len   = attr_char_value.init_len;
		attr_char_value.p_value   = encoded_report_ref;

		err_code = sd_ble_gatts_descriptor_add(serviceData->characteristicHandles.value_handle,
				&attr_char_value,
				&serviceData->report_ref_handle);
		if (err_code != NRF_SUCCESS)
		{
			return err_code;
		}
	}
	else
	{
		serviceData->report_ref_handle = BLE_GATT_HANDLE_INVALID;
	}
	*/
}


/*
 * Semantic validity.
 * All bytes are OTA and could be garbled (despite CRC check.)
 * We don't check ALL semantics,
 * i.e. that value byte is valid.
 * Users of the characteristic still might need to check value.
 *
 * My characteristics are UUID type 3,... (long UUID)
 * Type 2 is UUID of service?
 * Type 1 is UUID of std characteristic predfined by BT org
 */
bool Characteristic::isValidWrite(const ble_gatts_evt_write_t * aWrite)
{
	bool result = true;
	if ( !( aWrite->uuid.type == 3 ) ) { 	// UUID is long
		result = false;
		RTTLogger::log("UUID wrong.\n");
	}
	else if ( ! ( aWrite->len == Provisioner::ProvisionedCharacteristicLength ) ) {
		result = false;
		RTTLogger::log("Length wrong.\n");
	}
	else if ( aWrite->data[1] > 3 ){
		/*
		 * data[1] is index, UInt8
		 * Multiplexing many provisionable values through one characteristic
		 * requires that index is in range of provisionable values.
		 */
		result = false;
		RTTLogger::log("Index wrong.\n");
	}
	return result;
}
