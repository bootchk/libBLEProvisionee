libBLEProvisionee


Attributes
-
   - library
   - for Nordic nRF5x family SoC
   - uses Bluetooth BLE
   - provides provisioning service: writeable config vars
   - NRF SDK 14.2
   - cmake
   - C++

Brief
-

A provisioner app runs on a smart phone.  See https://github.com/bootchk/BLEProvisionerSwift.  Provisioner talks to provisionee using BT protocol.

The provisionee library runs a provisioning session.
During a session, the library advertises its service: able to be provisioned.
A provisioner app scans for a provisionee; when it finds one it uses the service (writes a BT characteristic.)
The provisionee (library) disconnects after every write and ends the session.
Sessions provided by provisionee timeout, are short duration, and use little power.
A calling app (to the library) can determine any provisioned value.

After a provisioning session, the calling app is free to use the radio and other HW resources,
in a constrained fashion as documented for the Nordic Softdevice.
That is, the library uses the Softdevice compatibly with sequential multiprotocol.
Multiprotocol means the app can use the Softdevice for any other protocol or BT again.

A provisionable app must run provisioning sessions often enough,
and a provisioner app must scan long enough,
for success.

Use Cases
-

You have an app that doesn't use Bluetooth for anything other than provisioning, but the Nordic SoC and its ARM processor is convenient platform.

You have an app that uses the 2.4Ghz radio of the Nordic SoC but needs a convenient remote control GUI implemented on a smart phone talking over Bluetooth.

Artifacts
-

The project builds:

    - the library
    - a minimal app that calls the library (a test harness)
    
See CMakeLists.txt, which documents the built artifacts, their sources, and their dependencies.

Most of the sources for the library are in the objects directory.
Those are facade classes, from the library to Nordic SDK functions.
(Objects directory is a link another repository multiProtocolNordic, until I unify the two repositories.)

All the other sources (e.g. main.cpp) are for the executable target, the test harness app.
Other sources for the test harness (the Nordic functions) are specified by calls to macros in nRFCmake.
That includes the startup files, and other files used by the library.
The library itself does not contain any code from Nordic, the app must contain them.

The executable target also depends on the .ld loader script

Both the library and the executable are configured by sdk_config.h (a Nordic construct.)
Some of the macros used in the library are configured by sdk_config.h???

Test harness app
-

main.cpp is a example app using the API of the library (the Provisioner class) and thus providing 
periodic provisioning service

main2.cpp is even more minimal, just calling one function of the library (Softdevice::enable()).
Use this to test the build process.

Dependencies
-

Building depends on scripts from nRF5Cmake repository

The library depends on other static libraries:

    - https://github.com/bootchk/radioSoCSD (the SD compatible build)
    
which in turn depends on

    - https://github.com/bootchk/nRF5x
    - https://github.com/bootchk/embeddedMath
    - https://github.com/bootchk/libNRFDrivers
    
The library sources only include headers from the first.

All libraries must be made with the same float API (usually hard, for nrf52xxx family.)

Unfortunately, not all the other libraries have cmake scripts yet.

The test harness depends on sources from nRFSDK, the .ld script, and sdk_config.h


Configuring the SDK
-

See sdk_config.h.

A loose discussion of what I might have changed follows.  This depends on the SDK version and it's default sdk_config.h

NRF_SDH_ENABLED 1		using Softdevice
NRF_SDH_BLE_ENABLED 1   using BLE stack on Softdevice
NRF_BLE_GATT_ENABLED 1  using GATT module (to handle GATT events for us)
CLOCK_ENABLED 1         using the SD compatible LF/HF clock module

APP_TIMER_ENABLED 0        not using, instead implementing timers ourselves
BLE_ADVERTISING_ENABLED 0  not using advertising module, instead implementing only one fast advertising


Building
-

Uses CMake and a command line.

First edit CMakeEnv.cmake (in your clone of nRF5Cmake repository)

Then

    mkdir Debug52   ( or another name)
    cmake -H. -B"Debug52" -G "Unix Makefiles"    (or "Ninja")
    cmake --build "Debug52" --target testLibBLEProvisionee

The former creates the build directory and Makefiles.
The latter builds.

At least once burn the Softdevice (a layer implementing BT) to a separate area of ROM:

    cmake --build "cmake-build" --target FLASH_SOFTDEVICE
    
To burn to a NRF52DK dev board:

    cmake --build "cmake-build" --target FLASH_testLibBLEProvisionee
    
The app will start running on the DK, and periodically advertise its "provisioning" service.
In very short bursts (2mSec) which you might need to change.

Testing
-

Use a sniffer on a smart phone, such as  https://www.nordicsemi.com/eng/Products/Nordic-mobile-Apps/nRF-Connect-for-mobile-previously-called-nRF-Master-Control-Panel available on iOS and Android stores.

You should be able to sniff the app.  It will appear as a "Firefl" device (unless you have customized the project.)

Using the sniffer, browse the one characteristic and write to it.  Expect the provisionee to immediately disconnect and for the sniffer to indicate the disconnection.

Alternatively, build and use the iOS app https://github.com/bootchk/BLEProvisionerSwift following the instructions given there.  That app is specific to the provisioning service provided by this library (it won't sniff out any other BT devices.)

Putting it all together
-

To get a custom iOS app remote control over Bluetooth to an app on an embedded device:

    - flesh out the app (main.cpp) in this project (to do something useful)
    - modify the GUI of the BLEProvisionerSwift project (to be more specific and meaningful)
    - generate your own UUID's and copy them to both projects


IDE projects
-

The project in the repository is an Eclipse project.
I don't think it is proper (I use Eclipse only as editor, and build using a command line.)

You might try generating a proper Eclipse project using:

    https://cmake.org/Wiki/Eclipse_CDT4_Generator
    

TODO
-

    test multiprotocol, i.e. that the app can use the radio for other things while not provisioning, without conflicting with provisioning 
    
