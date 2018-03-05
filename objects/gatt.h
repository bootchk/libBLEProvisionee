
#pragma once

/*
 * Facade to Gatt module, an adapter to the SD.
 *
 * Alternative is to handle Gatt events using SD directly.
 */
class Gatt {
public:
	static void init();
};
