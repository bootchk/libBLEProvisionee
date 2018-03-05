#pragma once



/*
 * Facade to C functions in SDK
 *
 * In SDK 14.2 Requires include paths to
 * components/libraries/util/
 * components/libraries/strerror
 * components/libraries/experimental_log/src/
 * components/libraries/experimental_memobj
 * components/libraries/balloc
 */

class NRFLog {
public:
	static void enable();
	static void log(char const* string);
	static void logInt(int value);
	static void flush();
};

