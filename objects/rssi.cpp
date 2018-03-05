
#include "rssi.h"


#include "ble_conn_params.h"

namespace {

/*
 * RSSI is signed.
 * More negative is weaker signal.
 * Maximum is in positive direction.
 *
 * Typically, rssi returned from SD is always negative.
 */

// max RSSI during a connection
int8_t _maxRssi = -127;	// MAX negative int8_t

int8_t _lastRssi = -127;

uint16_t _connectionHandle;	// !!! Danger of null or closed?
}




void RSSI::startRSSIMeasurementOnConnection(uint16_t aConnectionHandle) {
	/*
	 *  Start getting events for RSSI changed on next connection exchange?
	 *  Connection must precede this.
	 *  This only generates events if threshold and skip_count filters succeed.
	 */
	sd_ble_gap_rssi_start(aConnectionHandle,
			8,	//  threshold_dbm,
			1	// 	skip_count
	);

	_connectionHandle = aConnectionHandle;
}


int8_t  RSSI::getLastRSSIOnConnection(uint16_t aConnectionHandle) {
	uint32_t   err_code;
	int8_t result = 0;

	err_code = sd_ble_gap_rssi_get(
			aConnectionHandle,
			&result);
	APP_ERROR_CHECK(err_code);
	return result;
}


int8_t RSSI::maxRSSI() { return _maxRssi; }
int8_t RSSI::lastRSSI() { return _lastRssi; }


void  RSSI::captureLastRSSI() {
	// !!! On the remembered connection
	_lastRssi = getLastRSSIOnConnection(_connectionHandle);
}


void RSSI::updateFromEvent(ble_evt_t const * bleEvent) {
	int8_t receivedRssi;
	receivedRssi = bleEvent->evt.gap_evt.params.rssi_changed.rssi;
	//RTTLogger::log((uint8_t)receivedRssi);

	/*
	 * Take max rssi.
	 * We never stop taking rssi.
	 */
	if (receivedRssi > _maxRssi)
		_maxRssi = receivedRssi;
}

// TODO return magnitude as inverseRSSI

int8_t RSSI::getConnectionRSSI() {
	int8_t result;

	// Return the more positive one
	if ( _maxRssi < _lastRssi )
		result = _lastRssi;
	else
		result = _maxRssi;
	return result;
}
