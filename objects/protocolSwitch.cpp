
#include "protocolSwitch.h"

namespace {

// Should this be volatile?  Could be set from an interrupt?
ProtocolMode mode;
ProtocolMode previousMode;
}

ProtocolMode ProtocolSwitch::protocolMode(){
	return mode;
}

void ProtocolSwitch::setMode(ProtocolMode aMode){
	previousMode = mode;
	mode = aMode;
}

bool ProtocolSwitch::didChangeMode(){
	return previousMode != mode;
}
