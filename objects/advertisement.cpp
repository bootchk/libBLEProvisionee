
#include "advertisement.h"


// Implementation

#include <inttypes.h>

#include "ble_advdata.h"
#include "app_error.h"	// APP_ERROR_CHECK

#include "../objects/uuid.h"



bool _isInit = false;


/**@brief Function for initializing the Advertising functionality.
 *
 * @details Encodes the required advertising data and passes it to the stack.
 *          Also builds a structure, m_adv_params, to be passed to the stack
 *          when starting advertising.
 */
void Advertisement::init()
{
    uint32_t      err_code;
    ble_advdata_t advdata;
    uint8_t       flags = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;


    /*
     * Customize.
     * We provide only one custom service.
     * UUID generated once, and inserted here.
     */
    // BLE_UUID_TYPE_UNKNOWN       0x00 /**< Invalid UUID type. */
    // #define BLE_UUID_TYPE_BLE           0x01 /**< Bluetooth SIG UUID (16-bit). */
    // #define BLE_UUID_TYPE_VENDOR_BEGIN

    ble_uuid_t adv_uuids[1];
    adv_uuids[0]= *Uuid::getCustomServiceUUID();

    /* Static initialization for UUID's defined by BT standard
    {
        {ShortSERVICEUUID, BLE_UUID_TYPE_BLE},
    };
    */


    memset(&advdata, 0, sizeof(advdata));

    /*
     * !!! Advertisement is only 31 bytes.
     * Might not be enough for a long device name AND appearance.
     * If not enough room, device name is truncated.
     */
    advdata.name_type               = BLE_ADVDATA_FULL_NAME;	// Set how much of device name to advertise
    advdata.include_appearance      = false;	// Whether to advertise appearance
    advdata.flags                   = flags;
    advdata.uuids_complete.uuid_cnt = sizeof(adv_uuids) / sizeof(adv_uuids[0]);
    advdata.uuids_complete.p_uuids  = adv_uuids;

    err_code = ble_advdata_set(&advdata, NULL);
    APP_ERROR_CHECK(err_code);

    _isInit = true;
}


bool Advertisement::isInit() { return _isInit; }

