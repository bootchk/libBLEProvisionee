
#pragma once

#include <inttypes.h>
#include "ble_srv_common.h"



typedef enum
{
    BLE_EVT_NOTIFICATION_ENABLED,
    BLE_EVT_NOTIFICATION_DISABLED
} BleServiceEventKind;



typedef struct
{
	BleServiceEventKind evt_type;
} BleServiceEvent;


// Forward declaration.
struct ServiceData;

// Need "struct ServiceData" here since ServiceData is not typedefed
typedef void (*BleServiceEventHandler) (struct ServiceData *, BleServiceEvent *);

typedef void (*AppWriteEventHandler) (struct ServiceData *, uint8_t newValue);


/*
 * For some reason, struct for init data.
 */
struct ServiceDataInit
{
	BleServiceEventHandler        evt_handler;      /**< Event handler to be called for handling events in the Battery Service. */
    bool                          support_notification;           /**< TRUE if notification of Battery Level measurement is supported. */
    uint8_t                       initialValue;
    ble_srv_cccd_security_mode_t  battery_level_char_attr_md;     /**< Initial security level for battery characteristics attribute */
    //ble_srv_report_ref_t *        p_report_ref;                   /**< If not NULL, a Report Reference descriptor with the specified value will be added to the Battery Level characteristic */
    //ble_gap_conn_sec_mode_t       battery_level_report_read_perm; /**< Initial security level for battery report read attribute */
} ;



struct ServiceData
{
	AppWriteEventHandler          appEventHandler;
	uint16_t                      serviceHandle;             //  Handle of Service (as provided by the BLE stack).
	ble_gatts_char_handles_t      characteristicHandles;     //  Handles related to characteristic.
	//ble_srv_report_ref_t *        p_report_ref;
	//uint16_t                      report_ref_handle;              /**< Handle of the Report Reference descriptor. */
	//uint8_t                       battery_level_last;             /**< Last Battery Level measurement passed to the Battery Service. */
	uint16_t                      connectionHandle;           // Handle of current connection (as provided by the BLE stack, is BLE_CONN_HANDLE_INVALID if not in a connection). */
	bool                          isNotificationSupported;
};




/*
 * Implements a Service with one Characteristic.
 *
 * If specified, the module will support notification of the Battery Level characteristic
 *          through the ble_bas_battery_level_update() function.
 *          If an event handler is supplied by the application, the Battery Service will
 *          generate Battery Service events to the application.
 *
 * The app propagates BLE stack events to this Service module by calling
 * Service::onBleEvent()     ble_bas_on_ble_evt() from the softdevice_handler callback.
 */
class Service {
public:
	static void onBleEvent(ServiceData*, const ble_evt_t*);
	static uint32_t init();
	static const ServiceData* data();
};
