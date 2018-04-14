
#pragma once

/*
 * Hides implementation of advertising.
 *
 * Two alternatives, using either:
 * - Nordic Advertising module
 * - direct advertising to SD
 *
 * The former:
 * - also requires app_timer?
 * - can sequence through modes (fast, slow, etc.) i.e. dynamically vary rate of advertising
 * - can handle creating bonds
 *
 * The latter:
 * - requires handling more events in GAP?
 *
 * Deciding between alternatives:
 *
 * For a Provisioner, don't need bonds.
 * A constant advertising rate is as effective as a varying rate.
 */







class Advertiser {

public:
/*
 * A tag for a Bluetooth stack configuration.
 */
 //#define APP_BLE_CONN_CFG_TAG
 static const int ConnectionConfigTag = 1;
 /*
  * Duration between advertisements.
  * units of 0.625 ms.
  * 40 corresponds to 25 ms.
  * 0x20 (32) is minimum (20ms) for connectable mode advertising
  * 0xa0 is minimum (100ms) for non-connectable mode
  * 0x4000 is max (10.24seconds)
  * The smaller the value, the more frequent the advertising, and more power used.
  *
  * BLE_GAP_ADV_INTERVAL_MAX   0x4000
  */
 /*
  * If want to advertise 10 times in 0.2 sec timeout use 0x40
  * If want to advertise 5 times in 1 sec, use 0x140 (320 decimal
  */
 //#define APP_ADV_INTERVAL                 0x40
 static const int AdvertisingIntervalIn625uSec = 0x140;
 /*
  * How long to keep advertising,
  * If no central responds to our advertisement
  * (with a connection request or request for more adv data)
  * then SD quits advertising and sleeps.
  * units of seconds.
  *
  * Value 0 means: never timeout
  */
 /*
  * For provisioning, set it to the minimum (but not zero) so we will get a timeout.
  * But advertising interval is great, so we don't spend much energy.
  * We will only advertise (and listen briefly) about
  */
 // #define APP_ADV_TIMEOUT_IN_SECONDS       0
 static const int AdvertisingTimeoutInSeconds = 1;


	static void init();
	static void startAdvertising(bool);
	static void stopAdvertising(bool);
};
