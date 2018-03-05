
#include "advertiser.h"


#define DIRECT_ADVERTISE 1

#ifdef DIRECT_ADVERTISE
#include "advertisement.h"
#include "advertiserDirect.h"

// Delegate to DirectAdvertiser
void Advertiser::init() {  DirectAdvertiser::init();  }
void Advertiser::startAdvertising(bool eraseBonds) { DirectAdvertiser::startAdvertising(); }


#else

#include "adModule.h"
#include "appTimer.h"

// Delegate to AdModule
void Advertiser::init() { AdModule::init(); }
void Advertiser::startAdvertising(bool eraseBonds) { AdModule::startAdvertising(eraseBonds); }

#endif
