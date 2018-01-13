libBLEProvisionee


Attributes
-
   - library
   - for Nordic nRF5x family SoC
   - uses Bluetooth BLE
   - provides provisioning service: writeable config vars
   - NRF SDK 14.2

Brief
-

A provisioner app runs on a smart phone.  See https://github.com/bootchk/BLEProvisionerSwift.  Provisioner talks to provisionee using BT protocol.

The library runs a provisioning session.
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

All the other sources (e.g. main.cpp) are for the executable target, the test harness app.
Other sources for the test harness (the Nordic functions) are specified in CMake_nRF5x.cmake.
That includes the startup files, and other files used by the library.
The library itself does not contain any code from Nordic, the app must contain them.

The executable target also depends on the .ld loader script

Both the library and the executable are configured by sdk_config.h (a Nordic construct.)

Test harness app
-

main.cpp is a example app using the API of the library (the Provisioner class) and thus providing 
periodic provisioning service

main2.cpp is even more minimal, just calling one function of the library (Softdevice::enable()).
Use this to test the build process.

Dependencies
-

The library depends on other static libraries:
    - https://github.com/bootchk/radioSoC
which in turn depends on
    - https://github.com/bootchk/nRF5x
    - https://github.com/bootchk/embeddedMath
    
The library sources only include headers from the first.

All libraries must be made with the same float API (hard.)


Building
-

Uses CMake and a command line.

CMake scripts derived from https://github.com/Polidea/cmake-nRF5x

    cmake -H. -B"cmake-build" -G "Unix Makefiles"
    cmake --build "cmake-build" --target libBLEProvisionee

The former creates the build directory and Makefiles.
The latter builds.

IDE projects
-

The project in the repository is an Eclipse project.
I don't think it is proper (I use the command line.)

You might try generating a proper Eclipse project using:

    https://cmake.org/Wiki/Eclipse_CDT4_Generator
    

TODO
-

    test multiprotocol
    
Acknowledgements
-

Several cmake scripts come from https://github.com/Polidea/cmake-nRF5x

I modified them for SDK v14.2, and to allow imported libraries.
