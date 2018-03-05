
This is an obsolete readme.md from the multiProtocolNordic repository,
an ancestor project.  I keep it because it documents some of the history and some build settings.
The essential source files (the /objects directory) from that project
are now in this project.

 
Sequential (not concurrent) multiprotocol using Nordic NRF radio SoC.

Switches sequentially between BLE and proprietary protocols.
Radio and other HW devices used sequentially by protocols.

Use BLE to create a BT peripheral that briefly advertises a characteristic (property)
that a control app (a BT central) can write.
The control app has plenty of power to scan for the peripheral, which sleeps mostly.

The goal is a standalone app that advertises a characteristic, periodically,
shutting down the SoftDevice in between.

Then the goal is to make it a library
and integrate with an app that uses a proprietary protocol most of the time.

Status
-

 work in progress, not working.  The problem here is in the Eclipse build configurations, trying to mimic Nordic makefiles.
 
 But the objects/ dir is correct and used by bleAppTemplate and libBLEProvisionee, which do work.
 Those projects use Nordic Makefiles or CMake
 

Initially derived from NRF SDK examples/multiprotocol/ble_app_gzll
Modified to be object oriented: more understandable and modifiable.
Then derived from examples/ble_peripheral/ble_app_template, which uses more abstractions from the SDK.

Project is C++
Some hacks necessary to get C code to compile with C++
Project uses Eclipse managed make (no makefile, uses Project>Properties)

Architecture
-

Layers
    - Facade classes
    - Modules from SDK
    - SD
    
Modules from SDK require linking in certain .c files that implement the modules on top of SD.  Modules abtract away some complexities of the alternative: dealing with certain SD events yourself.

The SD is closed source.  SD usually burned to flash in a separate step.  Requires linking to SD headers.  Calls to the SD are via SVC instruction.  Callbacks with events from SD to registered handlers.


Dependencies
-

SDK 14.2

NRF52K

SD132

Burn it to NRF52DK along with SD 132 or 112?

Test it with NRF app for iPhone "NRF Connect."



Configuration
-

In general, the project properties must be set to mimic the Makefile from the example ble_app_template.
What is described here may not correspond to actual project properties.

Preprocessor symbols in Project Properties

    NRF_SD_BLE_API_VERSION=3
    NRF52
    BLE_STACK_SUPPORT_REQD

No BOARD symbol defined (buttons and leds not used.)


sdk_config.h

    NRF_SDH_BLE_VS_UUID_COUNT = 2       since using two vendor specifig UUIDS

Used many settings from ble_app_template's sdk_configh.h
such as NRF_FSTORAGE_ENABLED (when using Advertising module)

Currently enable these modules:


    BLE_ADVERTISING_ENABLED 1
    NRF_BLE_CONN_PARAMS_ENABLED 1
    NRF_BLE_GATT_ENABLED 1
    
    NRF_FSTORAGE_ENABLED 1	 required by advertising module
    
Not using:

    Peer Manager module (not interested in bonding BT devices.)
    NRF_PWR_MGMT_ENABLED (was enabled in original, but why?)
    FDS_ENABLED (was enabled in original, but why?)
    
    


Linker script
-

Modified to allow plenty or room for SD ram.
You can use the process described on Nordic website: you use the debugger to determine the optimal start address of application RAM: step through until the code where ram_start value is returned by the SD.


Other Notes
-

I started with SDK 12.3, there may be improper vestiges of that.

At first, I copied source from SDK into project and change their suffix to .cpp.
They should be linked files, and C flags created properly.
